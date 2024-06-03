#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define SIZE 50
#define MAX_ITER 75000
#define IS_VOWEL(c) ((c) == 'A' || (c) == 'E' || (c) == 'I' || (c) == 'O' || (c) == 'U' || (c) == 'Y')
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define LETTERS_LEN 2
const char LETTERS[LETTERS_LEN] = { 'E', 'C' }; // 0 - Samohlaska, 1 - Spoluhlaska

typedef struct
{
	char* val;
	size_t len;
} String;

typedef struct
{
	int* val;
	size_t len;
} Array;


String create_str(const char* str)
{
	String s;
	s.val = strdup(str);
	assert(s.val != NULL);
	s.len = strlen(str);
	return s;
}

// Kontrola 3 za sebou iducich samohlasok
bool is_vowel_heavy(String str)
{
	if (str.len < 3) return false;
	for (int i = 0; i < str.len - 2; i++)
		if (
			IS_VOWEL(str.val[i]) &&
			IS_VOWEL(str.val[i + 1]) &&
			IS_VOWEL(str.val[i + 2])
		) return true;
	return false;
}

// Kontrola 5 za sebou iducich samohlasok
bool is_consonant_heavy(String str)
{
	if (str.len < 5) return false;
	for (int i = 0; i < str.len - 4; i++)
		if (
			!IS_VOWEL(str.val[i]) &&
			!IS_VOWEL(str.val[i + 1]) &&
			!IS_VOWEL(str.val[i + 2]) &&
			!IS_VOWEL(str.val[i + 3]) &&
			!IS_VOWEL(str.val[i + 4])
		) return true;
	return false;
}

Array get_question_marks_indexes(const char* arr)
{
	Array indexes = { calloc(strlen(arr), sizeof(int)), 0 };
	for (size_t i = 0; i < strlen(arr); i++)
		if (arr[i] == '?')
		{
			indexes.val[indexes.len] = i;
			indexes.len++;
		}
	return indexes;
}

void resolve_unknown(String input)
{
	// Zoznam indexov kde su '?'
	Array q = get_question_marks_indexes(input.val);

	if (q.len == input.len && input.len > 3)
	{
		printf("neviem\n");
		return;
	}

	size_t total_combs = (size_t) pow(LETTERS_LEN, (size_t) q.len);
	bool all_likable = true, one_likable = false;

	// GENEROVANIE VSETKYCH KOMBINACII
	for(size_t i = 0; i < total_combs; i++)
	{
		size_t comb_index = i;  // Kombinacny index ktory ide od i az do total_combs
		for (int j = 0; j < q.len; j++)
		{
			input.val[q.val[j]] = LETTERS[comb_index % LETTERS_LEN];
			comb_index /= LETTERS_LEN;    // Dalsia kombinacia
		}
		// KONTROLA VSETKYCH KOMBINACII
		if (is_vowel_heavy(input) || is_consonant_heavy(input)) one_likable = true;
		else all_likable = false;
		if (one_likable && !all_likable) break;
	}


	// Vsetky sa mu pacia == paci
	if (all_likable) printf("paci\n");
		// Niektore sa mu pacia == neviem
	else if (one_likable) printf("neviem\n");
		// Inak sa mu nepaci
	else printf("nepaci\n");
}

int main()
{
	char* arr = (char*) malloc(SIZE*sizeof(char));

	while (scanf("%s", arr) > 0)
		resolve_unknown(create_str(arr));

	free(arr);
	return 0;
}
