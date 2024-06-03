#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define RANGE(value) ( value >= 1 && value <= 100 )

typedef enum
{
	FRIENDS = 0,
	ACTIVITIES = 0,
	TABLES = 1
} Layout;

typedef struct
{
	size_t* layout[2]; // Matica ktora drzi rozlozenie obsadenosati a poctu kamaratov
	size_t counters[2]; // Pole ktore drzi pocitadla pre aktivity (kamaratov na aktivitach) a stoly
} Solution;

// Sluzi na zoradovanie najvolnejsich stolov (podla obsadenosti)
int compare(const void* x, const void* y) {	return (int)(*(const size_t*)y - *(const size_t*)x); }

Solution create(size_t activities, size_t tables)
{
	Solution sol = {
		.counters = { activities, tables },
		.layout = {
			// Reprezentuje pocet kamaratov per kruzok. Index je "ID" kruzku. Cislo je pocet kamaratov na kruzku.
			(size_t*) calloc(activities, sizeof(size_t)),
			// Reprezentuje pocet volnych miest per stol. Index je "ID" stola. Cislo je pocet volnych miest na stole.
			(size_t*) calloc(tables, sizeof(size_t))
		}
	};

	for (size_t i = 0; i < sol.counters[ACTIVITIES]; i++)
		scanf("%zu", &sol.layout[FRIENDS][i]);
	for (size_t i = 0; i < sol.counters[TABLES]; i++)
		scanf("%zu", &sol.layout[TABLES][i]);

	qsort(sol.layout[FRIENDS], sol.counters[ACTIVITIES], sizeof(size_t), compare);
	qsort(sol.layout[TABLES], sol.counters[TABLES], sizeof(size_t), compare);
	return sol;
}

void destroy(Solution* s)
{
	free(s->layout[FRIENDS]);
	free(s->layout[TABLES]);
	s->layout[FRIENDS] = NULL;
	s->layout[TABLES] = NULL;
	s->counters[ACTIVITIES] = 0;
	s->counters[TABLES] = 0;
}

bool solve(const Solution* sol)
{
	// Pre kazdu aktivitu
	for (size_t i = 0; i < sol->counters[ACTIVITIES]; i++)
	{
		// Pre kazdeho kamarata v ramci aktivity
		for (size_t j = 0; j < sol->layout[FRIENDS][i]; j++)
		{
			/*
			 * Odratavam volne miesto pri stole. A posauvam sa o stol dalej.
			 * Tym porozhadzujem kamaratov z rovnakeho kruzku tak aby kazdy sedel za inym stolom.
			 */

			// Ak uz nie je miesto za stolami tak nemam riesenie
			if (sol->counters[TABLES]-1 < j || sol->layout[TABLES][j] == 0)
			{
				printf("0\n");
				return false;
			}
			sol->layout[TABLES][j]--;  // Odratavanie volneho miesta pri stole
		}
		// Zoradujeme stoly podla najvolnejsieho
		qsort(sol->layout[TABLES], sol->counters[TABLES], sizeof(size_t), compare);
	}
	return true;
}


int main()
{
	size_t activities, tables;
    while (scanf("%zu %zu", &activities, &tables) > 0)
	{
		if (!RANGE(activities) || !RANGE(tables) ) continue;
		Solution sol = create(activities, tables);
        if (sol.layout[FRIENDS][0] > tables) printf("0\n");  // Mame viacej kamaratov ako stolov je automaticky fail
        else if (solve(&sol)) printf("1\n");
		destroy(&sol);
    }
    return 0;
}