#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <assert.h>
#include <regex.h>

#define SIZE 13
#define BUFFER 256
#define LINE_REGEX "^([0-9]+)(\\ )([0-9]+)$"

/**
 * Zdroj:
 * https://www.geeksforgeeks.org/find-value-k-th-bit-binary-representation/
 * https://www.geeksforgeeks.org/print-kth-least-significant-bit-number/
 */
#define GET_BIT(subset, pos, len) (subset >> (len - pos))

typedef enum {
	MATCHED = 0,
	UNMACHED = 1,
	FAIL = 2
} MatchState;

typedef struct
{
	char* val;
	size_t len;
} String;

typedef struct
{
	String str;
	int target;
} Line;

typedef struct {
	Line line;
	MatchState state;
} Match;

typedef struct
{
	int count; // Pocet znamienok v retazci
	int value, best; // Momentalna a najlepsia hodnota
	size_t prev; // Predchadzajuca pozicia v ramci binarneho retazca
} Board;


String create_str(const char* str)
{
	String s;
	s.val = strdup(str);
	assert(s.val != NULL);
	s.len = strlen(str);
	return s;
}

String int_to_string(int num)
{
	char* str = (char*) calloc(BUFFER, sizeof(char));
	sprintf(str, "%d", num);
	String s = create_str(str);
	free(str);
	return s;
}

void free_str(String* s)
{
	if (s == NULL || s->val == NULL) return;
	free(s->val);
	s->val = NULL;
	s->len = 0;
}

int str_to_int(String str, size_t left, size_t right) {
	if (str.val == NULL || left + right >= str.len) return -1;
	if (right == 0) return atoi(str.val + left);

	// Orezavanie stringu z lava do prava
	String copy = create_str(str.val+left);
	char* temp= (char*) calloc(copy.len, sizeof(char));
	// Prekopirovanie n casti po len-right (oreze string z prava do leva)
	strncpy(temp, copy.val, copy.len - right);
	free_str(&copy);
	return atoi(temp);
}

int matches(const char* str, const char* regex)
{
	regex_t reg;
	if (regcomp(&reg, regex, REG_EXTENDED)) return INT_MIN;
	int reg_ret = regexec(&reg, str, 0, NULL, 0);
	if (!reg_ret) return true;
	else if (reg_ret == REG_NOMATCH) return false;
	return INT_MIN;
}

Match load_line()
{
	Match match = {.state = UNMACHED};
	char buf[BUFFER] = {0};
	fgets(buf, BUFFER, stdin);
	buf[strcspn(buf, "\n")] = '\0';

	int success = matches(buf, LINE_REGEX);
	if (success == INT_MIN) match.state = FAIL;
	else if (success)
	{
		match.state = MATCHED;
		const char* value = NULL;
		char* target = NULL;
		value = strtok_r(buf, " ", &target);
		match.line.target = atoi(target);
		match.line.str = create_str(value);
	}
	else match.line.str = create_str(buf);

	return match;
}

void add_plus_to_string(String str, size_t pos, size_t str_left, size_t str_right, Board* board)
{
	board->count++;  // Pocet znamienok v retazci
	board->value += str_to_int(str, str_left, str_right);
	// Nastavim novu poziciu predchadzajuceho bitu (aby som vedel odklial mam pokracovat shiftovanie)
	board->prev = !pos ? board->prev : pos;
}

bool read_line()
{
	Board board;
	board.best = INT_MAX;

	Match match = load_line();
	Line* line = match.state == MATCHED ? &match.line : NULL;

	if (match.state == UNMACHED)
	{
		free_str(&match.line.str);
		return false;
	}

	// Prechadzam cez vsetkych 2^n podmnozin
	for (size_t i = 0; i < pow(2, (double)line->str.len); i++)
	{
		board.prev = 0;
		board.value = 0;
		board.count = 0;

		// Prechadzam cez vsetky bitove podmnoziny
		for (size_t pos = 1; pos <= line->str.len; pos++)
			// Ak je least-significant bit 1, pridam do sumy  (resp. pridal som znamienko do vstupneho retazca)
			// Zdroj: https://stackoverflow.com/questions/6647783/check-value-of-least-significant-bit-lsb-and-most-significant-bit-msb-in-c-c
			if (GET_BIT(i, pos, line->str.len) % 2)
				add_plus_to_string(line->str, pos, board.prev, line->str.len - pos, &board);
			// Ak dalsia pozicia bude posledna. Vykoname posledne pridanie pluska
			else if (pos+1 > line->str.len && line->str.len+1 != board.prev)
				add_plus_to_string(line->str, 0, board.prev, 0, &board);

		// Kontrolujem ci som nasiel najlepsie riesenie. (Teda ci sa rovna value potrebnej target value a ci je pocet
		// znamienok lepsi resp. najmensi)
		if (board.value == line->target && board.count-1 < board.best) board.best = board.count-1;
	}
	printf("%s\n", board.best == INT_MAX ? "IMPOSSIBLE" : int_to_string(board.best).val);
	free(line->str.val);
	return true;
}


int main()
{
	while (read_line());
	return 0;
}

