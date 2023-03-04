#include "utils.h"
#include "parallel_buffer.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <set>
#include <vector>
#include <unordered_map>
#include <thread>

using namespace std;

pthread_mutex_t my_lock;

// #define PART_SIZE 1024 // 2^10

struct Chunk
{
    int index;
    vector<DataTuple> *dataTuples;
};

struct Partition
{
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
    void printBuffer(vector<Partition> &buffer, int chunk_size)
    {
        uint32_t sum = 0;
        for (size_t i = 0; i < buffer.size(); i++)
        {
            int chunks_full = 0;
            // cout << "Partition " << i << endl;
            auto p = buffer[i];
            for (auto c : *p.chunks)
            {
                if (c.dataTuples != nullptr)
                {
                    sum += c.dataTuples->size();
                    chunks_full += c.dataTuples->size() == chunk_size ? 1 : 0;
                }
            }
            // cout << "Partation's " << i << " has " << chunks_full << "/" << p.chunks->size() << " chunks fill" << endl;
        }

        cout << "Total should be " << 16777216 << endl;
        cout << "Total collected tuples are " << sum << endl;
    }
    void run(vector<DataTuple> *data_tuples, const int THREADS, const int hashbits, const int PARTITIONS)
    {
        cout << "ParallelBuffer roll out " << endl;
        const int COUNT = data_tuples->size();
        const int chunk_size = 16384;
        // int count_exp = 24;
        // int part_exp = log2(PARTITIONS);
        // int chunk_exp = count_exp - part_exp;
        // int chunk_size = 1 << chunk_exp;
        const int chunks_in_part = max((COUNT / PARTITIONS) / chunk_size, 1); // 2^24 / 2^10 = 2^14 = 16384
        cout << COUNT << endl;
        cout << PARTITIONS << endl;
        cout << (COUNT / PARTITIONS) << endl;
        cout << (COUNT / PARTITIONS) / chunk_size << endl;
        cout << chunks_in_part << endl;
        printf("chunk_size = %d, partition length = %d\n", chunk_size, chunks_in_part);

        if (pthread_mutex_init(&my_lock, NULL) != 0)
        {
            printf("\n mutex init has failed\n");
            return;
        }

        pthread_t threads[THREADS];
        vector<Partition> partation(PARTITIONS);
        for (auto &&p : partation)
        {
            p = {
                .chunks = new vector<Chunk>(chunks_in_part)
            };
        };


        auto start = Utils::hp_clock::now();
        vector<vector<DataTuple>> *chunks = Utils::split_vector(*data_tuples, THREADS);
        struct Payload *args[THREADS];

        for (size_t i = 0; i < chunks->size(); i++)
        {
            struct Payload *payload = (Payload *)malloc(sizeof(struct Payload));
            payload->partation = &partation;
            payload->chunks = &(chunks->at(i));
            payload->hashbits = hashbits;
            payload->chunk_size = chunk_size;
            args[i] = payload;

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

        auto end = Utils::hp_clock::now();
        auto diff = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        cout << "INSIDE Time elapsed: " << diff << " ms" << endl;

        printBuffer(partation, chunk_size);

        // cleanup
        // for (size_t i = 0; i < THREADS; i++)
        // {
        //     free(args[i]);
        // }
    }

    void *buf_worker(void *arg)
    {
        Payload *payload = (Payload *)arg;
        unordered_map<Partition *, Chunk *> chunk_map;
        auto partition = payload->partation;
        auto chunks = payload->chunks;
        auto chunk_count = chunks->size();
        auto hash_size = sizeof(uint64_t);
        auto hash_bits = payload->hashbits;
        auto chunk_size = payload->chunk_size;
        // thread::id threadId = this_thread::get_id();
        for (size_t i = 0; i < chunk_count; i++)
        {
            auto tuple = chunks->at(i);
            u_char *hash = Utils::sha256(tuple.second, hash_size);
            long long hashIdx = Utils::hashBitsToIdx(hash, hash_bits);

            auto partation = partition->at(hashIdx);
            struct Chunk *chunk = chunk_map[&partation];
            if (chunk != nullptr && chunk->index < chunk_size)
            {
                auto tuples_in_chunk = chunk->dataTuples;
                // cout << "THREAD: " << threadId << " - INSERTING IN OUR ASSIGNED CHUNK" << endl;
                (*tuples_in_chunk)[(chunk->index)++] = tuple;

            }
            else
            {
                pthread_mutex_lock(&my_lock);
                struct Chunk *new_chunk = new struct Chunk();
                new_chunk->dataTuples = new vector<DataTuple>(chunk_size);
                (*(new_chunk->dataTuples))[0] = tuple;
                new_chunk->index = 1;

                chunk_map[&partation] = new_chunk;
                partation.chunks->push_back(*new_chunk);
                // cout << "THREAD: " << threadId << " - CHUNK IS FULL -> GETTING NEW CHUNK AT: " << partIdx << endl;
                pthread_mutex_unlock(&my_lock);
                
            }
            delete[] hash;
        }
        pthread_exit(NULL);
    }
}