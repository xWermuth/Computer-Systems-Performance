#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <set>
#include <vector>
#include <tuple>
#include <cstdlib>
#include "utils.h"

using namespace std;

/******************************************* FOWARD REFRENCING *******************************************/
typedef std::pair<uint64_t, uint64_t> DataTuple;
vector<DataTuple> gen_tuples(int);
void concurrent_output(vector<DataTuple>);
void *partioning_worker(void *arg);
template <typename T>
vector<vector<T> > *split_vector(const vector<T> &vec, size_t n);

struct WorkerPayload
{
    vector<DataTuple> *buffer;
    vector<DataTuple> *chunks;
};

/******************************************* GLOBAL VARIABLES *******************************************/

#define COUNT 10
#define THREAD_COUNT 2
pthread_mutex_t my_lock;

/******************************************* ACTUAL CODE *******************************************/

int main()
{
    if (pthread_mutex_init(&my_lock, NULL) != 0)
    {
        printf("\n mutex init has failed\n");
        return 1;
    }
    uint64_t x = 0x427c3c55l;
    char *str = (char *)&x;
    u_char *hashed = Utils::sha256((u_char *)str, 12);
    // for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    // {
    //     cout << hex << setw(2) << setfill('0') << (int)hashed[i];
    // }
    // cout << endl;

    // cout << sha256(str, 12) << endl;
    // cout << sha256("1234567890_2") << endl;
    // cout << sha256("1234567890_3") << endl;
    // cout << sha256("1234567890_4") << endl;
    vector<DataTuple> tuples = gen_tuples(COUNT);
    concurrent_output(tuples);
    pthread_mutex_destroy(&my_lock);
    return 0;
}

vector<DataTuple> gen_tuples(int n)
{
    set<int> my_set;
    vector<DataTuple> tuples(n);

    for (size_t i = 0; i < n; i++)
    {
        uint64_t key = rand() << 31 | rand();
        while (my_set.find(key) != my_set.end())
        {
            key = rand() << 31 | rand();
        }

        tuples[i] = make_pair(key, rand() << 31 | rand());
    }

    return tuples;
}

void concurrent_output(vector<DataTuple> tuples)
{
    pthread_t threads[THREAD_COUNT];
    vector<DataTuple> buffer(COUNT);
    vector<vector<DataTuple> > *chunks = split_vector(tuples, THREAD_COUNT);

    for (size_t i = 0; i < chunks->size(); i++)
    {
        struct WorkerPayload payload;
        payload.buffer = &buffer;
        payload.chunks = &(chunks->at(i));
        cout << "Spawning thread" << endl;
        int rc = pthread_create(&threads[i], NULL, &partioning_worker, &payload);

        if (rc)
        {
            cout << "ERROR; return code from pthread_create() is " << rc << endl;
            break;
        }
    }

    for (size_t i = 0; i < THREAD_COUNT; i++)
    {
        pthread_join(threads[i], NULL);
    }
}

void *partioning_worker(void *arg)
{
    struct WorkerPayload *payload = (struct WorkerPayload *)arg;

    for (auto tuple : *payload->chunks)
    {
        auto dataRef = &tuple;
        u_char *hash = Utils::sha256(dataRef->second, sizeof(uint64_t));
        pthread_mutex_lock(&my_lock);
        Utils::print_hash(hash);
        payload->buffer->push_back(*dataRef);
        pthread_mutex_unlock(&my_lock);
        delete hash;
    }
}

/**
 * https://stackoverflow.com/questions/6861089/how-to-split-a-vector-into-n-almost-equal-parts
 */
template <typename T>
vector<vector<T> > *split_vector(const vector<T> &vec, size_t n)
{
    vector<vector<T> > *outVec = new vector<vector<T> >();

    size_t length = vec.size() / n;
    size_t remain = vec.size() % n;

    size_t begin = 0;
    size_t end = 0;

    for (size_t i = 0; i < min(n, vec.size()); ++i)
    {
        end += (remain > 0) ? (length + !!(remain--)) : length;

        outVec->push_back(vector<T>(vec.begin() + begin, vec.begin() + end));

        begin = end;
    }

    return outVec;
}
