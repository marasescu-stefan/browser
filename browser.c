//Marasescu Stefan-Gabriel, 311CA
#include <stdio.h>

#include <stdlib.h>

#include <string.h>

typedef struct {
	int id;
	char url[50];
	char *description;
} page;

typedef struct stack_node stack_node;
struct stack_node{
	int page_id;
	stack_node *next;
};

typedef struct {
	stack_node *head;
	unsigned int size;
} stack; //implemented as a singly linked list

typedef struct {
	int id;
	page *currentPage;
	stack *backwardStack;
	stack *forwardStack;
} tab;

typedef struct tab_node tab_node;
struct tab_node{
	tab *data;
	tab_node *prev, *next;
};

typedef struct {
	tab_node *head; //sau *santinela???
	unsigned int size;
	//de adaugat santinela?
} tabList; //implemented as a circular doubly linked list

typedef struct {
	tab *current;
	tabList list;
} browser;

int main(void)
{



	return 0;
}