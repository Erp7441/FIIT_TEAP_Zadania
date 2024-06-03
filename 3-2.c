#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define SIZE 51
#define POWER_SIZE 22
#define OFFSET 3

void run(size_t start, int parts);

typedef struct {
	char num[SIZE];
	size_t len;
} Binary;

typedef struct {
	char* val;
	size_t len;
} String;

typedef struct {
	Binary b;
	int best;
} Solution;

const char* POWER_5[] = {
	"1",
	"101",
	"11001",
	"1111101",
	"1001110001",
	"110000110101",
	"11110100001001",
	"10011000100101101",
	"1011111010111100001",
	"111011100110101100101",
	"100101010000001011111001",
	"10111010010000111011011101",
	"1110100011010100101001010001",
	"1001000110000100111001110010101",
	"101101011110011000100000111101001",
	"11100011010111111010100100110001101",
	"10001110000110111100100110111111000001",
	"1011000110100010101111000010111011000101",
	"110111100000101101101011001110100111011001",
	"100010101100011100100011000001001000100111101",
	"10101101011110001110101111000101101011000110001",
	"1101100011010111001001101011011100010111011110101",
};

Solution sol = {{{0 }, 0}, 0 };

char* itoa(int num)
{
	char* res = (char*) calloc(SIZE,sizeof(char));
	sprintf(res, "%d", num);
	return res;
}

String create(size_t len)
{
	return (String){ (char*) calloc(len + OFFSET, sizeof(char)), len };
}

String copy(const char* src)
{
	String destination = create(strlen(src));
	strcpy(destination.val, src);
	return destination;
}

void equal_parts(const char* curr, size_t len, size_t start, int parts, size_t rem)
{
	for(int i = 0; i < len; i++)
		if(sol.b.num[rem - i - 1] != curr[len - i - 1]) return;
	run(start + len, parts + 1);
}

void run(size_t start, int parts)
{
	size_t rem = sol.b.len - start;
	if (start == sol.b.len)
	{
		if(sol.best > parts) sol.best = parts;
		return;
	}

	for(int curr_i = 0; curr_i < POWER_SIZE; curr_i++)
	{
		String curr = copy(POWER_5[curr_i]);
		if(rem < curr.len) break;
		if(rem - curr.len > 0 && sol.b.num[rem - curr.len - 1] == '0') continue;
		else equal_parts(curr.val, curr.len, start, parts, rem);
	}
}

void evaluate()
{
	sol.b.len = strlen(sol.b.num);
	sol.best = INT_MAX;
	run(0, 0);
	printf("%s\n", sol.best == INT_MAX ? "-1" : itoa(sol.best));
}

int main()
{
	while (scanf("%s", sol.b.num) > 0) evaluate();
	return 0;
}