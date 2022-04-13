// Branislav Mikuška
// FIIT - 2 semester 21/22
// DSA - zadanie 1 - hashovacia tabulka - open addresing

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

long long int table_size_open = 1000000;
long long int prvkov_open = 0;
long long int novychPrvkov_open = 0;

typedef struct PRVOK_HASH_OPEN //štruktúra pre prvok
{
    char *slovo;
} PRVOK_HASH_OPEN;

typedef struct TABULKA_OPEN //štruktúra pre tabuľku do ktorej sa budú pridávať prvky
{
    PRVOK_HASH_OPEN **prvky;
} TABULKA_OPEN;

TABULKA_OPEN *vytvor_tabulku_open()
{
    TABULKA_OPEN *tabulka = malloc(sizeof(TABULKA_OPEN));
    tabulka->prvky = calloc(table_size_open, sizeof(PRVOK_HASH_OPEN *));
    return tabulka;
}

long long int check_if_available(TABULKA_OPEN *tabulka, long long int index, char *slovo) // rekurzívna funkcia zistenie že či miesto v hash tabulke je obsadené, ak hej index zväčší o 1
{
    PRVOK_HASH_OPEN *prvok = tabulka->prvky[index];
    if (prvok == NULL)
    {
        check_if_available(tabulka, index + 1, slovo);
    }
    else if (strcmp(prvok->slovo, slovo) == 0) // táto podmienka slúži pre search v hash tabulke
    {
        return index;
    }
    else if (index > table_size_open) // ak to prekročí max. dĺžku tabuľky začne hľadať index od znova || zväčší veľkosť tabuľky
    {
        check_if_available(tabulka, 0, slovo);
    }
    else
    {
        check_if_available(tabulka, index + 1, slovo);
    }
}

long long int check_if_empty(TABULKA_OPEN *tabulka, long long int index, char *slovo) // rekurzívna funkcia zistenie že či miesto v hash tabulke je obsadené, ak hej index zväčší o 1
{
    PRVOK_HASH_OPEN *prvok = tabulka->prvky[index];

    if (prvok == NULL) // táto podmienka slúži pre search v hash tabulke
    {
        return index;
    }
    else if (index > table_size_open) // ak to prekročí max. dĺžku tabuľky začne hľadať index od znova || zväčší veľkosť tabuľky
    {
        check_if_empty(tabulka, 0, slovo);
    }
    else
    {
        check_if_empty(tabulka, index + 1, slovo);
    }
}

int hash_open(TABULKA_OPEN *tabulka, char *slovo) // hashovacia funkcia na základe dĺžky slova, ascii hodnoty a vynásobením prvočíslom
{
    long long int value = 0;
    long long int dlzka_slova = strlen(slovo);

    for (long long int i = 0; i < dlzka_slova; i++)
    {
        value = value * 13 + slovo[i];
    }

    value = value % table_size_open;

    value = check_if_empty(tabulka, value, slovo);

    return value;
}
int hash_search_open(TABULKA_OPEN *tabulka, char *slovo) // hashovacia funkcia presearch prvku
{
    long long int value = 0;
    long long int dlzka_slova = strlen(slovo);

    for (long long int i = 0; i < dlzka_slova; i++)
    {
        value = value * 13 + slovo[i];
    }

    value = value % table_size_open;

    value = check_if_available(tabulka, value, slovo);

    return value;
}

int hash_deleted_open(TABULKA_OPEN *tabulka, char *slovo) // hashovacia funkcia pre delete prvku
{
    long long int value = 0;
    long long int dlzka_slova = strlen(slovo);

    for (long long int i = 0; i < dlzka_slova; i++)
    {
        value = value * 13 + slovo[i];
    }

    value = value % table_size_open;

    value = check_if_available(tabulka, value, slovo);

    return value;
}
void print_table(TABULKA_OPEN *tabulka)
{
    for (int i = 0; i < table_size_open; i++)
    {
        if (tabulka->prvky[i] != NULL)
        {
            printf("%d | %s\n", i, tabulka->prvky[i]->slovo);
        }
        else
        {
            printf("NULL\n");
        }
    }
}

TABULKA_OPEN *insert_hash2_open(TABULKA_OPEN *tabulka, char *slovo) // táto funkcia slúži ako druhá insertovacia funckia pri zväčšovaní tabuľky
{
    long long int index = hash_open(tabulka, slovo);              // hashvoacnie slova
    PRVOK_HASH_OPEN *novyPrvok = malloc(sizeof(PRVOK_HASH_OPEN)); // vytvorenei noveho prvku
    novyPrvok->slovo = malloc(strlen(slovo));
    novyPrvok->slovo = slovo;
    prvkov_open++;
    tabulka->prvky[index] = novyPrvok;

    return tabulka;
}

TABULKA_OPEN tabulka_resize(TABULKA_OPEN *tabulka, long long int velkost)
{
    printf("resizing table\n");
    table_size_open = velkost * 2;
    TABULKA_OPEN *temp_tabulka = vytvor_tabulku_open();
    for (int i = 0; i < velkost; i++)
    {
        PRVOK_HASH_OPEN *prvok = tabulka->prvky[i];
        if (prvok != NULL && prvok->slovo != NULL)
        {
            insert_hash2_open(temp_tabulka, prvok->slovo);
        }
    }

    PRVOK_HASH_OPEN **temp_prvky = tabulka->prvky;
    tabulka->prvky = temp_tabulka->prvky;
    temp_tabulka->prvky = temp_prvky;

    return *tabulka;
}

TABULKA_OPEN insert_hash_open(TABULKA_OPEN *tabulka, char *slovo) // hlavná insert funkcia
{
    long long int index = hash_open(tabulka, slovo); // hashvoacnie slova

    PRVOK_HASH_OPEN *novyPrvok = malloc(sizeof(PRVOK_HASH_OPEN)); // vytvorenei noveho prvku
    novyPrvok->slovo = malloc(strlen(slovo));
    novyPrvok->slovo = slovo;
    prvkov_open++;
    tabulka->prvky[index] = novyPrvok;

    if (prvkov_open > table_size_open / 2) // ak je viac než polovica prvkov obsadená v tabuľke, zväčší sa tabuľka
    {
        prvkov_open = 0;
        *tabulka = tabulka_resize(tabulka, table_size_open);
    }

    return *tabulka;
}

int search_open(TABULKA_OPEN *tabulka, char *slovo) // hlavná insert funkcia
{
    long long int index = hash_search_open(tabulka, slovo); // hashvoacnie slova

    PRVOK_HASH_OPEN *prvok = tabulka->prvky[index];

    if (prvok == NULL)
    {
        return 0;
    }

    if (prvok->slovo == slovo)
    {
        return 1;
    }
}

int delete_open(TABULKA_OPEN *tabulka, char *slovo)
{
    long long int index = hash_deleted_open(tabulka, slovo); // hashvoacnie slova

    PRVOK_HASH_OPEN *prvok = tabulka->prvky[index];

    if (prvok == NULL)
    {
        return 0;
    }

    if (strcmp(prvok->slovo, slovo) == 0)
    {
        tabulka->prvky[index] = NULL;

        return 1;
    }
}
