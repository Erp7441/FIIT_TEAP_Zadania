#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// N - nosnost
// K - pocet vozidiel co sa zmesti na most

// Prejazd je bezpecny ked sucet hmotnosti nepresahuje N

// Vstupny riadok vyzera nasledovne N, K, M, a M krat cisla


typedef struct
{
	int N;
	int K;
	int M;
	int* h;
	int sum;
} Line;

void quick_sort(int* tab, int p, int r);
int partition(int* tab, int p, int r);
void swap(int* a, int* b);
bool read_line();

int main()
{
	bool again = true;
	do
	{
		again = read_line();
	} while (again);
	return 0;
}

bool read_line()
{
	Line line;
	line.sum = 0;
	bool invalid = false;

	int args = scanf("%d %d %d", &line.N, &line.K, &line.M);
	getchar();



	if (args != 3) return false;

	line.h = (int*)calloc(line.M, sizeof(int));
	for (int i = 0; i < line.M; i++)
	{
		scanf("%d", &line.h[i]);
		getchar();

		if (!(1 <= line.h[i] && line.h[i] <= 1000))
		{
			invalid = true;
		}
	}

	if (
			invalid ||
			!(1 <= line.N && line.N <= 10000) ||
			!(1 <= line.K && line.K <= line.M) ||
			!(1 <= line.M && line.M <= 9)
			)
	{
		printf("Nie\n");
		return true;
	}

	quick_sort(line.h, 0, line.M - 1);

	for (int i = 0; i < line.K; i++)
	{
		line.sum += line.h[i];
	}

	if (line.sum <= line.N)
	{
		printf("Ano\n");
	}
	else
	{
		printf("Nie\n");
	}

	free(line.h);
	return true;
}

void swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

int partition(int* tab, int p, int r)
{
	int x = tab[r];
	int i = p - 1;
	for (int j = p; j < r; j++)
	{
		if (tab[j] >= x)
		{
			i++;
			swap(&tab[i], &tab[j]);
		}
	}
	swap(&tab[i + 1], &tab[r]);
	return i + 1;
}

void quick_sort(int* tab, int p, int r)
{
	if (p < r)
	{
		int q = partition(tab, p, r);
		quick_sort(tab, p, q - 1);
		quick_sort(tab, q + 1, r);
	}
}

