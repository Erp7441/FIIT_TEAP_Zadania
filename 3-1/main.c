#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// TODO:: Refactor variable names
// TODO:: Extract to functions

char arr[50];

bool is_number(int i)
{
	return (arr[i] >= 48) && (arr[i] < 58);
}

void fill_len_2(unsigned long long** mem, int length)
{
	for (int i = 0; i < length-1; ++i)
	{
		if ((is_number(i) || arr[i] == 45) && is_number(i+1)) mem[i][i + 1] = 1;
		else mem[i][i + 1] = 0;
	}
}

void fill_in_len_3_num(unsigned long long** mem, int j, int i)
{
	// TODO:: This is the same #1
	int good = 1;
	int now = j;
	while (now <= j+i-1)
	{
		if(!is_number(now))
		{
			good = 0;
			break;
		}
		now++;
	}
	mem[j][j + i - 1] = good;
}

void fill_in_len_3_not_num(unsigned long long** mem, int j, int i)
{
	unsigned long long int count = 0;
	for (int k = 0; k < i-2; ++k)
	{
		count += mem[j + 1][j + 1 + k] * mem[j + 2 + k][j + i - 1];
	}
	if (arr[j] == 45)
	{
		// TODO:: This is the same #1
		int good = 1;
		int now = j+1;
		while (now <= j+i-1)
		{
			if(!is_number(now))
			{
				good = 0;
				break;
			}
			now++;
		}
		count += good;
	}
	mem[j][j + i - 1] = count;
}

unsigned long long notation(int length)
{
	unsigned long long** mem = (unsigned long long**) malloc(length * sizeof(unsigned long long*));
	for (int i = 0; i < length; ++i)
	{
		mem[i] = (unsigned long long*) calloc(length, sizeof(unsigned long long));
	}

	// Incializacia diagonaly
	for (int i = 0; i < length; ++i)
	{
		if (is_number(i)) mem[i][i] = 1;
		else mem[i][i] = 0;
	}

	// Dlzka 2
	fill_len_2(mem, length);

	// Dlzka 3 az n
	for (int i = 3; i < length+1; ++i)
	{
		for (int j = 0; j < length - i + 1; ++j)
		{
			if (is_number(j)) fill_in_len_3_num(mem, j, i);
			else fill_in_len_3_not_num(mem, j, i);
		}
	}

	// Vypis
	for (int l = 0; l < length; ++l)
	{
		for (int j = 0; j < length; ++j)
		{
			printf("%2lld ", mem[l][j]);
		}
		printf("\n");
	}
	printf("\n");

	// Vysledok
	unsigned long long res = mem[0][length - 1];
	for (int i = 0; i < length; ++i)
	{
		free(mem[i]);
	}
	free(mem);
	return res;
}

int main()
{
	while (scanf("%s", arr) >= 1)
	{
		int length = 0;
		printf("%lld\n", notation(length));
	}
	return 0;
}