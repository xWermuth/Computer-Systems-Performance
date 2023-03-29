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
#include "concurrent_output.h"
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

void printBinSize(vector<vector<DataTuple> > &buffers);

/******************************************* ACTUAL CODE *******************************************/

int main(int argc, char const *argv[])
{
    int threads;
    int hashbits;
    bool perf = false;
    string algo;
    for (size_t i = 1; i < argc; i++)
    {
        string const arg = argv[i];

        if(arg == "-t")
        {
            threads = atoi(argv[i + 1]);
        } 
        else if(arg == "-h")
        {
            hashbits = atoi(argv[i + 1]);
        } 
        else if(arg == "-a")
        {
            algo = string(argv[i + 1]);
        }
        else if(arg == "-q")
        {
            Utils::quiet = true;
        }
        else if(arg == "-p")
        {
            perf = true;
        }
    }

    
    // This allows us to set an upper limit for the time it takes to generate tuples
    // in order to measure execution time with perf instead of hp_clock();
    std::thread wait_handle;
    if (perf)
    {
        uint64_t tuple_gen_perf_wait;
        #if PERF_TIMEOUT_SERVER
        tuple_gen_perf_wait = 8000;
        #else
        tuple_gen_perf_wait = 2500;
        #endif
        Utils::print("tuple_gen_perf_wait: %llu\n", tuple_gen_perf_wait);
        wait_handle = Utils::sleep_for_x(tuple_gen_perf_wait);
    }
    vector<DataTuple> tuples = Utils::gen_tuples(COUNT);
    if(perf)
    {
        wait_handle.join();
    }
    

    const int PARTITIONS = Utils::getPartations(hashbits);
    Utils::print("THREADS: %d\n",threads);
    Utils::print("HASH_BITS:  %d\n",hashbits);
    Utils::print("PARTITIONS:  %d\n",PARTITIONS);
    Utils::print("Algorithm: %s\n ",algo.c_str());
    Utils::print("TUPLE COUNT: %d\n ",COUNT);

    const auto start = Utils::hp_clock::now();
    if (algo == "parallel")
    {
        ParallelBuffer::run(tuples, threads, hashbits, PARTITIONS);
    } 
    else if(algo == "concurrent")
    {
        ConcurrentOutput::run(tuples, threads, hashbits, PARTITIONS);
    }
    auto end = Utils::hp_clock::now();
    auto diff = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    // Time result
    // Utils::print("Time elapsed: %lld ms\n", diff);
    cout << algo << "," << diff << "," << threads << "," << hashbits << endl;
    return 0;
}

