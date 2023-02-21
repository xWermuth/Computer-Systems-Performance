#include <iostream>
#include <cstdlib>
#include <openssl/sha.h>
#include <iomanip>
#include "utils.h"

namespace Utils
{

    /// @param bytes bytes to hash
    /// @param size the amount of bytes that @p `bytes` points to
    /// @return 32-byte sha256 hash
    u_char *sha256(u_char *bytes, size_t size)
    {
        return _sha256(bytes, size);
    }

    /// @param bytes bytes to hash
    /// @param size the amount of bytes that @p `bytes` points to
    /// @return 32-byte sha256 hash
    u_char *sha256(uint64_t bytes, size_t size)
    {
        return _sha256((u_char *)&(bytes), size);
    }

    /// from https://stackoverflow.com/a/10632725
    /// @param bytes bytes to hash
    /// @param size the amount of bytes that @p `bytes` points to
    /// @return 32-byte sha256 hash
    u_char *_sha256(u_char *bytes, size_t size)
    {
        u_char *hash = (u_char *)malloc(sizeof(unsigned char) * SHA256_DIGEST_LENGTH);
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, bytes, size);
        SHA256_Final(hash, &sha256);
        return hash;
    }

    void print_hash(u_char *hash)
    {
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
        }
        std::cout << std::endl;
    }
}