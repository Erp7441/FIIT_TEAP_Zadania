#include <stdio.h>

// Kym num nebude 0 budeme otacat cislo, potom ho returneme.
int reverse(int num, int rev)
{
	return num == 0 ? rev : reverse(num / 10, rev * 10 + num % 10);
}

void variations(int n, int k, int curr, int original)
{
	if (curr == n) for (int r = reverse(original, 0); r > 0; r /= 10)
		printf("%d %s", r % 10, r / 10 == 0 ? "\n" : "");
	else for (int i = 0; i < k; i++)
		variations(n, k, curr + 1, original * 10 + i + 1);
}

int main()
{
	int n, k;  // N prvkove variace z mnoziny 1 az K
	scanf("%d %d", &n, &k);
	variations(n, k, 0, 0);
	return 0;
}