//! Does not work yet!!!
// TODO:: FIX

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LENGTH 50

bool isPowerOf5(long long int num) {
	while (num % 5 == 0) {
		num /= 5;
	}
	return num == 1;
}

int minParts(char *binary, int length) {
	int parts = 1;
	int start = 0;

	for (int i = 0; i < length; i++) {
		long long int num = 0;
		for (int j = i; j < length; j++) {
			num = num * 2 + (binary[j] - '0');
			if (num == 0) {
				break;
			}
			if (isPowerOf5(num)) {
				parts++;
				i = j;  // Posun na nasledujúci index po nájdení mocniny čísla 5
				break;
			}
		}
	}

	return parts == 1 ? -1 : (parts + 1) / 2;
}

int main() {
	char binary[MAX_LENGTH + 1];
	while (scanf("%s", binary) != EOF) {
		int length = strlen(binary);
		int result = minParts(binary, length);
		printf("%d\n", result);
	}
	return 0;
}
