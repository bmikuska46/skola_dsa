#include "avl2.h"
#include "hash_tabulka_open_addresing.h"
#include "hash_tabulka.h"
char *gen_random_string(int velkost) // funkcia na random string o určitej veľkosti, pracoval som s veľkosťou 4
{
    static char abeceda[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    char *randomString = NULL;

    if (velkost)
    {
        randomString = malloc(sizeof(char) * (velkost + 1));
        if (randomString)
        {
            for (int n = 0; n < velkost; n++)
            {
                int random_pozicia = rand() % (int)(sizeof(abeceda) - 1);
                randomString[n] = abeceda[random_pozicia];
            }
            randomString[velkost] = '\0';
        }
    }

    return randomString;
}
int main(void)
{
    PRVOK *koren = NULL;
    PRVOK *hladaj = NULL;
    long long int velkost = 100000; // veľkosť náhodne vygenerovaných slov
    char **pole = malloc(sizeof(char *) * velkost);

    for (long long int i = 0; i < velkost; i++) // vygenerovanie náhodného stringu o velkosti 4 do statického poľa
    {
        char *slovo = gen_random_string(4);
        pole[i] = slovo;
    }
    clock_t start, aktual, start_hash, aktual_hash, start_hash_open, aktual_hash_open;
    double done, done_hash, done_hash_open;
    srand(time(0));
    start = clock();

    for (long long int i = 0; i < velkost; i++) // insert v avl strome
    {

        koren = insert(koren, pole[i]);
    }
    aktual = clock();
    done = aktual - start;
    printf("Insert_AVL: %g s\n", done / CLOCKS_PER_SEC);

    for (long long int i = 0; i < velkost; i++) // search v avl strome
    {

        hladaj = search(koren, pole[i]);
    }
    aktual = clock();
    done = aktual - done;
    printf("Search_AVL: %g s\n", done / CLOCKS_PER_SEC);

    for (long long int i = 0; i < velkost; i++) // search v avl strome
    {
        koren = delete (koren, pole[i]);
    }
    aktual = clock();
    done = aktual - done;
    printf("Remove_AVL: %g s\n", done / CLOCKS_PER_SEC);

    //////////////////////////////////////////////////////////////////////////////////////
    printf("\n----------------------------------\n\n");
    TABULKA *tabulka = vytvor_tabulku();
    srand(time(0));
    start_hash = clock();
    // je nutné upraviť aj velkosť tabuľky v "hash_tabulka.h"
    for (long long int i = 0; i < velkost; i++) // insert v hash tabulke
    {
        insert_hash(tabulka, pole[i]);
    }
    aktual_hash = clock();
    done_hash = ((aktual_hash - start_hash));
    printf("Insert_hash_tabulka: %g s\n", done_hash / CLOCKS_PER_SEC);

    for (long long int i = 0; i < velkost; i++) // search v hash tabulke
    {
        search_hash(tabulka, pole[i]);
    }
    aktual_hash = clock();
    done_hash = ((aktual_hash - start_hash));
    printf("Search_hash_tabulka: %g s\n", done_hash / CLOCKS_PER_SEC);

    for (long long int i = 0; i < velkost; i++) // search v hash tabulke
    {
        delete_hash(tabulka, pole[i]);
    }
    aktual_hash = clock();
    done_hash = ((aktual_hash - start_hash));
    printf("Delete_hash_tabulka: %g s\n", done_hash / CLOCKS_PER_SEC);

    //////////////////////////////////////////////////////////////////////////////////////

    printf("\n----------------------------------\n\n");
    TABULKA_OPEN *tabulka_open = vytvor_tabulku_open();
    // je nutné upraviť aj velkosť tabuľky v "hash_tabulka_open_addresing.h"
    srand(time(0));
    start_hash_open = clock();
    for (long long int i = 0; i < velkost; i++) // insert v hash tabulke
    {
        insert_hash_open(tabulka_open, pole[i]);
    }
    aktual_hash_open = clock();
    done_hash_open = ((aktual_hash_open - start_hash_open));
    printf("Insert_hash_tabulka_open: %g s\n", done_hash_open / CLOCKS_PER_SEC);

    for (long long int i = 0; i < velkost; i++) // search v hash tabulke
    {
        search_open(tabulka_open, pole[i]);
    }
    aktual_hash_open = clock();
    done_hash_open = ((aktual_hash_open - start_hash_open));
    printf("Search_hash_tabulka_open: %g s\n", done_hash_open / CLOCKS_PER_SEC);

    for (long long int i = 0; i < velkost; i++) // search v hash tabulke
    {
        delete_open(tabulka_open, pole[i]);
    }
    aktual_hash_open = clock();
    done_hash_open = ((aktual_hash_open - start_hash_open));
    printf("Delete_hash_tabulka_open: %g s\n", done_hash_open / CLOCKS_PER_SEC);

    return 0;
}