#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <set>
#include <vector>
#include <tuple>
#include <cstdlib>
#include "utils.h"
#include "parallel_buffer.h"
#include <atomic>
#include <thread>

using namespace std;

/******************************************* FOWARD REFRENCING *******************************************/

struct Buffer
{
    vector<DataTuple> *tuples;
    atomic<int> *idx; // Current writing index
};

struct WorkerPayload
{
    vector<Buffer> *buffer;
    vector<DataTuple> *tuples;
    int start;
    int end;
    int hash_bits;
};

void concurrent_output(vector<DataTuple> tuples, const int THREAD_COUNT, const int HASH_BITS, const int PARTITIONS);
void partioning_worker(vector<DataTuple> &tuples, vector<Buffer> &buffers, int start, int end, int hash_bits);
void printBinSize(vector<Buffer> buffers);

/******************************************* GLOBAL VARIABLES *******************************************/

#define COUNT 16777216  // 2^24
// #define THREAD_COUNT 1 // 2 x AMD Opteron(tm) Processor 6386 SE
// #define HASH_BITS 18
/******************************************* ACTUAL CODE *******************************************/

int main(int argc, char const *argv[])
{
    // ./handin_1 -t 4 -h 8 -a parallel
    int threads;
    int hashbits;
    string algo;
    for (size_t i = 1; i < argc; i++)
    {
        string const arg = argv[i];

        if(arg == "-t")
        {
            threads = atoi(argv[i + 1]);
        } else if(arg == "-h")
        {
            hashbits = atoi(argv[i + 1]);
        } else if(arg == "-a")
        {
            algo = string(argv[i + 1]);
        }
        else if(arg == "-q")
        {
            Utils::quiet = true;
        }
    }


    vector<DataTuple> tuples = Utils::gen_tuples(COUNT);
    const int PARTITIONS = Utils::getPartations(hashbits);
    Utils::print("THREADS: %d\n",threads);
    Utils::print("HASH_BITS:  %d\n",hashbits);
    Utils::print("PARTITIONS:  %d\n",PARTITIONS);
    Utils::print("Algorithm: %s\n ",algo.c_str());
    Utils::print("TUPLE COUNT: %d\n ",COUNT);

    auto start = Utils::hp_clock::now();
    if (algo == "parallel")
    {
        ParallelBuffer::run(&tuples, threads, hashbits, PARTITIONS);
    } 
    else if(algo == "concurrent")
    {
        concurrent_output(tuples, threads, hashbits, PARTITIONS);
    }
    auto end = Utils::hp_clock::now();
    auto diff = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    // Time result
    // Utils::print("Time elapsed: %lld ms\n", diff);
    cout << algo << "," << diff << "," << threads << "," << hashbits << endl;
    return 0;
}

void concurrent_output(vector<DataTuple> tuples, const int THREAD_COUNT, const int HASH_BITS, const int PARTITIONS)
{
    thread threads[THREAD_COUNT];
    vector<Buffer> buffers(PARTITIONS);

    // Init our buffer
    for (int i = 0; i < PARTITIONS; i++)
    {
        struct Buffer buffer;
        buffer.tuples = new vector<DataTuple>((COUNT / PARTITIONS) * 1.2);
        buffer.idx = new atomic<int>{0};
        buffers[i] = buffer;
    }

    auto start = Utils::hp_clock::now();
    size_t chunk = tuples.size() / THREAD_COUNT;

    for (size_t i = 0; i < THREAD_COUNT; i++)
    {
        int start = i * chunk;
        threads[i] = thread(partioning_worker, ref(tuples), ref(buffers), start, start + chunk, HASH_BITS);
    }

    for (size_t i = 0; i < THREAD_COUNT; i++)
    {
        threads[i].join();
    }
    
    auto end = Utils::hp_clock::now();        
    auto diff = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    Utils::print("Time elapsed without initialization: %lld ms\n", diff);

    // printBinSize(buffers);
}

void partioning_worker(vector<DataTuple> &tuples, vector<Buffer> &buffers, int start, int end, int hash_bits)
{
    for(int i = start; i < end; i++)
    {
        auto tuple = tuples[i];
        u_char *hash = Utils::sha256(tuple.second, sizeof(uint64_t));
        long long idx = Utils::hashBitsToIdx(hash, hash_bits);
        Buffer buffer = buffers.at(idx);
        int newIdx = buffer.idx->fetch_add(1);
        (*buffer.tuples)[newIdx] = tuple;

        // Cleanup
        // delete hash;
    }
}

void printBinSize(vector<Buffer> buffers)
{
    int i = 0;
    for (auto buf : buffers)
    {
        int count = 0;
        for (auto tuple : *(buf.tuples))
        {
            if (tuple.first != 0)
            {
                count++;
            }
        }
        cout << "Bin " << (i++) << " count: " << count << endl;
    }
}
