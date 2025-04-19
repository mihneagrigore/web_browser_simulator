//Grigore Mihnea - Andrei - 311CA
//Tema_1 SDA 2025

typedef struct
{
    int id;
    char url[50];
    char *description;
} page;

typedef struct simple_node simple_node;
struct simple_node
{
    page *data;
    simple_node* next;
};

typedef struct
{
    simple_node *head;
    int data_size;
    int size;
} ll;

typedef struct
{
    ll *list;
} stack;

typedef struct
{
    int id;
    page *currentPage;
    stack *backwardStack;
    stack *forwardStack;
} tab;

typedef struct tab_node tab_node;
struct tab_node
{
    tab *data;
    tab_node *next;
    tab_node *prev;
};

typedef struct
{
    tab_node *head; // head will be used as santinel
    int size;
} tabsList;

typedef struct
{
    tab *current;
    tabsList list;
} browser;
