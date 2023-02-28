#include "utils.h"
#include "parallel_buffer.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <set>
#include <vector>

using namespace std;

struct Payload
{
    size_t start_idx;
    vector<DataTuple> *buffer;
    vector<DataTuple> *chunks;
};

namespace ParallelBuffer
{
    void run(vector<DataTuple> *data_tuples, int THREADS, int hashbits)
    {
        cout << "ParallelBuffer roll out " << endl;
        const int COUNT = data_tuples->size();
        size_t length = COUNT / THREADS;
        size_t remain = COUNT % THREADS;
        pthread_t threads[THREADS];
        struct Payload *args[THREADS];
        vector<DataTuple> buffer(COUNT);
        vector<vector<DataTuple>> *chunks = Utils::split_vector(*data_tuples, THREADS);

        for (size_t i = 0; i < chunks->size(); i++)
        {
            size_t partition_size = i == chunks->size() - 1 ? remain : length;

            struct Payload *payload = (Payload *) malloc(sizeof(struct Payload));
            payload->buffer = &buffer;
            payload->chunks = &(chunks->at(i));
            payload->start_idx = i * length;
            args[i] = payload;

            cout << "Spawning thread" << endl;
            int rc = pthread_create(&threads[i], NULL, buf_worker, payload);

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

        // cleanup
        for (size_t i = 0; i < THREADS; i++)
        {
            free(args[i]);
        }

        cout << "Parallel buffer finito" << endl;
    }

    void *buf_worker(void *arg)
    {
        Payload *payload = (Payload *)arg;
        for (size_t i = 0; i < payload->chunks->size(); i++)
        {
            auto tuple = payload->chunks->at(i);
            u_char *hash = Utils::sha256(tuple.second, sizeof(uint64_t));
            (*payload->buffer)[i + payload->start_idx] = tuple;
            delete hash;
        }

        pthread_exit(NULL);
    }

}