#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <set>
#include <vector>
#include <tuple>
#include <cstdlib>
#include <openssl/sha.h>

using namespace std;

/******************************************* GLOBAL VARIALBES *******************************************/
#define COUNT 10
#define THREAD_COUNT 2

/******************************************* FOWARD REFRENCING *******************************************/
typedef std::pair<int, int> DataTuple;
u_char* sha256(u_char *, size_t);
vector<DataTuple> gen_tuples(int);
void concurrent_output(vector<DataTuple>);
void *partioning_worker(void *arg);
template <typename T>
vector<vector<T> > split_vector(const vector<T>& vec, size_t n);

struct WorkerPayload {
    vector<DataTuple> buffer;
    vector<DataTuple> chunks;
};


/******************************************* ACTUAL CODE *******************************************/

/// @param bytes bytes to hash
/// @param size the amount of bytes that @p `bytes` points to
/// @return 32-byte sha256 hash
u_char* sha256(u_char* bytes, size_t size)
{
    u_char *hash = (u_char*)malloc(sizeof(unsigned char) * SHA256_DIGEST_LENGTH);
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, bytes, size);
    SHA256_Final(hash, &sha256);
    stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        cout << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    cout << endl;
    return hash;
    // return ss.str();
}

int main() {
    char *str = "1234567890_1";
    u_char *hashed = sha256((u_char*)str, 12);
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        cout << hex << setw(2) << setfill('0') << (int)hashed[i];
    }
    cout << endl;

    // cout << sha256(str, 12) << endl;
    // cout << sha256("1234567890_2") << endl;
    // cout << sha256("1234567890_3") << endl;
    // cout << sha256("1234567890_4") << endl;
    vector<DataTuple> tuples = gen_tuples(COUNT);
    // concurrent_output(tuples);
    return 0;
}



vector<DataTuple> gen_tuples(int n) {
    set<int> my_set;
    vector<DataTuple> tuples(n);
    
    for (size_t i = 0; i < n; i++)
    {
        int key = rand();
        while(my_set.find(key) != my_set.end())
        {
            key = rand();
        } 

        tuples[i] = make_pair(key, rand());
    }
    
    return tuples;
}

void concurrent_output(vector<DataTuple> tuples)
{
    pthread_t threads[THREAD_COUNT];
    vector<DataTuple> buffer(COUNT);
    vector<vector<DataTuple> > chunks = split_vector(tuples, THREAD_COUNT);

    for (size_t i = 0; i < chunks.size(); i++)
    {
        struct WorkerPayload *payload = (struct WorkerPayload*)malloc(sizeof(struct ))
        int rc = pthread_create(&threads[i], NULL, &partioning_worker, &chunks[i]);

        if(rc) 
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
    vector<DataTuple>* tuples_chunk = (vector<DataTuple>*)arg;
    for (auto tuple : *tuples_chunk)
    {
        cout << "I found a tuple " << tuple.first << endl; 
    }
    
}

/**
* https://stackoverflow.com/questions/6861089/how-to-split-a-vector-into-n-almost-equal-parts
*/
template<typename T>
vector<vector<T> > split_vector(const vector<T>& vec, size_t n)
{
    vector<vector<T> > outVec;

    size_t length = vec.size() / n;
    size_t remain = vec.size() % n;

    size_t begin = 0;
    size_t end = 0;

    for (size_t i = 0; i < min(n, vec.size()); ++i)
    {
        end += (remain > 0) ? (length + !!(remain--)) : length;

        outVec.push_back(vector<T>(vec.begin() + begin, vec.begin() + end));

        begin = end;
    }

    return outVec;
}
