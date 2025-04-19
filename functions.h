//Grigore Mihnea - Andrei - 311CA
//Tema_1 SDA 2025

browser* alloc_browser()
{
	//Start the browser
    browser* web = (browser*)malloc(sizeof(browser));
    DIE(web);

    web->list.size = 0;
    web->current = NULL;

    // Allocate and initialize the santinel of the list
    web->list.head = (tab_node*)malloc(sizeof(tab_node));
    DIE(web->list.head);
    web->list.head->data = NULL;

    // Initialize the circular linked list
    web->list.head->next = web->list.head;
    web->list.head->prev = web->list.head;

    return web;
}

tab_node* start_browser(browser *web)
{
    if (web->current)
        return NULL;

    // Create first tab
    tab* first_tab = (tab*)malloc(sizeof(tab));
    DIE(first_tab);

    first_tab->id = 0;
    first_tab->backwardStack = NULL;
    first_tab->forwardStack = NULL;

    // Create first page
    page *first_page = (page*)malloc(sizeof(page));
    DIE(first_page);

    first_page->id = 0;

    char url[] = "https://acs.pub.ro/";
    strcpy(first_page->url, url);

    char description[] = "Computer Science";
    first_page->description = (char*)malloc(strlen(description) + 1);
    DIE(first_page->description);
    strcpy(first_page->description, description);

    first_tab->currentPage = first_page;

    // Add first tab to browser
    web->current = first_tab;

    // Initialize the first tab in the list
    web->list.head->next = (tab_node*)malloc(sizeof(tab_node));
    DIE(web->list.head->next);

    web->list.head->next->data = first_tab;
    web->list.head->next->next = web->list.head;
    web->list.head->next->prev = web->list.head;
	web->list.head->prev = web->list.head->next;

    web->list.size = 1;

    return web->list.head->next;  // Return the first tab
}

