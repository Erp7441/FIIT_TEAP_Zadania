#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <assert.h>

#define RANGE(value, upper, lower) ( value >= lower && value <= upper )
#define ABS(x) ((x) < 0 ? -(x) : (x))

/**
 * Zdroje:
 * Cviko 2021 (https://drive.google.com/file/d/1eA7ZalviE8l9F8zhqJbM7zbTOt_JPZvQ/view)
 * Cviko 2020 (https://drive.google.com/file/d/18GDCnZnV9qbI7A0Ko8es5po3D8i_tjYZ/view)
 */

// Kvoli tomu ze musime vyriesit jednu a jednu hned preskocit. Takze ked mame menej ako 2 tak mozeme riesit len prvu
#define MIN_TASKS 2
#define MAX_TASKS 50
#define MIN_HAPP 0
#define MAX_HAPP 1000

typedef struct {
	int tasks_count, max_happ;
	int* happ_values;
	int* output;
	int output_n;
} Solution;

Solution s;

void reset_solution()
{
	if (s.happ_values) free(s.happ_values);
	if (s.output) free(s.output);
	s.tasks_count = 0;
	s.max_happ = 0;
	s.happ_values = NULL;
	s.output = NULL;
	s.output_n = 0;
}

void clear_stdin(int items)
{
	for(int i = 0; i < items; i++) scanf("%d", malloc(0));
}

int get_happ(int output, int current_task_index)
{
	// Horna cast pozice tasku a output
	double higher_frac = (current_task_index - output) / 2.0;
	int higher = (double)higher_frac != (int)higher_frac ? (int)higher_frac + 1 : (int)higher_frac;
	// Dolna cast output
	int lower = (int)(output / 2.0) + 1;
	return lower + higher;
}

// Kontrola ci momentalna radost neprekrocila maximum (tym padom koncime).
int* check_happ_level(int n)
{
	if (s.happ_values == NULL) return NULL;
	int* out = (int*) calloc(n, sizeof(int));
	for (int i = 0; i < n; i++)
	{
		int current_happiness = s.happ_values[i] - s.happ_values[n - 1];
		if (ABS(current_happiness) < s.max_happ) continue;
		out[s.output_n++] = i + 1;
	}
	return out;
}

// Video - 09:24 - linearne prehladavanie, minimum je nahraditelne takze nas az tak netrapi (12:35)
int find_max_happ_task(int i)
{
	if (s.output == NULL) return 0;
	int max = INT_MAX;
	for (int j = 0; j < s.output_n; j++)
	{
		int tmp = (int) get_happ(s.output[j], i);
		if (tmp < max) max = tmp;
	}
	return max;
}

// Side Note - Preskakovat ulohy mozeme len pokial sme nejake predchadzajuce vyriesili
bool find_solution()
{
	for (int i = MIN_TASKS; i <= s.tasks_count; i++)
		if ((s.output = check_happ_level(i)) && s.output_n != 0)
		{
			printf("%d\n", find_max_happ_task(i));
			return true;
		}
	return false;
}

bool init()
{
	if (s.tasks_count <= MIN_TASKS)
	{
		clear_stdin(s.tasks_count);
		printf("%d\n", s.tasks_count);
		return false;
	}
	s.happ_values = (int*) calloc(s.tasks_count, sizeof(int));
	assert(s.happ_values != NULL);
	for (int i = 0; i < s.tasks_count; i++)
	{
		scanf("%d", &(s.happ_values[i]));
		if (!RANGE(s.happ_values[i], MAX_HAPP+1, MIN_HAPP)) return false;
	}
	return true;
}

int main()
{
	while (scanf("%d %d", &s.tasks_count, &s.max_happ) > 0)
	{
		if (!RANGE(s.tasks_count, MAX_TASKS, MIN_TASKS-1) || !RANGE(s.max_happ, MAX_HAPP, MIN_HAPP+1) || !init())
			continue;
		// Ked nenajdeme rozdiel medzi najvacsiou a najnizsiou happiness tak printujeme len pocet uloh.
		if (!find_solution()) printf("%d\n", s.tasks_count);
		reset_solution();
	}
	return 0;
}