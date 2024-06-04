#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

// Zdroj: https://drive.google.com/file/d/1cchUXiK2rlAOdnS-4tPDBV4ut6MuEYN9/view

#define SIZE 50
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define IS_VOWEL(c) ((c) == 'A' || (c) == 'E' || (c) == 'I' || (c) == 'O' || (c) == 'U' || (c) == 'Y')
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define CONSONANT_COUNT 5
#define VOWEL_COUNT 3
#define LETTERS_LEN 2

typedef enum
{
	VOWEL = '0',
	VOWEL_I = 0,
	CONSONANT = '1',
	CONSONANT_I = 1,
	UNKNOWN = '2',
	UNKNOWN_I = 2
} Letter;

typedef struct
{
	char* val;
	size_t len;
} String;

typedef struct
{
	String str;
	String mask;
	size_t len;
	size_t unknown_count, unknown_sequence_size;
	bool is_vowel_heavy;
	bool is_consonant_heavy;
	bool one_at_least
} MaskedString;

MaskedString alloc_masked_string()
{
	MaskedString s;
	s.str.val = (char*) calloc(SIZE, sizeof(char));
	s.mask.val = (char*) calloc(SIZE, sizeof(char));
	s.len = 0;
	s.unknown_count = 0;
	s.unknown_sequence_size = 0;
	s.is_vowel_heavy = false;
	s.is_consonant_heavy = false;
	s.one_at_least = false;
	return s;
}

void free_masked_string(MaskedString* s)
{
	free(s->str.val);
	free(s->mask.val);
	s->str.val = NULL;
	s->mask.val = NULL;
	s->len = 0;
	s->unknown_count = 0;
	s->unknown_sequence_size = 0;
	s->is_vowel_heavy = false;
	s->is_consonant_heavy = false;
	s->one_at_least = false;
}

String create_str(const char* str)
{
	String s;
	s.val = strdup(str);
	assert(s.val != NULL);
	s.len = strlen(str);
	return s;
}

MaskedString copy_instance(MaskedString src)
{
	MaskedString dest = alloc_masked_string();
	dest.str = create_str(src.str.val);
	dest.mask = create_str(src.mask.val);
	dest.len = src.len;
	dest.unknown_count = src.unknown_count;
	dest.unknown_sequence_size = src.unknown_sequence_size;
	dest.is_vowel_heavy = src.is_vowel_heavy;
	dest.is_consonant_heavy = src.is_consonant_heavy;
	dest.one_at_least = src.one_at_least;
	return dest;
}

void copy_ptr(MaskedString* dest, MaskedString* src)
{
	dest->str = create_str(src->str.val);
	dest->mask = create_str(src->mask.val);
	dest->len = src->len;
	dest->unknown_count = src->unknown_count;
	dest->unknown_sequence_size = src->unknown_sequence_size;
	dest->is_vowel_heavy = src->is_vowel_heavy;
	dest->is_consonant_heavy = src->is_consonant_heavy;
	dest->one_at_least = src->one_at_least;
}

MaskedString copy_ptr_instance(MaskedString* src)
{
	MaskedString dest = alloc_masked_string();
	copy_ptr(&dest, src);
	free_masked_string(src);
	free(src);
	return dest;
}

String mask_string(const char* str)
{
	String s = create_str(str);
	for (int i = 0; i < s.len; i++)
		if (IS_VOWEL(s.val[i]))
			s.val[i] = VOWEL;
		else if (!IS_VOWEL(s.val[i]))
			s.val[i] = CONSONANT;
		else if (s.val[i] == '?')
			s.val[i] = UNKNOWN;
	return s;
}

MaskedString create_masked(const char* str)
{
	MaskedString s = alloc_masked_string();
	s.str = create_str(str);
	s.mask = mask_string(str);
	s.len = strlen(str);
	s.unknown_count = 0;
	s.unknown_sequence_size = 0;
	s.is_vowel_heavy = false;
	s.is_consonant_heavy = false;
	s.one_at_least = false;
	return s;
}

