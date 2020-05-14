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
// cplx *result, *vect;
char input[500], output[500];

typedef struct recourse {
	cplx *vect, *result;
	int step;
} recourse;

// void* threadFunction (void* var)
// {
// 	int thread_id = *(int *) var;
// 	int start = thread_id * ( ceil((double)N/P) );
// 	int end = fmin( N, (thread_id + 1) * ceil((double)N/P) );

// 	if (step < n) 
// 	{
// 		_fft(out, buf, n, step * 2);
// 		_fft(out + step, buf + step, n, step * 2);

// 		for (int i = 0; i < n; i += 2 * step) {
// 			cplx t = cexp(-I * PI * i / n) * out[i + step];
// 			buf[i / 2]     = out[i] + t;
// 			buf[(i + n)/2] = out[i] - t;
// 		}
// 	}

// 	return NULL;
// }

// void fft()
// void _fft(recourse data)
// {
// 	if (data.step < N) 
// 	{	
// 		 data.step *= 2;
// 		_fft(data);
// 		data.result += data.step/2;
// 		data.vect += data.step/2;
// 		_fft(data);
// 		data.vect -= data.step/2;
// 		data.result -= data.step/2;
// 		data.step /= 2;

// 		for (int i = 0; i < N; i += 2 * data.step) {
// 			cplx t = cexp(-I * PI * i / N) * data.result[i + data.step];
// 			data.vect[i / 2]     = data.result[i] + t;
// 			data.vect[(i + N)/2] = data.result[i] - t;
// 		}
// 	}
// }
 
void getArgs(int argc, char **argv)
{
	strcpy(input, argv[1]);
	strcpy(output, argv[2]);
	P = atoi(argv[3]);
}

void init(recourse *data)
{
	int i;
	FILE *file = fopen(input, "r");
	fscanf(file, "%d", &N);

	data->vect = (cplx *) calloc(N, sizeof(cplx));
	data->result = (cplx *) calloc(N, sizeof(cplx));
	for(i = 0; i < N; i++)
	{
		fscanf(file, "%lf", &data->vect[i]);
		data->result[i] = data->vect[i];
	}
	data->step = 1;
}

void writeResult(recourse data)
{
	int i;
	FILE *file = fopen(output, "w");

	fprintf(file, "%d\n", N);

	for(i = 0; i < N; i++)
		fprintf(file, "%lf %lf\n", creal(data.vect[i]), cimag(data.vect[i]));

}

int main(int argc, char * argv[]) 
{
	pthread_t tid[P];
	int thread_id[P];
	int i;
	recourse data;
	PI = atan2(1, 1) * 4;

	getArgs(argc, argv);
	init(&data);
    pthread_mutex_init(&mutex, NULL);
	// for(i = 0;i < P; i++)
	// 	thread_id[i] = i;
	

	// for(i = 0; i < P; i++) {
	// 	pthread_create(&(tid[i]), NULL, threadFunction, &(thread_id[i]));
	// }

	// for(i = 0; i < P; i++) {
	// 	pthread_join(tid[i], NULL);
	// }

	_fft(data);

	writeResult(data);

	return 0;
}
