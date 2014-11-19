#ifndef __QUEUE_H__

#define __QUEUE_H__
typedef struct __node_t {

  int fd;
  struct __node_t *next;
} node_t;

typedef struct __queue_t {

  node_t *head;
  node_t *tail;
  pthread_mutex_t headLock;
  pthread_mutex_t tailLock;
  int size;
  int maxsize;
} queue_t;

void Queue_Init(queue_t *q, int maxsize);

int Queue_Enqueue(queue_t *q, int value);

int Queue_Dequeue(queue_t *q, int *value);

#endif
