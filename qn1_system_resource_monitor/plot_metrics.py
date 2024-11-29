import matplotlib.pyplot as plt

# Read metrics from the file
def read_metrics(file_path):
    timestamps, cpu, memory, net_rx, net_tx = [], [], [], [], []
    with open(file_path, "r") as f:
        for line in f:
            parts = line.strip().split()
            if len(parts) < 5:
                continue
            timestamps.append(parts[0])  # Keep timestamp as a string for simplicity
            cpu.append(float(parts[1]))
            memory.append(float(parts[2]))
            net_rx.append(int(parts[3]))
            net_tx.append(int(parts[4]))
    return timestamps, cpu, memory, net_rx, net_tx

# Plot metrics
def plot_metrics(timestamps, cpu, memory, net_rx, net_tx, output_file="metrics_plot.png"):
    plt.figure(figsize=(10, 6))
    plt.plot(timestamps, cpu, label="CPU Usage (%)")
    plt.plot(timestamps, memory, label="Memory Usage (%)")
    plt.plot(timestamps, net_rx, label="Network RX (KB)")
    plt.plot(timestamps, net_tx, label="Network TX (KB)")
    plt.xlabel("Time")
    plt.ylabel("Usage")
    plt.title("System Resource Usage")
    plt.legend()
    plt.grid()

    # Save the plot to a file
    plt.savefig(output_file)
    print(f"Plot saved as {output_file}")

# Main function
if __name__ == "__main__":
    data_file = "usage.txt"
    timestamps, cpu, memory, net_rx, net_tx = read_metrics(data_file)
    plot_metrics(timestamps, cpu, memory, net_rx, net_tx)
