# Web Browser Simulator
# <h6>This project simulates a simplified web browser with support for multiple tabs, browsing history, and basic navigation commands. The core challenge addressed is managing dynamic tab and page navigation efficiently, while preserving history through stacks. The implementation uses circular doubly linked lists for tab management and linked stacks for tracking backward and forward navigation. Commands like opening/closing tabs, navigating between them, visiting pages, and printing history are read from an input file and processed in order to mimic realistic browser behavior.

## Table of Contents

1. [main](#main)
2. [alloc_browser](#1-alloc_browser)
3. [start_browser](#2-start_browser)
4. [finish](#3-finish)
5. [print_ids](#4-print_ids)
6. [new_tab](#5-new_tab)
7. [close_tab](#6-close_tab)
8. [open_id](#7-open_id)
9. [next_prev](#8-next_prev)
10. [empty_stack](#9-empty_stack)
11. [add_stack](#10-add_stack)
12. [delete_first_stack](#11-delete_first_stack)
13. [backward](#12-backward)
14. [forward](#13-forward)
15. [valid_id](#14-valid_id)
16. [open_page_id](#15-open_page_id)
17. [print_stack](#16-print_stack)
18. [print_history](#17-print_history)
    
---

## `main`
   - **Return Type**: `int`
   - **Description**: This is the main function that runs the entire browser simulation. It reads data from an input file (`tema1.in`), sets up the browser, processes commands, and manages browser tabs and pages accordingly.
   - **Functionality**:
     - Opens the input file (`tema1.in`) for reading and allocates the necessary memory for the browser and its tabs.
     - Reads the number of pages to be used in the simulation and stores them in an array of `page` structures. Each page is initialized with an ID, URL, and description.
     - Reads the number of commands to be executed and processes each command accordingly:
       - `NEW_TAB`: Adds a new tab to the browser.
       - `CLOSE`: Closes the current tab.
       - `OPEN <ID>`: Switches to a tab with the given ID.
       - `NEXT`: Moves to the next tab.
       - `PREV`: Moves to the previous tab.
       - `PAGE <ID>`: Opens a page with the given ID in the current tab.
       - `BACKWARD`: Navigates to the previous page in the current tab's history.
       - `FORWARD`: Navigates to the next page in the current tab's history.
       - `PRINT`: Prints the IDs of all open tabs and the description of the current page.
       - `PRINT_HISTORY <ID>`: Prints the browsing history of the specified tab ID.
     - After executing all commands, it frees all allocated memory (pages, descriptions, and browser structures) to prevent memory leaks.
     - Closes the input file and returns 0, indicating successful execution.

---

## 1. `alloc_browser`
   - **Return Type**: `browser*`
   - **Description**: Allocates memory for a new browser, initializing the tab list as a circular doubly linked list with a sentinel node, and sets the current tab to `NULL`.
   - **Functionality**:
     - Allocates memory for a new `browser` object.
     - Initializes the list of tabs as a circular doubly linked list with a sentinel node.
     - Sets the `current` pointer to `NULL` since no tab is open at this point.
     - Returns a pointer to the newly allocated browser object.

---

## 2. `start_browser`
   - **Return Type**: `tab_node*`
   - **Description**: Initializes the browser by creating the first tab and setting it as the current tab. The default tab will open the standard page with ID = 0. It then adds this tab to the list and returns a pointer to the tab node.
   - **Functionality**:
     - Creates a new tab with ID 0 and initializes the default page with a predefined URL and description.
     - Allocates memory for the tab's `backwardStack` and `forwardStack`, both of which are initially empty.
     - Adds the new tab to the browser's list of tabs, linking it as the first element in the circular doubly linked list.
     - Sets the newly created tab as the current tab.
     - Returns a pointer to the first tab node (which holds the first tab).

---

## 3. `finish`
   - **Return Type**: `void`
   - **Description**: Frees the memory used by the browser, tabs, and pages. Closes all tabs and deletes the tab list.
   - **Functionality**:
     - Checks if the `web` or its `list.head` is `NULL`. If either is `NULL`, it does nothing.
     - If only one tab is open, the function frees all associated memory (pages, stacks, tab data) and the tab itself.
     - If there are multiple tabs open, it iterates over all the tabs, frees the pages, stacks, and nodes associated with each tab.
     - Frees the sentinel node, the browser structure itself, and all allocated memory.

---

## 4. `print_ids`
   - **Return Type**: `void`
   - **Description**: This function prints the IDs of all open tabs, starting from the current tab, in the order of the circular linked list. It also prints the description of the current page in the current tab.
   - **Functionality**:
     - If `web` or `web->current` or `web->list.head` is `NULL`, the function returns early.
     - If there is only one tab, it simply prints its ID and the description of its current page.
     - Otherwise, it traverses the list starting from the current tab and prints the IDs of all the tabs.
     - The function ends by printing the description of the page in the current tab.

---

## 5. `new_tab`
   - **Return Type**: `void`
   - **Description**: Adds a new tab to the browser’s list of tabs and sets it as the current tab. The new tab will start with a predefined page (the default page with ID 0).
   - **Functionality**:
     - Allocates memory for a new `tab` and its `backwardStack` and `forwardStack`.
     - Creates the default page and sets it as the current page for the new tab.
     - Adds the new tab at the end of the list (circular doubly linked list), updating the appropriate pointers to maintain the list's circular structure.
     - Sets the new tab as the current tab.

---

## 6. `close_tab`
   - **Return Type**: `void`
   - **Description**: Closes the current tab and removes it from the browser's list. If there is only one tab (the initial one), it will not be closed, and an error message will be printed. The tab to the left of the closed tab becomes the new current tab.
   - **Functionality**:
     - Checks if the current tab is the initial tab (ID 0). If so, it prints an error message ("403 Forbidden") and returns.
     - If there are exactly two tabs, it closes the current tab and sets the remaining tab as the current tab.
     - For multiple tabs, it finds the tab to the left of the current one, removes the current tab from the list, and frees its associated memory (including stacks and page).
     - Updates the list's size.

---

## 7. `open_id`
   - **Return Type**: `void`
   - **Description**: Switches the current tab to the tab with the specified ID. If the tab with the given ID does not exist, it prints an error message ("403 Forbidden").
   - **Functionality**:
     - Searches for a tab with the specified ID in the list.
     - If the tab is found, it sets it as the current tab.
     - If the tab is not found, it prints an error message.

---

## 8. `next_prev`
   - **Return Type**: `tab*`
   - **Description**: Navigates to the next or previous tab based on the argument `arg`. If `arg` is 1, it moves to the next tab; if `arg` is -1, it moves to the previous tab. The function returns the new current tab.
   - **Functionality**:
     - If there is only one tab, it simply returns the current tab.
     - For other cases, it traverses the list of tabs to find the next or previous tab based on the value of `arg`.
     - Returns the next or previous tab after updating the `current` tab.

---

## 9. `empty_stack`
   - **Return Type**: `void`
   - **Description**: Clears all the elements from a given stack by iterating over the nodes and freeing the memory associated with each node.
   - **Functionality**:
     - Checks if the stack or the list within the stack is `NULL`. If so, it returns early.
     - Iterates over the list, frees the data of each node, and then frees the node itself.
     - Updates the list to indicate it is now empty.

---

## 10. `add_stack`
   - **Return Type**: `void`
   - **Description**: Adds a page to the stack. The new page is added to the top of the stack (beginning of the list).
   - **Functionality**:
     - Allocates memory for a new stack node and for the page data.
     - Copies the provided page data into the new node.
     - Adds the new node to the top of the stack by linking it to the current head of the list.
     - Updates the size of the stack.

---

## 11. `delete_first_stack`
   - **Return Type**: `void`
   - **Description**: Deletes the first element (top) from the stack.
   - **Functionality**:
     - Checks if the stack is empty. If the size of the list is 1, it frees the data and the node.
     - For larger stacks, it removes the first node, shifts the list's head pointer, and frees the memory for the node and its data.
     - Updates the stack's size.

---

## 12. `backward`
   - **Return Type**: `void`
   - **Description**: Navigates to the previous page in the browsing history (using the backward stack). The current page is moved to the forward stack.
   - **Functionality**:
     - Checks if the backward stack is empty. If it is, it prints an error message ("403 Forbidden").
     - Moves the current page to the forward stack.
     - Pops the most recent page from the backward stack and sets it as the current page.
     - Deletes the first node from the backward stack.

---

## 13. `forward`
   - **Return Type**: `void`
   - **Description**: Navigates to the next page in the browsing history (using the forward stack). The current page is moved to the backward stack.
   - **Functionality**:
     - Checks if the forward stack is empty. If it is, it prints an error message ("403 Forbidden").
     - Moves the current page to the backward stack.
     - Pops the most recent page from the forward stack and sets it as the current page.
     - Deletes the first node from the forward stack.

---

## 14. `valid_id`
   - **Return Type**: `page*`
   - **Description**: Checks if there is a page with a given ID in a list of pages. If the page with the specified ID exists, it returns a pointer to that page. Otherwise, it returns `NULL`.
   - **Functionality**:
     - Iterates over the list of pages and checks if any page has the specified ID.
     - Returns a pointer to the page if found, otherwise returns `NULL`.

---

## 15. `open_page_id`
   - **Return Type**: `void`
   - **Description**: Opens the page with the specified ID in the current tab. The current page is moved to the backward stack, and the forward stack is cleared.
   - **Functionality**:
     - Verifies if the page with the specified ID exists using the `valid_id` function.
     - If the page exists, it adds the current page to the backward stack and sets the new page as the current one.
     - Clears the forward stack to ensure it only holds pages from the current session.

---

## 16. `print_stack`
   - **Return Type**: `void`
   - **Description**: Prints the URLs of all pages in a stack, starting from the top of the stack.
   - **Functionality**:
     - Recursively prints the URL of each page in the stack.
     - Stops printing when there are no more pages in the stack.

---

## 17. `print_history`
   - **Return Type**: `void`
   - **Description**: Prints the browsing history of a tab, including pages in the forward stack, the current page, and pages in the backward stack.
   - **Functionality**:
     - Searches for the tab with the specified ID.
     - Prints the URLs from the forward stack (if any), the current page's URL, and the URLs from the backward stack (if any).
