#include <stdio.h>
#include <math.h>

#define MAX 1024
#define MIN 10
#define RIGHT(n, k) ((int)(k / pow(2, n)) % 2)
#define NTH(n, k) (k + ((int)(k / pow(2, n)) % 2 == 0) * (int)pow(2, n))

typedef struct
{
	int x, y, z;
} Vector3D;

typedef struct
{
	int boys, girls, pairs;
	char input[MIN][MIN];  // Vstupne data
	int combs[MIN][MAX][MIN];  // Vsetky kombinacie chalpcov a dievcat
} Solution;

Solution sol = {0, 0, 0};

void reset_3d_matrix()
{
	for (int i = 0; i < MIN; i++)
		for (int j = 0; j < MAX; j++)
			for (int k = 0; k < MIN; k++)
				sol.combs[i][j][k] = -1;
}

int solve(Vector3D pos)
{
	if (pos.z == sol.pairs || pos.x == sol.boys) return pos.z == sol.pairs;
	if (sol.combs[pos.x][pos.y][pos.z] != -1) return sol.combs[pos.x][pos.y][pos.z];

	// Vyriesnie prvej pozicie v matici
	int total = solve((Vector3D){pos.x + 1, pos.y, pos.z});
	for (int i = 0; i < sol.girls; i++)
	{
		// Posun smerom v pravo
		if (sol.input[pos.x][i] == 'Y' && RIGHT(i, pos.y) == 0)
			// Vyriesenie N-tej pozicie v matici
			total += solve((Vector3D){pos.x + 1, NTH(i, pos.y), pos.z + 1});
	}
	sol.combs[pos.x][pos.y][pos.z] = total;
	return total;
}

void init()
{
	char c;
	scanf("%d %d", &sol.boys, &sol.girls);
	for (int i = 0; i < sol.boys; i++)
	{
		scanf("%c", &c);
		for (int j = 0; j < sol.girls; j++) scanf("%c", &(sol.input[i][j]));
	}
}

int main()
{
	while (scanf("%d", &sol.pairs) > 0)
	{
		init();
		reset_3d_matrix();
		printf("%d\n", solve((Vector3D){0, 0, 0}));
	}
	return 0;
}
