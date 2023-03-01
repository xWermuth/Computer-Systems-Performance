#include "utils.h"
#include "parallel_buffer.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <set>
#include <vector>
#include <map>

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
    void run(vector<DataTuple> *data_tuples, int THREADS, int hashbits)
    {
        cout << "ParallelBuffer roll out " << endl;
        const int COUNT = data_tuples->size();
        const int partetions = Utils::getPartations(hashbits);
        const int chunk_size = (COUNT / partetions) / partetions;
        printf("COUNT = %d, partetions = %d, chunk_size = %d, partition length = %d\n", 
            COUNT, partetions, chunk_size, partetions + (partetions >> 1));

        pthread_t threads[THREADS];
        vector<Partition> partation(partetions);
        for (auto &&p : partation)
        {
            cout << "HELlo" << endl;
            p = {
                .chunks = new vector<Chunk>(partetions + (partetions >> 1)),
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
        for (size_t i = 0; i < payload->chunks->size(); i++)
        {
            auto tuple = payload->chunks->at(i);
            u_char *hash = Utils::sha256(tuple.second, sizeof(uint64_t));
            long long hashIdx = Utils::hashBitsToIdx(hash, payload->hashbits);

            cout << "hashIdx: " << hashIdx << endl;
            auto partation = payload->partation->at(hashIdx);
            cout << "size of part: " << payload->partation->size(); 
            cout << "SIZE: " << partation.chunks->size() << endl;
            // cout << "AFTER____" << endl;
            // cout << "AFTER____" << endl;
            // auto temp = chunk_map.find(&partation);
            // if(temp != chunk_map.end()) 
            // {
            //     // if(payload->chunk_size <= )
            //     struct Chunk *chunk = temp->second;
            //     auto tuples_in_chunk = chunk->dataTuples;
            //     if(tuples_in_chunk->size() < payload->chunk_size)
            //     {
            //         cout << "BUFERE BUSHING "<<endl;
            //         cout << "size:  " << tuples_in_chunk->size() <<endl;
            //         tuples_in_chunk->push_back(tuple);
            //         cout << "AFTER BUSHING "<<endl;
            //     }
            //     else
            //     {
            //         struct Chunk *new_chunk = new struct Chunk();
            //         new_chunk->dataTuples = new vector<DataTuple>{tuple};
            //         cout << "BUFERE inserting "<<endl;
            //         chunk_map[&partation] = new_chunk;
            //         cout << "AFTER inserting "<<endl;
            //     }
            // } 
            // else 
            // {
                // int partIdx = partation.index->fetch_add(1);
                // Create chunk
                // struct Chunk *new_chunk = new struct Chunk();
                // new_chunk->dataTuples = new vector<DataTuple>(payload->chunk_size);
                // cout << "partIdx: " << partIdx << endl;
                // (*partation.chunks)[partIdx] = *new_chunk;
                
                // chunk_map[&partation] = new_chunk;
            // }

            delete hash;
        }

        pthread_exit(NULL);
    }

}