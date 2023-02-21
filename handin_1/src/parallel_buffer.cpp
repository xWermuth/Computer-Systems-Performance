#include "utils.h"
#include "parallel_buffer.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <set>
#include <vector>

using namespace std;

void *worker(void *arg);

void run(vector<DataTuple> *data_tuples, const int THREADS)
{
    cout << "Autobots roll out " << endl;
    const int COUNT = data_tuples->size();
    size_t length = COUNT / THREADS;
    size_t remain = COUNT % THREADS;
    pthread_t threads[THREADS];
    vector<DataTuple> buffer(COUNT);
    vector<vector<DataTuple>> *chunks = Utils::split_vector(*data_tuples, THREADS);

    for (size_t i = 0; i < chunks->size(); i++)
    {
        size_t partition_size = i == chunks->size() - 1 ? remain : length;

        struct ParallelBuffer::Payload payload;
        payload.buffer = &buffer;
        payload.chunks = &(chunks->at(i));
        payload.start_idx = i * length;
        payload.end_idx = i * length + partition_size;

        cout << "Spawning thread" << endl;
        int rc = pthread_create(&threads[i], NULL, &worker, &payload);

        if (rc)
        {
            cout << "ERROR; return code from pthread_create() is " << rc << endl;
            break;
        }
    }

    for (size_t i = 0; i < THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    cout << "Parallel buffer finito" << endl;
}

void *worker(void *arg)
{
    struct ParallelBuffer::Payload *payload = (struct ParallelBuffer::Payload *)arg;
    for (size_t i = payload->start_idx; i < payload->end_idx; i++)
    {
        auto dataRef = payload->chunks->at(i);
        u_char *hash = Utils::sha256(dataRef.second, sizeof(uint64_t));
        (*payload->buffer)[i] = dataRef;
        delete hash;
    }

    pthread_exit(NULL);
}
