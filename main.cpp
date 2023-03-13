#include <iostream>
#include <exception>
#include <string>
#include <vector>
#include <cstring> // std::memcmp
#include <fstream> // std::ifstream

using namespace std;

class VersionException : public exception {
public:


    VersionException(const char* msg): msg(msg) {};
    int getVersion() {
        return version;
    }

private:
    std::string msg;
    int version;
};

class DataException : public exception {
public:
    DataException(exception msg) : exception(msg) {

    }

    DataException(const char* msg): msg(msg) {};
    string getData() {
        return data;
    }

private:
    string data;
    std::string msg;
};

class SizeException : public exception {
public:
    SizeException(exception msg) : exception(msg) {

    }
    SizeException(const char* msg): msg(msg) {};

    int getSize() {
        return size;
    }

private:
    int size;
    std::string msg;
};

struct Blob {
    int version;
    int size;
    string data;

    Blob() {

    }

    Blob(int version, int size, string data) {
        this->version = version;
        this->size = size;
        this->data = data;
    }
};


void do_fs(const std::string &filename) {
    std::ifstream reader(filename, std::ios::in | std::ios::binary);
    if (reader.is_open()) {
        // read bytes from file to buffer
        std::vector<char> bytes;
        int data_sz = 16;
        bytes.resize(data_sz);
        reader.read(reinterpret_cast<char *>(bytes.data()), data_sz);

        // compare text data
        const char* text_data = "Hello, World";
        if (!std::memcmp(bytes.data(), text_data, 12)) {
            std::cout << "Text data equal\n";
        } else {
            throw DataException("Text data NOT equal");
        }

        // compare int data
        std::uint32_t num = 0xC;
        if (!std::memcmp(bytes.data()+12,
                         reinterpret_cast<char *>(&num), 4)) {
            std::cout << "Int data equal\n";
            std::uint32_t big_num;
            std::memcpy(&big_num, bytes.data()+12, 4);
            std::cerr << "LIT_NUM " << big_num << '\n';
        } else {
            std::uint32_t big_num;
            std::memcpy(&big_num, bytes.data()+12, 4);
            std::cerr << "BIG_NUM " << big_num << '\n';
            throw DataException("Int data NOT equal");
        }

    } else {
        throw VersionException("invalid version!");
    }
}


int main() {
    try {
        do_fs("bin_data.bin");
    } catch (const DataException& ex) {
        std::cerr << ex.what() << '\n';
    } catch (const VersionException& ex) {
        std::cerr << ex.what() << '\n';
    } catch (const SizeException& ex) {
        std::cerr << ex.what() << '\n';
    }

    return 0;
}
