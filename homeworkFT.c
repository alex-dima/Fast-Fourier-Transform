#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <string.h>
#include <complex.h>

double PI;
typedef double complex cplx;
pthread_mutex_t mutex;

int N,P;
double *vect, *sumreal, *sumimag, **angle;
cplx *result;
char input[500], output[500];

void* threadFunction (void* var)
{
	int thread_id = *(int *) var;
	int start = thread_id * ( ceil((double)N/P) );
	int end = fmin( N, (thread_id + 1) * ceil((double)N/P) );

	for (int k = start; k < end; k++) 
	{  // For each output element
        for (int t = 0; t < N; t++) 
		{  // For each input element
            angle[t][k] = 2 * PI * t * k / N;
            sumreal[k] +=  vect[t] * cos(angle[t][k]);
            sumimag[k] +=  vect[t] * sin(angle[t][k]);;
        }
        result[k] = sumreal[k] - sumimag[k] * I;
    }

	return NULL;
}

void getArgs(int argc, char **argv)
{
	strcpy(input, argv[1]);
	strcpy(output, argv[2]);
	P = atoi(argv[3]);
}

void init()
{
	int i;
	FILE *file = fopen(input, "r");
	fscanf(file, "%d", &N);

	vect = (double *) calloc(N, sizeof(double));
	result = (cplx *) calloc(N, sizeof(cplx));
	sumreal = (double *) calloc(N, sizeof(double));
	sumimag = (double *) calloc(N, sizeof(double));
	angle = (double **) calloc(N, sizeof(double *));
	for(i = 0; i < N; i++)
		angle[i] = (double *) calloc(N, sizeof(double));
	
	for(i = 0; i < N; i++)
		fscanf(file, "%lf", &vect[i]);
}

void writeResult()
{
	int i;
	FILE *file = fopen(output, "w");

	fprintf(file, "%d\n", N);

	for(i = 0; i < N; i++)
		fprintf(file, "%lf %lf\n", creal(result[i]), cimag(result[i]));

}

int main(int argc, char * argv[]) 
{
	pthread_t tid[P];
	int thread_id[P];
	int i;

	PI = atan2(1, 1) * 4;

	getArgs(argc, argv);
	init();

    pthread_mutex_init(&mutex, NULL);
	for(i = 0;i < P; i++)
		thread_id[i] = i;
	

	for(i = 0; i < P; i++) {
		pthread_create(&(tid[i]), NULL, threadFunction, &(thread_id[i]));
	}

	for(i = 0; i < P; i++) {
		pthread_join(tid[i], NULL);
	}

	writeResult();

	return 0;
}
