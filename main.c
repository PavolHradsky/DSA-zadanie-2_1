/***********************************
 * DSA Zadianie 2 - Hash Table
 *
 * FIIT STU 2021
 * Pavol Hradsky
************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct node {   //vytvorenie struktury na ukladanie prvkov
    unsigned long key;   //kluc z funkcie hash
    unsigned long data;  //data (cislo)
    struct node *next;  //ukazovatel na dalsi prvok v spajanom zozname
} NODE;

typedef struct array {  //struktura tabulky, ukazovatel na prvok v danej bunke
    struct node *next;
} ARRAY;

unsigned long resize(unsigned long *SIZE);

ARRAY *table;   //globalna premenna table

unsigned long hash(unsigned long num, unsigned long SIZE) {     //funkcia hash, dostane cislo a vrati hash kod
    unsigned long h = 0;
    do {    //prechadza cifry cisla od zadu  a nasobi ich cislom 31, tieto hodnoty zratava
        h = 31 * h + (num%10);
        num /= 10;
    } while (num/10 != 0);
    h = 31 * h + (num%10);
    return h % SIZE;    //nakoniec vrati hodnotu h modulo velkost tabulky
}

NODE *create_node(unsigned long data, unsigned long SIZE) {    //funkcia vytvori uzol spajaneho zoznamu
    NODE *result = NULL;
    result = (NODE*)malloc(sizeof(NODE));
    result->data = data;
    result->key = hash(data, SIZE);
    result->next = NULL;
    return result;  //vrati NODE
}


void insert(unsigned long num, unsigned long *SIZE) {      //funkcia najde miesto kam vlozit prvok a vlozi ho tam
    NODE *toInsert = create_node(num, *SIZE);   //vytvori NODE
    unsigned long index = toInsert->key;         //najde index (hash kod prvku)
    NODE *tmp = table[index].next;          //vytvori tmp premennu
    int i = 0;      //v tomto loope funkcia pozera ci sa dany prvok nahodov nema dat na viac ako 6 miesto
    while (tmp != NULL) {   //na jednom indexe, ak ano, zvacsi tabulku
        i++;
        tmp = tmp->next;
    }
    if(i > 5) {
        *SIZE = resize(SIZE);
    }

    if(table[index].next == NULL) {     //ak je miesto na indexe volne, vlozi prvok sem
        table[index].next = toInsert;
    } else {    //inak vlozi prvok na koniec spajaneho zoznamu na tomto indexe
        tmp = table[index].next;
        while(tmp->next != NULL) {
            tmp = tmp->next;
        }
        tmp->next = toInsert;
    }

}

NODE *search(unsigned long num, unsigned long SIZE) { //funkcia hlada prvok v hash tabulke
    unsigned long index = hash(num, SIZE);   //najde index na ktorom by sa mal prvok nachadzat
    NODE *tmp = table[index].next;      //vytvori tmp premennu
    while (num != tmp->data) {      //v spajanom zozname na danom indexe hlada prvok, ak ho nenajde, vrati NULL
        if(tmp->next == NULL) {
            return NULL;
        }
        tmp = tmp->next;
    }
    return tmp;     //ak ho najde, vrati NODE prvku
}

void rehash(unsigned long *SIZE, int bigger) {    //funkcia rehash je volana z funkcie resize, aktualizuje tabulku,
                                        // prehashuje vsetky prvky
    NODE *tmp, *tmpb;   //premenne tmp a tmpb (tmpBefore)
    for (unsigned long i = 0; i < *SIZE / bigger; ++i) {  //postupne prechadza vsetky indexy tabulky
        tmp = table[i].next;    //tmp sa nastavi na prvy prvok na indexe
        while (tmp != NULL) {   //ak tam nieco najde, pokracuje
            if(tmp == table[i].next) {  //ak je to prvy prvok v poradi
                tmp->key = hash(tmp->data, *SIZE);  //aktualizuje key prvku
                if(tmp->key != i) {     //ak sa novy key nerovna indexu, musi sa presunut na novy index a vymazat
                    table[i].next = tmp->next;  //nastavi sa ukazovatel predosleho prvku na po-dalsi prvok
                    insert(tmp->data, SIZE);    //tmp sa vlozi na nove miesto
                    free(tmp);  //uvolni sa zo stareho miesta
                    tmp = table[i].next;    //a tmp je zase prvy prvok na danom indexe
                } else {    //ak sa novy kay == indexu, tmp sa posunie na dalsi prvok
                    tmpb = tmp;
                    tmp = tmp->next;
                }
            } else {    //ak to nie je prvy prvok na idexe, len sa zmeni table[i].next na tmpb, a zvysok je identicky
                tmp->key = hash(tmp->data, *SIZE);
                if(tmp->key != i) {
                    tmpb->next = tmp->next;
                    insert(tmp->data, SIZE);
                    free(tmp);
                    tmp = tmpb->next;
                } else {
                    tmpb = tmp;
                    tmp = tmp->next;
                }
            }
        }
    }
}


unsigned long resize(unsigned long *SIZE) { //funkcia resize zvacsi tabulku a zavola funkciu rehash
    int bigger = 10;    //bigger reprezentuje hodnotu, kolko krat sa ma tabulka zvacsit
    *SIZE *= bigger;    //zvacsi sa premenna SIZE
    ARRAY *tmp;
    tmp = realloc(table, *SIZE * sizeof(ARRAY));    //realokuje sa tabulka do premennej tmp
    if(tmp != NULL) {   //ak sa to podarilo, tmp sa skopiruje do table, inak napise chybu a vrati 0
        table = tmp;
    } else{
        printf("nepodarilo sa realokovat\n");
        return 0;
    }
    for (unsigned long i = *SIZE / bigger; i < *SIZE; ++i) {  //na novych miestach v tabulke sa nastavia ukazovatele next na NULL
        table[i].next = NULL;
    }
    rehash(SIZE, bigger);   //zavola sa funkcia rehash
    return *SIZE;   //vrati sa nova velkost
}

void print_table(unsigned long SIZE) {    //funkcia vypisuje tabulku
    for (unsigned long i = 0; i < SIZE; ++i) {    //prejde vsetky indexy a ak na danom indexe nieco je,
                                        //vypise ho aj so spajanym zoznamom v nom
        if(table[i].next != NULL) {
            NODE *tmp = table[i].next;
            printf("index %.2lu -", i);
            do {
                printf(" %.2lu, ", tmp->data);
                tmp = tmp->next;
            } while (tmp != NULL);
            printf("\n");
        }
    }
}

int main(void) {

    unsigned long size = 100;  //pociatocna velkost tabulky
    table = (ARRAY*)calloc(size, sizeof(ARRAY));    //alokuje sa tabulka

    long max = 1000000;
    long toFind = 500000;
    clock_t start, end;
    double cpu_time_used;

    start = clock();    //zapne casovac

    for (long i = 0; i < max; ++i) {     //vytvori tabulku z max cisel (1000000)
        insert(i, &size);
    }
    long num;
    for (long i = 0; i < toFind; ++i) {  //hlada nahodne cisla od 0 po max v tabulke toFind-krat (500000)
        num = (rand() % (max));
        search(num, size);
    }

    end = clock();      //skonci casovac
    cpu_time_used = ((double) (end - start));   //vyrata cas
    printf("Finding %d times in tree size %d\n", toFind, max);      //vypise vysledok
    printf("Time: %dms\n", (int) cpu_time_used);
    printf("Size of the table is %d\n", size);


    return 0;
}
