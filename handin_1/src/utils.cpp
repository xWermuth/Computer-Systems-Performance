#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <vector>
#include <set>
#include "utils.h"
#include <cmath>
#include <chrono>
#include <thread> // thread

using namespace std;

namespace Utils
{
    vector<DataTuple> gen_tuples(int n)
    {
        set<int> my_set;
        vector<DataTuple> my_tuples(n);

        for (size_t i = 0; i < n; i++)
        {
            uint64_t key = (((uint64_t)rand()) << 32) | rand();
            while (my_set.find(key) != my_set.end())
            {
                cout << "colission" << endl;
                key = (((uint64_t)rand()) << 32) | rand();
            }

            my_tuples[i] = make_pair(key, (((uint64_t)rand()) << 32) | rand());
        }

        return my_tuples;
    }

    int getPartations(int hashbites)
    {
        return pow(2.0, hashbites);
    }

    long long hashBitsToIdx(u_char *hash, int hashbits)
    {
        int *longerHash = (int *)hash;
        long long mask = ((1ULL << (hashbits & 0x3F)) & -(hashbits != 64)) - 1;
        return *longerHash & mask;
    }

    /// @brief spawns a thread which sleeps for \p `ms` milliseconds
    /// @param ms
    /// @return thread which exists after \p `ms` milliseconds
    thread sleep_for_x(const uint64_t ms)
    {
        return thread([=]()
        { 
            this_thread::sleep_for(chrono::milliseconds(ms)); 
        });
    }

    void print_bin_size(vector<vector<DataTuple>> &buffers)
    {
        int i = 0;
        for (auto buf : buffers)
        {
            int count = 0;
            for (auto tuple : buf)
            {
                if (tuple.first != 0)
                {
                    count++;
                }
            }
            cout << "Bin " << (i++) << " count: " << count << endl;
        }
    }

}
