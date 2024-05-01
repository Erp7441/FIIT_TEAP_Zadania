#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define SIZE 22

typedef struct node {
	int completed;
	struct node *left;
	struct node *right;
} Node;

Node* root = NULL;
int best = INT_MAX;

Node* create_node(int completed)
{
	Node* new = (Node*) malloc(sizeof(Node));
	new->completed = completed;
	new->left = NULL;
	new->right = NULL;
	return new;
}
void create_children(long long n, Node** node, int i, int j)
{
	if (n % 2 == 0)
	{
		if (!(*node)->left) (*node)->left = create_node(0);
		*node = (*node)->left;
		return;
	}

	if (!(*node)->right) (*node)->right = create_node(j == 2 * i - 1 ? 1 : 0);
	*node = (*node)->right;
}
Node* create_tree(size_t size)
{
	Node* base = create_node(1);
	for (int i = 1; i < size; i++)
	{
		Node* node = base;
		long long new_n = (long long) pow(5, i) / 2;
		for (int j = 0; new_n > 0; j++, new_n /= 2)
			create_children(new_n, &node, i, j);
	}
	return base;
}

void traverse_tree(char* w, int i, Node* node, int n)
{
	if (w[0] == '0') return;

	size_t w_len = strlen(w);
	if (i == w_len - 1 && n < best) best = n + 1;

	for (int j = i; j < w_len - 1; j++)
	{
		if (w[j + 1] == '0')
		{
			if (!node->left) return;
			node = node->left;
		}
		else if (node->right)
		{
			node = node->right;
			if (node->completed && j + 2 == w_len && n < best)
			{
				best = n + 1;
				return;
			}
			else if (node->completed) traverse_tree(w, j + 2, root, n + 1);
		}
		else
		{
			if (node->completed) traverse_tree(w, j + 1, root, n + 1);
			return;
		}
	}
}

void swap(char* a, char* b)
{
	char temp = *a;
	*a = *b;
	*b = temp;
}

void reverse(char* s)
{
	size_t i, j;
	for (i = 0, j = strlen(s) - 1; i < j; i++, j--) swap(&s[i], &s[j]);
}

int main()
{
	root = create_tree(SIZE);
	best = INT_MAX;

	char in[50];
	while(scanf("%s", &in) == 1)
	{
		reverse(in);
		traverse_tree(in, 0, root, 0);
		if (best == INT_MAX) printf("-1\n");
		else printf("%d\n", best);
		best = INT_MAX;
	}
}