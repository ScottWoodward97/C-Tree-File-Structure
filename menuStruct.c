#include <menuStruct.h>

/******************************************************
 * Function createNode
 *      Creates a node structure with attributes equal
 *      to those passed as argument.
 * Parameters:
 *      id - The ID assigned to the node
 *      string - A pointer to the description associated
 *               with the node
 * Returns:
 *      A pointer to the created node
 *      If the memory for the structure or description was
 *      not assigned, a NULL pointer is returned. 
 ******************************************************/
NODE *createNode(unsigned short int id, char *string){
    NODE *node;
    if (!(node = (NODE *)malloc(sizeof(NODE)))) {
        fprintf(stderr, "Memory allocation was unsuccessful\n.");
        return NULL;
    }
    node->id = id;
    if (!(node->desc = (char *)malloc(strlen(string)))){
        fprintf(stderr, "Memory allocation was unsuccessful\n.");
        /*Frees the memory allocated to the node pointer*/
        free(node);
        return NULL;
    }
    strcpy(node->desc, string);
    return node;
}

/******************************************************
 * Function createMenu
 *      Creates a node structure which points to a node
 *      structure.
 * Parameters:
 *      node - A pointer to a created node structure
 * Returns:
 *      A pointer to the created menu
 *      If the memory for the structure was not 
 *      assigned, a NULL pointer is returned. 
 ******************************************************/
MENU *createMenu(NODE *node){
    MENU *menu;
    if (!(menu = (MENU *)malloc(sizeof(MENU)))){
        fprintf(stderr, "Memory allocation was unsuccessful\n.");
        return NULL;
    }
    menu->node = node;
    menu->childRoot=NULL;
    menu->next=NULL;
    return menu;
}

/******************************************************
 * Function insertAll
 *      Creates and inserts a menu structure as a child of every
 *      instance of the parent. Each menu structure created is
 *      unique but they all share the same reference to the
 *      node structure.
 * Parameters:
 *      curr - A pointer to the current menu structure
 *      node - A pointer to the node which is to be inserted
 *      parentId - The id of the parent of 'node'
 * Returns:
 *      1 if the parentId was not found and thus never inserted,
 *      otherwise 0 is returned. 
 ******************************************************/
int insertAll(MENU *curr, NODE *node, unsigned short int parentId){
    int match = 1;
    while (curr != NULL){
        if (curr->node->id == parentId){
            MENU *pm = createMenu(node);
            insert(&(curr->childRoot), &pm);  
            match = 0;
        /*Will not search the children of a matching node*/
        }else if (curr->childRoot != NULL){
            /*Once an instance of a parent has been found, do not update the variable*/
            if (match == 0){
                insertAll(curr->childRoot, node, parentId);
            }else{
               match = insertAll(curr->childRoot, node, parentId); 
            }         
        }
        curr = curr->next;
    }
    return match;
}

/******************************************************
 * Function insert
 *      Insert a given node to the end of a linked list.
 * Parameters:
 *      curr - A pointer to a pointer of the current 
 *             node in the linked list.
 *      node - A pointer to the pointer of the node to
 *             be inserted into the linked list.
 * Returns:
 *      (nothing)
 ******************************************************/
void insert(MENU **curr, MENU **node){
    if (*curr==NULL){
        *curr = *node;
    }else{
        if ((*curr)->next == NULL){
            (*curr)->next = *node;
        }else{
            insert(&((*curr)->next), node);
        }
    }
}

/******************************************************
 * Function numOfDigits
 *      Calculates the number of digits that are in a
 *      given unsigned integer. Used in printMenu to
 *      allocate the correct amount of memory.
 * Parameters:
 *      n - An unsigned integer
 * Returns:
 *      The number of digits in n
 ******************************************************/
int numOfDigits(unsigned int n){
    if (n<10){
        return 1;
    }else{
        return 1 + numOfDigits(n/10);
    }
}

/******************************************************
 * Function printMenu
 *      Prints the menu structure.
 *      Each node is numbered, starting from 1, and its children
 *      are sub-numbered. The children of a node are also indented 
 *      by 3 spaces
 * Parameters:
 *      curr - A pointer to the current node being printed
 *      prefix - A string that is printed before each node. 
 *               It contains the indents and full parent index
 * Returns:
 *      (nothing)
 ******************************************************/
void printMenu(MENU *curr, char *prefix){
    unsigned int index = 1;
    if (prefix == NULL)prefix = "";
    while (curr!=NULL){
        printf("%s%hu %s\n", prefix, index, curr->node->desc);
        /*Only create a new prefix string if the current structure has children*/
        if (curr->childRoot !=NULL){
            int size = strlen(prefix) + numOfDigits(index) + 4; /*3 characters for spaces, 1 for the point*/
            char newPrefix[size];
            sprintf(newPrefix, "   %s%d.", prefix, index);
            printMenu(curr->childRoot, newPrefix);
        }
        curr = curr->next;
        index++;       
    }
}

/******************************************************
 * Function freeMenu
 *      Frees the memory allocated to each menu structure
 *      in a linked tree.
 *      NOTE: This will not free the memory allocated to
 *      the nodes
 * Parameters:
 *      root - A pointer to a pointer of the root node of
 *             a menu structure.
 * Returns:
 *      (nothing)
 ******************************************************/
void freeMenu(MENU **root){
    while (*root != NULL){
        MENU *t = *root;
        if ((*root)->childRoot != NULL) freeMenu(&((*root)->childRoot));
        *root = (*root)->next;
        free(t);
    }
}


