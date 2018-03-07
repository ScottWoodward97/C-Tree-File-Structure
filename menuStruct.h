#ifndef MENUSTRUCT_H
#define MENUSTRUCT_H

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/*This structure stores the data of each item in the file*/
struct node{
    char *desc;
    unsigned short int id;
};
typedef struct node NODE;
/*This structure represents each node in the tree with pointers to the data, next node and first child node*/
struct menu{
    struct node *node;
    struct menu *next;
    struct menu *childRoot;
};
typedef struct menu MENU;

/*Function prototypes*/
NODE *createNode(unsigned short int, char *);
MENU *createMenu(NODE *);
int numOfDigits(unsigned int);
int insertAll(MENU *, NODE *, unsigned short int);
void insert(MENU **, MENU **);
void freeMenu(MENU **);
void freeMenuAndNodes(MENU **);

#endif 