void finish(browser* web)
{
    if (web == NULL || web->list.head == NULL)
        return;

    //Only one tab is open - close it
    if (web->current != NULL && web->list.size == 1) {
        tab* first_tab = web->current;
        if (first_tab->backwardStack) {
			simple_node* node = first_tab->backwardStack->list->head;
			while (node != NULL && first_tab->backwardStack->list->size > 0) {
				simple_node* temp = node;
				node = (simple_node*)node->next;
				if (temp->data) {
					free(temp->data);
					temp->data = NULL;
				}
				if (temp) {
					free(temp);
					temp = NULL;
				}
				first_tab->backwardStack->list->size--;
			}
			free(first_tab->backwardStack->list);
			first_tab->backwardStack->list = NULL;
            free(first_tab->backwardStack);
            first_tab->backwardStack = NULL;
        }
        if (first_tab->forwardStack) {
			simple_node* node = first_tab->forwardStack->list->head;
			while (node != NULL && first_tab->forwardStack->list->size > 0) {
				simple_node* temp = node;
				node = (simple_node*)node->next;
				if (temp->data) {
					free(temp->data);
					temp->data = NULL;
				}
				if (temp) {
					free(temp);
					temp = NULL;
				}
				first_tab->forwardStack->list->size--;
			}
			free(first_tab->forwardStack->list);
			first_tab->forwardStack->list = NULL;
            free(first_tab->forwardStack);
            first_tab->forwardStack = NULL;
        }
        if (first_tab) {
            free(first_tab);
            first_tab = NULL;
        }

		if (web->list.head) {
			free(web->list.head);
			web->list.head = NULL;
		}

        if (web) {
            free(web);
            web = NULL;
        }
        return;
    }

	// Free all tabs - start from the last tab
    tab_node* current = (tab_node*)web->list.head->prev;

    while (current != web->list.head->next) {
        tab_node* aux = current;
        current = (tab_node*)current->prev;

        if (aux->data != NULL) {
			// Free current page
			if (aux->data->currentPage != NULL) {
				if (aux->data->currentPage->id == 0) {
					if (aux->data->currentPage->description != NULL) {
						free(aux->data->currentPage->description);
						aux->data->currentPage->description = NULL;
					}
					free(aux->data->currentPage);
					aux->data->currentPage = NULL;
				}
			}

            // Free forward stack
            if (aux->data->forwardStack != NULL) {
				if (aux->data->forwardStack->list != NULL) {
			 		simple_node* node = aux->data->forwardStack->list->head;
			 		while (node != NULL && aux->data->forwardStack->list->size > 0) {
			 			simple_node* temp = node;
						node = (simple_node*)node->next;
			 			if (temp->data->description) {
			 				free(temp->data->description);
			 				temp->data->description = NULL;
			 			}
			 			if (temp->data) {
			 				free(temp->data);
			 				temp->data = NULL;
			 			}
			 			if (temp){
			 				free(temp);
			 				temp = NULL;
			 			}
						aux->data->forwardStack->list->size--;
			 		}
			 	}
				free(aux->data->forwardStack->list);
				aux->data->forwardStack->list = NULL;
                free(aux->data->forwardStack);
                aux->data->forwardStack = NULL;
            }

            // Free backward stack
            if (aux->data->backwardStack != NULL) {
				if (aux->data->backwardStack->list != NULL && aux->data->backwardStack->list->size > 0) {
			 		simple_node* node = aux->data->backwardStack->list->head;
			 		while (node != NULL && aux->data->backwardStack->list->size > 0) {
			 			simple_node* temp = node;
			 			node = (simple_node*)node->next;
			 			if (temp->data) {
			 				free(temp->data);
			 				temp->data = NULL;
			 			}
			 			if (temp) {
			 				free(temp);
							temp = NULL;
			 			}
						aux->data->backwardStack->list->size--;
			 		}
			 	}
				free(aux->data->backwardStack->list);
				aux->data->backwardStack->list = NULL;
                free(aux->data->backwardStack);
                aux->data->backwardStack = NULL;
            }

            if (aux->data) {
                free(aux->data);
                aux->data = NULL;
            }
        }

        free(aux);
        aux = NULL;

        if (web->list.size > 0)
            web->list.size--;
    }

	// Free the first tab
	if (web->list.head->next != NULL) {
        if (web->list.head->next->data != NULL) {

            // Free stacks
            if (web->list.head->next->data->backwardStack != NULL) {
				if(web->list.head->next->data->backwardStack->list != NULL) {
					simple_node* node = web->list.head->next->data->backwardStack->list->head;
					while (node != NULL && web->list.head->next->data->backwardStack->list->size > 0) {
						simple_node* temp = node;
						node = (simple_node*)node->next;
						if (temp->data) {
							free(temp->data);
							temp->data = NULL;
						}
						if (temp) {
							free(temp);
							temp = NULL;
						}
						web->list.head->next->data->backwardStack->list->size--;
					}
					free(web->list.head->next->data->backwardStack->list);
					web->list.head->next->data->backwardStack->list = NULL;
				}

                free(web->list.head->next->data->backwardStack);
                web->list.head->next->data->backwardStack = NULL;
            }

            if (web->list.head->next->data->forwardStack != NULL) {
				simple_node* node = web->list.head->next->data->forwardStack->list->head;
				while (node != NULL && web->list.head->next->data->forwardStack->list->size > 0) {
					simple_node* temp = node;
					node = (simple_node*)node->next;
					if (temp->data) {
						free(temp->data);
						temp->data = NULL;
					}
					if (temp) {
						free(temp);
						temp = NULL;
					}
					web->list.head->next->data->forwardStack->list->size--;
				}
				if(web->list.head->next->data->forwardStack->list != NULL) {
					free(web->list.head->next->data->forwardStack->list);
					web->list.head->next->data->forwardStack->list = NULL;
				}

                free(web->list.head->next->data->forwardStack);
                web->list.head->next->data->forwardStack = NULL;
            }

            if (web->list.head->next->data) {
                free(web->list.head->next->data);
                web->list.head->next = NULL;
            }

        }

        free(web->list.head->next);
		web->list.head->next = NULL;
    }

	// Free the santinel
    if (web->list.head != NULL) {
        if (web->list.head->data != NULL) {

            // Free stacks
            if (web->list.head->data->backwardStack != NULL) {
	            free(web->list.head->data->backwardStack);
                web->list.head->data->backwardStack = NULL;
            }

            if (web->list.head->data->forwardStack != NULL) {
                free(web->list.head->data->forwardStack);
                web->list.head->data->forwardStack = NULL;
            }
        }

        free(web->list.head);
        web->list.head = NULL;
    }

	// Free the browser
    if (web) {
        free(web);
        web = NULL;
    }
}

