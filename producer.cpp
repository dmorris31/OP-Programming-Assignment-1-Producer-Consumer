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
    int buffer[BUFFER_SIZE];
    sem_t mutex, empty, full;
};

int main() {
    srand(time(nullptr));
    
    int shm_fd = shm_open("/shared_memory", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(SharedBuffer));
    SharedBuffer *buffer = static_cast<SharedBuffer*>(mmap(NULL, sizeof(SharedBuffer), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));
    
    sem_init(&buffer->mutex, 1, 1);
    sem_init(&buffer->empty, 1, BUFFER_SIZE);
    sem_init(&buffer->full, 1, 0);

    auto producer = [&]() {
        int item;
        while (true) {
            item = rand() % 100;  // Produce a random item
            sem_wait(&buffer->empty);
            sem_wait(&buffer->mutex);

            for (int i = 0; i < BUFFER_SIZE; i++) {
                if (buffer->buffer[i] == 0) {
                    buffer->buffer[i] = item;
                    break;
                }
            }

            std::cout << "Produced: " << item << std::endl;

            sem_post(&buffer->mutex);
            sem_post(&buffer->full);

            sleep(rand() % 3);  // Sleep for a random time
        }
    };

    std::thread producerThread(producer);

    producerThread.join();

    return 0;
}
