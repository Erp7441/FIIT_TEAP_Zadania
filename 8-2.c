#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 350
#define BUFF 256
#define RANGE(value) ( value >= 1 && value <= 100 )

typedef enum
{
	SOURCE = 2,
	SINK = 1
} Eval;

typedef enum
{
	FOOD = 0,
	DRINK = 1
} Beverage;

typedef struct node
{
	int data;
	struct node* top;
} Node;

typedef struct
{
	Node* top;
	size_t len;
} Stack;

typedef struct
{
	int drink;
	int counters[2];
	int food[BUFF];
} Storage;

typedef struct
{
	int kids, food, drink;
	int mat[SIZE][SIZE]; // Matica drinkov, jedla a deti.
	Stack* stack;
	Storage tmp;
} Solution;

Solution sol = {0, 0, 0, {0 }, NULL, {0, 0, 0,  {0} } };

Node* create_node(int data)
{
	Node* new = (Node*) malloc(sizeof(Node));
	new->data = data;
	new->top = NULL;
	return new;
}

Stack* init_stack()
{
	Stack* stack = (Stack*) malloc(sizeof(Stack));
	stack->top = NULL;
	stack->len = 0;
	return stack;
}

int pop()
{
	if (sol.stack == NULL || sol.stack->top == NULL) return -1;
	// Ulozime si prvok na vrchu stacku
	Node* old = sol.stack->top;
	int data = old->data;
	// Nastavime novy vrch stacku
	sol.stack->top = old->top;
	free(old);  // Zmazeme stary prvok
	sol.stack->len--;
	return data;
}

void push(int data)
{
	if (sol.stack == NULL) sol.stack = init_stack();
	Node* new = create_node(data);
	new->top = sol.stack->top;
	sol.stack->top = new;
	sol.stack->len++;
}

// SINK a SOURCE (zalezi od 'type')
size_t ev(Eval type)
{
	return sol.kids + sol.kids + sol.food + sol.drink + type;
}

void reset_matrix()
{
	for (int i = 0; i < ev(SOURCE); i++)
		for (int j = 0; j < ev(SOURCE); j++) sol.mat[i][j] = 0;
}

int kids_food(int pos)
{
	// Gets kids food position
	return sol.kids + sol.food + pos;
}

void getBeverage(int i, Beverage type)
{
	size_t counter = type == FOOD ? sol.tmp.counters[FOOD] : sol.tmp.counters[DRINK];
	for (size_t j = 0; j < counter; j++)
	{
		if (type == FOOD)
		{
			scanf("%d", &sol.tmp.food[j]);
			sol.mat[sol.tmp.food[j]][sol.food + i + 1] = 1;
		}
		else if(type == DRINK)
		{
			scanf("%d", &sol.tmp.drink);
			sol.mat[kids_food(i+1)][sol.kids + sol.tmp.drink + kids_food(0)] = 1;
		}
	}
}

void init_solution()
{
	// Nastavuje jedna na pociatocnych poziaciach SOURCE na 1
	for (int i = 0; i < sol.food; i++)
		sol.mat[0][i + 1] = 1;
	for (int i = 0; i < sol.kids; i++)
	{
		scanf("%d %d", &sol.tmp.counters[FOOD], &sol.tmp.counters[DRINK]);
		getBeverage(i, FOOD);
		getBeverage(i, DRINK);
		sol.mat[sol.food + i + 1][sol.food + i + 1 + sol.kids] = 1;
	}
	// Nastavuje drinky na poziciach SINK na 1
	for (int i = 0; i < sol.drink; i++)
		sol.mat[sol.kids + kids_food(i+1)][ev(SINK)] = 1;
}

// https://www.programiz.com/dsa/graph-bfs
bool bfs(int* parent)
{
	// Na prvom vrchole sa nachadzame (true), ostatne su zatial nenavstivene (false)
	bool visited[SIZE] = { true, };
	sol.stack = init_stack();
	push(0);

	while (sol.stack->top != NULL && sol.stack->len > 0)
	{
		int latest = pop();
		for (int i = 0; i < ev(SOURCE); i++)
		{
			if (sol.mat[latest][i] > 0 && !visited[i])
			{
				visited[i] = true;
				parent[i] = latest;
				if (i == ev(SINK)) return true;
				push(i);
			}
		}
	}
	return false;
}

// https://www.programiz.com/dsa/ford-fulkerson-algorithm
int edmonds_karp()
{
	int parent[SIZE] = {-1 }, max_flow;
	for (max_flow = 0; bfs(parent); max_flow++)
	{
		for (size_t vertex = ev(SINK); vertex != 0; vertex = parent[vertex])
		{
			sol.mat[parent[vertex]][vertex] = 0;
			sol.mat[vertex][parent[vertex]] = 1;
		}
	}
	return max_flow;
}

int main()
{
	while (scanf("%d %d %d", &sol.kids, &sol.food, &sol.drink) > 0)
	{
		if (!RANGE(sol.kids) || !RANGE(sol.food) || !RANGE(sol.drink)) continue;
		reset_matrix();
		init_solution();
		printf("%d\n", edmonds_karp());
	}
	return 0;
}
