# **Qn3. Client-Server Chat Program**

## **Overview**

The **Client-Server Chat Program** is a multi-threaded chat application implemented in C, enabling multiple clients to connect to a server and exchange messages in real-time. The application demonstrates the use of sockets for networking, threads for concurrency, and synchronization mechanisms for managing client interactions effectively.

The project includes:
- A **server program** to handle client connections and message routing.
- A **client program** for users to send and receive messages.
- Real-time display of connected users and chat messages.
- A **Bash script** to automate building, running, and testing the program.

---

## **Features**

The Client-Server Chat Program provides the following functionality:

### **Server Features**:
1. **Multi-threaded Design**:
   - Handles multiple client connections simultaneously using threads.
2. **User Management**:
   - Tracks active users and broadcasts the list to all connected clients.
3. **Message Routing**:
   - Routes messages from a sender to the intended recipient.
4. **Error Handling**:
   - Notifies users if they attempt to send messages to non-existent recipients.

### **Client Features**:
1. **Interactive Interface**:
   - Allows users to send messages and view received messages in real time.
2. **User-Friendly Prompts**:
   - Styled output using ANSI color codes for enhanced readability.
3. **Message Validation**:
   - Prevents users from sending messages to themselves or sending blank messages.

### **Automation Script**:
- Builds, compiles, and runs the server and client programs.
- Automates testing by simulating multiple clients connecting to the server.

---

## **Requirements**

To build and run the Client-Server Chat Program, ensure you have:
- **GCC**: GNU Compiler Collection for compiling the C programs.
- **Linux-based or macOS system** (or Windows with WSL or MinGW) for socket and thread support.
- A terminal with support for ANSI color codes for styled output.

---

## **Usage**

1. **Start the Server**:
   Compile and run the server program:
   ```bash
   gcc -pthread server.c -o server
   ./server
   ```

2. **Start the Client**:
   Compile and run the client program:
   ```bash
   gcc -pthread client.c -o client
   ./client
   ```

3. **Interact with the Chat**:
   - Enter a username when prompted.
   - Send messages to other users by specifying their usernames.

---

## **How It Works**

### **Server Workflow**:
1. **Accepts Connections**:
   - Listens for incoming connections from clients.
2. **Manages Clients**:
   - Tracks connected clients and their usernames.
3. **Routes Messages**:
   - Receives messages from a client and forwards them to the specified recipient.
4. **Broadcasts Active Users**:
   - Sends a list of active users to all connected clients whenever a user joins or leaves.

### **Client Workflow**:
1. **Connects to Server**:
   - Establishes a connection with the server.
2. **Interacts in Real-Time**:
   - Sends and receives messages from other connected clients.
3. **Displays Active Users**:
   - Receives and displays the list of currently connected users from the server.

### **Script Workflow**:
The automation script performs the following steps:
1. **Check Required Files**:
   - Verifies the presence of `server.c` and `client.c`.
2. **Compile Programs**:
   - Compiles the server and client programs using `gcc`.
3. **Run the Server**:
   - Starts the server in the background.
4. **Run Multiple Clients**:
   - Launches multiple client instances and simulates messaging.
5. **Monitor Output**:
   - Streams server and client logs to the terminal and saves them to log files.

---

## **File Structure**

- **server.c**: The server-side implementation of the chat application.
- **client.c**: The client-side implementation of the chat application.
- **run_chat.sh**: Automation script to build, run, and test the chat program.
- **server_log.txt**: Log file capturing the server's activity.
- **client_log.txt**: Log file capturing a single client's activity.

---

