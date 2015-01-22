#include "queue.h"

struct QueueNode
{
    struct GNS* gns_ptr;
    struct QueueNode* next;
}*front,*rear,*temp,*front1;

void enqueue (struct GNS* gns_ptr);
struct GNS* dequeue ();
int isEmpty ();
void initQueue();

/* Create an empty queue */
void initQueue()
{
	front = rear = NULL;
}


/* Enqueing the queue */
void enqueue (struct GNS* gns_ptr)
{
	if (rear == NULL)
	{
		rear = malloc (sizeof (struct QueueNode));
		rear->next = NULL;
		rear->gns_ptr = gns_ptr;
		front = rear;
	}
	else
	{
		temp = malloc (sizeof( struct QueueNode));
		rear->next = temp;
		temp->gns_ptr = gns_ptr;
		temp->next = NULL;

		rear = temp;
	}
}


/* Dequeing the queue */
struct GNS* dequeue ()
{
	front1 = front;
	struct GNS* to_return;

	if (front1 == NULL)
	{
		printf("\n Error: Trying to display elements from empty queue");
		return NULL;
	}
	else
		if (front1->next != NULL)
		{
			front1 = front1->next;
			to_return = front->gns_ptr;
			free (front);
			front = front1;
		}
		else
		{
			to_return = front->gns_ptr;
			free (front);
			front = NULL;
			rear = NULL;
		}
	return to_return;
}

/* Display if queue is empty or not */
int isEmpty ()
{
	if ((front == NULL) && (rear == NULL))
		return TRUE;
	else
		return FALSE;
}
