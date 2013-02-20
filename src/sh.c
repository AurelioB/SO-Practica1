#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "hashtable.h"


int main(int argc, char **argv) {
	HashTable *table = createHashTable(100);


	setVar(table, "TEST1", "VAL1");
	setVar(table, "TEST1", "VAL");

	setVar(table, "abs", "VALfsdf");

	Var *res = findVar(table, "TEST1");
	printf("%s    %s  \n", res->key, res->val);


	res = findVar(table, "abs");
	printf("%s    %s  \n", res->key, res->val);




	freeTable(table);
	return 0;
}
