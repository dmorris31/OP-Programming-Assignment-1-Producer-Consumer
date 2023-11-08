CC = g++
CFLAGS = -pthread -lrt
PRODUCER_SRC = producer.cpp
CONSUMER_SRC = consumer.cpp
PRODUCER_BIN = producer
CONSUMER_BIN = consumer

all: producer consumer

producer:
	$(CC) $(PRODUCER_SRC) $(CFLAGS) -o $(PRODUCER_BIN)

consumer:
	$(CC) $(CONSUMER_SRC) $(CFLAGS) -o $(CONSUMER_BIN)

clean:
	rm -f $(PRODUCER_BIN) $(CONSUMER_BIN)
