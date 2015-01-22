#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

struct GNS;
static struct QueueNode* head;
static struct QueueNode* tail;


void initQueue();
void clearQueue();
void enqueue(struct GNS* gns_ptr);
struct GNS* dequeue();
int isEmpty();
