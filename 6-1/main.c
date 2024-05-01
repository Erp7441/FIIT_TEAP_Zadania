#include <stdio.h>

int reverse(int n)
{
	int r = 0;
	while (n > 0)
	{
		r = r * 10 + n % 10;
		n /= 10;
	}
	return r;
}

void print_var(int m, int curr, int k, int o)
{

	if (curr == m)
	{
		int r = reverse(o);
		while (r > 0)
		{
			printf("%d ", r % 10);
			r /= 10;
		}
		printf("\n");
	}
	else for (int i = 0; i < k; i++) print_var(m, curr + 1, k, o * 10 + i + 1);
}


int main()
{
	int n, k;
	scanf("%d %d", &n, &k);
	print_var(n, 0, k, 0);
	return 0;
}
