#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <memory.h>

#define MAX_CIPHER 9

typedef struct
{
	int** paths;
	int* current_path;
	int* found_path;
	int len, min_found_path_len;
} Solution;

Solution* create_solution()
{
	Solution* s = (Solution*) malloc(sizeof(Solution));
	scanf("%d", &s->len);

	s->paths = (int**) malloc(s->len * sizeof(int*));
	s->found_path = (int*) calloc(s->len, sizeof(int));
	s->current_path = (int*) calloc(s->len, sizeof(int));
	s->min_found_path_len = INT_MAX;

	for (int i = 0; i < s->len; i++)
	{
		s->paths[i] = (int*) malloc(s->len * sizeof(int));
		s->found_path[i] = -1;
		s->current_path[i] = -1;
	}
	return s;
}

void delete_solution(Solution** s)
{
	for (int i = 0; i < (*s)->len; i++) free((*s)->paths[i]);
	free((*s)->paths);
	free((*s)->found_path);
	free((*s)->current_path);
	free(*s);
}

int get_path_len(const Solution* s)
{
	if (!s || !s->len || !s->current_path) return 0;
	int cost = 0;
	for (int i = 0; i < s->len - 1; i++)
		cost += s->paths[s->current_path[i]][s->current_path[i + 1]];
	return cost;
}

bool in_arr(int n, const int* arr, int k)
{
	if (!arr) return false;
	for (int i = 1; i <= MAX_CIPHER; i++)
		if (i < n && arr[i] == k) return true;
	return false;
}

bool find_smaller_path(const Solution* s)
{
	if (!s || !s->len || !s->found_path || !s->current_path) return false;
	// Cyklicky porovnavame momentalnu (current_path) cestu s doteraz najlacensjiou cestou (found_path)
	for (int i = 0; i < s->len; i++)
		if (s->current_path[i] < s->found_path[i]) return true;
		else if (s->current_path[i] > s->found_path[i]) return false;
	return false;
}

/**
 * Rekurzivne generovanie vsetkych moznych ciest a nasledne vyskusanie pomocou backtrackingu.
 * Parameter `d` je aktualna hlbka cesty. Ak je `d` rovne dlzke grafu, znamena to, ze bola najdena cesta a potom ju
 * skontroluje voci "doteraz" najkratsej ceste.
 *
 * @param d Aktualna hlbka
 * @param s Instancia riesenia
 *
 * Zdroje:
 * https://stackoverflow.com/questions/8889942/how-to-write-documentation-comments-in-ansi-c  - Fancy doc komentar
 * https://www.programiz.com/dsa/backtracking-algorithm - Backtracking algoritmus
 */
void backtrack(Solution* s, int d)
{
	if (!s || !s->len || !s->current_path || !s->found_path) return;
	// Pokial sme dosiahli 5 vrcholov v ceste. Cize mame kompletnu cestu
	if (d == s->len)
	{
		// Vypocitame cenu kompletnej cesty
		int cost = get_path_len(s);

		// Pokial je dlzka cesty kratsia nez do teraz najkratsia najdena cesta
		// Zdroj: https://stackoverflow.com/questions/33660077/fast-way-to-copy-an-array
		if (cost < s->min_found_path_len && (s->min_found_path_len = cost))
			memcpy(s->found_path, s->current_path, s->len * sizeof(int));
		// Pokial ma cesta rovnaku dlzku. Kontrolujeme hodnoty jednotlivych vrcholov.
		// Ak najdeme mensiu hodnotu, znamena to, ze je to lepsia (menej narocna) cesta.
		else if (cost == s->min_found_path_len && find_smaller_path(s))
			memcpy(s->found_path, s->current_path, s->len * sizeof(int));
	}
		// Vyskusame vsetky mozne kombinacie dalsich vrcholov od momentalneho bodu v ceste
	else for (int i = 1; i < s->len; i++)
		// Pokial vrchol nie je v ceste tak o pridaj do momentalnej cesty
		if ((d < 1 || !in_arr(d, s->current_path, i)) && (s->current_path[d] = i))
			backtrack(s, d + 1);
}

void load_vertices(Solution* s)
{
	if (!s || !s->len || !s->paths || !s->current_path) return;
	for (int i = 0; i < s->len; i++)
		for (int j = 0; j < s->len; j++)
			scanf("%d", &(s->paths[i][j]));
	s->current_path[0] = 0; // Zaciname od prveho vrchola
}

void print_solution(Solution s)
{
	if (!s.len || !s.found_path || !s.min_found_path_len) return;
	printf("%d\n", s.min_found_path_len);
	for (int i = 0; i < s.len; i++)
		printf("%d ", s.found_path[i] + 1);
	printf("\n");
}

int main()
{
	Solution* s = create_solution();
	load_vertices(s);
	backtrack(s, 1);
	print_solution(*s);
	delete_solution(&s);
	return 0;
}
