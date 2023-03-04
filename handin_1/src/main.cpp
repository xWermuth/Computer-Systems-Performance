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
    vector<DataTuple> *chunks;
};

void concurrent_output(vector<DataTuple>);
void *partioning_worker(void *arg);
void printBinSize(vector<Buffer> buffers);

/******************************************* GLOBAL VARIABLES *******************************************/

#define COUNT 16777216  // 2^24
#define THREAD_COUNT 1 // 2 x AMD Opteron(tm) Processor 6386 SE
#define HASH_BITS 18
typedef std::chrono::high_resolution_clock hp_clock;
/******************************************* ACTUAL CODE *******************************************/

int main(int argc, char const *argv[])
{
    cout << "THREADS: " << THREAD_COUNT << endl;
    cout << "HASH_BITS: " << HASH_BITS << endl;
    cout << "TUPLE COUNT: " << COUNT << endl;

    // ./handin_1 -t 4 -h 8 -a parallel
    int threads;
    int hashbits;
    string algo;
    for (size_t i = 1; i < argc; i++)
    {
        string const arg = argv[i];
        switch (arg)
        {
        case "-t":
            threads = 1;
            break;

        case "-h":
            hashbits = 1;
            break;

        case "-a":
            algo = "";
            break;
        default:
            break;
        }
    }
    

    /******************************************* CON BUFFER *******************************************/

    // vector<DataTuple> tuples = Utils::gen_tuples(COUNT);
    // auto a = Utils::hashBitsToIdx(hashed, 10);
    // cout << "KEY: " << a << endl;
    // concurrent_output(tuples);

    /******************************************* PAR BUFFER *******************************************/
    vector<DataTuple> tuples = Utils::gen_tuples(COUNT);
    ParallelBuffer::run(&tuples, THREAD_COUNT, HASH_BITS);

    cout << "Life is a highway" << endl;
    return 0;
}

void concurrent_output(vector<DataTuple> tuples)
{
    int partetions = Utils::getPartations(HASH_BITS);
    cout << "partetions: " << partetions << endl;
    pthread_t threads[THREAD_COUNT];
    vector<Buffer> buffers(partetions);

    // Init our buffer
    for (int i = 0; i < partetions; i++)
    {
        struct Buffer buffer;
        buffer.tuples = new vector<DataTuple>(COUNT / 4);
        buffer.idx = new atomic<int>{0};
        buffers[i] = buffer;
    }

    vector<vector<DataTuple>> *chunks = Utils::split_vector(tuples, THREAD_COUNT);

    for (size_t i = 0; i < chunks->size(); i++)
    {
        struct WorkerPayload *payload = (WorkerPayload *)malloc(sizeof(struct WorkerPayload));
        payload->buffer = &buffers;
        payload->chunks = &(chunks->at(i));
        cout << "Spawning thread: " << endl;
        int rc = pthread_create(&threads[i], NULL, partioning_worker, payload);

        if (rc)
        {
            cout << "ERROR; return code from pthread_create() is " << rc << endl;
            break;
        }
    }

    for (size_t i = 0; i < THREAD_COUNT; i++)
    {
#ifdef METRICS
        void *ret;
        pthread_join(threads[i], &ret);
#else
        pthread_join(threads[i], NULL);
#endif
    }

    printBinSize(buffers);
}

void *partioning_worker(void *arg)
{
    WorkerPayload *payload = (WorkerPayload *)arg;

    for (auto tuple : *payload->chunks)
    {
        auto dataRef = &tuple;
        u_char *hash = Utils::sha256(dataRef->second, sizeof(uint64_t));
        // Utils::print_hash(hash);
        // Compute hash bits as index
        long long idx = Utils::hashBitsToIdx(hash, HASH_BITS);
        // cout << "HASH idx: " << idx << endl;
        Buffer buffer = (payload->buffer)->at(idx);
        int newIdx = buffer.idx->fetch_add(1);
        (*buffer.tuples)[newIdx] = tuple;

        // Cleanup
        delete hash;
    }
    pthread_exit(NULL);
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
