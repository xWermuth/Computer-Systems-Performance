#include <vector>
#include <sys/types.h>
#include <stdio.h>
#include <chrono>
#include <thread>

typedef std::pair<uint64_t, uint64_t> DataTuple;

#define COUNT 16777216  // 2^24

namespace Utils
{
    typedef std::chrono::high_resolution_clock hp_clock;
    /**
     * https://stackoverflow.com/questions/6861089/how-to-split-a-vector-into-n-almost-equal-parts
     */
    template <typename T>
    std::vector<std::vector<T> > *split_vector(const std::vector<T> &vec, size_t n)
    {
        std::vector<std::vector<T> > *outVec = new std::vector<std::vector<T> >();

        size_t length = vec.size() / n;
        size_t remain = vec.size() % n;

        size_t begin = 0;
        size_t end = 0;

        for (size_t i = 0; i < std::min(n, vec.size()); ++i)
        {
            end += (remain > 0) ? (length + !!(remain--)) : length;

            outVec->push_back(std::vector<T>(vec.begin() + begin, vec.begin() + end));

            begin = end;
        }

        return outVec;
    }

    inline bool quiet = false;
    std::vector<DataTuple> gen_tuples(int);
    int getPartations(int hashbites);
    long long hashBitsToIdx(u_char *hash, int hashbits);
    std::thread sleep_for_x(const uint64_t ms);

    template<typename... Args>
    inline void print(const char * fmt, Args... args)
    {
        if(quiet)
            return;
        printf(fmt, args...);
    }

    void print_bin_size(std::vector<std::vector<DataTuple>> &buffers);
}