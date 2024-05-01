#include <stdio.h>

#define MAX 1024
#define MIN 10

typedef struct
{
	int n, m, k;
	char a[MIN][MIN];
	int b[MIN][MAX][MIN];
} State;

void rst(State* s)
{
	for (int i = 0; i < MIN; i++)
		for (int j = 0; j < MAX; j++)
			for (int k = 0; k < MIN; k++)
				s->b[i][j][k] = -1;
}

int rec(int x, int y, int z, State* s)
{
	if (z == s->k) return 1;
	if (x == s->n) return 0;
	if (s->b[x][y][z] != -1) return s->b[x][y][z];

	int total = rec(x + 1, y, z, s);
	for (int w = 0; w < s->m; w++)
	{
		if (s->a[x][w] == 'Y' && ((y >> w) & 1) == 0)
			total += rec(x + 1, y | (1 << w), z + 1, s);
	}
	s->b[x][y][z] = total;
	return total;
}

void get_input(State* s)
{
	char c;
	scanf("%d %d", &s->n, &s->m);
	for (int i = 0; i < s->n; i++)
	{
		scanf("%c", &c);
		for (int j = 0; j < s->m; j++) scanf("%c", &(s->a[i][j]));
	}
}

int main()
{
	State s = {0, 0, 0};
	while (scanf("%d", &s.k) > 0)
	{
		get_input(&s);
		rst(&s);
		printf("%d\n", rec(0, 0, 0, &s));
	}
	return 0;
}
