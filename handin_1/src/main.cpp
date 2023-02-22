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
void concurrent_output(vector<DataTuple>);
void *partioning_worker(void *arg);

struct WorkerPayload
{
    vector<DataTuple> *buffer;
    vector<DataTuple> *chunks;
};

/******************************************* GLOBAL VARIABLES *******************************************/

#define COUNT 16777216 // 2^24
#define THREAD_COUNT 32 // 2 x AMD Opteron(tm) Processor 6386 SE 
pthread_mutex_t my_lock;
typedef std::chrono::high_resolution_clock hp_clock;
/******************************************* ACTUAL CODE *******************************************/

int main(int argc, char const *argv[])
{
    uint64_t x = 0x427c3c55l;
    char *str = (char *)&x;
    u_char *hashed = Utils::sha256((u_char *)str, 12);

    /******************************************* CON BUFFER *******************************************/

    // if (pthread_mutex_init(&my_lock, NULL) != 0)
    // {
    //     printf("\n mutex init has failed\n");
    //     return 1;
    // }
    // vector<DataTuple> tuples = Utils::gen_tuples(COUNT);
    // concurrent_output(tuples);
    pthread_mutex_destroy(&my_lock);

    /******************************************* PAR BUFFER *******************************************/
    vector<DataTuple> tuples = Utils::gen_tuples(10);
    ParallelBuffer::run(&tuples, 2);

    cout << "Life is a highway" << endl;
    return 0;
}

void concurrent_output(vector<DataTuple> tuples)
{
    pthread_t threads[THREAD_COUNT];
    vector<DataTuple> buffer(COUNT);
    // vector<vector<DataTuple>> *chunks = Utils::split_vector(tuples, THREAD_COUNT);

    // for (size_t i = 0; i < chunks->size(); i++)
    // {
    //     struct WorkerPayload payload;
    //     payload.buffer = &buffer;
    //     payload.chunks = &(chunks->at(i));
    //     cout << "Spawning thread" << endl;
    //     int rc = pthread_create(&threads[i], NULL, &partioning_worker, &payload);

    //     if (rc)
    //     {
    //         cout << "ERROR; return code from pthread_create() is " << rc << endl;
    //         break;
    //     }
    // }

    // for (size_t i = 0; i < THREAD_COUNT; i++)
    // {
    //     #ifdef METRICS
    //     void *ret;
    //     pthread_join(threads[i], &ret);
    //     #else
    //     pthread_join(threads[i], NULL);
    //     #endif
    // }

}

void *partioning_worker(void *arg)
{
    struct WorkerPayload *payload = (struct WorkerPayload *)arg;

    for (auto tuple : *payload->chunks)
    {
        auto dataRef = &tuple;
        u_char *hash = Utils::sha256(dataRef->second, sizeof(uint64_t));
        pthread_mutex_lock(&my_lock);
        // Utils::print_hash(hash);
        payload->buffer->push_back(*dataRef);
        pthread_mutex_unlock(&my_lock);
        delete hash;
    }
    pthread_exit(NULL);
}
