#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 22000
#define BUFFER_SIZE 1024

int main() {
    int client_sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create the socket
    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost

    // Connect to the server
    if (connect(client_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(client_sock);
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server. Type 'exit' to quit.\n");

    // Chat loop
    while (1) {
        bzero(buffer, BUFFER_SIZE);
        printf("Client: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        send(client_sock, buffer, strlen(buffer), 0);

        // Check if client wants to exit
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Disconnected from server.\n");
            break;
        }

        bzero(buffer, BUFFER_SIZE);
        int bytes_received = recv(client_sock, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            printf("Server disconnected.\n");
            break;
        }

        printf("Server: %s", buffer);
    }

    close(client_sock);
    return 0;
}
