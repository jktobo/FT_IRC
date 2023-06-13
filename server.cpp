// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   server.cpp                                         :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: joldosh <joldosh@student.42.fr>            +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2023/06/03 19:57:50 by dkaratae          #+#    #+#             */
// /*   Updated: 2023/06/13 00:04:17 by joldosh          ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include <iostream>
// #include <sys/types.h>
// #include <unistd.h>
// #include <sys/socket.h>
// #include <netdb.h>
// #include <arpa/inet.h>
// #include <string.h>
// #include <string>

// using namespace std;

// int main()
// {
//     // Create a socket
//     // Bind the socket to a IP / port
//     //

//     int listening = socket(AF_INET, SOCK_STREAM, 0);
//     if (listening == -1)
//     {
//         cerr << "Can't create a socket!";
//         return -1;
//     }
//     sockaddr_in hint;
//     hint.sin_family = AF_INET;
//     hint.sin_port = htons(54000);
//     inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);  // 127.0.0.1
    
//     if (bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1)
//     {
//         cerr << "Can't bind to IP/port";
//         return -2;
//     }
    
//     if (listen(listening, SOMAXCONN) == -1)
//     {
//         cerr << "Can't listen!";
//         return -3;
//     }

//     sockaddr_in client;
//     socklen_t clientSize = sizeof(client);
//     char host[NI_MAXHOST];
//     char svc[NI_MAXSERV];
    
//     int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
    
//     if (clientSocket == -1)
//     {
//         cerr << "Problem with client connectiong!";
//         return -4;
//     }

//     close(listening);

//     memset(host, 0, NI_MAXHOST);
//     memset(svc, 0, NI_MAXSERV);

//     int result = getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);
    
//     if (result)
//     {
//         cout << host << " connected on " << svc << endl;
//     }
//     else
//     {
//         inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
//         cout << host << " connected on " << ntohs(client.sin_port) << endl;
//     }

//     char buf[4096];
//     while (true)
//     {
//         memset(buf, 0, 4096);
//         int bytesRecv = recv(clientSocket, buf, 4096, 0);
//         if (bytesRecv == -1)
//         {
//             cerr << "There was a connection issue" << endl;
//             break;
//         }
        
//         if (bytesRecv == 0)
//         {
//             cout << "The client disconnected" << endl;
//             break;
//         }
        
//         cout << "Received: " << string(buf, 0, bytesRecv) << endl;

//         send(clientSocket, buf, bytesRecv + 1, 0);
//     }
//     close(clientSocket);
// }

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
 
using namespace std;
 
int main()
{
    // Create a socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1)
    {
        cerr << "Can't create a socket! Quitting" << endl;
        return -1;
    }
 
    // Bind the ip address and port to a socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
 
    bind(listening, (sockaddr*)&hint, sizeof(hint));
 
    // Tell Winsock the socket is for listening
    listen(listening, SOMAXCONN);
 
    // Wait for a connection
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
 
    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
 
    char host[NI_MAXHOST]; // Client's remote name
    char service[NI_MAXSERV]; // Service (i.e. port) the client is connect on
 
    memset(host, 0, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);
 
    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
    {
        cout << host << " connected on port " << service << endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << " connected on port " << ntohs(client.sin_port) << endl;
    }
 
    // Close listening socket
    close(listening);
 
    // While loop: accept and echo message back to client
    char buf[4096];
 
    while (true)
    {
        memset(buf, 0, 4096);
 
        // Wait for client to send data
        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        if (bytesReceived == -1)
        {
            cerr << "Error in recv(). Quitting" << endl;
            break;
        }
 
        if (bytesReceived == 0)
        {
            cout << "Client disconnected " << endl;
            break;
        }
 
        cout << string(buf, 0, bytesReceived) << endl;
 
        // Echo message back to client
        send(clientSocket, buf, bytesReceived + 1, 0);
    }
 
    // Close the socket
    close(clientSocket);
 
    return 0;
}