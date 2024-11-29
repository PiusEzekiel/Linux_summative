#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

#define USAGE_FILE "usage.txt"
#define PROCESSES_FILE "processes.txt"

int keep_running = 1;

void handle_sigint(int sig) {
    keep_running = 0;
}

void write_metrics(FILE *usage_fp) {
    FILE *fp;
    char buffer[1024];
    time_t rawtime;
    struct tm *timeinfo;
    double cpu_usage = 0.0, mem_usage = 0.0;
    unsigned long net_rx = 0, net_tx = 0;

    // Timestamp
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    fprintf(usage_fp, "%02d:%02d:%02d ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

    // CPU Usage
    fp = fopen("/proc/stat", "r");
    if (fp) {
        unsigned long user, nice, system, idle;
        fscanf(fp, "cpu %lu %lu %lu %lu", &user, &nice, &system, &idle);
        fclose(fp);
        static unsigned long prev_user = 0, prev_nice = 0, prev_system = 0, prev_idle = 0;
        unsigned long total_diff = (user + nice + system + idle) - (prev_user + prev_nice + prev_system + prev_idle);
        unsigned long idle_diff = idle - prev_idle;
        cpu_usage = 100.0 * (1.0 - (double)idle_diff / total_diff);
        prev_user = user, prev_nice = nice, prev_system = system, prev_idle = idle;
    }
    fprintf(usage_fp, "%.2f ", cpu_usage);

    // Memory Usage
    fp = fopen("/proc/meminfo", "r");
    if (fp) {
        unsigned long total_mem, free_mem;
        fscanf(fp, "MemTotal: %lu kB\nMemFree: %lu kB\n", &total_mem, &free_mem);
        fclose(fp);
        mem_usage = 100.0 * (1.0 - (double)free_mem / total_mem);
    }
    fprintf(usage_fp, "%.2f ", mem_usage);

    // Network Usage
    fp = fopen("/proc/net/dev", "r");
    if (fp) {
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (strstr(buffer, "eth0:") || strstr(buffer, "wlan0:")) {
                sscanf(buffer, "%*s %lu %*u %*u %*u %*u %*u %*u %*u %lu", &net_rx, &net_tx);
                break;
            }
        }
        fclose(fp);
    }
    fprintf(usage_fp, "%lu %lu\n", net_rx / 1024, net_tx / 1024);
    fflush(usage_fp);
}

void list_processes() {
    FILE *fp = popen("ps -aux", "r");
    FILE *out_fp = fopen(PROCESSES_FILE, "w");
    char buffer[1024];

    if (fp && out_fp) {
        while (fgets(buffer, sizeof(buffer), fp)) {
            fprintf(out_fp, "%s", buffer);
        }
    }

    if (fp) pclose(fp);
    if (out_fp) fclose(out_fp);
}

int main() {
    signal(SIGINT, handle_sigint);

    FILE *usage_fp = fopen(USAGE_FILE, "w");
    if (!usage_fp) {
        perror("Failed to open usage.txt");
        return EXIT_FAILURE;
    }

    while (keep_running) {
        write_metrics(usage_fp);
        list_processes();
        sleep(2);
    }

    fclose(usage_fp);
    printf("Monitoring stopped.\n");
    return 0;
}
