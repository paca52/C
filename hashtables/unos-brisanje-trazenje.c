#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TABLE_SIZE 10

typedef struct table{
    int key;
    char ime[30];
    struct table *sledeci;
}table;

int hash(char *key){
    int hash_val = 0;
    for(int i=0; key[i] != '\0'; i++)
        hash_val += key[i];
    return hash_val % TABLE_SIZE;
}

void insert(table* hashtable[], char *ime){
    int index = hash(ime);
    //printf("hash(%s) = %d\n", ime, index);

    table *novi = (table*)malloc(sizeof(table));
    novi->key = index;
    strncpy(novi->ime, ime, 30);
    novi->sledeci = hashtable[index];
    hashtable[index] = novi;
}

void print(table *hashtable[]){
    printf("Start:\n");
    for(int i=0; i<TABLE_SIZE; i++){
        if(hashtable[i] == NULL)
            printf("%d.\t---\n", i);
        else{
            printf("%d.\t", i);
            table *tmp = hashtable[i];
            while(tmp != NULL){
                printf("%s(%d)", tmp->ime, tmp->key);
                if(tmp->sledeci != NULL)
                    printf(" -> ");
                tmp = tmp->sledeci;
            }
            printf("\n");
        }
    }
    printf("End.\n");
}

void init_table(table *hashtable[]){
    for(int i=0; i<TABLE_SIZE; i++)
        hashtable[i] = NULL;
}

table *find(table* hashtable[], char *ime){
    int index = hash(ime);

    if(hashtable[index] != NULL){
        table *tmp = hashtable[index];
        while(tmp != NULL){
            if(strncmp(tmp->ime, ime, 30) == 0)
                return tmp;
            tmp = tmp->sledeci;
        }
    }
    return NULL;
}

int del(table *hashtable[], char *ime){
    int index = hash(ime);

    if(hashtable[index] != NULL){
        table *tmp = hashtable[index];
        table *prev = NULL;
        while(tmp != NULL){
            if(strncmp(tmp->ime, ime, 30) == 0){
                if(prev == NULL){
                    hashtable[index] = tmp->sledeci;
                    free(tmp);
                    return 1;
                }
                else{
                    prev->sledeci = tmp->sledeci;
                    free(tmp);
                    return 1;
                }
            }
            prev = tmp;
            tmp = tmp->sledeci;
        }
    }
    return 0;
}

int main(){

    table* hashtable[TABLE_SIZE];
    init_table(hashtable);
    print(hashtable);

    char ime[30];

    FILE *unos = fopen("unos.txt", "r");

    if(unos == NULL){
        printf("file nije otvoren.\n");
        return 0;
    }

    while(fscanf(unos, "%s", &ime) != EOF){
        //printf("bruh\nime = %s\n", ime);
        insert(hashtable, ime);
    }
    fclose(unos);

    print(hashtable);

    ime[0] = 0;

    printf("unesi ime: ");
    scanf("%s", &ime);
    table *tmp = find(hashtable, ime);
    if(tmp == NULL)
        printf("'%s' nije pronadjen/a\n", ime);
    else
        printf("'%s' je pronadjen/a\n", ime);

    printf("unesi ime koje treba obrisati: ");
    scanf("%s", &ime);
    int a = del(hashtable, ime);
    if(a == 0)
        printf("'%s' nije obrisan/a\n", ime);
    else
        printf("'%s' je obrisan/a\n", ime);

    print(hashtable);

    return 0;
}
