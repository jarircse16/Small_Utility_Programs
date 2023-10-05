#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

using namespace std;

void createRandomFile(const string& fileName, size_t fileSize) {
    ofstream file(fileName.c_str(), ios::out | ios::binary);
    if (!file) {
        cerr << "Error creating file: " << fileName << endl;
        exit(1);
    }

    const size_t bufferSize = 1024; // Buffer size for writing data
    char buffer[bufferSize];

    for (size_t i = 0; i < bufferSize; ++i) {
        buffer[i] = rand() % 256; // Fill buffer with random data
    }

    for (size_t i = 0; i < fileSize; i += bufferSize) {
        size_t bytesToWrite = min(bufferSize, fileSize - i);
        file.write(buffer, bytesToWrite);
    }

    file.close();
}

int main() {
    string fileName = "random_data.dat"; // File name
    size_t freeSpaceSizeMB;

    cout << "Enter the size of free space in megabytes (MB): ";
    cin >> freeSpaceSizeMB;

    size_t fileSize = freeSpaceSizeMB * 1024 * 1024; // Convert MB to bytes

    createRandomFile(fileName, fileSize);

    // Securely delete the file (optional)
    remove(fileName.c_str());

    cout << "Free space creation complete." << endl;

    return 0;
}
