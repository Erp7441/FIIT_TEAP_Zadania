#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

typedef struct {
	int n, k;
	int* arr;
	int* out;
	bool suc;
} Solution;

int* check(const int* arr, int n, int k, int* f) {
	int* o = (int*) calloc(n, sizeof(int));
	for (int i = 0; i < n - 1; i++)
		if (abs(arr[i] - arr[n - 1]) >= k) o[(*f)++] = i + 1;
	return o;
}

int eval_best(int n_o , const int* o, int i)
{
	if (o == NULL) return 0;
	int b = 100;
	for (int j = 0; j < n_o; j++)
	{
		int tmp = (int) (floor((double) o[j] / 2) + 1 + ceil((double) (i - o[j]) / 2));
		if (tmp < b) b = tmp;
	}
	return b;
}

void find_solution(Solution* s)
{
	s->suc = false;
	for (int i = 2; i <= s->kids; i++)
	{
		int n_o = 0;
		s->out = check(s->arr, i, s->k, &n_o);
		if (n_o != 0)
		{
			printf("%d\n", eval_best(n_o, s->out, i));
			s->suc = true;
			break;
		}
	}
}

int main() {
	Solution s;
	while (scanf("%d %d", &s.kids, &s.k) > 0) {
		s.arr = (int*) calloc(s.kids, sizeof(int));
		for (int i = 0; i < s.kids; i++) scanf("%d", &(s.arr[i]));
		if (s.kids < 3)
		{
			printf("%d\n", s.kids);
			continue;
		}
		find_solution(&s);
		if (!s.suc) printf("%d\n", s.kids);
	}
	return 0;
}