// Obdoba load_masked_string ale evaluujeme priamo masku, nenacitavame zo vstupu a nemenime retazec ani masku
void evaluate_likeness_lin(MaskedString* masked)
{
	size_t counters[LETTERS_LEN] = { 0 };  // Pocitadla pre po sebe iduce hlasky
	for (int i = 0; i < masked->len; i++)
	{
		// Pokial som nasiel otaznik, resetnem countre lebo mozem za neho dat hocico
		if (masked->mask.val[i] == UNKNOWN)
		{
			// Resetnem countre pokial uz nenasli pozadovany pocet hlasok
			counters[VOWEL_I] = counters[VOWEL_I] != VOWEL_COUNT ? 0 : counters[VOWEL_I];
			counters[CONSONANT_I] = counters[CONSONANT_I] != CONSONANT_COUNT ? 0 : counters[CONSONANT_I];
		}
		// Pokial som na samohlaske tak inkrementujem vowel counter a resetnem consonant counter
		else if (masked->mask.val[i] == VOWEL)
		{
			counters[VOWEL_I]++;
			counters[CONSONANT_I] = counters[CONSONANT_I] < CONSONANT_COUNT ? 0 : counters[CONSONANT_I];
		}
		// Obdobne operacie pre spoluhlasku ako pri samohlaske
		else if (masked->mask.val[i] == CONSONANT)
		{
			counters[CONSONANT_I]++;
			counters[VOWEL_I] = counters[VOWEL_I] < VOWEL_COUNT ? 0 : counters[VOWEL_I];
		}

	}
	masked->is_consonant_heavy = counters[CONSONANT_I] >= CONSONANT_COUNT;
	masked->is_vowel_heavy = counters[VOWEL_I] >= VOWEL_COUNT;
}

char* trim(const char* input, char ch)
{
	char* str = (char*) calloc(strlen(input), sizeof(char));
	strncpy(str, input, strlen(input)-1);

	int start = 0;
	int end = strlen(str) - 1;

	while (str[start] == ch)
		start++;
	while (end >= start && str[end] == ch)
		end--;

	int i = 0;
	while (start <= end)
		str[i++] = str[start++];
	str[i] = '\0';

	return str;
}

// Nacita vstup ako MaskedString
MaskedString load_masked_string()
{
	size_t counters[LETTERS_LEN] = { 0 };  // Pocitadla pre po sebe iduce hlasky
	size_t unknowns_after[LETTERS_LEN] = { 0 };
	size_t unknowns_before[LETTERS_LEN] = { 0 };
	size_t unknown_sequence_count = 0;
	Letter previous_letter = UNKNOWN, previous_symbol = UNKNOWN;
	MaskedString input = alloc_masked_string();

	int i = 0;
	for (;scanf("%c", &input.str.val[i]) > 0; i++)
	{
		if (input.str.val[i] == '\n') break;
		// Pokial som nasiel otaznik. Nastavim masku na unknown a resetnem countre lebo mozem za neho dat hocico
		if (input.str.val[i] == '?')
		{
			input.mask.val[i] = UNKNOWN;
			input.unknown_count++;
			// Resetnem countre pokial uz nenasli pozadovany pocet hlasok
			if (previous_letter == CONSONANT)
			{
				unknowns_after[CONSONANT_I]++;
				unknowns_after[VOWEL_I] = 0;
				counters[VOWEL_I] = counters[VOWEL_I] != VOWEL_COUNT ? 0 : counters[VOWEL_I];
			}
			else if (previous_letter == VOWEL)
			{
				unknowns_after[VOWEL_I]++;
				unknowns_after[CONSONANT_I] = 0;
				counters[CONSONANT_I] = counters[CONSONANT_I] != CONSONANT_COUNT ? 0 : counters[CONSONANT_I];
			}
			unknown_sequence_count++;
			input.unknown_sequence_size = MAX(unknown_sequence_count, input.unknown_sequence_size);
		}
		// Pokial som na samohlaske tak nastavim masku na VOWEL inkrementujem vowel counter a resetnem consonant counter
		else if (IS_VOWEL(input.str.val[i]))
		{
			input.mask.val[i] = VOWEL;
			previous_letter = VOWEL;
			counters[VOWEL_I]++;

			// Kedze sme stupli na samohlasku tak resetneme countre pre spoluhlasky
			counters[CONSONANT_I] = counters[CONSONANT_I] < CONSONANT_COUNT ? 0 : counters[CONSONANT_I];
			unknowns_after[CONSONANT_I] = 0;

			// Pokial previous symbol bol unknown
			if (previous_symbol == UNKNOWN)
			{
				// Aktualizujeme hodnotu unknown symbolovou lebo ich sekvencia sa prave skoncila
				unknowns_before[VOWEL_I] = MAX(unknowns_before[VOWEL_I], unknowns_after[VOWEL_I]);
				// Pokial consonant nesplna poctovu podmienku tak resetneme counter pre consonant
				unknowns_before[CONSONANT_I] = unknowns_before[CONSONANT_I] + counters[CONSONANT_I] < CONSONANT_COUNT ? 0 :
						unknowns_before[CONSONANT_I];
				unknown_sequence_count = 0;
			}
		}
		// Obdobne operacie pre spoluhlasku ako pri samohlaske
		else if (!IS_VOWEL(input.str.val[i]))
		{
			input.mask.val[i] = CONSONANT;
			previous_letter = CONSONANT;
			counters[CONSONANT_I]++;

			// Kedze sme stupli na spoluhlasku tak resetneme countre pre samohlasky
			counters[VOWEL_I] = counters[VOWEL_I] < VOWEL_COUNT ? 0 : counters[VOWEL_I];
			unknowns_after[VOWEL_I] = 0;

			// Pokial previous symbol bol unknown
			if (previous_symbol == UNKNOWN)
			{
				// Aktualizujeme hodnotu unknown symbolovou lebo ich sekvencia sa prave skoncila
				unknowns_before[CONSONANT_I] = MAX(unknown_sequence_count, unknowns_before[CONSONANT_I]);
				// Pokial vowel nesplna poctovu podmienku tak resetneme counter pre vowel
				unknowns_before[VOWEL_I] = unknowns_before[VOWEL_I] + counters[VOWEL_I] < VOWEL_COUNT ? 0 : unknowns_before[VOWEL_I];
				unknown_sequence_count = 0;
			}
		}
		if (
			!input.one_at_least &&
			(
				unknowns_after[VOWEL_I] + counters[VOWEL_I] >= VOWEL_COUNT ||
			    unknowns_before[VOWEL_I] + counters[VOWEL_I] >= VOWEL_COUNT ||
			    unknowns_after[CONSONANT_I] + counters[CONSONANT_I] >= CONSONANT_COUNT ||
				unknowns_before[CONSONANT_I] + counters[CONSONANT_I] >= CONSONANT_COUNT
			)
		) input.one_at_least = true;
		previous_symbol = input.mask.val[i];
	}
	input.len = i;
	input.mask.len = input.len;
	input.str.len = input.len;
	input.is_consonant_heavy = counters[CONSONANT_I] >= CONSONANT_COUNT;
	input.is_vowel_heavy = counters[VOWEL_I] >= VOWEL_COUNT;
	return input;
}

