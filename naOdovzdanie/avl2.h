// Branislav Mikuška
// FIIT - 2 semester 21/22
// DSA - zadanie 1 - AVL strom
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct PRVOK
{

    char *nazov; // jednoznacny identifikator
    int vyska;
    struct PRVOK *vlavo;
    struct PRVOK *vpravo;

} PRVOK;

PRVOK *novyPrvok(char *nazov) // vytvorenie nového prvku
{
    PRVOK *prvok = (PRVOK *)malloc(sizeof(PRVOK));
    prvok->nazov = nazov;
    prvok->vyska = 1;
    prvok->vlavo = NULL;
    prvok->vpravo = NULL;

    return prvok;
}

int vyska(PRVOK *prvok)
{
    if (prvok == NULL)
    {
        return 0;
    }

    return prvok->vyska;
}

int max(int a, int b)
{
    if (a > b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

int min(int a, int b)
{
    if (a < b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

PRVOK *rotaciaVpravo(PRVOK *prvok)
{
    PRVOK *prvokVlavo = prvok->vlavo;
    PRVOK *pomocnyPrvok = prvokVlavo->vpravo;
    prvokVlavo->vpravo = prvok;
    prvok->vlavo = pomocnyPrvok;
    prvok->vyska = max(vyska(prvok->vlavo), vyska(prvok->vpravo)) + 1; // zrotovali sme prvky, musíme prepočítať ich výšku
    prvokVlavo->vyska = max(vyska(prvokVlavo->vlavo), vyska(prvokVlavo->vpravo)) + 1;

    return prvokVlavo;
}

PRVOK *rotaciaVlavo(PRVOK *prvok)
{
    PRVOK *prvokVpravo = prvok->vpravo;       // uchováme si prvok ktorý je od pôvodného vpravo
    PRVOK *pomocnyPrvok = prvokVpravo->vlavo; //ľavý prvok pôvodného prvku vpravo
    prvokVpravo->vlavo = prvok;
    prvok->vpravo = pomocnyPrvok;
    prvok->vyska = max(vyska(prvok->vlavo), vyska(prvok->vpravo)) + 1; // zrotovali sme prvky, musíme prepočítať ich výšku
    prvokVpravo->vyska = max(vyska(prvokVpravo->vlavo), vyska(prvokVpravo->vpravo)) + 1;

    return prvokVpravo;
}

PRVOK *insert(PRVOK *prvok, char *nazov) // vkladanie prvku
{

    if (prvok == NULL) // vkladanie prvku
    {
        return (novyPrvok(nazov));
    }

    if ((strcmp(nazov, prvok->nazov)) < 0) // vkladanie prvku vľavo (ak je hodnota vloženého prvku menšia ako hodnota pôvodného prvku)
    {
        prvok->vlavo = insert(prvok->vlavo, nazov);
    }

    else if ((strcmp(nazov, prvok->nazov)) > 0) // vkladanie prvku vpravo (ak je hodnota vloženého prvku väčšia ako hodnota pôvodného prvku)
    {
        prvok->vpravo = insert(prvok->vpravo, nazov);
    }

    else
    {
        return prvok;
    }

    prvok->vyska = 1 + max(vyska(prvok->vlavo), vyska(prvok->vpravo));
    int VahaStromu = vyska(prvok->vlavo) - vyska(prvok->vpravo);

    // rotácie na zákalde na akú stranu je strom nevyvážený
    // rotácia vpravo ak je strom nevyvážený vľavo
    if (VahaStromu > 1 && strcmp(nazov, prvok->vlavo->nazov) < 0)
    {
        return rotaciaVpravo(prvok);
    }

    // rotácia vľavo ak je strom nevyvážený vpravo
    else if (VahaStromu < -1 && strcmp(nazov, prvok->vpravo->nazov) > 0)
    {
        return rotaciaVlavo(prvok);
    }

    // rotácia najprv vˇavo a potom vpravo, strom je nevyvýžený vľavo
    else if (VahaStromu > 1 && strcmp(nazov, prvok->vlavo->nazov) > 0)
    {
        prvok->vlavo = rotaciaVlavo(prvok->vlavo);

        return rotaciaVpravo(prvok);
    }
    // rotácia najprv vpravo a potom vľavo, strom je nevyvýžený vpravo
    else if (VahaStromu < -1 && strcmp(nazov, prvok->vpravo->nazov) < 0)
    {
        prvok->vpravo = rotaciaVpravo(prvok->vpravo);

        return rotaciaVlavo(prvok);
    }

    else
    {
        return prvok;
    }
}

PRVOK *najmensiPrvok(PRVOK *prvok) // používa sa pri vymazávaní keď to čo chceme vymazať má prvky aj vľavo aj vpravo
{
    PRVOK *momentalnyPrvok = prvok;

    while (momentalnyPrvok->vlavo != NULL)
    {
        momentalnyPrvok = momentalnyPrvok->vlavo;
    }
    return momentalnyPrvok;
}

PRVOK *delete (PRVOK *prvok, char *nazov)
{
    if (prvok == NULL)
    {
        return NULL;
    }

    if (strcmp(nazov, prvok->nazov) < 0) // prechádzame ľavú časť stromu
    {
        prvok->vlavo = delete (prvok->vlavo, nazov);
    }
    else if (strcmp(nazov, prvok->nazov) > 0) // prechádzame pravú časť stromu
    {
        prvok->vpravo = delete (prvok->vpravo, nazov);
    }

    else // našli sme konkrétny prvok
    {
        if (prvok->vpravo == NULL && prvok->vlavo == NULL)
        {
            prvok = NULL;
        }
        else if (prvok->vlavo == NULL && prvok->vpravo != NULL) // ak má iba najviac jeden ďalší prvok pod sebou
        {
            prvok = prvok->vpravo;
        }

        else if (prvok->vpravo == NULL && prvok->vlavo != NULL)
        {
            prvok = prvok->vlavo;
        }
        else
        {
            PRVOK *pomocnyPrvok = najmensiPrvok(prvok->vpravo);
            prvok->nazov = pomocnyPrvok->nazov;
            prvok->vpravo = delete (prvok->vpravo, pomocnyPrvok->nazov);
        }
    }

    // toto je balancovanie stromu po vymazaní prvku, rotácie rovnaké ako pri vkladaní

    if (prvok == NULL) // ak strom mal iba jeden prvok
    {
        return prvok;
    }
    prvok->vyska = 1 + max(vyska(prvok->vlavo), vyska(prvok->vpravo));
    int VahaStromu = vyska(prvok->vlavo) - vyska(prvok->vpravo);

    if (VahaStromu > 1 && (vyska(prvok->vlavo->vlavo) - vyska(prvok->vlavo->vpravo)) >= 0)
    {
        return rotaciaVpravo(prvok);
    }

    else if (VahaStromu < -1 && (vyska(prvok->vpravo->vlavo) - vyska(prvok->vpravo->vpravo)) <= 0)
    {
        return rotaciaVlavo(prvok);
    }

    else if (VahaStromu > 1 && (vyska(prvok->vlavo->vlavo) - vyska(prvok->vlavo->vpravo)) < 0)
    {
        prvok->vlavo = rotaciaVlavo(prvok->vlavo);

        return rotaciaVpravo(prvok);
    }

    else if (VahaStromu < -1 && (vyska(prvok->vpravo->vlavo) - vyska(prvok->vpravo->vpravo)) > 0)
    {
        prvok->vpravo = rotaciaVpravo(prvok->vpravo);

        return rotaciaVlavo(prvok);
    }

    return prvok;
}
PRVOK *search(PRVOK *prvok, char *hladanaHodnota)
{
    PRVOK *najdenyPrvok = NULL; // rekurzívne hľadáme či sa prvok rovná tomu hľadanému, začiname v koreni
    if (strcmp(hladanaHodnota, prvok->nazov) < 0)
    {
        if (prvok->vlavo != NULL)
        {
            najdenyPrvok = search(prvok->vlavo, hladanaHodnota);
        }
        else
        {
            return NULL;
        }
    }

    else if (strcmp(hladanaHodnota, prvok->nazov) > 0)
    {
        if (prvok->vpravo != NULL)
        {
            najdenyPrvok = search(prvok->vpravo, hladanaHodnota);
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        najdenyPrvok = prvok;
    }

    return najdenyPrvok;
}
