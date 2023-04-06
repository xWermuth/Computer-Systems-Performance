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
#include <climits>

using namespace std;

// #define PART_SIZE 1024 // 2^10

namespace ParallelBuffer
{
    void printBuffer(Buffers &buffer, const int chunk_size_limit)
    {
        uint32_t sum = 0;
        const auto buf_size = buffer.size();
        for (size_t i = 0; i < buf_size; i++)
        {
            // int chunks_full = 0;
            //         // cout << "Partition " << i << ", s: " << buffer->size() << endl;
            Partition *const p = buffer.at(i);
            for (auto c : *p)
            {
                // cout << "c: " << c << endl;
                // if (c != nullptr)
                // {
                const auto tuples_in_chunk = c->size();
                sum += tuples_in_chunk;
                // chunks_full += tuples_in_chunk == chunk_size_limit ? 1 : 0;
                // }
                // cout << "1" << endl;
            }
            //         // cout << "Partation's " << i << " has " << chunks_full << "/" << p.chunks->size() << " chunks fill" << endl;
        }

        Utils::print("Total should be %d\n", 16777216);
        Utils::print("Total collected tuples are %d\n", sum);
    }

    void buf_worker(const vector<DataTuple> &tuples, Buffers &buffers, vector<mutex> &mutexes, const int start, const int end, const int hash_bits, const int chunk_size)
    {
        unordered_map<Partition *, Chunk *> chunk_map;

        for (size_t i = start; i < end; i++)
        {
            auto const tuple = &tuples.at(i);

            const int hashIdx = tuple->first % buffers.capacity();
            auto const partation = buffers.at(hashIdx);
            auto const chunk = chunk_map[partation];
            const int curr_chunk_size = chunk == nullptr ? INT_MAX : chunk->size();
            if (curr_chunk_size < chunk_size)
            {
                chunk->push_back(tuple);
            }
            else
            {
                mutex &l = mutexes.at(hashIdx);
                l.lock();
                auto const new_chunk = new vector<const DataTuple *const>();
                new_chunk->push_back(tuple);
                partation->push_back(new_chunk);
                chunk_map[partation] = new_chunk;
                l.unlock();
            }
        }
    }

    void run(const vector<DataTuple> &data_tuples, const int THREADS, const int hashbits, const int PARTITIONS)
    {
        const int chunk_size = 16384;
        const int chunks_in_part = max((COUNT / PARTITIONS) / chunk_size, 1); // 2^24 / 2^10 = 2^14 = 16384
        Utils::print("chunk_size %d, partition length = %d\n", chunk_size, chunks_in_part);

        thread my_threads[THREADS];
        auto buffers = vector<Partition *const>();
        for (size_t i = 0; i < PARTITIONS; i++)
        {
            auto partition = new vector<Chunk *const>();
            buffers.push_back(partition);
        }

        auto mutexes = vector<mutex>(PARTITIONS);

        // const auto start_time = Utils::hp_clock::now();

        const int chunk = COUNT / THREADS;

        for (size_t i = 0; i < THREADS; i++)
        {
            const int start = i * chunk;
            my_threads[i] = thread(buf_worker, ref(data_tuples), ref(buffers), ref(mutexes), start, start + chunk, hashbits, chunk_size);
        }

        cout << "hello ---- " << mutexes.size() << endl;
        for (size_t i = 0; i < THREADS; i++)
        {
            cout << "j" << endl;
            my_threads[i].join();
        }

        // const auto end_time = Utils::hp_clock::now();
        // const auto diff = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

        // Utils::print("Time elapsed without initialization: %lld ms \n", diff);
        printBuffer(buffers, chunk_size);
    }
}