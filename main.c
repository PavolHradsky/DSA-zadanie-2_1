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
} NODE;

#define SIZE 50000
NODE table[SIZE];

unsigned int hash(unsigned int num) {
    unsigned int h = 0;
//    for (int i = 0; i < strlen(str); ++i) {
//        h = 31 * h + str[i];
//    }
//    return h % SIZE;

    do {
        h = 31 * h + (num%10);
        num /= 10;
    } while (num/10 != 0);
    h = 31 * h + (num%10);
    return h % SIZE;

}

NODE *create_node(unsigned int data) {
    NODE *result = NULL;
    result = (NODE*)malloc(sizeof(NODE));
    result->data = data;
    result->key = hash(data);
    result->next = NULL;
    return result;
}


void insert(unsigned int num) {
    NODE *toInsert = create_node(num);
    unsigned int index = toInsert->key;

    if(table[index].next == NULL) {
        table[index].next = toInsert;
    } else {
        NODE *tmp = table[index].next;
        while(tmp->next != NULL) {
            tmp = tmp->next;
        }
        tmp->next = toInsert;
    }

}

void search(unsigned int num) {
    unsigned int index = hash(num);
    NODE *tmp = table[index].next;
    while (num != tmp->data) {
        tmp = tmp->next;
    }
    //printf("Finded %d with key %d\n", tmp->data, tmp->key);
}

void print_table() {
    for (int i = 0; i < SIZE; ++i) {
        if(table[i].next != NULL) {
            NODE *tmp = table[i].next;
            printf("index %d -", i);
            do {
                printf(" %d, ", tmp->data);
                tmp = tmp->next;
            } while (tmp != NULL);
            printf("\n");
        }
    }

}

int main(void) {

    int max = 1000000;
    int toFind = 500000;
    clock_t start, end;
    double cpu_time_used;

    start = clock();    //zapne casovac

    for (int i = 0; i < max; ++i) {     //vytvori strom z max cisel (1000000)
        insert(i);
    }
    int num;
    for (int i = 0; i < toFind; ++i) {  //hlada nahodne cisla od 0 po max v strome toFind-krat (500000)
        num = (rand() % (max));
        search(num);
    }

    end = clock();      //skonci casovac
    cpu_time_used = ((double) (end - start));   //vyrata cas
    printf("finding %d times in tree size %d\n", toFind, max);      //vypise vysledok
    printf("Time: %dms\n", (int) cpu_time_used);

    return 0;
}
