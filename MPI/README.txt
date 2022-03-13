//Partea 1
--Voi exemplifica cu secvente de cod din Master0 tinand sa comentez ca aceasta parte este aproape identica pt toti coordonatorii
Pentru rezolvarea topologiei am urmat urmatoarea logica. 
Fiecare coordonator a citit numarul de workeri pe care il are, apoi l-a trimis celorlalte procese coordonatoare. 

//citire din fisier a nr de workeri
        FILE *fisier0 = fopen("cluster0.txt", "rt");
        fscanf(fisier0, "%d", &nr_procese[Master0]);
        procese_Master0 = (int*)malloc(sizeof(int)*nr_procese[Master0]);

//trimit nr de workeri ai lui master0 la ceilalti coordonatori
        MPI_Send(&nr_procese[Master0], 1, MPI_INT, Master1, tag01, MPI_COMM_WORLD);

//primesc nr de workeri de la ceilalti coordonatori
        MPI_Recv(&nr_procese[Master1], 1, MPI_INT, Master1, tag01, MPI_COMM_WORLD, &status);

In programul meu procesele coordonatoare se numesc Master. 
Procesele Master citesc apoi din fisier si stocheaza intr-un vector rank urile proceselor worker specifice
lor si trimit accest vector spre ceilalti masteri.

//citesc workerii specifici coordonatorului
        for(int i = 0; i < nr_procese[Master0]; i++)
            fscanf(fisier0, "%d", &procese_Master0[i]);

//trimit workerii celorlalti coordonatori
        MPI_Send(procese_Master0, nr_procese[Master0], MPI_INT, Master1, tag01, MPI_COMM_WORLD);

//primesc vectorii de workeri de la ceilalti coordonatori
        MPI_Recv(procese_Master1, nr_procese[Master1], MPI_INT, Master1, tag01, MPI_COMM_WORLD, &status);

Dupa acest proces fiecare master stie topologia completa si o va trimite catre workerii sai.

//Trimiterea topologiei catre workeri
for(int i = 0; i < nr_procese[Master0]; i++)
        {   
		//trimiterea numarul de procese al fiecarui Master
            MPI_Send(&nr_procese[Master0], 1, MPI_INT, procese_Master0[i], tag0, MPI_COMM_WORLD);
            MPI_Send(&nr_procese[Master1], 1, MPI_INT, procese_Master0[i], tag0, MPI_COMM_WORLD);
            MPI_Send(&nr_procese[Master2], 1, MPI_INT, procese_Master0[i], tag0, MPI_COMM_WORLD);
		//trimiterea vectorilor efectivi
            MPI_Send(procese_Master0, nr_procese[Master0], MPI_INT, procese_Master0[i], tag0, MPI_COMM_WORLD);
            MPI_Send(procese_Master1, nr_procese[Master1], MPI_INT, procese_Master0[i], tag0, MPI_COMM_WORLD);
            MPI_Send(procese_Master2, nr_procese[Master2], MPI_INT, procese_Master0[i], tag0, MPI_COMM_WORLD);
        }

Apoi ramane doar de afisat.


//Partea a 2-a
Pentru partea a 2-a gandirea a fost urmatoarea: Voi imparti numarul de elemente N al vectorului la numarul de workeri si voi
trimite fiecarui worker numarul de elemente rezultat. In cazul impartirii cu rest al lui N la nr de workeri, surplusul il
voi trimite ultimului worker al coordonatorului 2.

--In Master0 am implementat:
Variabila size este variabila in care am salvat numarul de elemente minim pe care ar trebui sa il faca un worker:
size = N / nr_procese_total;
Aceasta variabila am trimis-o fiecarui master iar apoi fiecarui worker.

Am calculat si stocat marimea fiecarui vector atribuit unui master cu urmatoarea formula size * nr_procese[Master#], cu exceptia
ultimului Master unde am adaugat si surplusul (size * nr_procese[Master2]) + (N % nr_procese_total).
Am transmis marimile coordonatorilor asociati.

Cu ajutorul unui for am transmis celorlalti coordonatori partea din vector de care sunt responsabili:
//Trimit vectorii task-urilor master
        for(int i = 0; i < Sizes[Master1]; i++)
            MPI_Send(&V[i+Sizes[Master0]], 1, MPI_INT, Master1, tag01, MPI_COMM_WORLD);
Am pus i+Sizes[Master0] pentru a sari peste partea lui Master0. Acelasi lucru l-am facut si pentru Master2 doar ca am adaugat
Sizes[Master0]+Sizes[Master1]

Urmatorul lucru pe care l-am facut a fost sa trimit vectorul specific fiecarui master workerilor lui.
//Trimit vectorii task-urilor worker
        int index = 0;  //variabila pe care o cresc cu size la sf unei iteratii ca
                        //sa trimtit din size in size worker-ilor

        for(int i = 0; i < nr_procese[Master0]; i++)
        {
            for(int j = 0 + index; j < size + index; j++)
            {
                MPI_Send(&V_Master0[j], 1, MPI_INT, procese_Master0[i], tag0, MPI_COMM_WORLD);
                printf("M(%d,%d)\n", Master0, procese_Master0[i]);
            }
            index = index + size;
        }

Index este o variabila care ma ajuta sa sar peste partea deja trimisa.

Aceeasi logica o au si Master1 si Master2 dar nu contin calculele sau declararea vectorului V.
O schimbare mare ar fi in Master2 unde pentru ultimul worker al sau trimit un size diferit si astfel trimit mai multe elemente.

for(int j = 0 + index; j < size_aux + index; j++)
      	MPI_Send(&V_Master2[j], 1, MPI_INT, procese_Master2[k], tag2, MPI_COMM_WORLD);

In procesele worker am primit size-ul de la fiecare master si elementele din vector alocate lor, am dublat si am trimis inapoi.




