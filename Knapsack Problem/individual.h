#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H
#include <pthread.h>
#include <stdio.h>

// structure for an individual in the genetic algorithm; the chromosomes are an array corresponding to each sack
// object, in order, where 1 means that the object is in the sack, 0 that it is not
typedef struct _individual {
	int fitness;
	int *chromosomes;
    int chromosome_length;
	int index;
} individual;

typedef struct _arguments_run_genetic_algorithm
{	
	individual *current_generation;
	individual *next_generation;
	individual *tmp;
	const sack_object *objects;
	int object_count;
	int generations_count;
	int sack_capacity;
	int thread_id;
	int nr_threads;
	pthread_barrier_t *bar;
} arguments_run_genetic_algorithm;

#endif