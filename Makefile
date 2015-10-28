all:    producer consumer producer_without_s consumer_without_s

producer.c: config.h semaphore.h
consumer.c: config.h semaphore.h
producer_without_s.c: config.h semaphore.h
consumer_without_s.c: config.h semaphore.h

clean:
	rm -f producer consumer producer_without_s consumer_without_s
