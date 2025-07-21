#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 22000
#define BUFFER_SIZE 1024

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(client_addr);

    // Create the socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set server address properties
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the address and port
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_sock, 5) < 0) {
        perror("Listen failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    // Accept a client connection
    client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_len);
    if (client_sock < 0) {
        perror("Accept failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    printf("Client connected.\n");

    // Chat loop
    while (1) {
        bzero(buffer, BUFFER_SIZE);

        // Receive message from client
        int bytes_received = recv(client_sock, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            perror("recv failed");
            break;
        }
        
        printf("Client: %s", buffer);

        // Check if client sent "exit" to end the chat
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Client disconnected.\n");
            break;
        }

        // Server sends a response
        bzero(buffer, BUFFER_SIZE);
        printf("Server: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        send(client_sock, buffer, strlen(buffer), 0);

        // Check if server sent "exit" to end the chat
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Server disconnected.\n");
            break;
        }
    }

    close(client_sock);
    close(server_sock);
    return 0;
}
