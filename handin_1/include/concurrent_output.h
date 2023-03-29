#include <atomic>

namespace ConcurrentOutput
{
    void run(std::vector<DataTuple>& tuples, const int THREAD_COUNT,  const int HASH_BITS, const int PARTITIONS);
    void con_worker(std::vector<DataTuple> &tuples, std::vector<std::vector<DataTuple> > &buffers, std::vector<std::atomic_int> &aIdx, int start, int end, int hash_bits);
}
