// Branislav Mikuška
// FIIT - 2 semester 21/22
// DSA - zadanie 1 - hashovacia tabulka linked list

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

long long int table_size = 500000;
long long int prvkov = 0;
long long int novychPrvkov = 0;

typedef struct PRVOK_HASH //štruktúra pre prvok
{
    char *slovo;
    struct PRVOK_HASH *next;
} PRVOK_HASH;

typedef struct TABULKA //štruktúra pre tabuľku do ktorej sa budú pridávať prvky
{
    PRVOK_HASH **prvky;
} TABULKA;

TABULKA *vytvor_tabulku()
{
    TABULKA *tabulka = malloc(sizeof(TABULKA));
    tabulka->prvky = malloc(sizeof(PRVOK_HASH *) * table_size);

    for (long long int i = 0; i < table_size; i++)
    {
        tabulka->prvky[i] = NULL;
    }
    return tabulka;
}

int hash(char *slovo) // hashovacia funkcia na základe dĺžky slova, ascii hodnoty a vynásobením prvočíslom
{
    long long int value = 0;
    long long int dlzka_slova = strlen(slovo);

    for (long long int i = 0; i < dlzka_slova; i++)
    {
        value = value * 13 + slovo[i];
    }

    value = value % table_size;

    return value;
}

TABULKA *insert_hash2(TABULKA *tabulka, char *slovo) // táto funkcia slúži ako druhá insertovacia funckia pri zväčšovaní tabuľky
{
    long long int index = hash(slovo);
    PRVOK_HASH *temp = tabulka->prvky[index];
    PRVOK_HASH *predosli;
    PRVOK_HASH *novyPrvok = malloc(sizeof(PRVOK_HASH));
    novyPrvok->slovo = malloc(strlen(slovo));
    novyPrvok->slovo = slovo;
    novyPrvok->next = NULL;
    if (tabulka->prvky[index] == NULL)
    {
        tabulka->prvky[index] = novyPrvok;
    }

    else
    {
        while (temp != NULL)
        {
            if (temp == NULL)
            {
                temp = novyPrvok;
                break;
            }
            else
            {
                predosli = temp;
                temp = predosli->next;
            }
        }
        predosli->next = novyPrvok;
    }
    return tabulka;
}

void insert_hash(TABULKA *tabulka, char *slovo) // hlavná insert funkcia
{
    long long int index = hash(slovo); // hashvoacnie slova

    PRVOK_HASH *predosli;
    PRVOK_HASH *novyPrvok = malloc(sizeof(PRVOK_HASH)); // vytvorenei noveho prvku
    novyPrvok->slovo = malloc(strlen(slovo));
    novyPrvok->slovo = slovo;
    novyPrvok->next = NULL;
    if (tabulka->prvky[index] == NULL)
    {
        prvkov++;
        tabulka->prvky[index] = novyPrvok;
        if (prvkov >= table_size / 2) // zväčšovanie zabuľky
        {
            TABULKA *staraTabulka = malloc(sizeof(TABULKA));
            staraTabulka->prvky = malloc(sizeof(PRVOK_HASH *) * table_size);
            for (long long int i = 0; i < table_size; i++)
            {
                staraTabulka->prvky[i] = NULL;
            }
            memcpy(staraTabulka, tabulka, table_size);
            long long int staraVelkost = table_size;
            table_size *= 2;
            tabulka->prvky = malloc(sizeof(PRVOK_HASH *) * table_size);
            for (long long int i = 0; i < table_size; i++)
            {
                tabulka->prvky[i] = NULL;
            }

            for (long long int i = 0; i < staraVelkost; i++)
            {
                PRVOK_HASH *prvok = staraTabulka->prvky[i];

                if (prvok == NULL)
                {
                    continue;
                }

                while (1)
                {
                    if (prvok == NULL)
                    {
                        break;
                    }
                    else
                    {
                        insert_hash2(tabulka, prvok->slovo);
                        novychPrvkov++;
                    }

                    prvok = prvok->next;
                }
            }
        }
    }

    else
    {
        PRVOK_HASH *temp = tabulka->prvky[index];
        while (tabulka->prvky[index] != NULL) // pri vzniknutej kolízii sa prvky vložia na rovnaké miseto s pomocou linked listu
        {
            if (temp == NULL)
            {
                temp = novyPrvok;
                break;
            }
            else
            {
                predosli = temp;
                temp = predosli->next;
            }
        }
        predosli->next = novyPrvok;
    }
}

int search_hash(TABULKA *tabulka, char *slovo)
{
    long long int index = hash(slovo);

    PRVOK_HASH *prvok = tabulka->prvky[index];

    if (prvok == NULL)
    {
        return 0;
    }

    while (1)
    {
        if (prvok->slovo == slovo)
        {
            return 1;
        }

        prvok = prvok->next;
    }
}

int delete_hash(TABULKA *tabulka, char *slovo)
{
    long long int index = hash(slovo);

    PRVOK_HASH *prvok = tabulka->prvky[index];

    if (prvok == NULL)
    {
        return 0;
    }

    while (1)
    {
        if (strcmp(prvok->slovo, slovo) == 0)
        {
            if (prvok->next == NULL)
            {
                tabulka->prvky[index] = NULL;
                return 1;
            }
            else
            {
                tabulka->prvky[index] = prvok->next;
                return 1;
            }
        }
        else
        {
            prvok = prvok->next;
        }
    }
    return 1;
}