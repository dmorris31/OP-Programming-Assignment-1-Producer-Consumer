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

    int shm_fd = shm_open("/shared_memory", O_RDWR, 0666);
    SharedBuffer *buffer = static_cast<SharedBuffer*>(mmap(NULL, sizeof(SharedBuffer), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));

    auto consumer = [&]() {
        int item;
        while (true) {
            sem_wait(&buffer->full);
            sem_wait(&buffer->mutex);

            for (int i = 0; i < BUFFER_SIZE; i++) {
                if (buffer->buffer[i] != 0) {
                    item = buffer->buffer[i];
                    buffer->buffer[i] = 0;
                    break;
                }
            }

            std::cout << "Consumed: " << item << std::endl;

            sem_post(&buffer->mutex);
            sem_post(&buffer->empty);

            sleep(rand() % 3);  // Sleep for a random time
        }
    };

    std::thread consumerThread(consumer);

    consumerThread.join();

    return 0;
}
