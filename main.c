/***********************************
 * DSA Zadianie 2 - Hash Table
 *
 * FIIT STU 2021
 * Pavol Hradsky
************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct node {
    unsigned int key;
    unsigned int data;
    struct node *next;
    //struct node *before;
} NODE;

typedef struct array {
    struct node *next;
} ARRAY;

int resize(int *SIZE);

ARRAY *table;

unsigned int hash(unsigned int num, int SIZE) {
    unsigned int h = 0;
    do {
        h = 31 * h + (num%10);
        num /= 10;
    } while (num/10 != 0);
    h = 31 * h + (num%10);
    return h % SIZE;

}

NODE *create_node(unsigned int data, int SIZE) {
    NODE *result = NULL;
    result = (NODE*)malloc(sizeof(NODE));
    result->data = data;
    result->key = hash(data, SIZE);
    result->next = NULL;
    //result->before = NULL;
    return result;
}


void insert(unsigned int num, int *SIZE) {
    NODE *toInsert = create_node(num, *SIZE);
    unsigned int index = toInsert->key;
    NODE *tmp = table[index].next;
    int i = 0;
    while (tmp != NULL) {
        i++;
        tmp = tmp->next;
    }
    if(i > 5) {
        *SIZE = resize(SIZE);
    }

    if(table[index].next == NULL) {
        table[index].next = toInsert;
    } else {
        tmp = table[index].next;
        while(tmp->next != NULL) {
            tmp = tmp->next;
        }
        tmp->next = toInsert;
        //tmp->next->before = tmp;
    }

}

NODE * search(unsigned int num, int SIZE) {
    unsigned int index = hash(num, SIZE);
    NODE *tmp = table[index].next;
    while (num != tmp->data) {
        if(tmp->next == NULL) {
            //printf("Prvok nebol najdeny\n");
            return NULL;
        }
        tmp = tmp->next;
    }
    //printf("Najdeny prvok %d s indexom %d\n", tmp->data, tmp->key);
    return tmp;
}

void rehash(int *SIZE, int bigger) {
    NODE *tmp, *tmpb;
    for (int i = 0; i < *SIZE / bigger; ++i) {
        tmp = table[i].next;
        while (tmp != NULL) {
            if(tmp == table[i].next) {
                tmp->key = hash(tmp->data, *SIZE);
                if(tmp->key != i) {
                    table[i].next = tmp->next;
                    insert(tmp->data, SIZE);
                    free(tmp);
                    tmp = table[i].next;
                } else {
                    tmpb = tmp;
                    tmp = tmp->next;
                }
            } else {
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



//void rehash(int *SIZE, int bigger) {
//    NODE *tmp;
//    for (int i = 0; i < *SIZE / bigger; ++i) {
//        tmp = table[i].next;
//        while (tmp->next != NULL) {
//            tmp = tmp->next;
//        }
//        do {
//            if(tmp->before != NULL) {
//                tmp = tmp->before;
//                tmp->next->key = hash(tmp->next->data, *SIZE);
//            } else{
//                tmp->key = hash(tmp->data, *SIZE);
//                insert(tmp->data, SIZE);
//                if(tmp->key != i) {
//                    if(tmp->next != NULL) {
//                        tmp = tmp->next;
//                        NODE *tmp1 = tmp->before;
//                        table[i].next = tmp;
//                        tmp->before = NULL;
//                        free(tmp1);
//                    } else{
//                        free(table[i].next);
//                    }
//                }
//                continue;
//            }
//            if(tmp->next->key != tmp->key) {
//                insert(tmp->next->data, SIZE);
//                if (tmp->next->next != NULL) {
//                    NODE *tmp1 = tmp->next;
//                    tmp->next->next->before = tmp;
//                    tmp->next = tmp->next->next;
//                    free(tmp1);
//                } else {
//                    free(tmp->next);
//                    tmp->next = NULL;
//                }
//            }
//        } while(tmp->before != NULL);
//    }
//}

int resize(int *SIZE) {
    int bigger = 10;
    *SIZE *= bigger;
    ARRAY *tmp;
    tmp = realloc(table, *SIZE * sizeof(ARRAY));
    if(tmp != NULL) {
        table = tmp;
    } else{
        printf("nepodarilo sa realokovat\n");
        return 0;
    }
    for (int i = *SIZE / bigger; i < *SIZE; ++i) {
        table[i].next = NULL;
    }
    rehash(SIZE, bigger);
    return *SIZE;
}

void print_table(int SIZE) {
    for (int i = 0; i < SIZE; ++i) {
        if(table[i].next != NULL) {
            NODE *tmp = table[i].next;
            printf("index %.2d -", i);
            do {
                printf(" %.2d, ", tmp->data);
                tmp = tmp->next;
            } while (tmp != NULL);
            printf("\n");
        }
    }
}

int main(void) {

    int size = 10;
    table = (ARRAY*)calloc(size, sizeof(ARRAY));

//    for (int i = 0; i < 400; ++i) { //341
//        insert(i, &size);
//    }
//    print_table(size);

    int max = 1000000;
    int toFind = 500000;
    clock_t start, end;
    double cpu_time_used;

    start = clock();    //zapne casovac

    for (int i = 0; i < max; ++i) {     //vytvori strom z max cisel (1000000)
        insert(i, &size);
    }
    int num;
    for (int i = 0; i < toFind; ++i) {  //hlada nahodne cisla od 0 po max v strome toFind-krat (500000)
        num = (rand() % (max));
        search(num, size);
    }

    end = clock();      //skonci casovac
    cpu_time_used = ((double) (end - start));   //vyrata cas
    printf("finding %d times in tree size %d\n", toFind, max);      //vypise vysledok
    printf("Time: %dms\n", (int) cpu_time_used);
    printf("Size of the table is %d\n", size);


    return 0;
}
