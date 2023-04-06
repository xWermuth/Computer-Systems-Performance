#include <mutex>

typedef std::vector<const DataTuple *const> Chunk;
typedef std::vector<Chunk *const> Partition;
typedef std::vector<Partition *const> Buffers;

namespace ParallelBuffer
{
    void run(std::vector<DataTuple> &data_tuples, const int THREADS, const int hashbits, const int PARTITIONS);
    void buf_worker(std::vector<DataTuple> &tuples, Buffers &buffers, std::vector<std::mutex> &mutexes, const int start, const int end, const int hash_bits, const int chunk_size);
}