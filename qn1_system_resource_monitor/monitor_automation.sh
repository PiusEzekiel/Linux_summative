#!/bin/bash

# Script to automate building, compiling, and running the programs

echo -e "\n🛠️ \033[1;32mStarting Build and Test Process...\033[0m\n"

# Define file names and directories
C_FILE="monitor.c"
SETUP_FILE="setup.py"
PYTHON_SCRIPT="plot_metrics.py"
BUILD_DIR="build"
OUTPUT_FILE="usage.txt"
PROCESS_FILE="processes.txt"

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
for FILE in "$C_FILE" "$SETUP_FILE" "$PYTHON_SCRIPT"; do
    if [ ! -f "$FILE" ]; then
        print_error "Required file '$FILE' is missing."
    else
        print_success "Found '$FILE'."
    fi
done
wait_time

# Step 2: Clean up existing build directory
print_header "Cleaning Build Directory"
if [ -d "$BUILD_DIR" ]; then
    rm -rf "$BUILD_DIR" && print_success "Removed existing build directory."
else
    print_success "No existing build directory found."
fi
wait_time

# Step 3: Compile the C program
print_header "Compiling the C Program"
if gcc "$C_FILE" -o monitor; then
    print_success "C program compiled successfully."
else
    print_error "Error during C program compilation."
fi
wait_time

# Step 4: Compile the C extension
print_header "Compiling the C Extension"
if python3 "$SETUP_FILE" build; then
    print_success "C extension compiled successfully."
else
    print_error "Error during C extension compilation."
fi
wait_time

# Step 5: Move the compiled .so file
print_header "Moving Compiled Shared Object (.so) File"
COMPILED_SO=$(find "$BUILD_DIR" -name "*.so" 2>/dev/null)
if [ -n "$COMPILED_SO" ]; then
    cp "$COMPILED_SO" . && print_success "Moved compiled .so file to the current directory."
else
    print_error "Compiled .so file not found."
fi
wait_time

# Step 6: Run the C monitor program
print_header "Running the Monitor Program"
if ./monitor & then
    MONITOR_PID=$!
    print_success "Monitor program is running in the background (PID: $MONITOR_PID)."
else
    print_error "Error during monitor program execution."
fi
wait_time


# Step 7: Wait for data collection
print_header "Collecting Data"
echo "Allowing the monitor program to gather data for 20 seconds..."
sleep 20
kill "$MONITOR_PID" && print_success "Stopped the monitor program."
wait_time

# Step 8: Verify output files
print_header "Verifying Output Files"
for FILE in "$OUTPUT_FILE" "$PROCESS_FILE"; do
    if [ -f "$FILE" ]; then
        print_success "Output file '$FILE' is present."
    else
        print_error "Output file '$FILE' is missing."
    fi
done
wait_time

# Step 9: Run the Python script
print_header "Running the Python Plotting Script"
if python3 "$PYTHON_SCRIPT"; then
    print_success "Python script executed successfully."
else
    print_error "Error during Python script execution."
fi
wait_time

# Final success message
print_header "Process Completed"
echo -e "\033[1;34m🎉 All steps were completed successfully!🎉\033[0m\n"
