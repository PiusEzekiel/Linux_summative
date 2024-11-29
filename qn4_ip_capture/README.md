# **Qn4. IP Capture Program**

## **Overview**

The **IP Capture Program** is a client-server application designed to identify and log the IP addresses and hostnames of devices on the same network. It demonstrates the use of **socket programming** in C to facilitate network communication between a client and server. This project provides a practical way to track devices in a network, which can be useful for network administration tasks.

The project includes:
- A **C program** implementing the client-server IP capture logic.
- A **Bash script** to automate the building, execution, and testing of the program.
- Real-time logging of IP addresses and hostnames to a text file for analysis.

---

## **Features**

The IP Capture program provides the following functionality:
1. **Client Program**:
   - Scans the local network for connected devices.
   - Identifies the IP addresses and hostnames of all discovered nodes.
   - Sends the captured data to the server.

2. **Server Program**:
   - Listens for incoming data from the client.
   - Logs received IP addresses and hostnames to a text file.
   - Supports multiple client connections.

3. **Automation Script**:
   - Builds, compiles, and runs both the client and server programs.
   - Logs the server and client output to separate files for debugging and analysis.

4. **Network Scanning**:
   - Utilizes ICMP (ping) to discover devices in the local network.

5. **Synchronization**:
   - Ensures smooth communication between the client and server using TCP sockets.

---

## **Requirements**

To build and run the IP Capture program, ensure you have:
- **GCC**: GNU Compiler Collection for compiling the C program.
- **Linux-based or macOS system** (or Windows with WSL or MinGW) for socket support.
- Network access to test communication between the client and server.

---

## **Usage**

1. **Run the Automation Script**:  
   Use the `run_ip_capture.sh` script to compile, run, and test the program seamlessly:
   ```bash
   ./run_ip_capture.sh
   ```

2. **Monitor Output**:  
   - View real-time logs from the server and client in the terminal.
   - Review the logged output in `server_log.txt` and `client_log.txt` after the program completes.

---

## **How It Works**

### **Script Workflow**
The `run_ip_capture.sh` script automates the following steps:
1. **Check Required Files**:  
   Ensures the C source files (`server.c` and `client.c`) are present before proceeding.

2. **Clean Existing Build Directory**:  
   Deletes any previous build files to prevent conflicts and creates a fresh build directory.

3. **Compile the C Programs**:  
   - Compiles `server.c` to create the server executable.
   - Compiles `client.c` to create the client executable.

4. **Run the Server**:  
   Starts the server program in the background and logs its output to `server_log.txt`.

5. **Run the Client**:  
   Starts the client program, which scans the network and sends data to the server. Logs output to `client_log.txt`.

6. **Terminate Execution**:  
   Stops all running processes gracefully after a set duration.

7. **Verify Output Files**:  
   Confirms the presence of the server and client log files for further inspection.

---

### **File Structure**
- `server.c`: The C implementation of the server logic, responsible for logging IPs and hostnames.
- `client.c`: The C implementation of the client logic, responsible for discovering devices and sending data to the server.
- `test_automation.sh`: The automation script for building, running, and testing the program.
- `server_log.txt`: The log file capturing the server’s output.
- `client_log.txt`: The log file capturing the client’s output.

---

