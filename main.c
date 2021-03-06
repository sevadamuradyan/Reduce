#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define N 5
int array[] = { 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181,6765,10946,17711};

void* reduce(void* (functionPointerSum)(void*), void *datapointer) {
	pthread_t threadHandle[N];
    for (int i = 0; i < N; i++) {
        int* a = malloc(sizeof(int));
        *a = i * (1000 / N);
        if (pthread_create(&threadHandle[i], NULL, functionPointerSum, a) != 0) {
            perror("Did not create thread");
        }
    }
    int result = 1;
    int* r;
    for (int i = 0; i < N; i++) {
        if (pthread_join(threadHandle[i], (void**) &r) != 0) {
            perror("Did not join thread");
        }
        result += (*r);
        
        free(r);
    }
    printf("%d\n",result);
	int* c = &result;
    void* v = c;
	return v;
	
}

void* functionPointerSum(void *arg) {
	int index = *(int*)arg;
    int mul = 0;
    for (int j = 0; j < (1000 / N); j++) {
        mul += array[index + j];
    }
    *(int*)arg = mul;
    return arg;
}

int main(int argc, char** argv) {
    clock_t cTime;
    cTime = clock();
    printf("Timer starts\n");
	reduce(functionPointerSum, array);
	cTime = clock() - cTime;
    double taken = ((double)cTime)/CLOCKS_PER_SEC;
    printf("The program get %f seconds to execute", taken);
	return 0;
}
