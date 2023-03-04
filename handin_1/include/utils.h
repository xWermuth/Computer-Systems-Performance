#include <vector>
#include <sys/types.h>
#include <stdio.h>

typedef std::pair<uint64_t, uint64_t> DataTuple;

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
    u_char *_sha256(u_char *bytes, size_t size);
    u_char *sha256(u_char *bytes, size_t size);
    u_char *sha256(uint64_t bytes, size_t size);
    int getPartations(int hashbites);
    void print_hash(u_char *hash);
    long long hashBitsToIdx(u_char *hash, int hashbits);
    
    template<typename... Args> 
    inline void print(const char * fmt, Args... args)
    {
        if(quiet)
            return;
        printf(fmt, args...);
    }
}