#include <mutex>

typedef std::vector<const DataTuple *> Chunk;
typedef std::vector<Chunk *> Partition;
typedef std::vector<Partition *> Buffers;

namespace ParallelBuffer
{
    void run(const std::vector<DataTuple> &data_tuples, const int THREADS, const int hashbits, const int PARTITIONS);
    void buf_worker(const std::vector<DataTuple> &tuples, Buffers &buffers, std::vector<std::mutex> &mutexes, const int start, const int end, const int hash_bits, const int chunk_size);
}