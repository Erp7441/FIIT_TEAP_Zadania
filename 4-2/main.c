#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 55
#define IS_VOWEL(c) ((c) == 'A' || (c) == 'E' || (c) == 'I' || (c) == 'O' || (c) == 'U' || (c) == 'Y')

// Utility
void inc_n_reset(char* inc, char* reset)
{
	(*inc)++;
	(*reset) = 0;
}
void g_inc_n_reset(char* inc, char* reset, char* g, int g_val)
{
	inc_n_reset(inc, reset);
	(*g) = g_val;
}

// All good
bool is_good(const char* arr, int len)
{
	char s_a = 0, s_g = 0;
	for (int i = 0; i < len; i++)
	{
		switch (arr[i])
		{
			case 0:
				inc_n_reset(&s_a, &s_g);
				if (s_a == 3) return true;
				break;
			case 1:
				inc_n_reset(&s_g, &s_a);
				if (s_g == 5) return true;
				break;
			default:
				break;
		}
	}
	return false;
}

// One good
void handle_stack_o(int i, char* s_o, const char* arr)
{
	int j = i - 1;
	*s_o = 0;

	while (arr[j] == 2)
	{
		(*s_o)++;
		j--;
		if (j == -1) break;
	}
}
bool one_good(const char* arr, int len)
{
	char s_a = 0, s_g = 0, s_o = 0;
	for (int i = 0; i < len; i++)
	{
		switch (arr[i])
		{
			case 0:
				s_a++;
				if (s_g != 0) handle_stack_o(i, &s_o, arr);
				if (s_a + s_o >= 3) return true;
				s_g = 0;
				break;
			case 1:
				s_g++;
				if (s_a != 0) handle_stack_o(i, &s_o, arr);
				if (s_g + s_o >= 5) return true;
				s_a = 0;
				break;
			case 2:
				s_o++;
				if (
					(s_o >= 3 && arr[i] == 2 && arr[i - 1] == 2 && arr[i - 2] == 2) ||
					(s_g + s_o >= 5) ||
					(s_a != 0 && s_a + s_o >= 3)
				) return true;
				break;
			default:
				break;
		}
	}
	return false;
}

// Greedy
void handle_greedy2else(char* greedy, int i, char* s_a, char* s_g, int len)
{
	if (i - 1 >= 0 && greedy[i - 1] == 0)
	{
		if (i + 1 < len && (greedy[i + 1] == 0 || greedy[i + 1] == 2))
			g_inc_n_reset(s_g, s_a, &greedy[i], 1);
		else if (i + 1 < len && greedy[i + 1] == 1)
			inc_n_reset(s_a, &greedy[i]);
	}
	if (i - 1 >= 0 && greedy[i - 1] == 1)
	{
		if (i + 1 < len && (greedy[i + 1] == 1 || greedy[i + 1] == 2))
			g_inc_n_reset(s_a, s_g, &greedy[i], 0);
		else if (i + 1 < len && greedy[i + 1] == 0)
		{
			greedy[i] = 1;
			(*s_g)++;
		}
	}
}
void handle_greedy2(char* greedy, int i, char* s_a, char* s_g, int len)
{
	if (i == 0)
	{
		if (i + 1 < len && greedy[i + 1] == 0) greedy[i] = 1;
		if (i + 1 < len && greedy[i + 1] == 1) greedy[i] = 0;
		return;
	}
	if (*s_a >= 2)
	{
		*s_a = 0;
		*s_g = 1;
		greedy[i] = 1;
		return;
	}
	else if (*s_g >= 4)
	{
		*s_a = 1;
		*s_g = 0;
		greedy[i] = 0;
		return;
	}
	handle_greedy2else(greedy, i, s_a, s_g, len);
}
char* g_false(const char* arr, int len)
{
	char s_a = 0, s_g = 0;
	char* greedy = (char*) calloc(len, sizeof(char));

	for (int i = 0; i < len; ++i)
		greedy[i] = arr[i];

	for (int i = 0; i < len; i++)
	{
		switch (greedy[i])
		{
			case 0:
				inc_n_reset(&s_a, &s_g);
				break;
			case 1:
				inc_n_reset(&s_g, &s_a);
				break;
			case 2:
				handle_greedy2(greedy, i, &s_a, &s_g, len);
				break;
			default:
				break;
		}
	}
	return greedy;
}

// Main
void handle_arr_vals(char** arr, int* len)
{
	while (scanf("%c", &(*arr)[*len]) >= 1)
	{
		if ((*arr)[*len] == '\n') break;
		if (IS_VOWEL((*arr)[*len])) (*arr)[*len] = 0;
		else if ((*arr)[*len] == 63) (*arr)[*len] = 2;
		else (*arr)[*len] = 1;
		(*len)++;
	}
}
void process_input(char** arr, int* len)
{
	if (IS_VOWEL((*arr)[0])) (*arr)[0] = 0;
	else if ((*arr)[0] == '?') (*arr)[0] = 2;
	else (*arr)[0] = 1;

	handle_arr_vals(arr, len);

	if (is_good(g_false((*arr), *len), *len) == 1) printf("paci\n");
	else if (one_good((*arr), *len)) printf("neviem\n");
	else printf("nepaci\n");

	for (int i = 0; i < SIZE; ++i) (*arr)[i] = -1;
	*len = 1;
}
int main()
{
	char* arr = (char*) malloc(SIZE*sizeof(char));
	int len = 1;

	while (scanf("%c", &arr[0]) >= 1)
		process_input(&arr, &len);

	free(arr);
	return 0;
}
