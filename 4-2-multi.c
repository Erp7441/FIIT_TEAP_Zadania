#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>

#define SIZE 50
#define IS_VOWEL(c) ((c) == 'A' || (c) == 'E' || (c) == 'I' || (c) == 'O' || (c) == 'U' || (c) == 'Y')
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define LETTERS_LEN 2
#define THREADS_COUNT 1000
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

typedef struct
{
	String input;
	size_t total_combs, start, end;
	bool* all_likable, *one_likable;
	Array q;
	pthread_mutex_t* lock;
} thread_args;


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


void* routine(void* arg)
{
	thread_args* args = (thread_args*) arg;
	// GENEROVANIE VSETKYCH KOMBINACII
	for(size_t i = 0; i < args->total_combs; i++)
	{
		size_t comb_index = i;  // Kombinacny index ktory ide od i az do total_combs
		for (int j = 0; j < args->q.len; j++)
		{
			args->input.val[args->q.val[j]] = LETTERS[comb_index % LETTERS_LEN];
			comb_index /= LETTERS_LEN;    // Dalsia kombinacia
		}

		bool _local_one_likable = false, _local_all_likable = true;
		// KONTROLA VSETKYCH KOMBINACII
		if (is_vowel_heavy(args->input) || is_consonant_heavy(args->input)) _local_one_likable = true;
		else _local_all_likable = false;

		pthread_mutex_lock(args->lock);
		if (_local_all_likable != *(args->all_likable)) *(args->all_likable) = _local_all_likable;
		if (_local_one_likable != *(args->one_likable)) *(args->one_likable) = _local_one_likable;
		if (args->one_likable && !args->all_likable) break;
		pthread_mutex_unlock(args->lock);
	}
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

	// Vytvori thread_args
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_init(&lock, NULL);
	pthread_t threads[THREADS_COUNT];

	// Vytvori vsetky thready
	thread_args args[THREADS_COUNT];
	for (int i = 0; i < THREADS_COUNT; i++)
	{
		thread_args curr_args = { input, total_combs, -1, -1, &all_likable, &one_likable, q, &lock };
		if (i == 0)
		{
			curr_args.start = 0;
			curr_args.end = (total_combs / THREADS_COUNT);
		}
		else
		{
			curr_args.start = args[i-1].end;
			curr_args.end = args[i-1].end + (total_combs / THREADS_COUNT);
		}
		args[i] = curr_args;
		pthread_create(&threads[i], NULL, routine, &curr_args);
	}

	// Cakam na vsetky thready
	for (int i = 0; i < THREADS_COUNT; i++)
	{
		int ret = pthread_join(threads[i], NULL);
		while (ret != 0)
		{
			ret = pthread_join(threads[i], NULL);
		}
	}
	pthread_mutex_destroy(&lock);


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
