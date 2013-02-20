#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct _Var_ {
    char *key;
    char *val;
    struct _Var_ *next;
} Var;

typedef struct _HashTable_ {
    int size;
    Var **table;
} HashTable;


HashTable *createHashTable(int size)
{
    HashTable *hashTable;
    
    // Limit min size
    if (size < 1) return NULL;

    //* Allocate memory for table struct
    if ((hashTable = malloc(sizeof(HashTable))) == NULL) {
        return NULL;
    }
    
    // Allocate memory for table array
    if ((hashTable->table = malloc(sizeof(Var *) * size)) == NULL) {
        return NULL;
    }

    // Init table elements
    int i;
    for(i=0; i < size; i++) hashTable->table[i] = NULL;

    //  Table size
    hashTable->size = size;

    return hashTable;
}

unsigned int hash(HashTable *hashTable, char *key)
{
    unsigned int hashVal = 0;

    // Hash by shifting
    for(; *key != '\0'; key++)
        hashVal = *key + (hashVal << 5) - hashVal;

    /* we then return the hash value mod the hashtable size so that it will
     * fit into the necessary range
     */
    return hashVal % hashTable->size;
}

Var *findVar(HashTable *hashtable, char *key)
{
    Var *var;
    unsigned int hashval = hash(hashtable, key);

    for(var = hashtable->table[hashval]; var != NULL; var = var->next) {
        if (strcmp(key, var->key) == 0) return var;
    }
    return NULL;
}



int setVar(HashTable *hashTable, char *key, char *val)
{
    Var *var;
    unsigned int hashVal = hash(hashTable, key);

    var = findVar(hashTable, key);
    if(var == NULL) {
        // Attempt to allocate memory for new var
        if ((var = malloc(sizeof(Var))) == NULL) return 1;
        var->key = strdup(key);
        var->val = strdup(val);
        var->next = hashTable->table[hashVal];
        hashTable->table[hashVal] = var;
    } else {
        // Replace var
        var->key = strdup(key);
        var->val = strdup(val);
    }
    return 0;
}

void freeTable(HashTable *hashTable)
{
    int i;
    Var *var, *temp;

    if (hashTable==NULL) return;

    // Free memory for every var
    for(i=0; i < hashTable->size; i++) {
        var = hashTable->table[i];
        while(var!=NULL) {
            temp = var;
            var = var->next;
            free(temp->key);
            free(temp);
        }
    }

    // Free table
    free(hashTable->table);
    free(hashTable);
}