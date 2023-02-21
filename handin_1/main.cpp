#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;

#include <openssl/sha.h>
string sha256(const string str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}

int main() {
    cout << sha256("1234567890_1") << endl;
    cout << sha256("1234567890_2") << endl;
    cout << sha256("1234567890_3") << endl;
    cout << sha256("1234567890_4") << endl;
    return 0;
}


// #include <openssl/sha.h>
// #include <vector>
// #include <iostream>

// using namespace std;


// // typedef std::tuple<int, int> DataTuple;

// // std::vector<DataTuple> gen_tuples() {

// // }

// void sha256_hash_string (unsigned char hash[SHA256_DIGEST_LENGTH], char outputBuffer[65])
// {
//     int i = 0;

//     for(i = 0; i < SHA256_DIGEST_LENGTH; i++)
//     {
//         sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
//     }

//     outputBuffer[64] = 0;
// }


// void sha256_string(char *string, char outputBuffer[65])
// {
//     unsigned char hash[SHA256_DIGEST_LENGTH];
//     SHA256_CTX sha256;
//     SHA256_Init(&sha256);
//     SHA256_Update(&sha256, string, strlen(string));
//     SHA256_Final(hash, &sha256);
//     int i = 0;
//     for(i = 0; i < SHA256_DIGEST_LENGTH; i++)
//     {
//         sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
//     }
//     outputBuffer[64] = 0;
// }

// int sha256_file(char *path, char outputBuffer[65])
// {
//     FILE *file = fopen(path, "rb");
//     if(!file) return -534;

//     unsigned char hash[SHA256_DIGEST_LENGTH];
//     SHA256_CTX sha256;
//     SHA256_Init(&sha256);
//     const int bufSize = 32768;
//     unsigned char *buffer = malloc(bufSize);
//     int bytesRead = 0;
//     if(!buffer) return ENOMEM;
//     while((bytesRead = fread(buffer, 1, bufSize, file)))
//     {
//         SHA256_Update(&sha256, buffer, bytesRead);
//     }
//     SHA256_Final(hash, &sha256);

//     sha256_hash_string(hash, outputBuffer);
//     fclose(file);
//     free(buffer);
//     return 0;
// }


// int main(){
//     cout << "HELLO WORLD " << endl;
//     static unsigned char buffer[65];
//     sha256_string("string", buffer);
//     printf("%s\n", buffer);
//     return 0;
// }

// void concurrent_output(){

// }

// void partioning_task(){}