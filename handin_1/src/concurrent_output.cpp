#include "utils.h"
#include "concurrent_output.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <set>
#include <vector>
#include <tuple>
#include <cstdlib>
#include <atomic>
#include <thread>

using namespace std;

namespace ConcurrentOutput
{
    void run(const vector<DataTuple> &tuples, const int THREAD_COUNT, const int HASH_BITS, const int PARTITIONS)
    {
        thread threads[THREAD_COUNT];
        vector<vector<DataTuple>> buffers(PARTITIONS, vector<DataTuple>((COUNT / PARTITIONS) * 1.1));
        vector<atomic<int>> aIdx(PARTITIONS);

        // const auto start = Utils::hp_clock::now();
        const auto chunk = tuples.size() / THREAD_COUNT;

        for (size_t i = 0; i < THREAD_COUNT; i++)
        {
            const int start = i * chunk;
            threads[i] = thread(con_worker, ref(tuples), ref(buffers), ref(aIdx), start, start + chunk, HASH_BITS);
        }

        for (size_t i = 0; i < THREAD_COUNT; i++)
        {
            threads[i].join();
        }

        // const auto end = Utils::hp_clock::now();
        // const auto diff = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        // Utils::print("Time elapsed without initialization: %lld ms\n", diff);
    }
    void con_worker(const vector<DataTuple> &tuples, vector<vector<DataTuple>> &buffers, vector<atomic_int> &aIdx, const int start, const int end, const int hash_bits)
    {
        const int buffer_cap = buffers.capacity();
        for (int i = start; i < end; i++)
        {
            const auto tuple = tuples.at(i);
            const int hashIdx = tuple.first % buffer_cap;
            const int newIdx = aIdx[hashIdx].fetch_add(1);
            buffers[hashIdx][newIdx] = tuple;
        }
    }
}
