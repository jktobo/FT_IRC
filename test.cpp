#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <algorithm>

#define PORT 6667
#define MAX_CLIENTS 10

struct Client {
    int socket;
    std::string nickname;
    std::vector<std::string> channels;
};

std::vector<Client> clients;

void send_message(int socket, const std::string& message) {
    send(socket, message.c_str(), message.length(), 0);
}

void broadcast_message(const std::string& message, const std::vector<int>& exclude = {}) {
    for (auto& client : clients) {
        if (std::find(exclude.begin(), exclude.end(), client.socket) == exclude.end()) {
            send_message(client.socket, message);
        }
    }
}

void handle_message(int socket, const std::string& message) {
    std::string command = message.substr(0, message.find(' '));
    std::string params = message.substr(message.find(' ') + 1);

    if (command == "NICK") {
        std::string nickname = params.substr(0, params.find(' '));
        auto it = std::find_if(clients.begin(), clients.end(), [&](const Client& client) {
            return client.nickname == nickname;
        });
        if (it != clients.end()) {
            send_message(socket, "Nickname is already in use\n");
        } else {
            auto& client = clients[socket];
            client.nickname = nickname;
            send_message(socket, "Nickname set to " + nickname + "\n");
        }
    } else if (command == "JOIN") {
        std::string channel = params.substr(0, params.find(' '));
        auto& client = clients[socket];
        if (std::find(client.channels.begin(), client.channels.end(), channel) != client.channels.end()) {
            send_message(socket, "You are already in " + channel + "\n");
        } else {
            client.channels.push_back(channel);
            std::string message = client.nickname + " has joined " + channel + "\n";
            broadcast_message(message, {socket});
        }
    } else if (command == "PART") {
        std::string channel = params.substr(0, params.find(' '));
        auto& client = clients[socket];
        auto it = std::find(client.channels.begin(), client.channels.end(), channel);
        if (it == client.channels.end()) {
            send_message(socket, "You are not in " + channel + "\n");
        } else {
            client.channels.erase(it);
            std::string message = client.nickname + " has left " + channel + "\n";
            broadcast_message(message, {socket});
        }
    } else if (command == "PRIVMSG") {
        std::string recipient = params.substr(0, params.find(' '));
        std::string message = params.substr(params.find(' ') + 1);
        auto it = std::find_if(clients.begin(), clients.end(), [&](const Client& client) {
            return client.nickname == recipient;
        });
        if (it == clients.end()) {
            send_message(socket, "No such nick/channel\n");
        } else {
            std::string sender = clients[socket].nickname;
            std::string message = ":" + sender + " PRIVMSG " + recipient + " :" + message + "\n";
            send_message(it->socket, message);
        }
    } else if (command == "MODE") {
        // Handle user modes
    } else {
        send_message(socket, "Unknown command\n");
    }
}

int main() {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Create a socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to a port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Accept incoming connections
    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Add the new client to the list of clients
        clients.push_back({new_socket});

        // Send a welcome message to the client
        const char* welcome_message = "Welcome to the IRC server!\n";
        send(new_socket, welcome_message, strlen(welcome_message), 0);

        // Handle communication with the client
        while (true) {
            valread = read(new_socket, buffer, 1024);
            if (valread <= 0) {
                // Client disconnected
                auto it = std::find_if(clients.begin(), clients.end(), [&](const Client& client) {
                    return client.socket == new_socket;
                });
                if (it != clients.end()) {
                    std::string message = it->nickname + " has quit\n";
                    broadcast_message(message, {new_socket});
                    clients.erase(it);
                }
                break;
            }
            std::string message(buffer, valread);
            handle_message(new_socket, message);
        }
    }

    return 0;
}