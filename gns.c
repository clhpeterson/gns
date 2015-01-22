#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "queue.h"

struct GNS
{
	int** generators;
	int num_generators;
	int index_of_effective;
	int* frobenius;
	int** hole_set;
	struct GNS** children;
};

struct GNS* create_genus_zero (int d);

int main (int argc, char** argv)
{
	initQueue();
	struct GNS* new_gns = malloc (sizeof (struct GNS));
	new_gns->index_of_effective = 5;
	enqueue(new_gns);
	printf("%p %p\n", head, tail);
	new_gns = dequeue();
	printf ("%d\n", new_gns->index_of_effective);
	return 0;
}

struct GNS* create_genus_zero (int d)
{
	// obviously there will be d generators
	struct GNS* to_return = malloc (sizeof (struct GNS));
	int** generators = malloc (d*sizeof (int *));
	for (int i = 0; i < d; i++)
	{
		int* new_generator = calloc(d, sizeof (int));
		new_generator[i] = 1;
		generators[i] = new_generator;
	}
	to_return->generators = generators;
	to_return->num_generators = d;
	to_return->index_of_effective = 0;
	to_return->frobenius = NULL;
	to_return->hole_set = NULL;
	to_return->children = malloc (d*sizeof (struct GNS*));
	return to_return;
}
