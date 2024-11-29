#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_TO_LENGTH 50
#define MAX_MESSAGE_LENGTH 970 // Leave room for ":" and null terminator

// ANSI color codes for styling
#define RESET "\033[0m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define RED "\033[31m"
#define YELLOW "\033[33m"

// Global variable for the username
char username[50];

// Function to clear the current line in the terminal
void clear_current_line() {
    printf("\33[2K\r");  // Clear the current line and reset the cursor
    fflush(stdout);
}

// Function to handle incoming messages
void *receive_messages(void *socket) {
    int client_socket = *(int *)socket;
    char buffer[BUFFER_SIZE];
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received > 0) {
            clear_current_line();

            // Check message type
            if (strncmp(buffer, "[Active Users]:", 15) == 0) {
                printf("%s%s%s\n", YELLOW, buffer, RESET); // Display active users
            } 
            else if (strncmp(buffer, "[Error]:", 8) == 0) {
                printf("%s%s%s\n", RED, buffer, RESET); // Display errors
            } 
            else {
                printf("%s%s%s\n", GREEN, buffer, RESET); // Display received messages
            }

            printf("To (username): ");
            fflush(stdout);
        } 
        else {
            printf("%sDisconnected from server.%s\n", RED, RESET);
            exit(1);
        }
    }
    return NULL;
}

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create and connect socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        return 1;
    }
    printf("%sSocket created successfully.%s\n", BLUE, RESET);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);


    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        return 1;
    }
    printf("%sConnected to the server.%s\n", BLUE, RESET);

    // Prompt the user for their username
    printf("Enter username: ");
    scanf("%49s", username);
    send(client_socket, username, strlen(username), 0);
    printf("%sUsername sent to server: %s%s\n", BLUE, username, RESET);

    // Start receiving messages
    pthread_t thread;
    if (pthread_create(&thread, NULL, receive_messages, &client_socket) != 0) {
        perror("Failed to create receiving thread");
        return 1;
    }
    printf("%sReceiving thread started.%s\n", BLUE, RESET);

    // Loop for sending messages
    while (1) {
        char to[MAX_TO_LENGTH];
        char message[MAX_MESSAGE_LENGTH];
        printf("To (username): ");
        scanf("%49s", to);
        getchar(); // Consume newline

        // Prevent sending messages to yourself
        if (strcmp(to, username) == 0) {
            printf("%s[Error]: You cannot send a message to yourself.%s\n", RED, RESET);
            continue;
        }

        printf("Message: ");
        fgets(message, MAX_MESSAGE_LENGTH, stdin);

        size_t len = strlen(message);
        if (len > 0 && message[len - 1] == '\n') {
            message[len - 1] = '\0';
        }

        snprintf(buffer, BUFFER_SIZE, "%.*s:%.*s",
                 MAX_TO_LENGTH - 1, to,        // Truncate 'to'
                 MAX_MESSAGE_LENGTH - 1, message); // Truncate 'message'

        send(client_socket, buffer, strlen(buffer), 0);

        clear_current_line();
        printf("%s[Sent]: To [%s]: %s%s\n", BLUE, to, message, RESET);

        // Prompt for the next message
        // printf("To (username): ");
        fflush(stdout);
    }

    close(client_socket);
    return 0;
}
