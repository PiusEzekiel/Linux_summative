# **Qn3. Producer-Consumer Program**

## **Overview**

The **Producer-Consumer Program** demonstrates the use of multi-threading in C to simulate a real-world producer-consumer model. It leverages threading, mutexes, and condition variables to synchronize the producer and consumer threads while managing a shared queue with a fixed size.

The project includes:
- A **C program** implementing the producer-consumer logic.
- A **Bash script** to automate the building, execution, and testing of the program.
- Real-time console output display and log generation for performance analysis.

---

## **Features**

The Producer-Consumer program provides the following functionality:
1. **Producer Thread**:
   - Produces items (bottles) at a fixed interval of 2 seconds.
   - Adds items to the shared queue, halting production if the queue is full.
2. **Consumer Thread**:
   - Consumes items (bottles) at a fixed interval of 3 seconds.
   - Removes items from the queue, waiting if the queue is empty.
3. **Synchronization**:
   - Uses mutexes and condition variables to synchronize thread access to the queue.
4. **Automation Script**:
   - Builds, compiles, and runs the program.
   - Displays real-time output while logging to a file.
5. **Queue Management**:
   - Fixed-size queue (maximum capacity of 10 items) managed by the producer and consumer.

---

## **Requirements**

To build and run the Producer-Consumer program, ensure you have:
- **GCC**: GNU Compiler Collection for compiling the C program.
- **Linux-based or macOS system** (or Windows with WSL or MinGW) for thread support.
- A terminal with support for ANSI color codes to display styled output.

---

## **Usage**

1. **Run the Automation Script**:  
   Use the `run_producer_consumer.sh` script to compile, run, and test the program seamlessly:
   ```bash
   ./run_producer_consumer.sh
   ```
 
2. **Monitor Output**:  
   - View real-time console output displaying the producer and consumer activities.
   - Review the logged output in `producer_consumer_output.txt` after the program terminates.

---

## **How It Works**

### **Script Workflow**
The `run_producer_consumer.sh` script automates the following steps:
1. **Check Required Files**:  
   Ensures the C source file (`producer_consumer.c`) is present before proceeding.

2. **Clean Existing Build Directory**:  
   Deletes any previous build files to prevent conflicts and creates a fresh build directory.

3. **Compile the C Program**:  
   Compiles `producer_consumer.c` with `gcc`, enabling multi-threading via the `-pthread` flag.

4. **Run the Program**:  
   Executes the compiled program, streams real-time output to the console, and logs it to `producer_consumer_output.txt`.

5. **Terminate Execution**:  
   Runs the program for 20 seconds, then terminates the process gracefully.

6. **Verify Output File**:  
   Confirms the presence of the output log file (`producer_consumer_output.txt`).

### **File Structure**
- `producer_consumer.c`: The C implementation of the producer-consumer logic.
- `run_producer_consumer.sh`: The automation script for building, running, and testing the program.
- `producer_consumer_output.txt`: The log file capturing program output.

---


