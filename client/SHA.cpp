#include"peer_header.h"

string sha1HashToString(const char* hash) {
    char sha1Buffer[2 * SHA_DIGEST_LENGTH + 1];
    sha1Buffer[2 * SHA_DIGEST_LENGTH] = 0; // Null-terminate the buffer

    for (int i = 0; i < SHA_DIGEST_LENGTH; ++i) {
        sprintf(&sha1Buffer[2 * i], "%02x", static_cast<unsigned char>(hash[i]));
    }

    return std::string(sha1Buffer);
}

string fileSHA(string filepath) {
    // std::string filename = "your_file.txt"; // Replace with your file's path
    
    int fileDescriptor = open(filepath.c_str(), O_RDONLY);

    if (fileDescriptor == -1) {
        // std::cerr << "Error opening the file." << std::endl;
        throw std::runtime_error("Error opening file");
    }

    SHA_CTX shaContext;
    SHA1_Init(&shaContext);

    char buffer[8192];
    ssize_t bytesRead;

    while ((bytesRead = read(fileDescriptor, buffer, sizeof(buffer))) > 0) {
        SHA1_Update(&shaContext, buffer, bytesRead);
    }

    char sha1Hash[SHA_DIGEST_LENGTH];
    SHA1_Final(reinterpret_cast<unsigned char*>(sha1Hash), &shaContext);

    close(fileDescriptor);

    std::string sha1Hex = sha1HashToString(sha1Hash);
    std::cout << "SHA-1 Hash: " << sha1Hex << std::endl;

    return sha1Hex;
}

string calculateChunkSHA1(const char* buffer, size_t length) {
    unsigned char sha1Hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(buffer), length, sha1Hash);

    char sha1Hex[2 * SHA_DIGEST_LENGTH + 1];
    sha1Hex[2 * SHA_DIGEST_LENGTH] = 0;

    for (int i = 0; i < SHA_DIGEST_LENGTH; ++i) {
        sprintf(&sha1Hex[2 * i], "%02x", sha1Hash[i]);
    }

    return string(sha1Hex);
}