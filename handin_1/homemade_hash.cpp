// #include <iostream>
// #include <cstdlib>

// using namespace std;

// uint64_t q_hash(char *);
// void run(uint64_t);

// int main(int argc, char const *argv[])
// {
//     for (size_t i = 0; i < 100; i++)
//     {
//         run((rand() << 31) | rand());
//     }

//     return 0;
// }

// void run(uint64_t value)
// {
//     printf("%016llx\n", q_hash((char *) &value));
// }

// uint64_t q_hash(char* input)
// {
//     // printf("%d\n", sizeof(input));
//     // printf("%llx\n", *input);
//     const int size = 64;
//     uint64_t hash = (0x3ee6f1faff5533 * sizeof(input) | 0x555555555555) & 0xfeeddadbadbeef19;
//     while (*input)
//     {
//         hash ^= *input++;
//         hash = ((hash << (hash & 0x27)) | (hash >> (size - (hash & 0x13))));
//         // printf("%llx\n", hash << 7);
//         // printf("%llx\n", hash >> (size - 7));
//     }

//     return hash;
// }