#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Define constants
#define MAX_QUEUE_SIZE 10

// Shared resources
int queue_size = 0;  // Number of bottles in the queue
pthread_mutex_t mutex;  // Mutex lock for synchronization
pthread_cond_t cond_producer;  // Condition variable for producer
pthread_cond_t cond_consumer;  // Condition variable for consumer

// ANSI color codes
#define COLOR_PRODUCER "\033[1;32m"  // Green
#define COLOR_CONSUMER "\033[1;34m"  // Blue
#define COLOR_RESET "\033[0m"        // Reset to default

// Producer function
void* producer(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        // Wait if the queue is full
        while (queue_size == MAX_QUEUE_SIZE) {
            printf(COLOR_PRODUCER "Producer: Queue is full. Waiting...\n" COLOR_RESET);
            pthread_cond_wait(&cond_producer, &mutex);
        }

        // Add a bottle to the queue
        queue_size++;
        printf(COLOR_PRODUCER "Producer: Added a bottle. Queue size: %d\n" COLOR_RESET, queue_size);

        // Signal the consumer that a bottle is available
        pthread_cond_signal(&cond_consumer);

        pthread_mutex_unlock(&mutex);
        sleep(2);  // Simulate production delay
    }
    return NULL;
}

// Consumer function
void* consumer(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        // Wait if the queue is empty
        while (queue_size == 0) {
            printf(COLOR_CONSUMER "Consumer: Queue is empty. Waiting...\n" COLOR_RESET);
            pthread_cond_wait(&cond_consumer, &mutex);
        }

        // Remove a bottle from the queue
        queue_size--;
        printf(COLOR_CONSUMER "Consumer: Removed a bottle. Queue size: %d\n" COLOR_RESET, queue_size);

        // Signal the producer that space is available
        pthread_cond_signal(&cond_producer);

        pthread_mutex_unlock(&mutex);
        sleep(3);  // Simulate consumption delay
    }
    return NULL;
}

// Main function
int main() {
    // Initialize mutex and condition variables
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_producer, NULL);
    pthread_cond_init(&cond_consumer, NULL);

    // Create threads for producer and consumer
    pthread_t producer_thread, consumer_thread;
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Wait for threads to finish (in this case, they run indefinitely)
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Destroy mutex and condition variables
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_producer);
    pthread_cond_destroy(&cond_consumer);

    return 0;
}
