#include <mutex>

typedef std::vector<DataTuple>* Chunk;
typedef std::vector<Chunk>* Partition;
typedef std::vector<Partition>* Buffers;

namespace ParallelBuffer
{
    void run(std::vector<DataTuple> &data_tuples, const int THREADS, const int hashbits, const int PARTITIONS);
    void buf_worker(std::vector<DataTuple> & tuples, Buffers buffers, std::vector<std::mutex> *mutexes, int start, int end, int hash_bits, int chunk_size);
}