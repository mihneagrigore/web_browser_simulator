//Grigore Mihnea - Andrei - 311CA
//Tema_1 SDA 2025

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "struct.h"
#include "errors.h"
#include "functions.h"

int main(void)
{
    FILE *file = fopen("tema1.in", "r");
    DIE(file);

    browser *web = alloc_browser();
	int last_id = 0;
    tab_node *first_tab = start_browser(web);
	page *first_page = first_tab->data->currentPage;

    int page_number;
    fscanf(file, "%d", &page_number);

    page *pages = malloc(page_number * sizeof(page));
    DIE(pages);

    for (int i = 0; i < page_number; i++) {
        char buffer[1024];
        fscanf(file, "%d", &pages[i].id);
        fgetc(file);

        fgets(pages[i].url, 50, file);
        if (pages[i].url[strlen(pages[i].url) - 1] == '\n') {
            pages[i].url[strlen(pages[i].url) - 1] = '\0';
        }

        fgets(buffer, 1024, file);
        if (buffer[strlen(buffer) - 1] == '\n') {
            buffer[strlen(buffer) - 1] = '\0';
        }

        pages[i].description = malloc(strlen(buffer) + 1);
        DIE(pages[i].description);
        strcpy(pages[i].description, buffer);
    }

    int command_number;
    fscanf(file, "%d", &command_number);
    for (int i = 0; i < command_number; i++) {
        char command[100];
        fscanf(file, "%s", command);
        if (strcmp(command, "NEW_TAB") == 0)
            new_tab(web, ++last_id);
        if (strcmp(command, "CLOSE") == 0)
            close_tab(web);
        if (strcmp(command, "OPEN") == 0) {
            int id;
            fscanf(file, "%d", &id);
            open_id(web, id);
        }
        if (strcmp(command, "NEXT") == 0)
            web->current = next_prev(web, 1);
        if (strcmp(command, "PREV") == 0)
            web->current = next_prev(web, -1);
        if (strcmp(command, "PAGE") == 0) {
            int id;
            fscanf(file, "%d", &id);
            open_page_id(web, web->current, id, pages, page_number);
        }
        if (strcmp(command, "BACKWARD") == 0)
            backward(web, web->current->forwardStack, web->current->backwardStack);
		if (strcmp(command, "FORWARD") == 0)
            forward(web, web->current->forwardStack, web->current->backwardStack);
		if (strcmp(command, "PRINT") == 0)
            print_ids(web);
		if (strcmp(command, "PRINT_HISTORY") == 0) {
			int id;
            fscanf(file, "%d", &id);
			print_history(web, id);
		}
    }

	// Free the memory allocated for the browser
    for (int i = 0; i < page_number; i++)
	{
	  	if(pages[i].description != NULL)
	   	{
          	free(pages[i].description);
           	pages[i].description = NULL;
	   	}
	}

	for(int i = 0; i < page_number; i++)
		pages[i].id = -1;

	if (pages != NULL) {
      	free(pages);
      	pages = NULL;
	}

	if (first_page->description != NULL)
		free(first_page->description);
	if (first_page != NULL)
		free(first_page);

	finish(web);

	if(first_tab)
		free(first_tab);

    fclose(file);
    return 0;
}