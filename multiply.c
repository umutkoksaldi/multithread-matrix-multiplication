#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

double** matResult;
double** mat1;
double** mat2;
int matSize1x;
int matSize1y;
int matSize2x;
int matSize2y;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *runner(void *args) {
    // set the row that this thread is responsible for
    int index = *((int *) args);
    double partialSum = 0;
    // multiply the row with all columns in matrix_2
    for (int j = 0; j < matSize2y; j++) {
        for(int i = 0; i < matSize1y; i++) {
            partialSum += (mat1[index][i]) * (mat2[i][j]);
        }
        // set the corresponding row_col of the resulting matrix
        pthread_mutex_lock(&mutex);
        matResult[index][j] = partialSum;
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(0);
}

int main(int argc, char **argv) {

    // check if arguments were supplied
    if (argc <= 2) {
        printf("No arguments were passed in...\n");
        exit(1);
    }
    // get the file names from the arguments
    char *fileName1 = argv[1];
    char *fileName2 = argv[2];

    FILE *file;
    FILE *file2;
    file=fopen(fileName1, "r");

    // determine dimensions of the matrix
    fscanf(file, "%d", &matSize1x);
    fscanf(file, "%d", &matSize1y);

    // allocate space for the matrix
    mat1=malloc(matSize1x*sizeof(double*)); 

    // allocate the columns
    for(int i = 0; i < matSize1x; ++i) {
        mat1[i]=malloc(matSize1y*sizeof(double));
    }

    // read from the file and initialize matrix
    for(int i = 0; i < matSize1y; i++)
    {
        for(int j = 0; j < matSize1y; j++) 
        {
         if (!fscanf(file, "%lf", &mat1[i][j])) 
             break;
        }
    }

    // close the first file
    fclose(file);

    // same procedure for the second matrix
    file2=fopen(fileName2, "r");

    // acquire the dimensions
    fscanf(file2, "%d", &matSize2x);
    fscanf(file2, "%d", &matSize2y);

    mat2=malloc(matSize2x*sizeof(double*)); 

    // exit if two matrices are not compatible for multiplication
    if (matSize1y != matSize2x) {
        printf("Matrix dimensions do not match for multiplication\n");
        exit(1);
    }
    // allocate space for the second matrix
    for(int i = 0; i < matSize2x; ++i) {
        mat2[i]=malloc(matSize2y*sizeof(double));
    }

    // read from the second file
    for(int i = 0; i < matSize2x; i++)
    {
        for(int j = 0; j < matSize2y; j++) 
        {
            if (!fscanf(file, "%lf", &mat2[i][j])) 
                 break;
        }
    }
    fclose(file2);
    
    // array to hold thread ids
    pthread_t tids[matSize1x];
    // array to specify the row for each thread 
    int indices[matSize1x];

    // initialize the result matrix with its dimensions
    matResult = malloc(matSize1x*sizeof(double*));
    for(int i = 0; i < matSize1x; i++) {
        matResult[i] = malloc(matSize2y*sizeof(double));
    }
    // populate the indices for the threads
    for(int i = 0; i < matSize1x; i++) {
        indices[i] = i;
    }
    // create the threads as much as there is rows in matrix1
    for (int i = 0; i < matSize1x; i++) {
        pthread_create(&tids[i], NULL, runner, &indices[i]);
    }
    // join the threads to the main thread
    for (int i = 0; i < matSize1x; i++) {
        pthread_join(tids[i], NULL);
    }
    // print the resulting matrix in format
    for(int i = 0; i < matSize1x; i++) {
        for (int j = 0; j < matSize2y; j++) {
            printf("%lf ", matResult[i][j]);
        }
        printf("\n");
    }

    return 0;

}