#include <iostream>
#include <cstdlib>
#include <openssl/sha.h>
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
    /// @param bytes bytes to hash
    /// @param size the amount of bytes that @p `bytes` points to
    /// @return 32-byte sha256 hash
    u_char *sha256(u_char *bytes, size_t size)
    {
        return _sha256(bytes, size);
    }

    /// @param bytes bytes to hash
    /// @param size the amount of bytes that @p `bytes` points to
    /// @return 32-byte sha256 hash
    u_char *sha256(uint64_t bytes, size_t size)
    {
        return _sha256((u_char *)&(bytes), size);
    }

    /// from https://stackoverflow.com/a/10632725
    /// @param bytes bytes to hash
    /// @param size the amount of bytes that @p `bytes` points to
    /// @return 32-byte sha256 hash
    u_char *_sha256(u_char *bytes, size_t size)
    {
        u_char *hash = (u_char *)malloc(sizeof(unsigned char) * SHA256_DIGEST_LENGTH);
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, bytes, size);
        SHA256_Final(hash, &sha256);
        return hash;
    }

    void print_hash(u_char *hash)
    {
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        {
            cout << hex << setw(2) << setfill('0') << (int)hash[i];
        }
        cout << endl;
    }

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
