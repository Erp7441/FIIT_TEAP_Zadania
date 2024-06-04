#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#define BUFFER 50

/*

vyhodnocovanie vyrazov v prefixovom tvare

/-+*13x

/xa

13 je konstanta

x je premenna

a je parameter

Kontrola uzatvorkovania (vzdy viac otvorenych ako zatvorenych a na konci 0)

-/7650 - - 100

prve minusko musi byt binarne

druhe musi byt binarne

tretie moze byt unarne alebo binarne

+*\/-77

Prilis malo operatorov, viem vyhodnit 0 sposobmi 🙂

Vyraz ktory zacina cifrov musi pozostavat len z cifier

123+/223 —> Neda sa

+1010-/555* —> Neda sa lebo krat na konci nemam s cim operovat

-123+5*\/-5 —> Neda sa privela operatorov

-56 —> 2

+56 —> 1 (5 + 6) nemoze byt +56 (nie je unarne)

+-123 —> (3 moznosti)

- -1 + 23
- -12 + 3
- 1 - 2 + 3

- - + 1 1 —> 0 sposob

- - 1 - 1 —> 1 sposob

- - - 1 - 1 - - 1 - 1 —> 7x ‘-’ 4x ‘x’

Dalej by slo 15 ‘-’ 8 ‘x’.

Jedna z podmienok technicky Pokial je pocet operatorov 2* pocet cifier. Koncovka sa nebude dat. Treba dokazat…

!!!
Dynamicke programovanie —> Tabulka podretazcov s rozbitim vyrazu na mensie.
!!!

+*-/7650 - -100 + 123 - 456

*/

typedef struct
{
	size_t** v;
	size_t l;
} Matrix;

char arr[BUFFER];

Matrix* alloc_matrix(size_t l)
{
	// Alokacia matice
	Matrix* matrix = (Matrix*) calloc(1, sizeof(Matrix));
	matrix->l = l;
	matrix->v = (size_t**)malloc(sizeof(size_t*) * l);
	assert(matrix->v != NULL);

	// Alokacia riadkov + inicializacia
	for (int i = 0; i < l; i++)
	{
		matrix->v[i] = (size_t*)calloc(sizeof(size_t), l);
		assert(matrix->v[i] != NULL);
		matrix->v[i][i] = (bool)isdigit(arr[i]); // Trackujeme pocet cifiere a znakov na diagonale pomocou booleanu
	}
	return matrix;
}

void dealloc_matrix(Matrix* m)
{
	if (!m) return;
	if (m->v != NULL)
	{
		for (int i = 0; i < m->l; i++)
			if (m->v[i]) free(m->v[i]);
		free(m->v);
		m->v = NULL;
	}
	m->l = 0;
	free(m);
	m = NULL;
}

void print_matrix(Matrix matrix)
{
	for (int i = 0; i < matrix.l; i++)
	{
		for (int j = 0; j < matrix.l; j++)
			printf("%zu ", matrix.v[i][j]);
		printf("\n\n");
	}
}

void identify_digit(Matrix matrix)
{
	for (int i = 0; i < matrix.l-1; i++)
		// Ak je na diagonale cislo alebo minusko a hned vedla je cislo tak to bereieme ako cislo
		// Minusko moze byt unarne ak je pred nim dalsie minusko
		matrix.v[i][i + 1] = (isdigit(arr[i]) || arr[i] == '-') && isdigit(arr[i+1]);
}

int identify_arr_digits(int i, int j, const int offset)
{
	for (int k = j + offset; k <= j + i - 1; k++)
		if(!isdigit(arr[k])) return false;
	return true;
}

void combs_on_diagonal(Matrix matrix, int i, int j)
{
	if (isdigit(arr[j]))
	{
		matrix.v[j][j + i - 1] = identify_arr_digits(i, j, 0);
		return;
	}

	size_t count = 0;
	// Hladame kombinacie na diagonale (resp. hornej polovice ohranicenej diagonalou)
	for (int k = 0; k < i-2; k++)
		count += matrix.v[j + 1][j + 1 + k] * matrix.v[j + 2 + k][j + i - 1];

	if (arr[j] == '-')
		count += identify_arr_digits(i, j, 1); // Offset 1 lebo mozeme mat aj zaporne cislo. (Unarne minus)
	matrix.v[j][j + i - 1] = count;
}

size_t notation(size_t len)
{
	Matrix* matrix = alloc_matrix(len);
	assert(matrix != NULL);

	identify_digit(*matrix); // 0 - 2
	for (int i = 3; i <= len; i++)  // 3 - n
		for (int j = 0; j < len - i + 1; ++j)
			combs_on_diagonal(*matrix, i, j);
	size_t res = matrix->v[0][len - 1];
	dealloc_matrix(matrix);
	return res;
}

int main()
{
	while (scanf("%s", arr) > 0)
	{
		printf("%zu\n", notation(strlen(arr)));
		memset(arr, 0, BUFFER);
	}
	return 0;
}