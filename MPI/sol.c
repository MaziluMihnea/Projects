#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

#define Master0 0
#define Master1 1
#define Master2 2

#define tag01 0  //canal comunicatie intre procesul 0 si procesul 1
#define tag02 1  //canal comunicatie intre procesul 0 si procesul 2
#define tag12 2  //canal comunicatie intre procesul 1 si procesul 2

#define tag0 3  //canal de comunicatie intre procesul master0 si workerii sai
#define tag1 4  //canal de comunicatie intre procesul master1 si workerii sai
#define tag2 5  //canal de comunicatie intre procesul master2 si workerii sai

int main (int argc, char *argv[])
{
    int numtasks, rank;
    int nr_procese[3];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status status;

    int *procese_Master0;
    int *procese_Master1;  
    int *procese_Master2;

    if(rank == Master0)
    {   
        //citire din fisier a nr de workeri
        FILE *fisier0 = fopen("cluster0.txt", "rt");
        fscanf(fisier0, "%d", &nr_procese[Master0]);
        procese_Master0 = (int*)malloc(sizeof(int)*nr_procese[Master0]);

        //trimit nr de workeri ai lui master0 la ceilalti coordonatori
        MPI_Send(&nr_procese[Master0], 1, MPI_INT, Master1, tag01, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", Master0, Master1);
        MPI_Send(&nr_procese[Master0], 1, MPI_INT, Master2, tag02, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", Master0, Master2);

        //primesc nr de workeri de la ceilalti coordonatori
        MPI_Recv(&nr_procese[Master1], 1, MPI_INT, Master1, tag01, MPI_COMM_WORLD, &status);
        MPI_Recv(&nr_procese[Master2], 1, MPI_INT, Master2, tag02, MPI_COMM_WORLD, &status);
        
        //citesc workerii specifici coordonatorului
        for(int i = 0; i < nr_procese[Master0]; i++)
            fscanf(fisier0, "%d", &procese_Master0[i]);
        
        // printf("%d : p0 are %d\n", rank, nr_procese[Master0]);
        // printf("%d : p1 are %d\n", rank, nr_procese[Master1]);
        // printf("%d : p2 are %d\n", rank, nr_procese[Master2]);

        procese_Master1 = (int*)malloc(sizeof(int)*nr_procese[Master1]);
        procese_Master2 = (int*)malloc(sizeof(int)*nr_procese[Master2]);

        //trimit workerii celorlalti coordonatori
        MPI_Send(procese_Master0, nr_procese[Master0], MPI_INT, Master1, tag01, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", Master0, Master1);
        MPI_Send(procese_Master0, nr_procese[Master0], MPI_INT, Master2, tag02, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", Master0, Master2);

        //primesc vectorii de workeri de la ceilalti coordonatori
        MPI_Recv(procese_Master1, nr_procese[Master1], MPI_INT, Master1, tag01, MPI_COMM_WORLD, &status);
        MPI_Recv(procese_Master2, nr_procese[Master2], MPI_INT, Master2, tag02, MPI_COMM_WORLD, &status);

        //afisez topologia
        printf("%d ->", rank);
        printf(" %d:", Master0);
        for(int i = 0; i < nr_procese[Master0]; i++)
        {
            printf("%d", procese_Master0[i]);
            if(i != nr_procese[Master0] - 1)
                printf(",");
        }
        printf(" %d:", Master1);
        for(int i = 0; i < nr_procese[Master1]; i++)
        {
            printf("%d", procese_Master1[i]);
            if(i != nr_procese[Master1] - 1)
                printf(",");
        }
        printf(" %d:", Master2);
        for(int i = 0; i < nr_procese[Master2]; i++)
        {
            printf("%d", procese_Master2[i]);
            if(i != nr_procese[Master2] - 1)
                printf(",");
        }
        printf("\n");

                        // for(int i = 0; i < nr_procese[Master0]; i++)
                        //     printf("P[%d] are procesul: %d\n", rank, procese_Master0[i]);

                        // for(int i = 0; i < nr_procese[Master1]; i++)
                        //     printf("P[%d] are procesul: %d\n", rank+1, procese_Master1[i]);

                        // for(int i = 0; i < nr_procese[Master2]; i++)
                        //     printf("P[%d] are procesul: %d\n", rank+2, procese_Master2[i]);

        //Trimiterea topologiei catre workeri
        for(int i = 0; i < nr_procese[Master0]; i++)
        {
            MPI_Send(&nr_procese[Master0], 1, MPI_INT, procese_Master0[i], tag0, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", Master0, procese_Master0[i]);
            MPI_Send(&nr_procese[Master1], 1, MPI_INT, procese_Master0[i], tag0, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", Master0, procese_Master0[i]);
            MPI_Send(&nr_procese[Master2], 1, MPI_INT, procese_Master0[i], tag0, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", Master0, procese_Master0[i]);

            MPI_Send(procese_Master0, nr_procese[Master0], MPI_INT, procese_Master0[i], tag0, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", Master0, procese_Master0[i]);
            MPI_Send(procese_Master1, nr_procese[Master1], MPI_INT, procese_Master0[i], tag0, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", Master0, procese_Master0[i]);
            MPI_Send(procese_Master2, nr_procese[Master2], MPI_INT, procese_Master0[i], tag0, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", Master0, procese_Master0[i]);

        }

        //Partea de calcule
        int N = atoi(argv[1]);
        int *V;
        V = (int*)malloc(sizeof(int)*N);

        for(int i = 0; i < N; i++)
            V[i] = i;

        //calculez nr de workeri total
        int nr_procese_total = 0;
        for(int i = 0; i < 3; i++)
            nr_procese_total = nr_procese_total + nr_procese[i];
        
        //calculez cate elemente din vectorul V trebuie sa aiba fiecare worker
        int size;
        size = N / nr_procese_total;

        int size_aux = size + (N % nr_procese_total);

        //Trimit size catre procesele Master
        MPI_Send(&size, 1, MPI_INT, Master1, tag01, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", Master0, Master1);
        MPI_Send(&size, 1, MPI_INT, Master2, tag02, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", Master0, Master2);

        //Trimit size catre procesele worker
        for(int i = 0; i < nr_procese[Master0]; i++)
        {
            MPI_Send(&size, 1, MPI_INT, procese_Master0[i], tag0, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", Master0, procese_Master0[i]);
        }

        //stochez marimea partii de vector pentru fiecare coordonator
        int Sizes[3];
        Sizes[Master0] = size * nr_procese[Master0];
        Sizes[Master1] = size * nr_procese[Master1];
        Sizes[Master2] = (size * nr_procese[Master2]) + (N % nr_procese_total);

        //Trimit marimea vectorilor task-urilor master
        MPI_Send(&Sizes[Master1], 1, MPI_INT, Master1, tag01, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", Master0, Master1);
        MPI_Send(&Sizes[Master2], 1, MPI_INT, Master2, tag02, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", Master0, Master2);

        // printf("eu sunt numarul: %d\n", V[Sizes[Master1]]);

        //Trimit vectorii task-urilor master
        for(int i = 0; i < Sizes[Master1]; i++)
        {
            MPI_Send(&V[i+Sizes[Master0]], 1, MPI_INT, Master1, tag01, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", Master0, Master1);
        }

        for(int i = 0; i < Sizes[Master2]; i++)
        {
            MPI_Send(&V[i+Sizes[Master0]+Sizes[Master1]], 1, MPI_INT, Master2, tag02, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", Master0, Master2);
        }

        int *V_Master0;
        V_Master0 = (int*)malloc(sizeof(int)*Sizes[Master0]);

        for(int i = 0; i < Sizes[Master0]; i++)
            V_Master0[i] = V[i];

        // for(int i = 0; i < Sizes[Master0]; i++)
        //     printf("%d ", V_Master0[i]);
        // printf("\n");

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

        //Primesc vectorii
        index = 0;
        for(int i = 0; i < nr_procese[Master0]; i++)
        {
            for(int j = 0 + index; j < size + index; j++)
                MPI_Recv(&V[j], 1, MPI_INT, procese_Master0[i], tag0, MPI_COMM_WORLD, &status);
            index = index + size;
        }

        for(int i = 0; i < Sizes[Master1]; i++)
            MPI_Recv(&V[i+Sizes[Master0]], 1, MPI_INT, Master1, tag01, MPI_COMM_WORLD, &status);

        for(int i = 0; i < Sizes[Master2]; i++)
            MPI_Recv(&V[i+Sizes[Master0]+Sizes[Master1]], 1, MPI_INT, Master2, tag02, MPI_COMM_WORLD, &status);

        printf("Rezultat: ");
        for(int i = 0; i < N; i++)
            printf("%d ", V[i]);    
        printf("\n");   

    }

    if(rank == Master1)
    {  
        //Citirea numarul de procese pe care le are sub comanda din fisier
        FILE *fisier1 = fopen("cluster1.txt", "rt");
        fscanf(fisier1, "%d", &nr_procese[Master1]);
        procese_Master1 = (int*)malloc(sizeof(int)*nr_procese[Master1]);

        MPI_Send(&nr_procese[Master1], 1, MPI_INT, Master0, tag01, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", Master1, Master0);
        MPI_Send(&nr_procese[Master1], 1, MPI_INT, Master2, tag12, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", Master1, Master2);

        MPI_Recv(&nr_procese[Master0], 1, MPI_INT, Master0, tag01, MPI_COMM_WORLD, &status);
        MPI_Recv(&nr_procese[Master2], 1, MPI_INT, Master2, tag12, MPI_COMM_WORLD, &status);

        //Citirea proceselor pe care le are sub comanda din fisier
        for(int i = 0; i < nr_procese[Master1]; i++)
            fscanf(fisier1, "%d", &procese_Master1[i]);

        // printf("%d : p0 are %d\n", rank, nr_procese[Master0]);
        // printf("%d : p1 are %d\n", rank, nr_procese[Master1]);
        // printf("%d : p2 are %d\n", rank, nr_procese[Master2]);

        procese_Master0 = (int*)malloc(sizeof(int)*nr_procese[Master0]);
        procese_Master2 = (int*)malloc(sizeof(int)*nr_procese[Master2]);

        MPI_Send(procese_Master1, nr_procese[Master1], MPI_INT, Master0, tag01, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", Master1, Master0);
        MPI_Send(procese_Master1, nr_procese[Master1], MPI_INT, Master2, tag12, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", Master1, Master2);

        MPI_Recv(procese_Master0, nr_procese[Master0], MPI_INT, Master0, tag01, MPI_COMM_WORLD, &status);
        MPI_Recv(procese_Master2, nr_procese[Master2], MPI_INT, Master2, tag12, MPI_COMM_WORLD, &status);

        printf("%d ->", rank);
        printf(" %d:", Master0);
        for(int i = 0; i < nr_procese[Master0]; i++)
        {
            printf("%d", procese_Master0[i]);
            if(i != nr_procese[Master0] - 1)
                printf(",");
        }
        printf(" %d:", Master1);
        for(int i = 0; i < nr_procese[Master1]; i++)
        {
            printf("%d", procese_Master1[i]);
            if(i != nr_procese[Master1] - 1)
                printf(",");
        }
        printf(" %d:", Master2);
        for(int i = 0; i < nr_procese[Master2]; i++)
        {
            printf("%d", procese_Master2[i]);
            if(i != nr_procese[Master2] - 1)
                printf(",");
        }
        printf("\n");
        
                        // for(int i = 0; i < nr_procese[Master1]; i++)
                        //     printf("P[%d] are procesul: %d\n", rank, procese_Master1[i]);

        //Trimiterea topologiei catre workeri
        for(int i = 0; i < nr_procese[Master1]; i++)
        {
            MPI_Send(&nr_procese[Master0], 1, MPI_INT, procese_Master1[i], tag1, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", Master1, procese_Master1[i]);
            MPI_Send(&nr_procese[Master1], 1, MPI_INT, procese_Master1[i], tag1, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", Master1, procese_Master1[i]);
            MPI_Send(&nr_procese[Master2], 1, MPI_INT, procese_Master1[i], tag1, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", Master1, procese_Master1[i]);

            MPI_Send(procese_Master0, nr_procese[Master0], MPI_INT, procese_Master1[i], tag1, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", Master1, procese_Master1[i]);
            MPI_Send(procese_Master1, nr_procese[Master1], MPI_INT, procese_Master1[i], tag1, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", Master1, procese_Master1[i]);
            MPI_Send(procese_Master2, nr_procese[Master2], MPI_INT, procese_Master1[i], tag1, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", Master1, procese_Master1[i]);

        }


        //Primirea partii din vector de care se ocupa Master1
        //Primesc size de la Master0
        int size;
        MPI_Recv(&size, 1, MPI_INT, Master0, tag01, MPI_COMM_WORLD, &status);

        for(int i = 0; i < nr_procese[Master1]; i++)
        {
            MPI_Send(&size, 1, MPI_INT, procese_Master1[i], tag1, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", Master1, procese_Master1[i]);
        }

        //Primesc partea de vector alocata lui Master1
        int size_Master1;
        int *V_Master1;

        MPI_Recv(&size_Master1, 1, MPI_INT, Master0, tag01, MPI_COMM_WORLD, &status);

        V_Master1 = (int*)malloc(sizeof(int)*size_Master1);

        for(int i = 0; i < size_Master1; i++)
            MPI_Recv(&V_Master1[i], 1, MPI_INT, Master0, tag01, MPI_COMM_WORLD, &status);

        // for(int i = 0; i < size_Master1; i++)
        //     printf("%d ", V_Master1[i]);
        // printf("\n");

        int index = 0;  //variabila pe care o cresc cu size la sf unei iteratii ca
                        //sa trimtit din size in size worker-ilor

        for(int i = 0; i < nr_procese[Master1]; i++)
        {
            for(int j = 0 + index; j < size + index; j++)
                {
                    MPI_Send(&V_Master1[j], 1, MPI_INT, procese_Master1[i], tag1, MPI_COMM_WORLD);
                    printf("M(%d,%d)\n", Master1, procese_Master1[i]);
                }
            index = index + size;
        }

        index = 0;

        for(int i = 0; i < nr_procese[Master1]; i++)
        {
            for(int j = 0 + index; j < size + index; j++)
                MPI_Recv(&V_Master1[j], 1, MPI_INT, procese_Master1[i], tag1, MPI_COMM_WORLD, &status);
            index = index + size;
        }

        for(int i = 0; i < size_Master1; i++)
        {
            MPI_Send(&V_Master1[i], 1, MPI_INT, Master0, tag01, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", Master1, Master0);
        }

    }

    if(rank == Master2)
    {  
        
        FILE *fisier2 = fopen("cluster2.txt", "rt");
        fscanf(fisier2, "%d", &nr_procese[Master2]);
        procese_Master2 = (int*)malloc(sizeof(int)*nr_procese[Master2]);

        MPI_Send(&nr_procese[Master2], 1, MPI_INT, Master0, tag02, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", Master2, Master0);
        MPI_Send(&nr_procese[Master2], 1, MPI_INT, Master1, tag12, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", Master2, Master1);

        MPI_Recv(&nr_procese[Master0], 1, MPI_INT, Master0, tag02, MPI_COMM_WORLD, &status);
        MPI_Recv(&nr_procese[Master1], 1, MPI_INT, Master1, tag12, MPI_COMM_WORLD, &status);

        for(int i = 0; i < nr_procese[Master2]; i++)
            fscanf(fisier2, "%d", &procese_Master2[i]);
        
        // printf("%d : p0 are %d\n", rank, nr_procese[Master0]);
        // printf("%d : p1 are %d\n", rank, nr_procese[Master1]);
        // printf("%d : p2 are %d\n", rank, nr_procese[Master2]);

        procese_Master1 = (int*)malloc(sizeof(int)*nr_procese[Master1]);
        procese_Master0 = (int*)malloc(sizeof(int)*nr_procese[Master0]);

        MPI_Send(procese_Master2, nr_procese[Master2], MPI_INT, Master0, tag02, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", Master2, Master0);
        MPI_Send(procese_Master2, nr_procese[Master2], MPI_INT, Master1, tag12, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", Master2, Master1);

        MPI_Recv(procese_Master1, nr_procese[Master1], MPI_INT, Master1, tag12, MPI_COMM_WORLD, &status);
        MPI_Recv(procese_Master0, nr_procese[Master0], MPI_INT, Master0, tag02, MPI_COMM_WORLD, &status);

        printf("%d ->", rank);
        printf(" %d:", Master0);
        for(int i = 0; i < nr_procese[Master0]; i++)
        {
            printf("%d", procese_Master0[i]);
            if(i != nr_procese[Master0] - 1)
                printf(",");
        }
        printf(" %d:", Master1);
        for(int i = 0; i < nr_procese[Master1]; i++)
        {
            printf("%d", procese_Master1[i]);
            if(i != nr_procese[Master1] - 1)
                printf(",");
        }
        printf(" %d:", Master2);
        for(int i = 0; i < nr_procese[Master2]; i++)
        {
            printf("%d", procese_Master2[i]);
            if(i != nr_procese[Master2] - 1)
                printf(",");
        }
        printf("\n");
        
                        // for(int i = 0; i < nr_procese[Master2]; i++)
                        //     printf("P[%d] are procesul: %d\n", rank, procese_Master2[i]);
        
        //Trimiterea topologiei catre workeri
        for(int i = 0; i < nr_procese[Master2]; i++)
        {
            MPI_Send(&nr_procese[Master0], 1, MPI_INT, procese_Master2[i], tag2, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", Master2, procese_Master2[i]);
            MPI_Send(&nr_procese[Master1], 1, MPI_INT, procese_Master2[i], tag2, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", Master2, procese_Master2[i]);
            MPI_Send(&nr_procese[Master2], 1, MPI_INT, procese_Master2[i], tag2, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", Master2, procese_Master2[i]);

            MPI_Send(procese_Master0, nr_procese[Master0], MPI_INT, procese_Master2[i], tag2, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", Master2, procese_Master2[i]);
            MPI_Send(procese_Master1, nr_procese[Master1], MPI_INT, procese_Master2[i], tag2, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", Master2, procese_Master2[i]);
            MPI_Send(procese_Master2, nr_procese[Master2], MPI_INT, procese_Master2[i], tag2, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", Master2, procese_Master2[i]);

        }


        //Primirea partii din vector de care se ocupa Master2
        //Primesc size de la Master0
        int size;
        MPI_Recv(&size, 1, MPI_INT, Master0, tag02, MPI_COMM_WORLD, &status);

        //Primesc partea de vector alocata lui Master1
        int size_Master2;
        int *V_Master2;

        MPI_Recv(&size_Master2, 1, MPI_INT, Master0, tag02, MPI_COMM_WORLD, &status);

        int size_aux = size + (size_Master2 - (size * nr_procese[Master2]));

        V_Master2 = (int*)malloc(sizeof(int)*size_Master2);

        for(int i = 0; i < nr_procese[Master2]; i++)
            if(i != nr_procese[Master2] - 1)
            {
                MPI_Send(&size, 1, MPI_INT, procese_Master2[i], tag2, MPI_COMM_WORLD);
                printf("M(%d,%d)\n", Master2, procese_Master2[i]);
            }
            else
            {
                MPI_Send(&size_aux, 1, MPI_INT, procese_Master2[i], tag2, MPI_COMM_WORLD);
                printf("M(%d,%d)\n", Master2, procese_Master2[i]);
            }   

        for(int i = 0; i < size_Master2; i++)
            MPI_Recv(&V_Master2[i], 1, MPI_INT, Master0, tag02, MPI_COMM_WORLD, &status);

        // for(int i = 0; i < size_Master2; i++)
        //     printf("%d ", V_Master2[i]);
        // printf("\n");

        int index = 0;  //variabila pe care o cresc cu size la sf unei iteratii ca
                        //sa trimtit din size in size worker-ilor
        int k = 0;

        for(int i = 0; i < nr_procese[Master2] - 1; i++)
        {
            for(int j = 0 + index; j < size + index; j++)
                {
                    MPI_Send(&V_Master2[j], 1, MPI_INT, procese_Master2[i], tag2, MPI_COMM_WORLD);
                    printf("M(%d,%d)\n", Master2, procese_Master2[i]);
                }
            index = index + size;
            k++;
        }

        for(int j = 0 + index; j < size_aux + index; j++)
        {
            MPI_Send(&V_Master2[j], 1, MPI_INT, procese_Master2[k], tag2, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", Master2, procese_Master2[k]);
        }

        index = 0;
        k = 0;

        for(int i = 0; i < nr_procese[Master2] - 1; i++)
        {
            for(int j = 0 + index; j < size + index; j++)
                {
                    MPI_Recv(&V_Master2[j], 1, MPI_INT, procese_Master2[i], tag2, MPI_COMM_WORLD, &status);
                }
            index = index + size;
            k++;
        }

        for(int j = 0 + index; j < size_aux + index; j++)
        {
            MPI_Recv(&V_Master2[j], 1, MPI_INT, procese_Master2[k], tag2, MPI_COMM_WORLD, &status);
        }

        for(int i = 0; i < size_Master2; i++)
        {
            MPI_Send(&V_Master2[i], 1, MPI_INT, Master0, tag02, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", Master2, Master0);
        }
        
    }

    if(rank != Master0 && rank != Master1 && rank != Master2)
    {   
        MPI_Recv(&nr_procese[Master0], 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(&nr_procese[Master1], 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(&nr_procese[Master2], 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        // printf("%d : p0 are %d\n", rank, nr_procese[Master0]);
        // printf("%d : p1 are %d\n", rank, nr_procese[Master1]);
        // printf("%d : p2 are %d\n", rank, nr_procese[Master2]);

        procese_Master0 = (int*)malloc(sizeof(int)*nr_procese[Master0]);
        procese_Master1 = (int*)malloc(sizeof(int)*nr_procese[Master1]);
        procese_Master2 = (int*)malloc(sizeof(int)*nr_procese[Master2]);

        MPI_Recv(procese_Master0, nr_procese[Master0], MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(procese_Master1, nr_procese[Master1], MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(procese_Master2, nr_procese[Master2], MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        
        int my_master = status.MPI_SOURCE;
        int my_tag = status.MPI_TAG;
        
        printf("%d ->", rank);
        printf(" %d:", Master0);
        for(int i = 0; i < nr_procese[Master0]; i++)
        {
            printf("%d", procese_Master0[i]);
            if(i != nr_procese[Master0] - 1)
                printf(",");
        }
        printf(" %d:", Master1);
        for(int i = 0; i < nr_procese[Master1]; i++)
        {
            printf("%d", procese_Master1[i]);
            if(i != nr_procese[Master1] - 1)
                printf(",");
        }
        printf(" %d:", Master2);
        for(int i = 0; i < nr_procese[Master2]; i++)
        {
            printf("%d", procese_Master2[i]);
            if(i != nr_procese[Master2] - 1)
                printf(",");
        }
        printf("\n");

        //Primirea vectorului de la task ul master specific worker-ului
        int size;
        MPI_Recv(&size, 1, MPI_INT, my_master, my_tag, MPI_COMM_WORLD, &status);
        // printf("rank: %d am size %d\n", rank, size);

        int *V;
        V = (int*)malloc(sizeof(int)*size);

        for(int i = 0; i < size; i++)
            MPI_Recv(&V[i], 1, MPI_INT, my_master, my_tag, MPI_COMM_WORLD, &status);
        
        // for(int i = 0; i < size; i++)
        //     printf("%d: primesc %d ", rank, V[i]);
        // printf("\n");

        for(int i = 0; i < size; i++)
            V[i] = V[i] * 2;
        
        // printf("rank %d: ", rank);
        // for(int i = 0; i < size; i++)
        //     printf("%d ", V[i]);
        // printf("\n");

        for(int i = 0; i < size; i++)
        {
            MPI_Send(&V[i], 1, MPI_INT, my_master, my_tag, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", rank, my_master);
        }

        
    }

    MPI_Finalize();
}