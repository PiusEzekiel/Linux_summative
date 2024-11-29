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

# Plot both graphs in a single output file
def plot_all(timestamps, cpu, memory, net_rx, net_tx, output_file="system_metrics_plot.png"):
    # Create subplots: 2 rows, 1 column
    fig, axes = plt.subplots(2, 1, figsize=(12, 10), sharex=True)

    # Plot CPU and Memory usage
    axes[0].plot(timestamps, cpu, label="CPU Usage (%)", marker="o")
    axes[0].plot(timestamps, memory, label="Memory Usage (%)", marker="x")
    axes[0].set_ylabel("Usage (%)")
    axes[0].set_title("CPU and Memory Usage Over Time")
    axes[0].legend()
    axes[0].grid()

    # Plot Network RX and TX
    axes[1].plot(timestamps, net_rx, label="Network RX (KB)", marker="o")
    axes[1].plot(timestamps, net_tx, label="Network TX (KB)", marker="x")
    axes[1].set_ylabel("Data (KB)")
    axes[1].set_xlabel("Time")
    axes[1].set_title("Network RX and TX Over Time")
    axes[1].legend()
    axes[1].grid()

    # Rotate X-axis labels for better readability
    plt.xticks(rotation=45)

    # Adjust layout and save the plot
    plt.tight_layout()
    plt.savefig(output_file)
    print(f"Combined plot saved as {output_file}")

# Main function
if __name__ == "__main__":
    data_file = "usage.txt"
    timestamps, cpu, memory, net_rx, net_tx = read_metrics(data_file)
    plot_all(timestamps, cpu, memory, net_rx, net_tx)
