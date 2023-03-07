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
    void printBuffer(Buffers buffer, int chunk_size)
    {
        uint32_t sum = 0;
        for (size_t i = 0; i < buffer->size(); i++)
        {
            int chunks_full = 0;
            // cout << "Partition " << i << ", s: " << buffer->size() << endl;
            for (size_t j = 0; j < buffer->at(i)->size(); j++)
            {
                cout << i << "," << j << " - " << buffer->at(i)->at(j)->size() << endl;
                /* code */
            }
            
            for (auto c : *buffer->at(i))
            {
                // cout << "c: " << c << endl;
                // if (c != nullptr)
                // {
                    sum += c->size();
                    chunks_full += c->size() == chunk_size ? 1 : 0;
                // }
                // cout << "1" << endl;
            }
            // cout << "Partation's " << i << " has " << chunks_full << "/" << p.chunks->size() << " chunks fill" << endl;
        }

        Utils::print("Total should be %d\n", 16777216);
        Utils::print("Total collected tuples are %d\n", sum);
    }

    void buf_worker(vector<DataTuple> & tuples, Buffers buffers, std::vector<std::mutex> *mutexes, int start, int end, int hash_bits, int chunk_size)
    {
        unordered_map<vector<Chunk>*, vector<DataTuple>*> chunk_map;
        auto hash_size = sizeof(uint64_t);
        uint32_t count = 0;
        for (size_t i = start; i < end; i++)
        {
            count++;
            auto tuple = tuples[i];

            int hashIdx = tuple.second % buffers->capacity();
            vector<Chunk>* partation = buffers->at(hashIdx);
            vector<DataTuple>* chunk = chunk_map[partation];
            int curr_chunk_size = chunk == nullptr ? INT_MAX : chunk->size();
            if (curr_chunk_size < chunk_size)
            {
                chunk->push_back(tuple);
            }
            else
            {
                // (*mutexes)[hashIdx].lock();
                mutex *l = &(*mutexes)[hashIdx];
                l->lock();
                vector<DataTuple>* new_chunk = new vector<DataTuple>();
                new_chunk->push_back(tuple);
                partation->push_back(new_chunk);
                chunk_map[partation] = new_chunk;
                l->unlock();
            }
        }
        Utils::print("this count: %u\n", count);
    }

    void run(vector<DataTuple> &data_tuples, const int THREADS, const int hashbits, const int PARTITIONS)
    {
        const int COUNT = data_tuples.size();
        const int chunk_size = 16384;
        const int chunks_in_part = max((COUNT / PARTITIONS) / chunk_size, 1); // 2^24 / 2^10 = 2^14 = 16384
        Utils::print("chunk_size %d, partition length = %d\n", chunk_size, chunks_in_part);

        thread my_threads[THREADS];
        Buffers buffers = new vector<Partition>();
        for (size_t i = 0; i < PARTITIONS; i++)
        {
            buffers->push_back(new vector<Chunk>());
        }

        vector<mutex> *mutexes = new vector<mutex>(PARTITIONS);

        auto start = Utils::hp_clock::now();

        size_t chunk = COUNT / THREADS;

        for (size_t i = 0; i < THREADS; i++)
        {
            int start = i * chunk;
            my_threads[i] = thread(buf_worker, ref(data_tuples), buffers, mutexes, start, start + chunk, hashbits, chunk_size);
        }

        for (size_t i = 0; i < THREADS; i++)
        {
            my_threads[i].join();
        }

        auto end = Utils::hp_clock::now();
        auto diff = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        Utils::print("Time elapsed without initialization: %lld ms \n", diff);
        printBuffer(buffers, chunk_size);
    }
}