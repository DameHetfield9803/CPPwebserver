#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <Winsock2.h>
#include <string>

int main() {
  SOCKET wsocket;
  SOCKET new_wsocket;
  WSADATA wsaData;

  struct sockaddr_in server;
  int server_len;
  int BUFFER_SIZE = 37020;

  if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    std::cout << "WSAStartup failed. Cannot be started\n";
  }
  wsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(wsocket == INVALID_SOCKET) {
    std::cout << "Socket creation failed. Cannot be created\n";
  }

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_port = htons(8080);
  server_len = sizeof(server);
  if (bind(wsocket, (SOCKADDR *)&server, server_len) != 0) {
    std::cout<< "Could not bind the socket to localhost\n";
  }

  if (listen(wsocket, 20) != 0) {
    std::cout << "Could not start listening on socket\n";
  }

  std::cout << "Listening on 127.0.0.1:8080...\n";
  int bytes = 0;
  while (true) {
    // accept client request
    new_wsocket = accept(wsocket, (SOCKADDR *)&server, &server_len);
    if(new_wsocket == INVALID_SOCKET) {
      std::cout << "Could not accept connection\n";
    }

    // read request
    char buff[30720] = {0};
    bytes = recv(new_wsocket, buff, BUFFER_SIZE, 0);
    if(bytes < 0) {
      std::cout << "Could not read client request\n";
    }

    std::string serverMsg =
        "HTTP 1.1 200 OK \nContent-Type: text/html\n\n<html><body><h1>Hello, "
        "World!</h1></body></html>";
    std::string res = "<html><h1>Hello World</h1></html>";

    serverMsg.append(std::to_string(res.size()));
    serverMsg.append("\n\n");
    serverMsg.append(res);

    int bytesSent = 0;

    int totalBytesSent = 0;

    while (totalBytesSent < serverMsg.size()) {
        bytesSent = send(new_wsocket, serverMsg.c_str(), serverMsg.size(), 0);
        if (bytesSent == SOCKET_ERROR) {
            std::cout << "Could not send response to client\n";
        }
        totalBytesSent += bytesSent;
    }
    std::cout << "Sent response to client. \n";
    closesocket(new_wsocket);
  }
  closesocket(wsocket);
  WSACleanup();
  return 0;
}