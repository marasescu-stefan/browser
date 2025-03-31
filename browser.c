//Marasescu Stefan-Gabriel, 311CA
#include <stdio.h>

#include <stdlib.h>

#include <string.h>

struct page {
	int id;
	char url[50];
	char *description;
}

struct stack {
	unsigned int size;
	//de completat
}

struct tab {
	int id;
	struct page *currentPage;
	struct stack *backwardStack;
	struct stack *forwardStack;
}

struct tabList {
	unsigned int size;
	//de completat
}

struct browser {
	struct tab *current;
	struct tabList list;
}

int main(void)
{


	return 0;
}