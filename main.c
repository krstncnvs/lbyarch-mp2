#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

extern void imgCvtGrayFloatToInt(float *floatPixels, unsigned char *intPixels, int height, int width);

// function for checking correctness of output
void checkCorrectness(float* floatPixels, unsigned char* intPixels, int total) {
	int correct = 1; // assume correct
	unsigned char expected;

	for (int i = 0; i < total; i++) {
		expected = (unsigned char)(floatPixels[i] * 255.0f + 0.5f);
		if (intPixels[i] != expected) {
			correct = 0;
			printf("Mismatch at pixel %d: ASM=%d, Expected=%d\n",
				i, intPixels[i], expected);
			break;
		}
	}

	if (correct) {
		printf("\nCorrectness check: PASS\n");
	}
	else {
		printf("\nCorrectness check: FAIL\n");
	}
}

int main() {
	int height, width, choice;
	printf("Enter height and width: ");
	scanf_s("%d %d", &height, &width);

	int total = height * width;

	// memory allocations
	float *floatPixels;
	unsigned char *intPixels; //uint8 is needed

	floatPixels = calloc(total, sizeof(float));
	intPixels = calloc(total, sizeof(unsigned char));

	// options for pixels input
	printf("\nSelect below:\n");
	printf("1 - Enter pixels manually\n");
	printf("2 - Generate random pixels values\n");
	printf("Choice: ");
	scanf_s("%d", &choice);

	if (choice == 1) {
		printf("\nEnter %d float pixel values (0.0 to 1.0):\n", total);
		
		for (int i = 0; i < total; i++) {
			scanf_s("%f", &floatPixels[i]);
		}
	}
	else { // generate random values
		srand((unsigned int)time(0));

		for (int i = 0; i < total; i++) {
			floatPixels[i] = (float)rand() / (float)RAND_MAX;
		}

		printf("\nGenerated random pixel values:\n");
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				printf("%.2f ", floatPixels[i * width + j]);
			}
			printf("\n");
		}
	}

	// conversion and performance testing
	LARGE_INTEGER frequency, start, end;
	double total_ms = 0.0;
	int runs = 30;
	double run_times[30];

	QueryPerformanceFrequency(&frequency);

	for (int r = 0; r < runs; r++) {
		QueryPerformanceCounter(&start);
		imgCvtGrayFloatToInt(floatPixels, intPixels, height, width);
		QueryPerformanceCounter(&end);

		run_times[r] = (double)(end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;
		total_ms += run_times[r];
	}

	// output print
	printf("\nConverted integer pixel values:\n");
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			printf("%3d ", intPixels[i * width + j]);
		}
		printf("\n");
	}

	// printing of exec times 
	printf("\nAverage execution time in 30 runs: %.8f ms\n", total_ms / runs);
	printf("Execution time for each run:\n");
	for (int r = 0; r < runs; r++) {
		printf("Run %d: %.4f ms\n", r + 1, run_times[r]);
	}

	// correctness check
	printf("Checking correctness...");
	checkCorrectness(floatPixels, intPixels, total);
	
	free(floatPixels);
	free(intPixels);
	return 0;
}