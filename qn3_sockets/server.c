#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

typedef struct {
    int socket;
    char username[50];
} Client;

Client clients[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;

// Broadcast online users to all connected clients
void broadcast_online_users() {
    char message[BUFFER_SIZE] = "[Active Users]: ";
    pthread_mutex_lock(&client_mutex);
    for (int i = 0; i < client_count; i++) {
        strcat(message, clients[i].username);
        if (i < client_count - 1) strcat(message, ", ");
    }
    strcat(message, "\n");
    for (int i = 0; i < client_count; i++) {
        send(clients[i].socket, message, strlen(message), 0);
    }
    pthread_mutex_unlock(&client_mutex);
}

// Send a message from one user to another
void send_message(const char *from, const char *to, const char *message, int from_socket) {
    pthread_mutex_lock(&client_mutex);
    int recipient_found = 0;

    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].username, to) == 0) {
            char formatted_message[BUFFER_SIZE];
            snprintf(formatted_message, sizeof(formatted_message), "[%s]: %s\n", from, message);
            send(clients[i].socket, formatted_message, strlen(formatted_message), 0);
            recipient_found = 1;
            break;
        }
    }

    if (!recipient_found) {
        char error_message[BUFFER_SIZE];
        snprintf(error_message, sizeof(error_message), "[Error]: Username '%s' not found.\n", to);
        send(from_socket, error_message, strlen(error_message), 0);
    }
    pthread_mutex_unlock(&client_mutex);
}

// Handle a single client connection
void *handle_client(void *arg) {
    int client_socket = *(int *)arg;
    free(arg);

    char buffer[BUFFER_SIZE];
    char username[50];

    // Receive the username from the client
    recv(client_socket, username, sizeof(username), 0);

    // Add the client to the list
    pthread_mutex_lock(&client_mutex);
    clients[client_count].socket = client_socket;
    strncpy(clients[client_count].username, username, sizeof(clients[client_count].username) - 1);
    clients[client_count].username[sizeof(clients[client_count].username) - 1] = '\0';
    client_count++;
    pthread_mutex_unlock(&client_mutex);

    printf("%s has connected.\n", username);
    broadcast_online_users();


    // Handle communication
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) break;

        char to[50], message[BUFFER_SIZE];
        if (sscanf(buffer, "%49[^:]:%1023[^\n]", to, message) == 2) {
            send_message(username, to, message, client_socket);
        }
    }

    // Remove the client from the list
    pthread_mutex_lock(&client_mutex);
    for (int i = 0; i < client_count; i++) {
        if (clients[i].socket == client_socket) {
            clients[i] = clients[client_count - 1];
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&client_mutex);

    close(client_socket);
    broadcast_online_users();
    printf("%s has disconnected.\n", username);
    return NULL;
}

int main() {
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    // Create a socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_socket, MAX_CLIENTS);
    printf("Server listening on port %d\n", PORT);
    printf("Connect clients by executing './client' in another terminal\n");

    while (1) {
        addr_size = sizeof(client_addr);
        int *new_socket = malloc(sizeof(int)); // Allocate memory for new client socket
        *new_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);

        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, new_socket);
        pthread_detach(thread); // Automatically clean up thread resources
    }

    close(server_socket);
    return 0;
}
