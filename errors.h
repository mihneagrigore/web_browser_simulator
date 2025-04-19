//Grigore Mihnea - Andrei - 311CA
//Tema_1 SDA 2025

#define DIE(p) \
    do { \
        if ((p) == NULL) { \
            printf("Malloc error! (Line %d)\n", __LINE__); \
            exit(EXIT_FAILURE); \
        } \
    } while (0)
