#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//#define DEBUG

char arr[50];

bool is_number(int i)
{
	return (arr[i] >= 48) && (arr[i] < 58);
}

void print_matrix(unsigned long long** matrix, int len)
{
	for (int l = 0; l < len; ++l)
	{
		for (int j = 0; j < len; ++j)
		{
			printf("%2lld ", matrix[l][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void dealloc_matrix(unsigned long long** matrix, int len)
{
	for (int i = 0; i < len; ++i)
	{
		free(matrix[i]);
	}
	free(matrix);
}

void fill_len_2(unsigned long long** matrix, int len)
{
	for (int i = 0; i < len-1; ++i)
	{
		if ((is_number(i) || arr[i] == 45) && is_number(i+1)) matrix[i][i + 1] = 1;
		else matrix[i][i + 1] = 0;
	}
}

void fill_in_above_2(int* g, int* n, int j, int i)
{
	while (*n <= j+i-1)
	{
		if(!is_number(*n))
		{
			*g = 0;
			break;
		}
		(*n)++;
	}
}

void fill_in_len_3_num(unsigned long long** matrix, int j, int i)
{
	int g = 1, n = j;
	fill_in_above_2(&g, &n, j, i);
	matrix[j][j + i - 1] = g;
}

void fill_in_len_3_not_num(unsigned long long** matrix, int j, int i)
{
	unsigned long long int count = 0;
	for (int k = 0; k < i-2; ++k)
	{
		count += matrix[j + 1][j + 1 + k] * matrix[j + 2 + k][j + i - 1];
	}
	if (arr[j] == 45)
	{
		int g = 1, n = j + 1;
		fill_in_above_2(&g, &n, j, i);
		count += g;
	}
	matrix[j][j + i - 1] = count;
}

unsigned long long notation(int len)
{
	unsigned long long** matrix = (unsigned long long**) malloc(len * sizeof(unsigned long long*));
	for (int i = 0; i < len; ++i)
	{
		matrix[i] = (unsigned long long*) calloc(len, sizeof(unsigned long long));
	}

	// Incializacia diagonaly
	for (int i = 0; i < len; ++i)
	{
		if (is_number(i)) matrix[i][i] = 1;
		else matrix[i][i] = 0;
	}

	// Dlzka 2
	fill_len_2(matrix, len);

	// Dlzka 3 az n
	for (int i = 3; i < len+1; ++i)
	{
		for (int j = 0; j < len - i + 1; ++j)
		{
			if (is_number(j)) fill_in_len_3_num(matrix, j, i);
			else fill_in_len_3_not_num(matrix, j, i);
		}
	}

	// Vypis
	#ifdef DEBUG
		print_matrix(matrix, len);
	#endif

	// Vysledok
	unsigned long long res = matrix[0][len - 1];

	// Uvolnenie pamate
	dealloc_matrix(matrix, len);

	return res;
}

int main()
{
	while (scanf("%s", arr) >= 1)
	{
		int len = 0;
		while (arr[++len] != 0);
		printf("%lld\n", notation(len));
	}
	return 0;
}