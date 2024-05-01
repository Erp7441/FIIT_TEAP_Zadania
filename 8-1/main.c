#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
	int* a;
	int* b;
	int a_l;
	int b_l;
} Solution;

int compare(const void* x, const void* y) {	return *(int*)y - *(int*)x; }

Solution create(int n, int m)
{
	Solution s = {
		.a_l = n,
		.b_l = m,
		.a = (int*) calloc(n, sizeof(int)),
		.b = (int*) calloc(m, sizeof(int))
	};

	for (int i = 0; i < s.a_l; i++) scanf("%d", &s.a[i]);
	for (int i = 0; i < s.b_l; i++) scanf("%d", &s.b[i]);

	qsort(s.a, s.a_l, sizeof(int), compare);
	qsort(s.b, s.b_l, sizeof(int), compare);
	return s;
}

bool solve(Solution* s)
{
	for (int i = 0; i < s->a_l; i++)
	{
		for (int j = 0; j < s->a[i]; j++)
		{
			if (s->b[j] == 0)
			{
				printf("0\n");
				return true;
			}
			s->b[j]--;
		}
		qsort(s->b, s->b_l, sizeof(int), compare);
	}
	return false;
}


int main()
{
    int n, m;
    while (scanf("%d %d", &n, &m) > 0)
	{
		Solution  s = create(n, m);
        if (s.a[0] > m) printf("0\n");
        else if (!solve(&s)) printf("1\n");
    }
    return 0;
}