# Producer-Consumer-CS-33211


# Producer-Consumer Problem with Shared Memory and Semaphores

This is a C++ program that demonstrates the Producer-Consumer problem using shared memory and semaphores in a Linux/Unix environment. The producer generates items and places them in a shared buffer, while the consumer removes items from the buffer. This implementation ensures mutual exclusion and proper synchronization between the producer and consumer.

# How to Compile:

To compile the producer and consumer programs, use the provided Makefile:

make

This command will build the producer and consumer executables.


# How to Run:

After compilation, you can run the producer and consumer programs in the same terminal. Open a terminal and navigate to the directory containing the executables, then run:

./producer & ./consumer &

This command starts the producer and consumer processes in the background.


# Implementation Details

   - The producer and consumer programs use shared memory to create a common buffer accessible by both processes.
   - Semaphores (mutex, empty, and full) are employed to ensure proper synchronization between the producer and consumer.
   - The producer generates random items and adds them to the buffer.
   - The consumer retrieves items from the buffer.
   - The buffer has a maximum size of 2 items, and the producer/consumer sleep for random intervals to simulate real-world scenarios.

# Author

Devon Morris
