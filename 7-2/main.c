#include <stdio.h>
#include <math.h>

long long pal[10][1000];

void rst(long long mod)
{
	for (size_t i = 0; i < 10; i++)
		for (size_t j = 0; j <= mod; j++) pal[i][j] = 0;
}

long long calc_reminder(long long i, long long val)
{
	return i * (long long)pow(10, val);
}

void rec(long long d, long long len, long long mod)
{
	if (d > (len - 1) / 2) return;

	long long start = 0;
	if (d == (len - 1) / 2) start = 1;

	if (d == 0)
	{
		if (len & 1) for (int i = start; i < 10; i++) pal[d][calc_reminder(i, len/2) % mod] += 1;
		else for (int i = start; i < 10; i++)
			pal[d][(calc_reminder(i, len / 2) + calc_reminder(i, len / 2 - 1)) % mod] += 1;

		rec(d + 1, len, mod);
		return;
	}

	for (long long i = start; i < 10; i++)
	{
		int rem = (int)((calc_reminder(i, len / 2 + d) + calc_reminder(i, (len - 1) / 2 - d)) % mod);
		for (size_t j = 0; j < mod; j++)
			pal[d][(rem + j) % mod] += pal[d - 1][j];
	}
	rec(d + 1, len, mod);
}


int main()
{
	long long n = 0, mod = 0, len = 0;
	scanf("%lld", &n);
	for (size_t i = 0; i < n; i++)
	{
		scanf("%lld %lld", &mod, &len);
		rst(mod);
		rec(0, len, mod);
		printf("%lld\n", pal[(len - 1) / 2][0]);
	}
	return 0;
}
