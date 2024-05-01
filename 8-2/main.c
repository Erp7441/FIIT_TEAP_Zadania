#include <stdio.h>
#include <stdlib.h>

#define SIZE 402
#define BUFF 100
#define SZE(n, f, d) (n + n + f + d + 2)
#define SNK(n, f, d) (n + n + f + d + 1)

typedef struct node
{
	int data;
	struct node* next;
} Node;

typedef struct
{
	int mat[SIZE][SIZE];
	int n, f, d;
	Node* q;
} Solution;

Solution s = { { 0 }, 0, 0, 0, NULL};

Node* create_node(int data)
{
	Node* new = (Node*) malloc(sizeof(Node));
	new->data = data;
	new->next = NULL;
	return new;
}
void add(int data)
{
	if (s.q == NULL) s.q = create_node(data);
	else
	{
		Node* new = create_node(data);
		new->next = s.q;
		s.q = new;
	}
}
void reset_matrix()
{
	for (int i = 0; i < SZE(s.n, s.f, s.d); i++)
		for (int j = 0; j < SZE(s.n, s.f, s.d); j++) s.mat[i][j] = 0;
}

int pop()
{
	Node* old = s.q;
	int data = s.q->data;
	s.q = s.q->next;
	free(old);
	return data;
}

int bfs(int* p)
{
	int vis[SIZE] = { 0 };
	s.q = NULL;

	add(0);
	vis[0] = 1;

	while (s.q != NULL)
	{
		int u = pop();
		for (int i = 0; i < SZE(s.n, s.f, s.d); i++)
		{
			if (s.mat[u][i] > 0 && !vis[i])
			{
				p[i] = u;
				vis[i] = 1;
				if (i == SNK(s.n, s.f, s.d)) return 1;
				add(i);
			}
		}
	}
	return 0;
}

int fulkerson()
{
	int p[SIZE] = { -1 }, max_f = 0;
	while (bfs(p))
	{
		int v = SNK(s.n, s.f, s.d);
		max_f += 1;

		while (v != 0)
		{
			s.mat[p[v]][v] = 0;
			s.mat[v][p[v]] = 1;
			v = p[v];
		}
	}
	return max_f;
}

void get_food(int i, int f_c, int f_tmp[BUFF])
{
	for (int j = 0; j < f_c; j++)
	{
		scanf("%d", f_tmp + j);
		s.mat[f_tmp[j]][s.f + i + 1] = 1;
	}
}

void get_drink(int i, int d_c, int* d_tmp)
{
	for (int j = 0; j < d_c; j++)
	{
		scanf("%d", d_tmp);
		s.mat[s.n + s.f + i + 1][s.n + s.n + s.f + *d_tmp] = 1;
	}
}

void init_solution(int f_tmp[BUFF], int* d_tmp, int* f_c, int* d_c)
{
	for (int i = 0; i < s.f; i++) s.mat[0][i + 1] = 1;
	for (int i = 0; i < s.n; i++)
	{
		s.mat[s.f + i + 1][s.f + i + 1 + s.n] = 1;
		scanf("%d %d", f_c, d_c);
		get_food(i, *f_c, f_tmp);
		get_drink(i, *d_c, d_tmp);
	}
	for (int i = 0; i < s.d; i++) s.mat[s.n + s.n + s.f + i + 1][SNK(s.n, s.f, s.d)] = 1;
}

int main()
{
	int f_tmp[BUFF], d_tmp, f_c, d_c;
	while (scanf("%d %d %d", &s.n, &s.f, &s.d) > 0)
	{
		reset_matrix();
		init_solution(f_tmp, &d_tmp, &f_c, &d_c);
		printf("%d\n", fulkerson());
	}
	return 0;
}
