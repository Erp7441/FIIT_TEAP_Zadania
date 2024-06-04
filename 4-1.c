#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define SIZE 202
#define MOD_VAL 100000007
#define TASKS 20
#define MAX_WIDTH 5000
#define MIN_WIDTH 2
#define MIN_HEIGHT 1
#define MAX_HEIGHT 200

/*
 * Zaciname v bode X dole v ramci kopca. Musime prejst cez vrchol a vratit sa dole
 * https://drive.google.com/file/d/1MDmSs9UTquQKXy9Qlvd4ooVvq5fGGO6x/view
 */

typedef struct
{
	int width;
	int height;
} Dimensions;

typedef struct
{
	size_t good; // Dobry susedia
	size_t bad; // Zly susedia
} Hill;

typedef struct
{
	Hill* prev; // Predchadzajuce pohorie (schema)
	Hill* curr; // Aktualne riesene pohorie
	Dimensions dim;
} HillsRange;


void copy_curr_to_prev(HillsRange* range)
{
	for (int i = 0; i < SIZE; i++) range->prev[i] = range->curr[i];
}

HillsRange initialize_hills()
{
	HillsRange range = (HillsRange) {
			.prev = (Hill*) malloc(SIZE * sizeof(Hill)),
			.curr = (Hill*) malloc(SIZE * sizeof(Hill)),
			.dim = { .width = 0, .height = 0 }
	};


	for (int i = 0; i < SIZE; i++)
	{
		range.prev[i] = (Hill) { .bad = 0, .good = 0 };
		range.curr[i] = (Hill) { .bad = 0, .good = 0 };
	}

	do scanf("%d %d", &range.dim.width, &range.dim.height);
	while (
			range.dim.width < MIN_WIDTH || range.dim.width > MAX_WIDTH ||
			range.dim.height < MIN_HEIGHT || range.dim.height > MAX_HEIGHT
			);


	range.prev[1].good = range.dim.height == 1 ? 1 : 0;
	range.prev[1].bad = !range.prev[1].good;
	return range;
}

void dealloc_range(HillsRange* range)
{
	free(range->prev);
	free(range->curr);
	range->prev = NULL;
	range->curr = NULL;
	range->dim = (Dimensions) { .width = 0, .height = 0 };
}

bool update(const HillsRange* range, int j)
{
	// Momentalny riadok je koncovy
	if (j == range->dim.height)
	{
		/*
		 * Okolo 15.00 vo videu na google drive...
		 * Pocet dobrych susedov pre posledny riadok je sucet poctu dobrych susedov pre predposledny riadok,
		 * poctu dobrych susedov pre predpredposledny riadok a poctu zlych  susedov pre predpredposledny riadok
		 */

		// Vypocet dobrych susedov pre posledny riadok
		range->curr[j].good = range->prev[j].good + range->prev[j - 1].good + range->prev[j - 1].bad;
		return true;
	}

	// Ziska pocet vyskytov dobrych a zlych susedov voci current pozicii 'j'.

	/*
	 * Okolo 15.00 vo videu na google drive...
	 * Pocet dobrych susedov pre aktualny riadok je sucet poctu dobrych susedov pre aktualny riadok (minus jedna),
	 * poctu dobrych susedov pre aktualny riadok plus jedna a poctu zlych susedov pre aktualny riadok (minus jedna),
	 * modulo MOD_VAL lebo zadanie
	 */
	range->curr[j].good = (range->prev[j].good + range->prev[j - 1].good + range->prev[j + 1].good) % MOD_VAL;
	range->curr[j].bad = (range->prev[j].bad + range->prev[j - 1].bad + range->prev[j + 1].bad) % MOD_VAL;
	return false;
}

void run()
{
	HillsRange range = initialize_hills();

	for (int i = 0; i < range.dim.width - 2; i++)
	{
		for (int j = 1; !update(&range, j); j++);
		copy_curr_to_prev(&range);
	}

	printf("%zu\n", range.curr[1].good);
	dealloc_range(&range);
}

int main()
{
	int n = 0;
	scanf("%d", &n);
	for (int i = 0; i < n && !(n < 1 || n > TASKS); i++) run();
	return 0;
}