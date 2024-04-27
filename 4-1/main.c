#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define SIZE 202
#define MAX_HEIGHT 100000007

typedef struct
{
	int width;
	int height;
} Dimensions;

typedef struct
{
	long good;
	long bad;
} Hills;

typedef struct
{
	Hills* prev;
	Hills* curr;
	Dimensions dim;
} HillsRange;


void copy(HillsRange* range)
{
	for (int i = 0; i < SIZE; i++) range->prev[i] = range->curr[i];
}

HillsRange intialize_hills()
{
	HillsRange range = (HillsRange) {
		.prev = (Hills*) malloc(SIZE * sizeof(Hills)),
		.curr = (Hills*) malloc(SIZE * sizeof(Hills)),
		.dim = { .width = 0, .height = 0 }
	};
	for (int i = 0; i < SIZE; i++)
	{
		range.prev[i] = (Hills) { .bad = 0, .good = 0};
		range.curr[i] = (Hills) { .bad = 0, .good = 0};
	}

	scanf("%d %d", &range.dim.width, &range.dim.height);
	if (range.dim.height == 1) range.prev[1].good = 1;
	else range.prev[1].bad = 1;
	return range;
}

bool update(const HillsRange* range, int j)
{
	if (j == range->dim.height)
	{
		range->curr[j].good = range->prev[j].good + range->prev[j - 1].good + range->prev[j - 1].bad;
		return true;
	}
	range->curr[j].good = (range->prev[j].good + range->prev[j - 1].good + range->prev[j + 1].good) % MAX_HEIGHT;
	range->curr[j].bad = (range->prev[j].bad + range->prev[j - 1].bad + range->prev[j + 1].bad) % MAX_HEIGHT;
	return false;
}

void run()
{
	HillsRange range = intialize_hills();

	for (int i = 0; i < range.dim.width - 2; i++)
	{
		for (int j = 1; !update(&range, j); j++);
		copy(&range);
	}

	printf("%ld\n", range.curr[1].good);

	free(range.prev);
	free(range.curr);
}

int main()
{
	int n;
	scanf("%d", &n);
	for (int i = 0; i < n; i++) run();
	return 0;
}