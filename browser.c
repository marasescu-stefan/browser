//Marasescu Stefan-Gabriel, 311CA
#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <stdbool.h>

#define INPUT_FILENAME "tema1.in"
#define OUTPUT_FILENAME "tema1.out"

#define DEFAULT_ID 0
#define DEFAULT_URL "https://acs.pub.ro/"
#define DEFAULT_DESCRIPTION "Computer Science"

#define ERROR_MESSAGE "403 Forbidden\n"

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

tab_node *search_current(browser *b)
{
	//searches b->current in the tabList and returns a pointer to the node
	tab_node *t = b->list.santinela->next; //first tab

	while (t != b->list.santinela && t->data->id != b->current->id) {
		t = t->next; //iterating through the list until i find the tab with that id
	}

	if (t == b->list.santinela) //the tab with that id doesn't exist
		return NULL;

	return t;
}

stack *create_stack(void)
{
	stack *s = malloc(sizeof(stack));
	if (!s) {
        fprintf(stderr, "Malloc failed");
        exit(1);
    }
	s->head = NULL;

	return s;
}
void push(stack *s, page *p)
{
	if (s == NULL)
		return;

	stack_node *new_node = malloc(sizeof(stack_node));
	if (!new_node) {
		fprintf(stderr, "Malloc failed");
		exit(1);
	}

	new_node->value = p;
	if (s->head == NULL) {
		new_node->next = NULL;
		s->head = new_node;
	} else {
		new_node->next = s->head;
		s->head = new_node;
	}
}

void pop(stack *s)
{
	if (s == NULL || s->head == NULL)
		return;

	stack_node *removed = s->head;
	s->head = removed->next;
	free(removed);
}

void free_stack_list(stack *s)
{
	if (s == NULL || s->head == NULL)
		return;

	while (s->head != NULL)
		pop(s);
}

page *peek(stack *s)
{
	if (s == NULL || s->head == NULL)
		return NULL;

	return s->head->value;
}

void print_list_reversed(stack_node *head, FILE *output_file)
{
	if (head == NULL)
	    return;

    print_list_reversed(head->next, output_file);
	fprintf(output_file, "%s\n", head->value->url);
}

