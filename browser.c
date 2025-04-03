//Marasescu Stefan-Gabriel, 311CA
#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#define INPUT_FILENAME "tema1.in"
#define OUTPUT_FILENAME "tema1.out"

#define DEFAULT_ID 0
#define DEFAULT_URL "https://acs.pub.ro/"
#define DEFAULT_DESCRIPTION "Computer Science"

typedef struct {
	int id;
	char url[50];
	char *description;
} page;

typedef struct stack_node stack_node;
struct stack_node{
	page *value;
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
struct tab_node {
	tab *data;
	tab_node *prev, *next;
};

typedef struct {
	tab_node *santinela;
	unsigned int size;
} tabList; //implemented as a circular doubly linked list

typedef struct {
	tab *current;
	tabList list;
} browser;

void dll_add_last(tabList *list, tab *current)
{
	//de facut (santinela e deja alocata, dar nu initializata)
	//in data pun current-ul (nu cred ca e nevoie cu memcpy)
	tab_node *new_tab_node = malloc(sizeof(tab_node));
	if (!new_tab_node) {
		fprintf(stderr, "Malloc failed");
		exit(1);
	}

	new_tab_node->data = malloc(sizeof(tab));
	if (!new_tab_node->data) {
		fprintf(stderr, "Malloc failed");
		exit(1);
	}
	memcpy(new_tab_node->data, current, sizeof(tab));

	new_tab_node->prev = list->santinela->prev;
	new_tab_node->next = list->santinela;

	list->santinela->prev->next = new_tab_node;
	list->santinela->prev = new_tab_node;

	list->size++;
}

stack *create_stack(void)
{
	stack *s = malloc(sizeof(stack));
	if (!s) {
        fprintf(stderr, "Malloc failed");
        exit(1);
    }
	s->head = NULL;
	s->size = 0;

	return s;
}

browser *create_browser(page *pages)
{
	browser *b = malloc(sizeof(browser));
	if (!b) {
		fprintf(stderr, "Malloc failed");
		exit(1);
	}

	b->current = malloc(sizeof(tab));
	if (!b->current) {
		fprintf(stderr, "Malloc failed");
		exit(1);
	}

	b->current->id = 0;

	b->current->currentPage = pages; //indica la adresa lui pages[0]
	b->current->backwardStack = create_stack();
	b->current->forwardStack = create_stack();

	b->list.santinela = malloc(sizeof(tab_node));
	if (!b->list.santinela) {
		fprintf(stderr, "Malloc failed");
		exit(1);
	}
	b->list.santinela->data = NULL;
	b->list.santinela->prev = b->list.santinela;
	b->list.santinela->next = b->list.santinela;

	b->list.size = 0;

	dll_add_last(&b->list, b->current); //adding current tab in list

	return b;
}

void new_tab(browser *b, page *pages)
{
	tab *new_tab = malloc(sizeof(tab));
	if (!new_tab) {
		fprintf(stderr, "Malloc failed");
		exit(1);
	}
	b->current = new_tab;

	b->current->id = b->list.size;
	b->current->currentPage = pages;
	b->current->backwardStack = create_stack();
	b->current->forwardStack = create_stack();

	dll_add_last(&b->list, b->current); //adding current tab in list
}

void read_pages(page *pages, unsigned int page_count, FILE *input_file)
{
	//initializing the default page, which is pages[0]
	pages[0].id = DEFAULT_ID;
	strcpy(pages[0].url, DEFAULT_URL);

	char description[] = DEFAULT_DESCRIPTION;
	pages[0].description = malloc((strlen(description) + 1) * sizeof(char));
	if (!pages[0].description) {
		fprintf(stderr, "Malloc failed");
		exit(1);
	}
	strcpy(pages[0].description, description);

	for (unsigned int i = 1; i <= page_count; i++) {
		fscanf(input_file, "%d", &pages[i].id);
		fgetc(input_file); //reading the \n after the integer

		fgets(pages[i].url, sizeof(pages[i].url), input_file);
		pages[i].url[strlen(pages[i].url) - 1] = '\0';

		char buffer[512];
		fgets(buffer, sizeof(buffer), input_file);

		unsigned int buffer_size = strlen(buffer);
		buffer[buffer_size - 1] = '\0'; //remove the \n at the end
		//allocating buffer_size bytes, which includes the \0 at the end
		pages[i].description = malloc(buffer_size * sizeof(char));//sa nu uit sa eliberez!
		if (!pages[i].description) {
			fprintf(stderr, "Malloc failed");
			exit(1);
		}
		strcpy(pages[i].description, buffer);
	}
}

void read_commands(FILE *input_file, browser *b, page *pages, unsigned int page_count)
{
	char command[256];
	unsigned int command_count = 0;

	fscanf(input_file, "%u", &command_count);
	fgetc(input_file); //read the \n after the integer

	printf("command count: %u\n", command_count);

	for (unsigned int i = 0; i < command_count; i++) {
		fgets(command, sizeof(command), input_file);
		unsigned int command_length = strlen(command) - 1;
		if (i != command_count - 1) //the last line doesn't have \n (or does it?)
			command[command_length] = '\0'; //remove the \n
		printf("comanda este: %s\n", command);

		if (strcmp(command, "NEW_TAB") == 0) {
			new_tab(b, pages);
		} else if (strcmp(command, "CLOSE") == 0) {

		} else if (strncmp(command, "OPEN", 4) == 0) {

		} else if (strcmp(command, "NEXT") == 0) {
			
		} else if (strcmp(command, "PREV") == 0) {

		} else if (strncmp(command, "PAGE", 4) == 0) {

		} else if (strcmp(command, "BACKWARD") == 0) {

		} else if (strcmp(command, "FORWARD") == 0) {

		} else if (strcmp(command, "PRINT") == 0) {

		} else if (strncmp(command, "PRINT_HISTORY", 13) == 0) {

		} 
	}
}

int main(void)
{
	FILE *input_file = fopen(INPUT_FILENAME, "rt");

	FILE *output_file = fopen(OUTPUT_FILENAME, "wt");

	if (!input_file || !output_file) {
		fprintf(stderr, "Could not open file");
		exit(1);
	}

	unsigned int page_count = 0;
	fscanf(input_file, "%u", &page_count);

	page pages[51];
	read_pages(pages, page_count, input_file);

	browser *b = create_browser(pages);

	read_commands(input_file, b, pages, page_count);

	return 0;
}