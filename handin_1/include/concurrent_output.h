#include <atomic>

namespace ConcurrentOutput
{
    void run(const std::vector<DataTuple> &tuples, const int THREAD_COUNT, const int HASH_BITS, const int PARTITIONS);
    void con_worker(const std::vector< DataTuple> &tuples, std::vector<std::vector<DataTuple>> &buffers, std::vector<std::atomic_int> &aIdx, const int start, const int end, const int hash_bits);
}
