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
    void run(vector<DataTuple> &tuples, const int THREAD_COUNT, const int HASH_BITS, const int PARTITIONS)
    {
        thread threads[THREAD_COUNT];
        vector<vector<DataTuple>> buffers(PARTITIONS, vector<DataTuple>((COUNT / PARTITIONS) * 1.1));
        vector<atomic<int>> aIdx(PARTITIONS);

        auto start = Utils::hp_clock::now();
        size_t chunk = tuples.size() / THREAD_COUNT;

        for (size_t i = 0; i < THREAD_COUNT; i++)
        {
            int start = i * chunk;
            threads[i] = thread(con_worker, ref(tuples), ref(buffers), ref(aIdx), start, start + chunk, HASH_BITS);
        }

        for (size_t i = 0; i < THREAD_COUNT; i++)
        {
            threads[i].join();
        }

        auto end = Utils::hp_clock::now();
        auto diff = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        Utils::print("Time elapsed without initialization: %lld ms\n", diff);
    }
    void con_worker(vector<DataTuple> &tuples, vector<vector<DataTuple>> &buffers, vector<atomic_int> &aIdx, int start, int end, int hash_bits)
    {
        for (int i = start; i < end; i++)
        {
            auto tuple = tuples[i];
            int hashIdx = tuple.first % buffers.capacity();
            int newIdx = aIdx[hashIdx].fetch_add(1);
            buffers[hashIdx][newIdx] = tuple;
        }
    }
}
