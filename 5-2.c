#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <memory.h>

// TODO:: Toto nie je moje dielo takze som to zmazal z turingu.

#define SIZE 50

typedef struct
{
	char val[SIZE];
	size_t len;
} String;

String result;

void reset() {
	memset(result.val, '.', SIZE);
}

size_t to_num(const char* arr, int len, size_t num, size_t i)
{
	return i >= len || len == 0 ? num : to_num(arr, len, arr[i] == '<' ? num*2+1 : num*2, i + 1);
}

void inc(size_t* index, size_t* section, size_t multiplier)
{
	*index += multiplier * *section;
	*section /= 4;
}
bool l_bad(size_t num, int len)
{
	if (len == 0) return true;
	size_t power = (size_t) pow(2, len - 1);
	size_t index = num < power ? 0 : power;
	size_t section = power / 4;

	while (section >= 2)
	{
		if (num < index + 3 * section && num >= index + 2 * section) return true;
		else if (num < index + 4 * section && num >= index + 3 * section) inc(&index, &section, 3);
		else if (num < index + 2 * section && num >= index + section) inc(&index, &section, 1);
		else if (num < index + section && num >= index) section /= 4;
	}
	return num % 2;
}
bool r_bad(size_t num, int len)
{
	if (len == 0) return true;
	size_t power = (size_t) pow(2, len - 1);
	if (num < power) return true;

	size_t index = power - 1;
	size_t section = power / 4;

	while (section >= 2)
	{
		if (num <= index + 3 * section && num > index + 2 * section) return true;
		else if (num <= index + 4 * section && num > index + 3 * section) inc(&index, &section, 3);
		else if (num <= index + 2 * section && num > index + section) inc(&index, &section, 1);
		else if (num <= index + section && num > index) section /= 4;
	}
	return false;
}

void eval(const char* arr, int len)
{
	// Vzdy miznu dvojicky
	for (int i = 0; i < len; i+=2)
	{
		size_t first_part = to_num(arr, i, 0, 0);
		size_t second_part = to_num(arr + i + 1, len - i - 1, 0, 0);

		if (!l_bad(first_part, i)) first_part = -1;
		if (!r_bad(second_part, len - i - 1)) second_part = -1;

		if (first_part != -1 && second_part != -1) result.val[i] = 'o';
	}
}


int main()
{
	String input = { { 0 }, 1 };
	while (scanf("%s", input.val) >= 1)
	{
		input.len = strlen(input.val);
		eval(input.val, input.len);
		// Vypise iba prvych "len" znakov "val".
		// Zdroj: https://stackoverflow.com/questions/2239519/is-there-a-way-to-specify-how-many-characters-of-a-string-to-print-out-using-pri
		printf("%*.*s\n", input.len, input.len, result.val);
		input.len = 1;
		reset();
	}
	return 0;
}