MaskedString fill_out_mask(MaskedString masked, Letter filling)
{
	MaskedString filled = copy_instance(masked);
	for (int i = 0; i < filled.len; i++)
		if (masked.mask.val[i] == UNKNOWN)
			filled.mask.val[i] = filling;
	return filled;
}

void set_counters(MaskedString* masked, Letter new_val, size_t val_pos, size_t* increase, size_t* reset)
{
	masked->mask.val[val_pos] = new_val;
	(*increase)++;
	*reset = 0;
}

// Actual kulehy na rozbijanie retazca
void handle_unknown(MaskedString* bad, Letter previous, Letter next, size_t i, size_t* counter)
{
	if (i == 0)
	{
		if (i + 1 < bad->mask.len && bad->mask.val[i + 1] == VOWEL)
			bad->mask.val[i] = CONSONANT;
		if (i + 1 < bad->mask.len &&  bad->mask.val[i + 1] == CONSONANT)
			bad->mask.val[i] = VOWEL;
	}
	// Pokial skoro mame samohlasky tak na tomto otazniku ich rozbijeme naschval
	else if (counter[VOWEL_I] >= VOWEL_COUNT-1)
		set_counters(bad, CONSONANT, i, &counter[CONSONANT_I], &counter[VOWEL_I]);
	else if (counter[CONSONANT_I] >= CONSONANT_COUNT-1)
		set_counters(bad, VOWEL, i, &counter[VOWEL_I], &counter[CONSONANT_I]);
	// Pokial sa nam countre nepriblizuju k hodnotam tak pozerame dopredu a dozadu.
	else if (i+1 < bad->len)
		// Ak na predchadzajucej pozicii bola samohlaska a dalsia bude tiez samohlaska alebo neznama. Tak aktualnu
		// unknown nastavim na spoluhlasku aby som rozbil postupnost potenctionalnych 2 SAMOHLASOK.
		if (previous == VOWEL && (next == UNKNOWN || next == VOWEL))
			set_counters(bad, CONSONANT, i, &counter[CONSONANT_I], &counter[VOWEL_I]);
		// Ak na predchadzajucej pozicii bola samohlaska a dalsia bude spoluhlaska tak nastavim na aktualnu unknown hodnotu
		// samohlasku aby som rozbil postupnost potenctionalnych 2 SPOLUHLASOK
		else if (previous == VOWEL && next == CONSONANT)
			set_counters(bad, VOWEL, i, &counter[VOWEL_I], &counter[CONSONANT_I]);
		// Ak na predchadzajucej pozicii bola spoluhlaska a dalsia bude samohlaska alebo neznama. Tak nastavim na aktualnu
		// unknown hodnotu na spoluhlasku
		else if (previous == CONSONANT && (next == UNKNOWN || next == CONSONANT))
			set_counters(bad, VOWEL, i, &counter[VOWEL_I], &counter[CONSONANT_I]);
		// Ak na predchadzajucej pozicii bola spoluhlaska a dalsia bude tiez spolhlaska. Tak nastavim na aktualnu unknown
		// hodnotu samohlasku aby som rozbil postupnost potenctionalnych 2 SPOLUHLASOK
		else if (previous == CONSONANT && next == VOWEL)
			set_counters(bad, CONSONANT, i, &counter[CONSONANT_I], &counter[VOWEL_I]);
}

