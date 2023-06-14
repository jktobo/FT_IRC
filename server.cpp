/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joldosh <joldosh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 19:57:50 by dkaratae          #+#    #+#             */
/*   Updated: 2023/06/15 00:22:08 by joldosh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

const int BUFFER_SIZE = 1024;

Server::Server(int port, const std::string& password)// : port(port), password(password) {
{
    this->port = port;
    this->password = password;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        exit(1);
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Error binding socket" << std::endl;
        exit(1);
    }

    if (listen(server_socket, 10) == -1) {
        std::cerr << "Error listening on socket" << std::endl;
        exit(1);
    }

    pollfd server_pollfd = {server_socket, POLLIN, 0};
    fds.push_back(server_pollfd);
}

void Server::run() {
    while (true) {
        int poll_result = poll(&fds[0], fds.size(), -1);

        if (poll_result == -1) {
            std::cerr << "Error polling sockets" << std::endl;
            break;
        }

        for (size_t i = 0; i < fds.size(); ++i) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == server_socket) {
                    sockaddr_in client_addr;
                    socklen_t client_addr_len = sizeof(client_addr);
                    int client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_addr_len);

                    if (client_socket == -1) {
                        std::cerr << "Error accepting connection" << std::endl;
                    } else {
                        pollfd client_pollfd = {client_socket, POLLIN, 0};
                        fds.push_back(client_pollfd);
                        clients[client_socket] = "";
                    }
                } else {
                    char buffer[BUFFER_SIZE];
                    memset(buffer, 0, BUFFER_SIZE);
                    int bytes_received = recv(fds[i].fd, buffer, BUFFER_SIZE - 1, 0);
                    std::cout << "msg: " << buffer;
                    if (bytes_received <= 0) {
                        close(fds[i].fd);
                        clients.erase(fds[i].fd);
                        fds.erase(fds.begin() + i);
                        --i;
                    } else {
                        buffer[bytes_received] = '\0';
                        std::string message(buffer);

                        if (message.substr(0, 5) == "PASS ") {
                            std::cout << "res: " << message.substr(5) << "\n";
                            if (message.substr(5) == password + "\n") {
                                clients[fds[i].fd] = "authenticated";
                                std::string welcome = "Welcome to the IRC server!\r\n";
                                send(fds[i].fd, welcome.c_str(), welcome.size(), 0);
                            } else {
                                std::string error = "Invalid password\r\n";
                                send(fds[i].fd, error.c_str(), error.size(), 0);
                                close(fds[i].fd);
                                clients.erase(fds[i].fd);
                                fds.erase(fds.begin() + i);
                                --i;
                            }
                        } else if (clients[fds[i].fd] == "authenticated") {
                            for (size_t j = 0; j < fds.size(); ++j) {
                                if (fds[j].fd != server_socket && fds[j].fd != fds[i].fd) {
                                    send(fds[j].fd, buffer, bytes_received, 0);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    close(server_socket);
}
