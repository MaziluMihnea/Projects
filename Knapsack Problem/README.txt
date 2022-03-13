Mazilu Mihnea Mihai 333AA

Pentru rezolvarea temei am incercat sa paralelizez cat mai multe for-uri si am inlocuit de asemenea functia 
de sort cu algoritmul OETS implementat in laborator.

Primul lucru pe care l-am facut a fost sa creez o structura cu toate variabilele necesare rularii functiei 
run_genetic_algorithm. Aceasta structura am realizat-o in individual.h si are urmatoarea forma:

typedef struct _arguments_run_genetic_algorithm
{	
	individual *current_generation;    //variabila pentru generatia curenta
	individual *next_generation;	   //variabila pentru generatia urmatoare
	individual *tmp;		   //variabila temporara de care ma voi ajuta pentru interschimbarea datelor
	const sack_object *objects; 	   //obiectele
	int object_count;		   //numarul obiectelor
	int generations_count;	           //numarul generatiilor
	int sack_capacity;		   //capacitatea sacului
	int thread_id;			   //id-ul thread-ului care executa o anumita parte de cod
	int nr_threads; 		   //numarul thread-urilor
	pthread_barrier_t *bar;	   	   //bariera pe care o voi folosi
} arguments_run_genetic_algorithm;

In main tot ce am facut a fost sa imi declar varibilele si sa fac un vector "arguments_run_genetic_algorithm *v;"
pentru numarul meu de thread-uri. Am apelat pthread_create pentru functia mea run_genetic_algorithm, dupa care am 
apelat pthread_join. Am distrus bariera si am eliberat memoria.

Majoritatea schimbarilor au avut loc in genetic_algoritm.c. Aici am incercat sa paralelizez cat mai multe for-uri
posibile pentru a avea o acceleratie cat mai mare. Am vrut sa paralelizez si functiile de print dar dupa ce am para-
lelizat functia print_objects am observat ca nu este nevoie si m-am oprit.

Urmatoarea functie paralelizata a fost compute_fitness_function. I-am adaugat ca argumente un int start si un int end,
pe care le-am folosit la primul for al functiei. Startul si endul au fost calculate in functia run_genetic_algorithm 
astfel:
	start = str.thread_id * (double)str.object_count / str.nr_threads;
	end = min((str.thread_id + 1) * (double)str.object_count / str.nr_threads, str.object_count);

Dupa aceea am paralelizat majoritatea for-urilor din functia run_genetic_algorithm astfel:
	- primul for (linia 207) tot cu start-ul si end-ul de mai sus
	- bariera pentru ca toate thread-urile sa isi termine treaba
	- urmatoarea functie apelata este cumpute_fitness_function (deja paralelizata)
	- bariera deoarece urmeaza sortarea
	- sortarea este OETS si este cea facuta in laborator
	- paralelizarea for-ului de linia 284 cu alt start si end (calculate pe baza unui "count"
	- bariera
	- paralelizarea for-ului de la 298 cu alt start si end (pentru nou-ul count)
	- nu am nevoie de o bariera intre urmatoarele 2 for-uri pentru ca nu am zona critica (unul e pentru primii 20%
	iar urmatorul pentru urmatorii 20% deci nu se intercaleaza)
	- bariera
	- crossover-ul l-am facut pe un singur thread
	- bariera pentru ca toate thread-urile sa astepte finalizarea functiei crossover
	- schimbarea dintre current_generation si next_generation o fac tot pe un singur thread
	- bariera la sfarsitul for-ului pentru ca toate thread-urile sa termine o iteratie si apoi sa o inceapa pe urmatoarea
	- din nou compute_fitness_function (paralelizat) cu un nou start si end
	- bariera
	- qsort pe un singur thread
	- bariera
	- print_best_fitness pe un singur thread 

