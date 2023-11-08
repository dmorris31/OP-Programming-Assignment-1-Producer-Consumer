#include <iostream>
#include <thread>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

const int BUFFER_SIZE = 2;

struct SharedBuffer {
    int buffer[BUFFER_SIZE];  // Shared buffer to store data
    sem_t mutex, empty, full;  // Semaphores for synchronization
};

int main() {
    srand(time(nullptr));  // Initialize random number generator

    // Open and map a shared memory region for the shared buffer
    int shm_fd = shm_open("/shared_memory", O_RDWR, 0666);
    SharedBuffer *buffer = static_cast<SharedBuffer*>(mmap(NULL, sizeof(SharedBuffer), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));

    // Define a consumer function to run in a separate thread
    auto consumer = [&]() {
        int item;
        while (true) {
            sem_wait(&buffer->full);  // Wait if the buffer is empty
            sem_wait(&buffer->mutex);  // Acquire the mutex to access the buffer safely

            // Iterate through the buffer to find and consume an item
            for (int i = 0; i < BUFFER_SIZE; i++) {
                if (buffer->buffer[i] != 0) {
                    item = buffer->buffer[i];
                    buffer->buffer[i] = 0;  // Mark the consumed item as 0
                    break;
                }
            }

            std::cout << "Consumed: " << item << std::endl;  // Print the consumed item

            sem_post(&buffer->mutex);  // Release the mutex
            sem_post(&buffer->empty);  // Signal that an empty slot is available in the buffer

            sleep(rand() % 3);  // Sleep for a random time to simulate variable processing time
        }
    };

    std::thread consumerThread(consumer);  // Create a consumer thread

    consumerThread.join();  // Wait for the consumer thread to finish

    return 0;
}
