#!/bin/bash

# Script to automate building, compiling, and running the producer-consumer program

echo -e "\n🛠️ \033[1;32mStarting Build and Test Process...\033[0m\n"

# Define file names and directories
C_FILE="producer_consumer.c"
BUILD_DIR="build"
EXECUTABLE="producer_consumer"
OUTPUT_FILE="producer_consumer_output.txt"

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
    echo -e "\033[1;33m⏳ Waiting for 2 seconds...\033[0m"
    sleep 2
}

# Step 1: Check for required files
print_header "Checking Required Files"
if [ ! -f "$C_FILE" ]; then
    print_error "Required file '$C_FILE' is missing."
else
    print_success "Found '$C_FILE'."
fi
wait_time

# Step 2: Clean up existing build directory
print_header "Cleaning Build Directory"
if [ -d "$BUILD_DIR" ]; then
    rm -rf "$BUILD_DIR" && print_success "Removed existing build directory."
else
    print_success "No existing build directory found."
fi
mkdir "$BUILD_DIR" && print_success "Created new build directory."
wait_time

# Step 3: Compile the C program
print_header "Compiling the C Program"
if gcc -pthread "$C_FILE" -o "$BUILD_DIR/$EXECUTABLE"; then
    print_success "C program compiled successfully."
else
    print_error "Error during C program compilation."
fi
wait_time

# Step 4: Run the Producer-Consumer Program
print_header "Running the Producer-Consumer Program"
cd "$BUILD_DIR"
stdbuf -oL -eL ./"$EXECUTABLE" | tee "$OUTPUT_FILE" &  # Disable output buffering and stream logs to console and file
EXEC_PID=$!
if [ "$EXEC_PID" ]; then
    print_success "Producer-Consumer program is running (PID: $EXEC_PID). Output is being displayed below:\n"
else
    print_error "Error during program execution."
fi

# Step 5: Allow the program to run for 20 seconds
wait_time
print_header "Program Execution in Progress..."
sleep 20
kill "$EXEC_PID" && print_success "Stopped the program after 20 seconds."

# Step 6: Verify output file
print_header "Verifying Output File"
if [ -f "$OUTPUT_FILE" ]; then
    print_success "Output file '$OUTPUT_FILE' is present."
else
    print_error "Output file '$OUTPUT_FILE' is missing."
fi

# Final success message
print_header "Process Completed"
echo -e "\033[1;34m🎉 All steps were completed successfully!🎉\033[0m\n"

# Return to initial directory
cd ..