void print_list(stack_node *head, FILE *output_file)
{
	if (head == NULL)
        return;

    while (head != NULL) {
        fprintf(output_file, "%s\n", head->value->url);
        head = head->next;
    }
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

	b->current->currentPage = pages; //points to the address of pages[0]
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

void free_browser(browser **b)
{
    free((*b)->current); //first field

	tab_node *curr = (*b)->list.santinela->next; //first tab

	while (curr != (*b)->list.santinela) {
		tab_node *next = curr->next;
		free_stack_list(curr->data->forwardStack); //freeing the stack list
		free_stack_list(curr->data->backwardStack); //freeing the stack list
		free(curr->data->forwardStack); //freeing the stack structure
		free(curr->data->backwardStack); //freeing the stack structure
		free(curr->data);
		free(curr);
		curr = next;
	}

	free((*b)->list.santinela);

	free(*b); //free the browser structure
}

void free_pages(page *pages, unsigned int pages_count)
{
	for (unsigned int i = 0; i < pages_count; i++) {
        free(pages[i].description);
    }
}

void new_tab(browser *b, page *pages, int *last_id)
{
	(*last_id)++;
	b->current->id = *last_id;
	b->current->currentPage = pages;

	/*not freeing the old stacks, because there is a reference to them
	in the tab list, in b->list, and if the user wants to open that old
	tab, the stacks need to remain intact*/
	b->current->backwardStack = create_stack(); //creating the new stacks
	b->current->forwardStack = create_stack();

	dll_add_last(&b->list, b->current); //adding current tab in list
}

void close_tab(browser *b, FILE *output_file)
{
	if (b->current->id == 0) {
		fprintf(output_file, ERROR_MESSAGE);
		return;
	}

	tab_node *t = search_current(b);

	tab_node *previous = t->prev;
	t->prev->next = t->next;
	t->next->prev = t->prev;

	free_stack_list(t->data->forwardStack); //freeing the stack list
    free_stack_list(t->data->backwardStack); //freeing the stack list
	free(t->data->forwardStack); //freeing the stack structure
	free(t->data->backwardStack); //freeing the stack structure
	free(t->data);
	free(t);

	b->current->id = previous->data->id;
	b->current->currentPage = previous->data->currentPage;
	b->current->backwardStack = previous->data->backwardStack;
	b->current->forwardStack = previous->data->forwardStack;


	b->list.size--;
}

void open_tab(browser *b, FILE *output_file, char *command)
{
	tab_node *t = b->list.santinela->next; //first tab

	if (strlen(command) < 6) {
		fprintf(output_file, ERROR_MESSAGE);
		return;
	}
	command = command + 5; //moving command ptr at the start of the id
	int id;
	sscanf(command, "%d", &id);

	while (t != b->list.santinela && t->data->id != id) {
		t = t->next; //iterating through the list until i find the tab with that id
	}

	if (t == b->list.santinela) { //the tab with that id doesn't exist
		fprintf(output_file, ERROR_MESSAGE);
	} else { //updating the current tab
		b->current->id = t->data->id;
		b->current->currentPage = t->data->currentPage;
		b->current->backwardStack = t->data->backwardStack;
		b->current->forwardStack = t->data->forwardStack;
	}
}

void open_next_tab(browser *b)
{
	tab_node *t = search_current(b);

	t = t->next; //moving t to the next tab
	if (t == b->list.santinela)
		t = t->next;

	b->current->id = t->data->id;
	b->current->currentPage = t->data->currentPage;
	b->current->backwardStack = t->data->backwardStack;
	b->current->forwardStack = t->data->forwardStack;
}

void open_previous_tab(browser *b)
{
	tab_node *t = search_current(b);

	t = t->prev; //moving t to the previous tab
	if (t == b->list.santinela)
		t = t->prev;

	b->current->id = t->data->id;
	b->current->currentPage = t->data->currentPage;
	b->current->backwardStack = t->data->backwardStack;
	b->current->forwardStack = t->data->forwardStack;
}

void open_page(browser *b, page *pages, unsigned int page_count, FILE *output_file, char *command)
{
	command = command + 5;
	int page_id;
	sscanf(command, "%d", &page_id);
	
	bool found = false;
	page *old_page = b->current->currentPage; //points to one element in the pages array

	for (unsigned int i = 0; i < page_count; i++) {
		if (pages[i].id == page_id) {
			b->current->currentPage = pages + i;
			tab_node *t = search_current(b);
			t->data->currentPage = pages + i;
			found = true;
			break;
		}
	}

	if (!found) {
		fprintf(output_file, ERROR_MESSAGE);
		return;
	}

	push(b->current->backwardStack, old_page);
	free_stack_list(b->current->forwardStack);
}

void open_backward_page(browser *b, FILE *output_file)
{
	if (b->current->backwardStack->head == NULL) { //stack is empty
		fprintf(output_file, ERROR_MESSAGE);
		return;
	}

	push(b->current->forwardStack, b->current->currentPage);
	b->current->currentPage = peek(b->current->backwardStack);
	tab_node *t = search_current(b);
	t->data->currentPage = peek(b->current->backwardStack);
	pop(b->current->backwardStack);
}

void open_forward_page(browser *b, FILE *output_file)
{
	if (b->current->forwardStack->head == NULL) {
		fprintf(output_file, ERROR_MESSAGE);
		return;
	}

	push(b->current->backwardStack, b->current->currentPage);
	b->current->currentPage = peek(b->current->forwardStack);
	tab_node *t = search_current(b);
	t->data->currentPage = peek(b->current->forwardStack);
	pop(b->current->forwardStack);
}

void print(browser *b, FILE *output_file)
{
	tab_node *current = search_current(b); //current tab node
	fprintf(output_file, "%d", current->data->id); //print the current tab id

	tab_node *t = current->next;
	while (t != current) {
		if (t == b->list.santinela) {
			t = t->next; //skip the santinela
		} else {
			fprintf(output_file, " %d", t->data->id);
        	t = t->next;
		}
	}

	fprintf(output_file, "\n");
	fprintf(output_file, "%s\n", current->data->currentPage->description);
}

void print_history(browser *b, FILE *output_file, char *command)
{
	tab_node *t = b->list.santinela->next; //first tab

	if (strlen(command) < 15) {
		fprintf(output_file, ERROR_MESSAGE);
		return;
	}
	command = command + 14; //moving command ptr at the start of the id
	int id;
	sscanf(command, "%d", &id);

	while (t != b->list.santinela && t->data->id != id) {
		t = t->next; //iterating through the list until i find the tab with that id
	}

	if (t == b->list.santinela) { //the tab with that id doesn't exist
		fprintf(output_file, ERROR_MESSAGE);
		return;
	}

	print_list_reversed(t->data->forwardStack->head, output_file);
	fprintf(output_file, "%s\n", t->data->currentPage->url);
	print_list(t->data->backwardStack->head, output_file);
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

	for (unsigned int i = 1; i < page_count; i++) {
		fscanf(input_file, "%d", &pages[i].id);
		fgetc(input_file); //reading the \n after the integer

		fgets(pages[i].url, sizeof(pages[i].url), input_file);
		pages[i].url[strlen(pages[i].url) - 1] = '\0';

		char buffer[512];
		fgets(buffer, sizeof(buffer), input_file);

		unsigned int buffer_size = strlen(buffer);
		buffer[buffer_size - 1] = '\0'; //remove the \n at the end
		//allocating buffer_size bytes, which includes the \0 at the end
		pages[i].description = malloc(buffer_size * sizeof(char));
		if (!pages[i].description) {
			fprintf(stderr, "Malloc failed");
			exit(1);
		}
		strcpy(pages[i].description, buffer);
	}
}

void read_commands(FILE *input_file, FILE *output_file, browser *b, page *pages, unsigned int page_count)
{
	char command[256];
	unsigned int command_count = 0;
	int last_id = 0;

	fscanf(input_file, "%u", &command_count);
	fgetc(input_file); //read the \n after the integer

	for (unsigned int i = 0; i < command_count; i++) {
		fgets(command, sizeof(command), input_file);
		unsigned int command_length = strlen(command) - 1;
		command[command_length] = '\0'; //remove the \n
		if (strcmp(command, "NEW_TAB") == 0) {
			new_tab(b, pages, &last_id);
		} else if (strcmp(command, "CLOSE") == 0) {
			close_tab(b, output_file);
		} else if (strncmp(command, "OPEN", 4) == 0) {
			open_tab(b, output_file, command);
		} else if (strcmp(command, "NEXT") == 0) {
			open_next_tab(b);
		} else if (strcmp(command, "PREV") == 0) {
			open_previous_tab(b);
		} else if (strncmp(command, "PAGE", 4) == 0) {
			open_page(b, pages, page_count, output_file, command);
		} else if (strcmp(command, "BACKWARD") == 0) {
			open_backward_page(b, output_file);
		} else if (strcmp(command, "FORWARD") == 0) {
			open_forward_page(b, output_file);
		} else if (strcmp(command, "PRINT") == 0) {
			print(b, output_file);
		} else if (strncmp(command, "PRINT_HISTORY", 13) == 0) {
			print_history(b, output_file, command);
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
	page_count++; //incrementing beacuse pages[0] is the default page

	page pages[51];
	read_pages(pages, page_count, input_file);

	browser *b = create_browser(pages);

	read_commands(input_file, output_file, b, pages, page_count);

	free_browser(&b);
	free_pages(pages, page_count);

	fclose(input_file);
	fclose(output_file);

	return 0;
}