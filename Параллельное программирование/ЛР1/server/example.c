#include <stdio.h>

#include "list.h"

static void printIntList(List* L) {
	ListNode* iter = L->head;

	do {
		printf("%d ", *(int*)iter->data);
		iter = iter->next;
	} while (iter);

	printf("\n");
}

int main(int argc, char *argv[]) {
	List* int_L = NULL;

	listInit(&int_L, sizeof(int), NULL);

	int value = 3;
	listPushBack(int_L, (void*)&value);
	// list: 3

	value = 4;
	listPushBack(int_L, (void*)&value);	
	// list: 3, 4

	value = 5;
	listPushBack(int_L, (void*)&value);	
	// list: 3, 4, 5

	printIntList(int_L);


	ListNode* iter = int_L->head;
	// iter->data == 3
	iter = iter->next;
	// iter->data == 4

	listDeleteNode(int_L, iter);
	// list: 3, 5


	value = 2;
	listPushFront(int_L, (void*)&value);
	// list: 2, 3, 5

	printIntList(int_L);

	listFree(&int_L);

	return 0;
}