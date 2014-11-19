#include "cs537.h"
#include "queue.h"

void Queue_Init(queue_t *q, int maxsize) {

	q->maxsize = maxsize;
	q->size = 0;
	node_t *tmp = malloc(sizeof(node_t));
	tmp->next = NULL;
	q->head = q->tail = tmp;
	pthread_mutex_init(&q->headLock, NULL);
	pthread_mutex_init(&q->tailLock, NULL);

}

int Queue_Enqueue(queue_t *q, int value) {

	node_t *tmp = malloc(sizeof(node_t));
	if(tmp == NULL)
		return -2;
	tmp->fd = value;
	tmp->next = NULL;
	if (q->size == q->maxsize)
		return -1;
	pthread_mutex_lock(&q->tailLock);
	q->tail->next = tmp;
	q->tail = tmp;
	q->size++;
	pthread_mutex_unlock(&q->tailLock);
	return 0;
}

int Queue_Dequeue(queue_t *q, int *value) {

	pthread_mutex_lock(&q->headLock);
	node_t *tmp = q->head;
	node_t *newHead = tmp->next;

	if (newHead == NULL) {
		pthread_mutex_unlock(&q->headLock);
		return -1; // queue was empty
	}
	
	*value = newHead->fd;
	q->head = newHead;
	q->size--;
	pthread_mutex_unlock(&q->headLock);
	free(tmp);
	return 0;
}
