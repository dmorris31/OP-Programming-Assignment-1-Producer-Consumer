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

    // Create and set up a shared memory region for the shared buffer
    int shm_fd = shm_open("/shared_memory", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(SharedBuffer));
    SharedBuffer *buffer = static_cast<SharedBuffer*>(mmap(NULL, sizeof(SharedBuffer), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));

    // Initialize semaphores to control access to the shared buffer
    sem_init(&buffer->mutex, 1, 1);   // Mutex semaphore for exclusive access
    sem_init(&buffer->empty, 1, BUFFER_SIZE);  // Semaphore to track available empty slots
    sem_init(&buffer->full, 1, 0);   // Semaphore to track the number of filled slots

    // Define a producer function to run in a separate thread
    auto producer = [&]() {
        int item;
        while (true) {
            item = rand() % 100;  // Produce a random item

            sem_wait(&buffer->empty);  // Wait if there are no empty slots in the buffer
            sem_wait(&buffer->mutex);  // Acquire the mutex to access the buffer safely

            // Find an empty slot in the buffer and store the produced item
            for (int i = 0; i < BUFFER_SIZE; i++) {
                if (buffer->buffer[i] == 0) {
                    buffer->buffer[i] = item;
                    break;
                }
            }

            std::cout << "Produced: " << item << std::endl;  // Print the produced item

            sem_post(&buffer->mutex);  // Release the mutex
            sem_post(&buffer->full);   // Signal that a slot is filled in the buffer

            sleep(rand() % 3);  // Sleep for a random time to simulate variable processing time
        }
    };

    std::thread producerThread(producer);  // Create a producer thread

    producerThread.join();  // Wait for the producer thread to finish

    return 0;
}
