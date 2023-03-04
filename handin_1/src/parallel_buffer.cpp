#include "utils.h"
#include "parallel_buffer.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <set>
#include <vector>
#include <map>
#include <thread>

using namespace std;

// #define PART_SIZE 1024 // 2^10

struct Chunk
{
    vector<DataTuple> *dataTuples;
};

struct Partition 
{
    atomic<int> *index;
    vector<Chunk> *chunks;
};

struct Payload
{
    int hashbits;
    int threadId;
    int chunk_size;
    vector<Partition> *partation;
    vector<DataTuple> *chunks;
};

namespace ParallelBuffer
{

    void printBuffer(vector<Partition>& buffer, int chunk_size) 
    {
        for (size_t i = 0; i < buffer.size(); i++)
        {
            int chunks_full = 0;
            cout << "Partition " << i << endl;
            auto p = buffer[i];
            for(auto c : *p.chunks)
            {
                // cout << "RIGHT before "<< endl;
                if(c.dataTuples != nullptr){
                    chunks_full += c.dataTuples->size() == chunk_size ? 1 : 0;
                }
            }
            // for (vector<Chunk>::iterator c = p.chunks->begin(); c != p.chunks->end(); c++)
            // {
            //     cout << "RIGHT before "<< endl;
            //     chunks_full += c->dataTuples->size() == chunk_size ? 1 : 0;
            // }
            cout << "Partation's " << i << " has " << chunks_full << "/" << p.chunks->size() << " chunks fill" << endl;
        }

    }
    void run(vector<DataTuple> *data_tuples, int THREADS, int hashbits)
    {
        cout << "ParallelBuffer roll out " << endl;
        const int COUNT = data_tuples->size();
        const int partetions = Utils::getPartations(hashbits);
        const int chunk_size = 1024;
        const int chunks_in_part = COUNT / chunk_size; // 2^24 / 2^10 = 2^14 = 16384
        printf("COUNT = %d, partetions = %d, chunk_size = %d, partition length = %d\n",
               COUNT, partetions, chunk_size, partetions + (partetions >> 1));

        pthread_t threads[THREADS];
        vector<Partition> partation(partetions);
        for (auto &&p : partation)
        {
            // .chunks = new vector<Chunk>(partetions + (partetions >> 1)),
            //     
            p = {
                .chunks = new vector<Chunk>(chunks_in_part),
                .index = new atomic<int>{0}};
        }

        vector<vector<DataTuple>> *chunks = Utils::split_vector(*data_tuples, THREADS);
        struct Payload *args[THREADS];

        for (size_t i = 0; i < chunks->size(); i++)
        {
            struct Payload *payload = (Payload *) malloc(sizeof(struct Payload));
            payload->partation = &partation;
            payload->chunks = &(chunks->at(i));
            payload->hashbits = hashbits;
            payload->chunk_size = chunk_size;
            args[i] = payload;

            cout << "Spawning thread" << endl;
            int rc = pthread_create(&threads[i], NULL, buf_worker, payload);

            if (rc)
            {
                cout << "ERROR; return code from pthread_create() is " << rc << endl;
                break;
            }
        }

        for (size_t i = 0; i < THREADS; i++)
        {
            pthread_join(threads[i], NULL);
        }

        printBuffer(partation, chunk_size);

        // cleanup
        // for (size_t i = 0; i < THREADS; i++)
        // {
        //     free(args[i]);
        // }

        cout << "Parallel partation finito" << endl;
    }

    void *buf_worker(void *arg)
    {
        cout << "thread called" << endl;
        map<Partition*, Chunk *> chunk_map;
        Payload *payload = (Payload *)arg;
        thread::id threadId = this_thread::get_id();
        for (size_t i = 0; i < payload->chunks->size(); i++)
        {
            auto tuple = payload->chunks->at(i);
            u_char *hash = Utils::sha256(tuple.second, sizeof(uint64_t));
            long long hashIdx = Utils::hashBitsToIdx(hash, payload->hashbits);

            auto partation = payload->partation->at(hashIdx);
            auto temp = chunk_map.find(&partation);
            if(temp != chunk_map.end()) 
            {
                struct Chunk *chunk = temp->second;
                auto tuples_in_chunk = chunk->dataTuples;
                if(tuples_in_chunk->size() < payload->chunk_size)
                {
                    // cout << "THREAD: " << threadId << " - INSERTING IN OUR ASSIGNED CHUNK" << endl;
                    tuples_in_chunk->push_back(tuple);
                }
                else
                {
                    struct Chunk *new_chunk = new struct Chunk();
                    new_chunk->dataTuples = new vector<DataTuple>{tuple};
                    chunk_map[&partation] = new_chunk;
                    int partIdx = partation.index->fetch_add(1);
                    // cout << "THREAD: " << threadId << " - CHUNK IS FULL -> GETTING NEW CHUNK AT: " << partIdx << endl;
                    (*partation.chunks)[partIdx] = *new_chunk;
                }
            } 
            else 
            {
                int partIdx = partation.index->fetch_add(1);
                // cout << "THREAD: " << threadId << " - GETTING NEW CHUNK AT: " << partIdx << endl;
                // Create chunk
                struct Chunk *new_chunk = new struct Chunk();
                new_chunk->dataTuples = new vector<DataTuple>(payload->chunk_size);
                (*partation.chunks)[partIdx] = *new_chunk;
                chunk_map[&partation] = new_chunk;
            }

            delete hash;
        }

        pthread_exit(NULL);
    }
}