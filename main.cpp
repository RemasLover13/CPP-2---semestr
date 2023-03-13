#include <iostream>
#include <exception>
#include <string>
#include <vector>
#include <cstring> // std::memcmp
#include <fstream> // std::ifstream

using namespace std;

struct VersionException :  std::exception {


    const char *what() const throw() {
        return "Version control exception!";
    }

};

struct DataException :  std::exception {
    const char *what() const throw() {
        return "Data Exception!";
    }
};

struct SizeException : std::exception {
    const char *what() const throw() {
        return "Size exception!";
    }
};

struct Blob {
    short version;
    int size;
    char data[12];

    Blob() {
        version = 0;
        size = 0;
    }

    void Print() {
        cout << version << size << data <<  endl;
    }

};


void do_fs(const std::string &filename) {
    // открываем файл для чтения
    std::ifstream reader(filename, std::ios::in | std::ios::binary);

    if (reader.is_open()) {
        Blob blob;
        // read bytes from file to buffer
        std::vector<char> bytes;
        int data_sz = 18;
        bytes.resize(data_sz);
        reader.read(reinterpret_cast<char *>(bytes.data()), data_sz);
        int version = 0;
        for (int i = 0; i < 1; i++) {
            version += (int)bytes[i];
        }
        if (version >= 1 && version <= 7) {
            blob.version = (short)version;
        } else {
            throw VersionException();
        }

        // compare text data
        const char* text_data = "Hello, World";
        if (!std::memcmp(bytes.data(), text_data, 12)) {
            std::cout << "Text data equal\n";
            for (int i = 0; i < 12; i++) {
                blob.data[i] = text_data[i];
            }
            cout << blob.data << endl;
        } else {
            throw DataException();
        }

        // compare int data
        std::uint32_t num = 0xC;
        if (!std::memcmp(bytes.data()+12,
                         reinterpret_cast<char *>(&num), 4)) {
            std::cout << "Int data equal\n";
            std::uint32_t big_num;
            std::memcpy(&big_num, bytes.data()+12, 4);
            std::cerr << "LIT_NUM " << big_num << '\n';
            blob.size = num;
        } else {
            std::uint32_t big_num;
            std::memcpy(&big_num, bytes.data()+12, 4);
            std::cerr << "BIG_NUM " << big_num << '\n';
            throw SizeException();
        }

        blob.Print();

    } else {
        throw std::exception();
    }
}


int main() {
    try {
        do_fs("/home/krmakov/homework_cpp/homework_3/bin_data.bin");
    } catch (const DataException& ex) {
        std::cerr << ex.what() << '\n';
    } catch (const VersionException& ex) {
        std::cerr << ex.what() << '\n';
    } catch (const SizeException& ex) {
        std::cerr << ex.what() << '\n';
    }



    return 0;
}
