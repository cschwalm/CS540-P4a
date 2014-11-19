#include "cs537.h"
#include "request.h"
#include "queue.h"

// 
// server.c: A very, very simple web server
//
// To run:
//  server <portnum (above 2000)>
//
// Repeatedly handles HTTP requests sent to this port number.
// Most of the work is done within routines written in request.c
//

void* workerLoop(void* param);


queue_t buff;
pthread_cond_t fill, use;
pthread_mutex_t mutex;

// CS537: Parse the new arguments too
void getargs(int *port, int argc, char *argv[], int *threads, int *buffers)
{
  if (argc != 4) {
	fprintf(stderr, "Usage: %s <port> <threads> <buffers>\n", argv[0]);
	exit(1);
    }
    *port = atoi(argv[1]);
		*threads = atoi(argv[2]);
		*buffers = atoi(argv[3]);
}


int main(int argc, char *argv[])
{
    int listenfd, connfd, port, clientlen, threads, buffers, i;
    struct sockaddr_in clientaddr;

    getargs(&port, argc, argv, &threads, &buffers);

		pthread_t workers[threads];

		pthread_mutex_init(&mutex, NULL);
    // 
    // CS537: Create some threads...
    //
   
		Queue_Init(&buff, buffers);
	
		for (i = 0; i < threads; i++)
		{
			pthread_create(&workers[i], NULL, workerLoop, NULL);
		}
		
    listenfd = Open_listenfd(port);
    while (1) {
			clientlen = sizeof(clientaddr);
			connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);

			// 
			// CS537: In general, don't handle the request in the main thread.
			// Save the relevant info in a buffer and have one of the worker threads 
			// do the work.
			// 
			//requestHandle(connfd);
			pthread_mutex_lock(&mutex);
			while(Queue_Enqueue(&buff, connfd) == -1)
			{
				pthread_cond_wait(&use, &mutex);
			}
			pthread_cond_signal(&fill);
			pthread_mutex_unlock(&mutex);
			//Close(connfd);
    }

}


void* workerLoop(void* param)
{
  int fd;
  while(1)
  {
    pthread_mutex_lock(&mutex);
    while (Queue_Dequeue(&buff, &fd) == -1)
    {
      pthread_cond_wait(&fill, &mutex);
    }
    requestHandle(fd);
    Close(fd);
    pthread_cond_signal(&use);
    pthread_mutex_unlock(&mutex);
  }

  pthread_exit(NULL);
  return NULL;
}



 
