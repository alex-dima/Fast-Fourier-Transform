#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <string.h>
#include <complex.h>

double PI;
typedef double complex cplx;

int N,P;
cplx *result, *vect;
char input[500], output[500];

void _fft(cplx *vect, cplx *result, int step )
{
	if (step < N) 
	{	
		_fft(result, vect, step * 2);
		_fft(result + step, vect + step, step * 2);

		for (int i = 0; i < N; i += 2 * step) {
			cplx t = cexp(-I * PI * i / N) * result[i + step];
			vect[i / 2]     = result[i] + t;
			vect[(i + N)/2] = result[i] - t;
		}
	}
}

void combine2(cplx * vect, cplx *result, int step)
{
	for (int i = 0; i < N; i += 2 * step) 
	{
		cplx t = cexp(-I * PI * i / N) * result[i + step];
		vect[i / 2]     = result[i] + t;
		vect[(i + N)/2] = result[i] - t;
	}
}

void* threadFunction1(void* args)
{
	_fft(vect, result, 1);
	return NULL;
}

void* threadFunction2(void* args)
{
	int thread_id = *(int *) args;
	if(thread_id == 0)
		_fft(result, vect, 2);
	else
		_fft(result + 1, vect + 1, 2);
	return NULL;
}

void* threadFunction4(void* args)
{
	int thread_id = *(int *) args;

	switch(thread_id)
	{
		case 0: _fft(vect, result, 4);
				break;
		case 1: _fft(vect + 2, result + 2, 4);
				break;
		case 2: _fft(vect + 1, result + 1, 4);
				break;
		case 3: _fft(vect + 3, result + 3, 4);
				break;
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

	vect = (cplx *) calloc(N, sizeof(cplx));
	result = (cplx *) calloc(N, sizeof(cplx));

	for(i = 0; i < N; i++)
	{
		fscanf(file, "%lf", &vect[i]);
		result[i] = vect[i];
	}

}

void writeResult()
{
	int i;
	FILE *file = fopen(output, "w");

	fprintf(file, "%d\n", N);

	for(i = 0; i < N; i++)
		fprintf(file, "%lf %lf\n", creal(vect[i]), cimag(vect[i]));

}

int main(int argc, char **argv) 
{
	pthread_t tid[P];
	int thread_id[P];
	int i;
	PI = atan2(1, 1) * 4;

	getArgs(argc, argv);
	init();

	for(i = 0;i < P; i++)
		thread_id[i] = i;
	
	if(P == 1)
		for(i = 0; i < P; i++)
			pthread_create(&(tid[i]), NULL, threadFunction1, &(thread_id[i]));
	else if(P == 2)
			for(i = 0; i < P; i++)
				pthread_create(&(tid[i]), NULL, threadFunction2, &(thread_id[i]));	
	else
		for(i = 0; i < P; i++)
			pthread_create(&(tid[i]), NULL, threadFunction4, &(thread_id[i]));
	
	for(i = 0; i < P; i++)
		pthread_join(tid[i], NULL);
	
	if(P == 2)
		combine2(vect, result, 1);
	else if(P == 4)
	{
		combine2(result, vect, 2);
		combine2(result + 1, vect + 1, 2);
		combine2(vect, result, 1);
	}

	writeResult();

	return 0;
}
