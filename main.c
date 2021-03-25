/***********************************
 * DSA Zadianie 2 - Hash Table
 *
 * FIIT STU 2021
 * Pavol Hradsky
************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct node {
    unsigned int key;
    char* data;
    struct node *next;
} NODE;

#define SIZE 2000
NODE table[SIZE];

unsigned int hash(char *str) {
    unsigned int h = 0;
    for (int i = 0; i < strlen(str); ++i) {
        h = 31 * h + str[i];
    }
    return h % SIZE;
}

NODE *create_node(char* data) {
    NODE *result = NULL;
    result = (NODE*)malloc(sizeof(NODE));
    result->data = data;
    result->key = hash(data);
    result->next = NULL;
    return result;
}


void insert(char *str) {
    NODE *toInsert = create_node(str);
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

void search(char *str) {
    unsigned int index = hash(str);
    NODE *tmp = table[index].next;
    while (strcmp(str, tmp->data) != 0) {
        tmp = tmp->next;
    }
    printf("Finded %s with key %d\n", tmp->data, tmp->key);
}

void print_table() {
    for (int i = 0; i < SIZE; ++i) {
        if(table[i].next != NULL) {
            NODE *tmp = table[i].next;
            printf("index %d -", i);
            do {
                printf(" %s -", tmp->data);
                tmp = tmp->next;
            } while (tmp != NULL);
            printf("\n");
        }
    }

}

int main(void) {

    insert("palo");
    insert("ondrej");
    insert("slovo");
    insert("em");
    insert("hahahahah");
    insert("vetb");
    insert("ah");

    print_table();

    search("palo");
    return 0;
}