// Kulehy na rozbijanie retazca
MaskedString* find_bad_string(MaskedString input)
{
	MaskedString* bad = (MaskedString*) malloc(sizeof(MaskedString));
	copy_ptr(bad, &input);
	size_t counter[LETTERS_LEN] = { 0 };
	Letter previous = UNKNOWN, next = bad->mask.val[0];
	bad->mask.val[0] = bad->mask.val[0] == UNKNOWN && bad->mask.len >= 1 ? bad->mask.val[1] : bad->mask.val[0];

	for(int i = 0; i < bad->len; i++)
	{
		if (bad->mask.val[i] == UNKNOWN)
			handle_unknown(bad, previous, next, i, counter);
		else if(bad->mask.val[i] == VOWEL)
		{
			counter[VOWEL_I]++;
			counter[CONSONANT_I] = 0;
		}
		else if(bad->mask.val[i] == CONSONANT)
		{
			counter[CONSONANT_I]++;
			counter[VOWEL_I] = 0;
		}
		previous = bad->mask.val[i]; // Moja current pozicka je I. ALe v dalsej iteracii bude I+1
		next = i + 2 < bad->len ? bad->mask.val[i+2] : UNKNOWN; // Kedze v dalsej iteraci bude I+1 tak next dalsej...
		// ...iteracie je I + 2
	}
	return bad;
}

bool is_valid(MaskedString input)
{
	return input.is_consonant_heavy || input.is_vowel_heavy;
}

bool resolve_unknown(MaskedString input)
{
	if (input.str.val[0] == '\0' || (strncmp(input.str.val, "\n", 1) == 0)) return false;

	// Spravanie zobrazene v tabulke na 1:31 google drive videa

	// Pokial sa nam sklada zo samych otaznikov tak nevieme
	if(input.str.len >= VOWEL_COUNT && input.unknown_count == input.str.len)
	{
		printf("neviem\n");
		free_masked_string(&input);
		return true;
	}
	// Napocitali sme pri zadavani vstupu potrebny pocet hlasok
	else if (!input.unknown_count && input.is_consonant_heavy || input.is_vowel_heavy)
	{
		printf("paci\n");
		free_masked_string(&input);
		return true;
	}
	// Neobsahuje patterny hlasok a ani nemame ziadne otazniky ktorymi by sme vedeli ten pattern vytvorit
	else if(!input.unknown_count && !input.is_consonant_heavy && !input.is_vowel_heavy)
	{
		printf("nepaci\n");
		free_masked_string(&input);
		return true;
	}

	// 7:15 Google Drive video
	MaskedString vowel_string = fill_out_mask(input, VOWEL);
	MaskedString consonant_string = fill_out_mask(input, CONSONANT);
	evaluate_likeness_lin(&vowel_string);
	evaluate_likeness_lin(&consonant_string);
	MaskedString bad = copy_ptr_instance(find_bad_string(input));
	evaluate_likeness_lin(&bad);
	MaskedString stripped = create_masked(trim(input.str.val, '?'));
	evaluate_likeness_lin(&stripped);

	//EE?FFF neviem
	//AA???CCCC neviem
	//EE?FFFF paci
	// H??LOWOR?? nepaci

	// ?FFFF neviem

	// AA?QQQ?A?QQQ?A?QQQ?A??QQQ?A?QQQ?A?QQQ?A?QQQ?A?QQQQ neviem

	// ?????????????????????AA?FVH?UE???????????????????? PACI

	// Spravanie zobrazene v tabulke na 1:31 google drive videa
	// Pokial sa mu paci najhorsi pripad tak sa mu bude string pacit v tom alebo lepsiom pripade
	if (is_valid(bad) && (is_valid(vowel_string) && is_valid(consonant_string)))
		printf("paci\n");
	// Pokial sa mu najhorsi pripad nepaci ale existuje nejaky iny ktory sa mu paci
	else if (input.one_at_least)
		printf("neviem\n");
	// Inak sa mu nepaci
	else printf("nepaci\n");

	free_masked_string(&input);
	free_masked_string(&vowel_string);
	free_masked_string(&consonant_string);
	return true;
}

int main()
{
	while (resolve_unknown(load_masked_string()));
}
