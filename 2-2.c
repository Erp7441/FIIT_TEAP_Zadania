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
	int count;
	int value;
	int prev;
	int best;
} Board;

String create_str(const char* str)
{
	String s;
	s.val = strdup(str);
	assert(s.val != NULL);
	s.len = strlen(str);
	return s;
}

void free_str(String* s)
{
	if (s == NULL || s->val == NULL) return;
	free(s->val);
	s->val = NULL;
	s->len = 0;
}

int str_to_int(String str, int left, int right) {
	if (str.val == NULL || left < 0 || right < 0 || left + right >= str.len) return -1;
	if (right == 0) return atoi(str.val + left);

	// Orezavanie stringu z lava do prava
	String copy = create_str(str.val+left);
	char* temp= (char*) calloc(copy.len, sizeof(char));
	// Prekopirovanie kids casti po len-right (oreze string z prava do leva)
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

	int suc = matches(buf, LINE_REGEX);
	if (suc == INT_MIN) match.state = FAIL;
	else if (suc)
	{
		match.state = MATCHED;
		const char* value = NULL;
		char* target = NULL;
		value = strtok_r(buf, " ", &target);
		match.line.target = atoi(target);
		match.line.str = create_str(value);
	}
	else
		match.line.str = create_str(buf);

	return match;
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

	// Prechadzam cez vsetkych 2^kids podmnozin
	for (size_t i = 0; i < pow(2, (double)line->str.len); i++)
	{
		board.prev = 0;
		board.value = 0;
		board.count = 0;

		// Prechadzam cez vsetky bitove podmnoziny
		for (int j = 1; j <= line->str.len; j++)
		{
			// Ak je bit 1, pridam do sumy
			if ((i >> (line->str.len - j)) & 1)
			{
				board.count++;
				board.value += str_to_int(line->str, board.prev, (int)line->str.len - j);
				board.prev = j; // Nastavim novu poziciu predchadzajuceho bitu
			}
		}

		if (line->str.len != board.prev)
		{
			board.value += str_to_int(line->str, board.prev, 0);
			board.count++;
		}
		if (board.value == line->target && board.count-1 < board.best) board.best = board.count-1;
	}
	if (board.best == INT_MAX) printf("IMPOSSIBLE\n");
	else printf("%d\n", board.best);

	free(line->str.val);
	return true;
}


int main()
{
	bool again = true;
	do again = read_line();
	while (again);
	return 0;
}

