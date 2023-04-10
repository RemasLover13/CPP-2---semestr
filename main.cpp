#include <iostream>
#include <chrono>
#include <vector>
#include <fstream>
#include <thread>

void writeToFileMultiThread(const char *filename, const char *data, size_t dataSize) {
    std::ofstream outFile(filename, std::ios::binary);
    const size_t blockSize = 0x1000;
    const size_t blockCount = dataSize / blockSize;
    const size_t lastBlockSize = dataSize % blockSize;

    std::vector<std::thread> threads;
    for (size_t i = 0; i < blockCount; ++i) {
        threads.emplace_back([&, i]() {
            const size_t offset = i * blockSize;
            outFile.seekp(offset, std::ios::beg);
            outFile.write(data + offset, blockSize);
        });
    }

    if (lastBlockSize > 0) {
        threads.emplace_back([&, blockCount]() {
            const size_t offset = blockCount * blockSize;
            outFile.seekp(offset, std::ios::beg);
            outFile.write(data + offset, lastBlockSize);
        });
    }

    for (auto &thread: threads) {
        thread.join();
    }

    outFile.close();
}

void writeToFileSync(const char *filename, const char *data, size_t dataSize) {
    std::ofstream outFile(filename, std::ios::binary);
    outFile.write(data, dataSize);
    outFile.close();
}


int main() {
    const char *filename = "test.bin";
    const size_t fileSize = 5ULL * 1024 * 1024 * 1024; // 5 GB
    std::vector<char> data(fileSize);


    auto startTime = std::chrono::high_resolution_clock::now();
    writeToFileSync(filename, data.data(), data.size());
    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << "Sync time: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()
              << " ms\n";

    startTime = std::chrono::high_resolution_clock::now();
    writeToFileMultiThread(filename, data.data(), data.size());
    endTime = std::chrono::high_resolution_clock::now();
    std::cout << "Multi-thread time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << " ms\n";

    return 0;
}
