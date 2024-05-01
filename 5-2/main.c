#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#define SIZE 50

char res[SIZE];

void rst() { for (int i = 0; i < SIZE; i++) res[i] = '.'; }

long long to_num(const char* arr, int len)
{
	long long res_num = 0;
	for (int i = 0; i < len; i++)
	{
		res_num <<= 1;
		if (arr[i] == '<') res_num++;
	}
	return res_num;
}

void inc(long long* index, long long* section, long long multiplier)
{
	*index += multiplier * *section;
	*section /= 4;
}
bool l_good(long long num, int len)
{
	if (len == 0) return false;
	long long power = (long long) pow(2, len - 1);
	long long index = num < power ? 0 : power;
	long long section = power / 4;

	while (section >= 2)
	{
		if (num < index + 3 * section && num >= index + 2 * section) return false;
		else if (num < index + 4 * section && num >= index + 3 * section) inc(&index, &section, 3);
		else if (num < index + 2 * section && num >= index + section) inc(&index, &section, 1);
		else if (num < index + section && num >= index) section /= 4;
	}
	return num % 2 == 0;
}
bool r_good(long long num, int len)
{
	if (len == 0) return false;
	long long power = (long long) pow(2, len - 1);
	if (num < power) return false;

	long long index = power - 1;
	long long section = power / 4;

	while (section >= 2)
	{
		if (num <= index + 3 * section && num > index + 2 * section) return false;
		else if (num <= index + 4 * section && num > index + 3 * section) inc(&index, &section, 3);
		else if (num <= index + 2 * section && num > index + section) inc(&index, &section, 1);
		else if (num <= index + section && num > index) section /= 4;
	}
	return true;
}

void eval(const char* arr, int len)
{
	for (int i = 0; i < len; i+=2)
	{
		long long first_part = to_num(arr, i);
		long long second_part = to_num(arr + i + 1, len - i - 1);

		if (l_good(first_part, i)) first_part = -1;
		if (r_good(second_part, len - i - 1)) second_part = -1;

		if (first_part != -1 && second_part != -1) res[i] = 'o';
	}
}

void get_input(char* arr, int* len)
{
	while (scanf("%c", &arr[*len]) >= 1)
	{
		if (arr[*len] == '\n') break;
		(*len)++;
	}
}
int main()
{
	int len = 1;
	char arr[SIZE] = { 0 };

	while (scanf("%c", &arr[0]) >= 1)
	{
		get_input(arr, &len);
		rst();
		eval(arr, len);
		printf("%*.*s\n", len, len, res);
		len = 1;
		rst();
	}
	return 0;
}
