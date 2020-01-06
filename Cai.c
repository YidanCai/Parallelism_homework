#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

int* prefixsum(int arr[], int size) {

	int d = log(size) / log(2);
	int* copy = (int*)malloc(size * sizeof(int));
	int* copy2 = (int*)malloc(size * sizeof(int));
	memcpy(copy, arr, size * sizeof(int));

	int i, j;
#pragma omp for private(i)
	for (i = 1; i <= d; i++) {
		for (j = pow(2, i) - 1; j < size; j = j + pow(2, i)) {
			int m = j - pow(2, i - 1);
			copy[j] = copy[j] + copy[m];
		}
	}
	memcpy(copy2, copy, size * sizeof(int));
	copy2[size - 1] = 0;
	for (i = d - 1; i >= 0; i--) {
		for (j = pow(2, i) - 1; j < size; j = j + pow(2, i)) {
			int s = pow(2, i + 1);
			if ((j + 1) % s != 0) {
				int mm = j + pow(2, i);
				copy2[j] = copy2[mm];
			}
			else {
				int mmm = j - pow(2, i);
				copy2[j] = copy2[j] + copy[mmm];
			}

		}
	}
	for (i = 0; i < size; i++) {
		copy2[i] = copy2[i] + arr[i];
	}

	return copy2;
}
int* reverse(int arr[], int size)
{
	int temp;

	double d = size/2;
	int i;
	for (i = 0; i < d ; i++) {
		temp = arr[i];
		arr[i] = arr[size - i - 1];
		arr[size - i - 1] = temp;
	}

}

int* surfix(int arr[], int size) {
	int* copy = (int*)malloc(size * sizeof(int));
	memcpy(copy, arr, size * sizeof(int));
	reverse(copy, size);
	copy = prefixsum(copy, size);
	reverse(copy, size);
	return copy;


}

int max(int a, int b) {
	if (a > b) {
		return a;
	}
	else {
		return b;
	}
}

int min(int arr[],int size) {
	int min,i;
	min = arr[0];
#pragma omp for private(i)
	for (i = 1; i < size; i++) {
		if (arr[i] <min) {

			min = arr[i];
		}

	}
	return min;
}

int* smax(int arr[], int size) {
	int* copy = (int*)malloc(size * sizeof(int));
	int* copy2 = (int*)malloc(size * sizeof(int));
	memcpy(copy, arr, size * sizeof(int));

	int i,j,k;
	int d = log(size) / log(2);
	for (i = 1; i <= d; i++) {
		for (j = 0; j < size; j = j + pow(2, i)) {
			k = j + pow(2, i-1);

			copy[j] = max(copy[j], copy[k]);
		}
	}

	memcpy(copy2, copy, size * sizeof(int));

	int m,mm;
	copy2[0] = min(arr,size);
#pragma omp for private(i)	
	for (i = d - 1; i >= 0; i--) {
		for (j = size - pow(2,i); j >= 0; j = j - pow(2, i)) {
			int s = pow(2, i + 1);
			if (j% s != 0) {
				m = j - pow(2, i);

				copy2[j] = copy2[m];
			}
			else {
				 mm = j + pow(2, i);
				copy2[j] = max(copy2[j], copy[mm]);

			}
		}
	}
	for (i = 0; i < size; i++) {
		copy2[i] = max(copy2[i], arr[i]);
	}
	
	return copy2;

}
int* pmax(int arr[], int size) {
	int* copy = (int*)malloc(size * sizeof(int));
	memcpy(copy, arr, size * sizeof(int));
	reverse(copy, size);
	copy = smax(copy, size);
	reverse(copy, size);
	return copy;
}
int* M(int arr[], int size) {
	int i;
	int* ps = (int*)malloc(size * sizeof(int));
	int* ss = (int*)malloc(size * sizeof(int));
	int* sm = (int*)malloc(size * sizeof(int));
	int* pm = (int*)malloc(size * sizeof(int));
	int* M = (int*)malloc(size * sizeof(int));
	ps = prefixsum(arr, size);
	ss = surfix(arr, size);
	sm = smax(ps, size);
	pm = pmax(ss, size);
	for (i = 0; i < size; i++) {
		M[i] = pm[i] - ss[i] + sm[i] - ps[i] + arr[i];
	}
	free(ps);
	free(ss);
	free(sm);
	free(pm);

	return M;

}



int main(int argc, char* argv[]) {

	FILE* finp;
	int     ptr[128];
	int     i, ctr = 0;

	finp = fopen(argv[1], "r");
	if (NULL == finp)
	{
		printf("Unable to open file\n");
		exit(-1);
	}

	while ((!feof(finp)) && (ctr < 128))
	{
		fscanf(finp, "%d ", &ptr[ctr++]);
	}




	fclose(finp); //Close the file pointer





	int array_size = ctr;

	int  j;


	int* arrayM;
	int max;
	arrayM = M(ptr, array_size);
	max = arrayM[0];
	for (i = 1; i < array_size; i++) {
		if (arrayM[i] > max) {

			max = arrayM[i];
		}

	}
	int start, end;
	for (i = 0; i < array_size; i++) {
		if (arrayM[i] == max) {
			start = i;
			end = start;
			if (end < array_size - 1) {
				for (j = start + 1; j < array_size; j++) {
					if (arrayM[j] == max) {
						end = end + 1;
					}
					else { break; }
				}

			}
			break;

		}


	}
	printf("%d ", max);

	for (i = start; i <= end; i++) {
		printf("%d", ptr[i]);
		if (i < end) {
			printf(" ");
		}
	}
	printf("\n");

	free(arrayM);


	return 0;
}