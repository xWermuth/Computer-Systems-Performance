namespace Utils 
{
    u_char *_sha256(u_char *bytes, size_t size);
    u_char *sha256(u_char *bytes, size_t size);
    u_char *sha256(uint64_t bytes, size_t size);
    void print_hash(u_char *hash);
}