void print_ids(browser* web)
{
	if (!web || !web->current || !web->list.head || !web->current->currentPage->description)
	 	return;

	if (web->list.size == 1) {
		printf("%d", web->list.head->next->data->id);
		goto EXIT;
	}

	//Search for current id
	tab_node *node = web->list.head->next;
	while (node->data->id != web->current->id)
		node = node->next;

	tab_node *current = node;

	//Is the first node - print in order
	if (node == web->list.head->next) {
		printf("%d ", current->data->id);
		current = web->list.head->next->next;
		while (current != web->list.head) {
			printf("%d ", current->data->id);
			current = current->next;
		}
		goto EXIT;
	}

	//Is inside the list - print to the last
	while (current != web->list.head) {
		printf("%d ", current->data->id);
		current = current->next;
	}

	//Print to the first node
	current = web->list.head->next;
	while (current != node) {
		printf("%d ", current->data->id);
		current = current->next;
	}

EXIT:
	printf("\n");
	printf("%s\n", web->current->currentPage->description);
}

void new_tab(browser* web, int id)
{
	if (!web || !web->list.head) {
		return;
	}

	//Create new tab to the list
	tab_node* new = (tab_node*)malloc(sizeof(tab_node));
	DIE(new);

	new->data = (tab*)malloc(sizeof(tab));
	DIE(new->data);

	//Set as current tab in browser
	web->current = new->data;

	//Set tab id
	new->data->id = id;

	new->data->backwardStack = NULL;
    new->data->forwardStack = NULL;

	//Create first page
	page *first_page = (page*)malloc(sizeof(page));
	DIE(first_page);

	first_page->id = 0;

	char url[] = "https://acs.pub.ro/";
	strcpy(first_page->url, url);

	char description[] = "Computer Science";
	first_page->description = (char*)malloc(sizeof(description) + 1);
	DIE(first_page->description);
	strcpy(first_page->description, description);

	//Add first page to the tab
	new->data->currentPage = first_page;

	//Add new tab to the tabsList (last node)
	new->next = web->list.head;
	if (web->list.head->prev != web->list.head){
    	new->prev = web->list.head->prev;
		web->list.head->prev->next = new;
	} else {
		new->prev = web->list.head;
		web->list.head->next = new;
	}
	web->list.head->prev = new;
    web->list.size++;

	//Set as current tab in browser
	web->current = new->data;
}

