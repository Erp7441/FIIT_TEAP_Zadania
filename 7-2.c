#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define MAX_TASKS 10
#define MAX_DIVISOR 1000
#define POW(i, val) (i * (size_t)pow(MAX_TASKS, val))
#define RANGE(value, upper, lower) ( value >= lower && value <= upper )

// https://stackoverflow.com/questions/55772634/palindromic-numbers-divisible-by-any-given-number

typedef struct {
	size_t tasks, divisor, len;
	size_t** pal;
	double hi_half, lo_half; // Pomocne premenne na optimalizaciu. Nemusime opakovane delit.
} Solution;


void alloc(size_t*** arr)
{
	*arr = (size_t*) calloc(sizeof(size_t*), MAX_TASKS);
	for (size_t i = 0; i < MAX_TASKS; i++) (*arr)[i] = (size_t*) calloc(sizeof(size_t), MAX_DIVISOR);
}

void dealloc(size_t*** arr)
{
	for (size_t i = 0; i < MAX_TASKS; i++) free((*arr)[i]);
	free(*arr);
}

void rst(Solution* sol)
{
	for (size_t i = 0; i < MAX_TASKS; i++)
		for (size_t j = 0; j <= MAX_DIVISOR; j++) sol->pal[i][j] = 0;
}

void solve(Solution sol)
{
	for (size_t dpt = 0; dpt < sol.lo_half + 1; dpt++)
	{
		size_t st = 0;
		if (dpt > sol.lo_half) continue;
		if (dpt == sol.lo_half) st = 1;

		if (dpt)
		{
			for (size_t i = st; i < MAX_TASKS; i++)
				for (size_t j = 0; j < sol.divisor; j++)
					sol.pal[dpt][(((POW(i, sol.hi_half + dpt) + POW(i, sol.lo_half - dpt)) % sol.divisor) + j) % sol.divisor] += sol.pal[dpt - 1][j];
		}
		else
		{
			if (sol.len % 2) for (size_t i = st; i < MAX_TASKS; i++)
				sol.pal[dpt][POW(i, sol.hi_half) % sol.divisor] += 1;
			else for (size_t i = st; i < MAX_TASKS; i++)
				sol.pal[dpt][(POW(i, sol.hi_half) + POW(i, sol.lo_half)) % sol.divisor] += 1;
		}
	}
}

int main()
{
	Solution sol = {
		.tasks = 0,
		.divisor = 0,
		.len = 0
	};
	alloc(&sol.pal);
	scanf("%zu", &sol.tasks);
	for (size_t i = 0; i < sol.tasks; i++)
	{
		scanf("%zu %zu", &sol.divisor, &sol.len);
		sol.hi_half = (size_t)(sol.len / 2);
		sol.lo_half = (size_t)((sol.len - 1) / 2);
		rst(&sol);
		solve(sol);
		printf("%zu\n", sol.pal[(size_t)sol.lo_half][0]);
	}
	dealloc(&sol.pal);
	return 0;
}