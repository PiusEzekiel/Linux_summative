#!/bin/bash

# Script to automate building, running, and testing the client-server socket program

echo -e "\n🛠️ \033[1;32mStarting Build and Test Process for Socket Program...\033[0m\n"

# Define file names and directories
SERVER_FILE="server.c"
CLIENT_FILE="client.c"
SERVER_EXEC="server"
CLIENT_EXEC="client"
SERVER_LOG="server_log.txt"


# Styled output functions
print_header() {
    echo -e "\n\033[1;34m===== $1 =====\033[0m\n"
}

print_success() {
    echo -e "\033[1;32m✔ $1\033[0m"
}

print_error() {
    echo -e "\033[1;31m✘ $1\033[0m"
    exit 1
}

# Wait function for better readability
wait_time() {
    echo -e "\033[1;33m⏳ Waiting for $1 seconds...\033[0m"
    sleep $1
}

# Step 1: Check for required files
print_header "Checking Required Files"
if [ ! -f "$SERVER_FILE" ] || [ ! -f "$CLIENT_FILE" ]; then
    print_error "Required files '$SERVER_FILE' or '$CLIENT_FILE' are missing."
else
    print_success "Found '$SERVER_FILE'"
    print_success "Found '$CLIENT_FILE'"
fi
wait_time 2

# Step 2: Clean up existing build directory
print_header "Cleaning Previous Builds"
if [ -f "$SERVER_EXEC" ]; then
    rm "$SERVER_EXEC" && print_success "Removed existing server executable ($SERVER_EXEC)."
else
    print_success "No existing server executable ($SERVER_EXEC) found."
fi

if [ -f "$CLIENT_EXEC" ]; then
    rm "$CLIENT_EXEC" && print_success "Removed existing client executable ($CLIENT_EXEC)."
else
    print_success "No existing client executable ($CLIENT_EXEC) found."
fi
wait_time 2

# Step 3: Compile the server and client programs
print_header "Compiling the Server and Client Programs"
if gcc -pthread "$SERVER_FILE" -o "$SERVER_EXEC"; then
    print_success "Server program compiled successfully."
else
    print_error "Error during server program compilation."
fi

if gcc -pthread "$CLIENT_FILE" -o "$CLIENT_EXEC"; then
    print_success "Client program compiled successfully."
else
    print_error "Error during client program compilation."
fi
wait_time 2

# Step 4: Run the server program
print_header "Running the Server Program"
./"$SERVER_EXEC"