void close_tab(browser *web)
{
    if (!web || !web->list.head || web->current->id == 0) {
        printf("403 Forbidden\n");
        return;
    }

	//Close if only 2 tabs are open
	if (web->list.size == 2) {

		tab_node *node = web->list.head->next->next;
		web->list.head->next->next = web->list.head;
		web->list.head->prev = web->list.head->next;
		web->list.size--;
		web->current = web->list.head->next->data;

		//Free tab
		if (node->data && node->data->backwardStack) {
			free(node->data->backwardStack);
			node->data->backwardStack = NULL;
		}

		if (node->data && node->data->forwardStack) {
			free(node->data->forwardStack);
			node->data->forwardStack = NULL;
		}

		if (node->data && node->data->currentPage)
			if (node->data->currentPage->id == 0) {
				if(node->data->currentPage->description) {
					free(node->data->currentPage->description);
					node->data->currentPage->description = NULL;
				}
				free(node->data->currentPage);
				node->data->currentPage = NULL;
			}

		if (node->data) {
			free(node->data);
			node->data = NULL;
		}

		if (node) {
			free(node);
			node = NULL;
		}

    return;
	}

    tab_node *prev_tab = web->list.head->next;

    // Get the previous tab
    while (prev_tab->next != web->list.head && prev_tab->next->data->id != web->current->id)
        prev_tab = prev_tab->next;

    tab_node *tab_to_close = prev_tab->next;

    web->current = prev_tab->data;

	if (tab_to_close->next != NULL)
        tab_to_close->next->prev = prev_tab;

    // Remove the tab from the list
    prev_tab->next = tab_to_close->next;

	// Free the memory of the closed tab
		if (tab_to_close && tab_to_close->data) {
			if (tab_to_close->data->backwardStack) {
				free(tab_to_close->data->backwardStack);
				tab_to_close->data->backwardStack = NULL;
			}

			if (tab_to_close->data->forwardStack) {
				free(tab_to_close->data->forwardStack);
				tab_to_close->data->forwardStack = NULL;
			}

			if (tab_to_close->data->currentPage) {
				if (tab_to_close->data->currentPage->id == 0) {
					if (tab_to_close->data->currentPage->description) {
						free(tab_to_close->data->currentPage->description);
						tab_to_close->data->currentPage->description = NULL;
					}
					free(tab_to_close->data->currentPage);
					tab_to_close->data->currentPage = NULL;
				}
			}
			free(tab_to_close->data);
			tab_to_close->data = NULL;
		}

		if (tab_to_close) {
			free(tab_to_close);
			tab_to_close = NULL;
		}

    web->list.size--;
}

void open_id(browser *web, int id)
{
	if (web->list.size == 1 && id != 0) {
		printf("403 Forbidden\n");
		return;
	}

	if (id == 0) {
        web->current = web->list.head->next->data;
		return;
    }

	//Search for the tab with the given id
	tab *node = NULL;
	tab_node *current = web->list.head->next;
	while (current != web->list.head) {
		if (current->data->id == id) {
            node = current->data;
            break;
        }
        current = current->next;
	}

	// The tab is not found
	if (!node) {
        printf("403 Forbidden\n");
		return;
	}
	web->current = node;
}

tab *next_prev(browser *browser, const int arg)
{
	if (browser->list.size == 1)
		return browser->current;

    if (!browser || !browser->current)
        return NULL;

	// Next
    if (arg == 1) {
		tab_node* current = browser->list.head->next;
		for (int i = 0; i < browser->list.size - 1; i++) {
			if (current->data->id == browser->current->id)
				return current->next->data;
			current = current->next;
		}
		return browser->list.head->next->data;

	} else if (arg == -1) { // Previous
		tab_node* current = browser->list.head->prev;
		for (int i = 0; i < browser->list.size - 1; i++) {
			if (current->data->id == browser->current->id)
				return current->prev->data;
			current = current->prev;
		}
		return browser->list.head->prev->data;
	}
    return NULL;
}

void empty_stack(stack* mystack)
{
    if (!mystack || !mystack->list || !mystack->list->head)
        return;

    simple_node* node = mystack->list->head;
    while (node != NULL) {
        simple_node* temp = node;
        node = (simple_node*)node->next;

        // free the data
        free(temp->data);
        temp->data = NULL;

        // move to the next node
        temp->next = NULL;
    }
}

void add_stack(stack* mystack, page *data)
{
	if (!mystack ||!mystack->list)
		return;

	//Alloc the list
	if (mystack->list->head == NULL) {
		simple_node* new_node = malloc(sizeof(simple_node));
        DIE(new_node);

		new_node->data = (page*)malloc(sizeof(page));
		DIE(new_node->data);

		new_node->data->description = (char*)malloc(sizeof(data->description) + 1);
		DIE(new_node->data->description);

        memcpy(new_node->data, data, sizeof(page));

        new_node->next = NULL;
        mystack->list->head = new_node;
        mystack->list->size = 1;
        return;
	}

	//Alloc the new node
    simple_node* new_node = malloc(sizeof(simple_node));
    DIE(new_node);

	new_node->data = (page*)malloc(sizeof(page));
	DIE(new_node->data);

    memcpy(new_node->data, data, sizeof(page));

    new_node->next = mystack->list->head;
    mystack->list->head = new_node;
    mystack->list->size++;
}

