#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>

#define PORT 9000
#define MAX 2000

int clientCount = 0;
int clientSockets[10];

// Function to handle client communication
void *clientHandler(void *socket_desc) {
    int sock = *(int *)socket_desc;
    char message[MAX];
    
    // Receive messages from client
    while (1) {
        memset(message, 0, sizeof(message));
        int read_size = recv(sock, message, sizeof(message), 0);
        if (read_size <= 0) {
            printf("Client disconnected\n");
            close(sock);
            return NULL;
        }
        printf("Message from client: %s\n", message);
    }
}

int main() {
    int sockfd, new_sock, *new_sock_ptr;
    struct sockaddr_in server, client;
    socklen_t client_len = sizeof(client);

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Bind socket
    if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("Bind failed");
        return 1;
    }

    // Listen for incoming connections
    if (listen(sockfd, 3) == 0) {
        printf("Server listening on port %d...\n", PORT);
    } else {
        perror("Listen failed");
        return 1;
    }

    // Accept incoming connections and spawn threads for each client
    while (1) {
        new_sock = accept(sockfd, (struct sockaddr *)&client, &client_len);
        if (new_sock < 0) {
            perror("Accept failed");
            continue;
        }

        clientSockets[clientCount] = new_sock;
        clientCount++;

        pthread_t sniffer_thread;
        new_sock_ptr = malloc(sizeof(int));
        *new_sock_ptr = new_sock;
        
        // Create a thread for each client
        if (pthread_create(&sniffer_thread, NULL, clientHandler, (void *)new_sock_ptr) < 0) {
            perror("Thread creation failed");
            return 1;
        }
    }

    close(sockfd);
    return 0;
}

