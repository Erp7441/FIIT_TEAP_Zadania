#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <limits.h>

// N pocet povolenych tahov
// N cisiel T_i --> povolene tahy
// A a B hranice poctu ceruziek
// Pre kolko cisiel X vyhra Bernard ak by hral s X ceruzkami a tahal ako prvy


typedef struct
{
	char* string;
	int target;
} Line;

typedef struct
{
	int count;
	int value;
	int previous;
	int best;
} Board;


bool read_line();
int str_to_int(char* string, int left, int right);


int main()
{
	bool again = true;
	do
	{
		again = read_line();
	} while (again);
	return 0;
}

bool read_line()
{
	Board board;
	board.best = INT_MAX;

	Line line;
	line.string = (char*)calloc(13, sizeof(char));
	line.target = 0;

	if (scanf("%s %d", line.string, &line.target) != 2)
	{
		free(line.string);
		return false;
	}

	for (unsigned long long i = 0; i < pow(2, strlen(line.string)); i++)
	{
		board.previous = 0;
		board.value = 0;
		board.count = -1;

		for (int j = 1; j <= strlen(line.string); j++)
		{
			if ((i >> (strlen(line.string) - j)) & 1)
			{
				board.value += str_to_int(line.string, board.previous, (int)strlen(line.string) - j);
				board.previous = j;
				board.count++;
			}
		}

		if (board.previous != strlen(line.string))
		{
			board.value += str_to_int(line.string, board.previous, 0);
			board.count++;
		}
		if (board.value == line.target && board.count < board.best) board.best = board.count;
	}
	if (board.best == INT_MAX) printf("IMPOSSIBLE\n");
	else printf("%d\n", board.best);

	free(line.string);
	return true;
}

int str_to_int(char* str, int left, int right) {
	return (strtol(str, NULL, 10) / (long) pow(10, right)) % (long) pow(10, strlen(str) - left - right);
}