void delete_first_stack(stack* mystack)
{
	if (!mystack ||!mystack->list || !mystack->list->head)
        return;

	//Free the first node
	if (mystack->list->size == 1) {
		free(mystack->list->head->data);
		mystack->list->head->data = NULL;

		free(mystack->list->head);
		mystack->list->head = NULL;

		mystack->list->size--;
		return;
	}

	//Free the top node of the stack
	simple_node* temp = mystack->list->head;
	mystack->list->head = mystack->list->head->next;

	free(temp->data);
	temp->data = NULL;

	free(temp);
	temp = NULL;
	mystack->list->size--;
}

void backward(browser* web, stack *forward, stack *backward)
{
	if (!backward || !backward->list || !backward->list->head) {
		printf("403 Forbidden\n");
		return;
	}

	add_stack(forward, web->current->currentPage);

	page *aux = (page*)malloc(sizeof(page));
	DIE(aux);

	aux->description = (char*)malloc(sizeof(backward->list->head->data->description) + 1);
	DIE(aux->description);

	memcpy(aux, backward->list->head->data, sizeof(page));
	web->current->currentPage = aux;

	delete_first_stack(backward);
}

void forward(browser* web, stack *forward, stack *backward)
{
	if (!forward || !forward->list || !forward->list->head || !forward->list->head->data) {
		printf("403 Forbidden\n");
		return;
	}

	add_stack(backward, web->current->currentPage);

	page *aux = (page*)malloc(sizeof(page));
	DIE(aux);

	aux->description = (char*)malloc(sizeof(backward->list->head->data->description) + 1);
	DIE(aux->description);

	memcpy(aux, forward->list->head->data, sizeof(page));
	web->current->currentPage = aux;

	delete_first_stack(forward);
}

page *valid_id(page *pages, int id, int n)
{
	if (id < 0)
		return NULL;

	for (int i = 0; i < n; i++)
		if (pages[i].id == id)
			return &pages[i];

	return NULL;
}

void open_page_id(browser *web, tab *current, int id, page *pages, int n)
{
	if (!web || !web->list.head || !current || !current->currentPage)
        return;

    page *mypage = valid_id(pages, id, n);
	if (mypage == NULL) {
		printf("403 Forbidden\n");
		return;
	}

	//Alloc stacks
	if (!current->backwardStack) {
		current->backwardStack = (stack*)malloc(sizeof(stack));
		DIE(current->backwardStack);

		current->backwardStack->list = (ll*)malloc(sizeof(ll));
		DIE(current->backwardStack->list);

		current->backwardStack->list->head = NULL;
		current->backwardStack->list->size = 0;
	}

	if (!current->forwardStack) {
		current->forwardStack = (stack*)malloc(sizeof(stack));
		DIE(current->forwardStack);

		current->forwardStack->list = (ll*)malloc(sizeof(ll));
		DIE(current->forwardStack->list);

		current->forwardStack->list->head = NULL;
		current->forwardStack->list->size = 0;
	}

	//Add the current page to the backward stack
	add_stack(current->backwardStack, current->currentPage);

	web->current->currentPage = mypage;

	//Free the forward stack
	if(current->forwardStack)
		empty_stack(current->forwardStack);
}

void print_stack(simple_node *node)
{
	if(!node)
		return;

	printf("%s\n", node->data->url);
	print_stack(node->next);
}

void print_history(browser *web, int id)
{
	if (!web || !web->list.head)
		return;

	//Search for the tab with the given id
	tab *node = NULL;
	tab_node *current = web->list.head->next;
	for (int i = 0; i < web->list.size; i++) {
		if (current->data->id == id) {
			node = current->data;
			break;
		}
		current = current->next;
	}

	if (!node) {
		printf("403 Forbidden\n");
		return;
	}

	//Print the forward stack
	if (node->forwardStack) {
		for (int i = 0; i < node->forwardStack->list->size; i++) {
			simple_node *temp = node->forwardStack->list->head;
			printf("%s\n", temp->data->url);
		}
	}

	printf("%s\n", node->currentPage->url);

	//Print the backward stack
	if (node->backwardStack != NULL)
		print_stack(node->backwardStack->list->head);

}
