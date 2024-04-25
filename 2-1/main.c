#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// N pocet povolenych tahov
// N cisiel T_i --> povolene tahy
// A a B hranice poctu ceruziek
// Pre kolko cisiel X vyhra Bernard ak by hral s X ceruzkami a tahal ako prvy


typedef struct
{
	int N;
	int* moves;
	int A;
	int B;
} Line;



bool read_line();


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
	int temp = 0;

	Line line;
	line.N = 0;

	scanf("%d", &line.N);
	if (line.N < 1 || line.N > 50) return false;

	line.moves = (int*)calloc(sizeof(int), line.N);
	for (int i = 0; i < line.N; i++)
	{
		scanf("%d", &temp);

		if (temp < 1 || temp > 100)
		{
			free(line.moves);
			return false;
		}

		line.moves[i] = temp;


	}

	scanf("%d %d", &line.A, &line.B);

	bool* turn = (bool*)calloc(sizeof(bool), line.B+1);

	for (int i = 1; i <= line.B; i++)
	{
		// Prvy mozny tah
		turn[i] = !turn[i - 1];
		if (turn[i]) continue;

		// Prehladavanie ostatnych tahov
		for (int j = 1; j < line.N; j++)
		{
			// Ak je mozne spravit povoleny tah (pocet ceruziek >= tah)
			if (i >= line.moves[j])
			{
				turn[i] = !turn[i - line.moves[j]];
				if (turn[i]) break;  // Vyherny stav
			}
		}
	}

	int count = 0;
	for (int i = line.A; i <= line.B; i++) if (turn[i]) count++;
	printf("%d\n", count);

	free(line.moves);
	free(turn);
	return true;
}

