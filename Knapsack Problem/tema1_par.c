#include <stdlib.h>
#include "genetic_algorithm.h"

int main(int argc, char *argv[]) 
{	

	int nr_threads;
	void *status;
	int r;
	
	// array with all the objects that can be placed in the sack
	sack_object *objects = NULL;

	// number of objects
	int object_count = 0;

	// maximum weight that can be carried in the sack
	int sack_capacity = 0;

	// number of generations
	int generations_count = 0;

	if (!read_input(&objects, &object_count, &sack_capacity, &generations_count, argc, argv)) {
		return 0;
	}

	individual *current_generation = (individual*) calloc(object_count, sizeof(individual));
	individual *next_generation = (individual*) calloc(object_count, sizeof(individual));
	 

	nr_threads = atoi(argv[3]);
	pthread_t threads[nr_threads];

	arguments_run_genetic_algorithm *v;
	v = malloc (object_count*sizeof(arguments_run_genetic_algorithm));

	pthread_barrier_t bar;
	pthread_barrier_init (&bar, NULL, nr_threads);

	for (int i = 0; i < nr_threads; i++)
	{	
		v[i].thread_id = i;
		v[i].objects = objects;
		v[i].object_count = object_count;
		v[i].generations_count = generations_count;
		v[i].sack_capacity = sack_capacity;
		v[i].nr_threads = nr_threads;
		v[i].current_generation = current_generation;
		v[i].next_generation = next_generation;
		v[i].tmp = NULL;
		v[i].bar = &bar;

		r = pthread_create(&threads[i], NULL, run_genetic_algorithm, &v[i]);
		if (r)
		{
            printf("Eroare la crearea thread-ului %d\n", i);
            exit(-1);
        }
	}

	for (int i = 0; i < nr_threads; i++)
	{
        r = pthread_join(threads[i], &status);
		if (r)
		{
            printf("Eroare la asteptarea thread-ului %d\n", i);
            exit(-1);
        }
    }
	
	pthread_barrier_destroy(&bar);
	free(objects);

	// free resources
	free(current_generation);
	free(next_generation);

	return 0;
}
