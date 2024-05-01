#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
	int** matrix;
	int* current;
	int* out;
	int len;
	int min;
} Solution;

Solution* create_solution()
{
	Solution* s = (Solution*) malloc(sizeof(Solution));
	scanf("%d", &s->len);

	s->matrix = (int**) malloc(s->len * sizeof(int*));
	s->out = (int*) calloc(s->len, sizeof(int));
	s->current = (int*) calloc(s->len, sizeof(int));

	for (int i = 0; i < s->len; i++)
	{
		s->matrix[i] = (int*) malloc(s->len * sizeof(int));
		s->out[i] = -1;
		s->current[i] = -1;
	}
	return s;
}

bool in_arr(int n, const int* arr, int k)
{
	for (int i = 1; i <= 8; i++) if (i < n && arr[i] == k) return true;
	return false;
}

bool find_smaller_path(const Solution* s)
{
	for (int i = 0; i < s->len; i++)
	{
		if (s->current[i] < s->out[i]) return true;
		else if (s->current[i] > s->out[i]) return false;
	}
	return false;
}

void gen(Solution* s, int d)
{
	if (d == s->len)
	{
		int curr_cost = 0;
		for (int i = 0; i < s->len - 1; i++) curr_cost += s->matrix[s->current[i]][s->current[i + 1]];

		if (curr_cost < s->min)
		{
			s->min = curr_cost;
			for (int i = 0; i < s->len; i++) s->out[i] = s->current[i];
		}
		else if (curr_cost == s->min)
		{
			if (!find_smaller_path(s)) return;
			for (int i = 0; i < s->len; i++) s->out[i] = s->current[i];
		}
		return;
	}

	if (d == 0)
	{
		s->current[0] = 0;
		gen(s, d + 1);
		return;
	}

	for (int i = 1; i < s->len; i++)
	{
		if (d > 1 && in_arr(d, s->current, i)) continue;
		s->current[d] = i;
		gen(s, d + 1);
	}
}

void populate_solution(Solution* s)
{
	for (int i = 0; i < s->len; i++)
		for (int j = 0; j < s->len; j++)
		{
			scanf("%d", &(s->matrix[i][j]));
			s->min += s->matrix[i][j];
		}
}

void print_solution(Solution s)
{
	printf("%d\n", s.min);
	for (int i = 0; i < s.len; i++) printf("%d ", s.out[i] + 1);
	printf("\n");
}

int main()
{
	Solution* s = create_solution();
	populate_solution(s);
	gen(s, 0);
	print_solution(*s);
	return 0;
}
