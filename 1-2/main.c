#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// Kazdy kamion ma nosnost 300 krabic
// Baliky pojmu 101 az 300 krabic


typedef struct
{
	int N;
	int* val;
	bool* pair;
} Line;



bool read_line();
int compare(const void* x, const void* y);
bool all_values_assigned(Line line);
void count_trucks(Line line, int* truck_count);

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
	int truck_count = 0;
	int temp = 0;
	int len = 0;

	Line line;
	line.N = 0;
	line.val = NULL;
	line.pair = NULL;

	scanf("%d", &line.N);
	if (line.N < 1 || line.N > 50) return false;

	line.val = (int*)calloc(line.N, sizeof(int));
	line.pair = (bool*)calloc(line.N, sizeof(bool));

	for (int i = 0; i < line.N; i++)
	{
		scanf("%d", &temp);
		if (temp >= 200)
		{
			truck_count++;
			continue;
		}

		line.val[i] = temp;
		line.pair[i] = false;
		len++;
	}

	if (len != 0) count_trucks(line, &truck_count);

	printf("%d\n", truck_count);
	free(line.val);
	free(line.pair);
	return true;
}

bool all_values_assigned(Line line)
{
	for (int i = 0; i < line.N; i++)
	{
		if (line.pair[i] == false) return false;
	}
	return true;
}

int compare(const void* x, const void* y)
{
	return *(int*)y - *(int*)x;
}

void count_trucks(Line line, int* truck_count)
{
	qsort(line.val, line.N, sizeof(int), compare);
	while (!all_values_assigned(line))
	{
		int curr_truck_sum = 0;
		for (int i = 0; i < line.N; i++)
		{
			if (line.pair[i] == false && curr_truck_sum + line.val[i] <= 300)
			{
				line.pair[i] = true;
				curr_truck_sum += line.val[i];
			}
		}
		(*truck_count)++;
	}
}