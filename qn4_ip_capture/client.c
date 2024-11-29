#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

void scan_network(char *network_prefix, char *output) {
    FILE *fp;
    char command[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];

    snprintf(command, sizeof(command), "ping -c 1 %s", network_prefix);

    // Run ping command and collect output
    fp = popen(command, "r");
    if (fp == NULL) {
        perror("Error running ping command");
        return;
    }

    // Collect command output
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        strcat(output, buffer);
    }

    pclose(fp);
}

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char network_prefix[] = "192.168.1"; // Example subnet
    char discovered_hosts[BUFFER_SIZE] = "IP and Hostnames:\n";

    // Simulate network scan (replace with real scanning logic)
    for (int i = 1; i <= 5; i++) { // Scan first 5 hosts for demonstration
        char ip[20];
        snprintf(ip, sizeof(ip), "%s.%d", network_prefix, i);
        char hostname[50];
        snprintf(hostname, sizeof(hostname), "host%d", i);
        char entry[100];
        snprintf(entry, sizeof(entry), "IP: %s, Hostname: %s\n", ip, hostname);
        strcat(discovered_hosts, entry);
    }

    // Create client socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Send discovered hosts to server
    send(client_socket, discovered_hosts, strlen(discovered_hosts), 0);
    printf("Data sent to server:\n%s", discovered_hosts);

    close(client_socket);
    return 0;
}
