#!/bin/bash

# Script to automate building, running, and testing the client-server socket program

echo -e "\n🛠️ \033[1;32mStarting Build and Test Process for Socket Program...\033[0m\n"

# Define file names and directories
SERVER_FILE="server.c"
CLIENT_FILE="client.c"
BUILD_DIR="build"
SERVER_EXEC="server"
CLIENT_EXEC="client"
SERVER_LOG="server_log.txt"
CLIENT_LOG="client_log.txt"

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

# Step 1: Clean up old build and logs
print_header "Cleaning Up Old Build and Logs"
rm -rf "$BUILD_DIR" "$SERVER_LOG" "$CLIENT_LOG"
mkdir "$BUILD_DIR" && print_success "Created new build directory."

# Step 2: Compile server and client
print_header "Compiling Server and Client Programs"
if gcc "$SERVER_FILE" -o "$BUILD_DIR/$SERVER_EXEC"; then
    print_success "Server compiled successfully."
else
    print_error "Error compiling server."
fi

if gcc "$CLIENT_FILE" -o "$BUILD_DIR/$CLIENT_EXEC"; then
    print_success "Client compiled successfully."
else
    print_error "Error compiling client."
fi

# Step 3: Run server in the background
print_header "Starting the Server"
cd "$BUILD_DIR"
stdbuf -oL -eL ./"$SERVER_EXEC" > "../$SERVER_LOG" 2>&1 &
SERVER_PID=$!
print_success "Server started (PID: $SERVER_PID). Logs: $SERVER_LOG"
sleep 2

# Step 4: Start the Client
print_header "Starting the Client"
stdbuf -oL -eL ./"$CLIENT_EXEC" > "../$CLIENT_LOG" 2>&1 &
CLIENT_PID=$!
print_success "Client started (PID: $CLIENT_PID). Logs: $CLIENT_LOG"
sleep 2

# Step 5: Allow time for communication
print_header "Allowing Communication Between Client and Server"
sleep 10

# Step 6: Terminate processes
print_header "Stopping All Processes"
kill "$SERVER_PID" && print_success "Server stopped."
# kill "$CLIENT_PID" && print_success "Client stopped."

# Step 7: Display Logs
print_header "Displaying Server Log"
cat "../$SERVER_LOG"

print_header "Displaying Client Log"
cat "../$CLIENT_LOG"

# Final message
print_header "Test Automation Completed"
echo -e "\033[1;34m🎉 All steps were completed successfully!🎉\033[0m\n"

# Return to initial directory
cd ..
