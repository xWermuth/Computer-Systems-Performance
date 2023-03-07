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
#include <mutex>

using namespace std;


// #define PART_SIZE 1024 // 2^10


namespace ParallelBuffer
{
    void printBuffer(Buffers &buffer, int chunk_size)
    {
        uint32_t sum = 0;
        for (size_t i = 0; i < buffer.size(); i++)
        {
            int chunks_full = 0;
            cout << "Partition " << i << endl;
            for (auto c : buffer[i])
            {
                // if (c != nullptr)
                // {
                    sum += c.size();
                    chunks_full += c.size() == chunk_size ? 1 : 0;
                // }
            }
            // cout << "Partation's " << i << " has " << chunks_full << "/" << p.chunks->size() << " chunks fill" << endl;
        }

        Utils::print("Total should be %d\n", 16777216);
        Utils::print("Total collected tuples are %d\n", sum);
    }

    void buf_worker(vector<DataTuple> &tuples, Buffers &buffers, vector<mutex> *mutexes, int start, int end, int hash_bits, int chunk_size)
    {
        unordered_map<Partition *, Chunk *> chunk_map;
        auto hash_size = sizeof(uint64_t);
        for (size_t i = start; i < end; i++)
        {
            auto tuple = tuples[i];
            u_char *hash = Utils::sha256(tuple.second, hash_size);
            long long hashIdx = Utils::hashBitsToIdx(hash, hash_bits);

            Partition partation = buffers[hashIdx];
            Chunk *chunk = chunk_map[&partation];
            if (chunk != nullptr && chunk->size() < chunk_size)
            {
                // cout << "THREAD: " << threadId << " - INSERTING IN OUR ASSIGNED CHUNK" << endl;
                (*chunk)[chunk->size() + 1] = tuple;

            }
            else
            {
                (*mutexes)[hashIdx].lock();
                Chunk new_chunk(chunk_size);
                new_chunk[0] = tuple;
                chunk_map[&partation] = &new_chunk;
                partation.push_back(new_chunk);
                // cout << "THREAD: " << threadId << " - CHUNK IS FULL -> GETTING NEW CHUNK AT: " << partIdx << endl;
                (*mutexes)[hashIdx].unlock();
                
            }
            delete[] hash;
        }
    }

    void run(vector<DataTuple> &data_tuples, const int THREADS, const int hashbits, const int PARTITIONS)
    {
        const int COUNT = data_tuples.size();
        const int chunk_size = 16384;
        const int chunks_in_part = max((COUNT / PARTITIONS) / chunk_size, 1); // 2^24 / 2^10 = 2^14 = 16384
        Utils::print("chunk_size %d, partition length = %d\n", chunk_size, chunks_in_part);

        thread my_threads[THREADS];
        Buffers buffers(PARTITIONS, Partition(chunks_in_part));
        vector<mutex> *mutexes = new vector<mutex>(PARTITIONS);


        auto start = Utils::hp_clock::now();

        size_t chunk = COUNT / THREADS;

        for (size_t i = 0; i < THREADS; i++)
        {
            int start = i * chunk;
            my_threads[i] = thread(buf_worker, ref(data_tuples), ref(buffers), mutexes, start, start + chunk, hashbits, chunk_size);
        }

        for (size_t i = 0; i < THREADS; i++)
        {
            my_threads[i].join();
        }

        auto end = Utils::hp_clock::now();
        auto diff = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        Utils::print("Time elapsed without initialization: %lld ms \n", diff);
        // printBuffer(buffers, chunk_size);
    }
}