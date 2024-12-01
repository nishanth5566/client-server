#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 9000
#define MAX 2000

int main() {
    int sock;
    struct sockaddr_in server;
    char message[MAX];
    char clientID[50];

    // Predefined valid client names
    const char *validClients[] = {"Nishanth", "Anish", "Subhash", "Prajwal"};

    // Asking for client ID
    printf("Enter the ID: ");
    fgets(clientID, sizeof(clientID), stdin);
    clientID[strcspn(clientID, "\n")] = 0; // Remove newline character from input

    // Check if the entered ID is valid
    int valid = 0;
    for (int i = 0; i < 4; i++) {
        if (strcmp(clientID, validClients[i]) == 0) {
            valid = 1;
            break;
        }
    }

    if (!valid) {
        printf("A traitor with a pirate symbol ☠️\n");
        return 0;  // Terminate if not a valid client
    }

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("Connection failed");
        return 1;
    }

    printf("Connection established with server...\n");

    // Client is now allowed to send messages
    while (1) {
        printf("You can send a message to the server (type 'EXIT' to quit): ");
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = 0; // Remove newline character

        if (strcmp(message, "EXIT") == 0) {
            break;
        }

        if (send(sock, message, strlen(message), 0) < 0) {
            perror("Send failed");
            return 1;
        }
    }

    close(sock);
    return 0;
}

