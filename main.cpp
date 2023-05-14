#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace boost::asio;
using namespace boost::system;
using namespace std;

class Session : public enable_shared_from_this<Session> {
public:
    Session(ip::tcp::socket socket) : socket_(move(socket)) {}

    void Start() {
        DoRead();
    }

private:
    void DoRead() {
        async_read_until(socket_, input_, "\n",
                         [self = shared_from_this()](const error_code& ec, size_t length) {
                             if (ec) {
                                 cerr << "Read error: " << ec.message() << endl;
                                 return;
                             }
                             string command = string(buffer_cast<const char*>(self->input_.data()), length - 1);
                             self->input_.consume(length);
                             self->DoCommand(command);
                         });
    }
    void DoCommand(const string& command) {
        if (command == "username\n") {
            DoWrite("username: user\n");
        } else if (command == "hostname\n") {
            DoWrite("hostname: host\n");
        } else if (command == "date\n") {
            time_t t = time(nullptr);
            char buf[256];
            strftime(buf, sizeof(buf), "%c", localtime(&t));
            DoWrite(string("date: ") + buf + "\n");
        } else if (command == "help\n") {
            DoWrite("Available commands: username, hostname, date, help, exit, ls, mkfile, mkdir, rmfile, rmdir\n");
        } else if (command == "exit\n") {
            socket_.close();
        } else if (command.substr(0, 3) == "ls ") {
            string path = command.substr(3);
            DIR* dir = opendir(path.c_str());
            if (dir == nullptr) {
                DoWrite("Cannot open directory\n");
                return;
            }
            string result;
            dirent* entry;
            while ((entry = readdir(dir)) != nullptr) {
                result += entry->d_name;
                result += "\n";
            }
            closedir(dir);
            DoWrite(result);
        } else if (command.substr(0, 7) == "mkfile ") {
            string path = command.substr(7);
            int fd = open(path.c_str(), O_WRONLY | O_CREAT | O_EXCL, 0666);
            if (fd == -1) {
                DoWrite("Cannot create file\n");
                return;
            }
            close(fd);
            DoWrite("File created\n");
        } else if (command.substr(0, 6) == "mkdir ") {
            string path = command.substr(6);
            if (mkdir(path.c_str(), 0777) == -1) {
                DoWrite("Cannot create directory\n");
                return;
            }
            DoWrite("Directory created\n");
        } else if (command.substr(0, 7) == "rmfile ") {
            string path = command.substr(7);
            if (unlink(path.c_str()) == -1) {
                DoWrite("Cannot remove file\n");
                return;
            }
            DoWrite("File removed\n");
        } else if (command.substr(0, 6) == "rmdir ") {
            string path = command.substr(6);
            if (rmdir(path.c_str()) == -1) {
                DoWrite("Cannot remove directory\n");
                return;
            }
            DoWrite("Directory removed\n");
        }

        else if (command.substr(0, 9) == "readfile ") {
            string path = command.substr(9);
            int fd = open(path.c_str(), O_RDONLY);
            if (fd == -1) {
                DoWrite("Cannot open file\n");
                return;
            }
            char buf[1024];
            ssize_t n;
            string result;
            while ((n = read(fd, buf, sizeof(buf))) > 0) {
                result += string(buf, n);
            }
            close(fd);
            DoWrite(result);
        }

        else {
            DoWrite("Unknown command\n");
        }

    }


    void DoWrite(const string& message) {
        async_write(socket_, buffer(message),
                    [self = shared_from_this()](const error_code& ec, size_t length) {
                        if (ec) {
                            cerr << "Write error: " << ec.message() << endl;
                            return;
                        }
                        self->DoRead();
                    });
    }

    ip::tcp::socket socket_;
    streambuf input_;
};

class Server {
public:
    Server(io_service& io_service, short port) : acceptor_(io_service, ip::tcp::endpoint(ip::tcp::v4(), port)) {}

    void Start() {
        DoAccept();
    }

private:
    void DoAccept() {
        acceptor_.async_accept(
                [this](const error_code& ec, ip::tcp::socket socket) {
                    if (ec) {
                        cerr << "Accept error: " << ec.message() << endl;
                    } else {
                        make_shared<Session>(move(socket))->Start();
                    }
                    DoAccept();
                });
    }

    ip::tcp::acceptor acceptor_;
};

int main() {
    try {
        io_service io_service;
        Server server(io_service, 1234);
        server.Start();
        io_service.run();
    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }
    return 0;
}
