#include <menuTrees.h>

/***************************************************************************/
/* Function: processFile
 *      Reads the contents of a file and creates a tree structure.
 *      The file contains the information associated with the nodes, which
 *      is of the format "'A'(Four Digit Id)(Description)" and the parents of 
 *      each node, in the format "'B'(Four Digit Id of Child)(Four Digit Id of Parent)"
 *      Failure to adhere to the require format will cause the function to exit
 * Parameters:
 *      fp - Points to an open file that contains the data.
 *      nodeRoot - Points to the linked list of nodes.
 * Returns:
 *      A pointer to the first child of the root node of the tree structure.
 *      If the format was not adhered to a NULL pointer is instead returned.
 ***************************************************************************/

MENU *processFile(FILE **fp, MENU **nodeRoot){
    unsigned short int id;
    char *string;
    char buffer[BUFSIZ]; /*POSIX recommended buffer size*/
    char c, prevChar;
    int firstRead;
    MENU *menuRoot = NULL;
    
    firstRead = 0;
    prevChar = 'X';
    fprintf(stderr, "Data values read from file:\n");
    while (fgets(buffer, sizeof(buffer), *fp) != NULL){
        if (sscanf(buffer, "%c%04hu%[^\n]s", &c, &id, string) == 3){      
        
        /*Ensures that an A record can only be read after another A record or as the first record in the file*/
        if (c == 'A' && (prevChar == 'A' || firstRead == 0)){
            if (firstRead) firstRead = 1;
            fprintf(stderr, "%4hu: %s\n", id, string);
            /*Reads and creates a node and menu item, storing it in a temporary linked list for later access*/
            if (findNode(id, *nodeRoot)!=NULL){
                fprintf(stderr, "A node of id &hu has already been defined. IDs must be unique.\n");
                freeNodes(*nodeRoot);
                freeMenu(nodeRoot);
                break;
            }
            NODE *n = createNode(id, string);
            MENU *m = createMenu(n);
            insert(/*&*/nodeRoot, &m); 
        }else if(c =='B'){
            unsigned short int parentId;
            if (sscanf(string, "%04hu", &parentId) != 1){
                fprintf(stderr, "Parent node not in correct format. It must be a four digit integer.\n");
                if (menuRoot != NULL){freeNodes(*nodeRoot); freeMenu(nodeRoot); freeMenu(&menuRoot);} /*freeMenuAndNodes(&menuRoot);*/
                break;
            }
            if (parentId == id){
                fprintf(stderr, "Parent node and child node cannot have the same ID.\n");
                if (menuRoot != NULL){freeNodes(*nodeRoot); freeMenu(nodeRoot); freeMenu(&menuRoot);} /*freeMenuAndNodes(&menuRoot);*/
                break;
            }
            
            /*Retrieves the node from the linked list*/
            NODE *pn = findNode(id, *nodeRoot); 
            if (pn == NULL){
                fprintf(stderr, "The child node %04u has not been defined.\n", id);
                if (menuRoot != NULL){freeNodes(*nodeRoot); freeMenu(nodeRoot); freeMenu(&menuRoot);} /*freeMenuAndNodes(&menuRoot);*/
                break;
            }else if((findNode(parentId, *nodeRoot) == NULL && parentId != 0)){
                fprintf(stderr, "The parent node %04u has not defined.\n", parentId);
                if (menuRoot != NULL){freeNodes(*nodeRoot); freeMenu(nodeRoot); freeMenu(&menuRoot);} /*freeMenuAndNodes(&menuRoot);*/
                break;
            }else{
                fprintf(stderr, "   Child %hu, Parent %hu\n", id, parentId);
                if (parentId != 0){
                    /*Checks if the insertAll function failed to find the parent node*/
                    if (insertAll(menuRoot, pn, parentId)){
                       fprintf(stderr, "The parent node %04u is not a direct descendant of the root node.\n", parentId);
                       if (menuRoot != NULL){freeNodes(*nodeRoot); freeMenu(nodeRoot); freeMenu(&menuRoot);} /*freeMenuAndNodes(&menuRoot);*/
                       break; 
                    }
                }else{
                    /*Inserts as a child of the root node*/
                    MENU *pm = createMenu(pn);
                    insert(&menuRoot, &pm);
                } 
            }      
        }else{
            if (c=='A'){
                fprintf(stderr, "Incorrect format. All entries beginning with 'A' must come before those beginning with 'B'.\n");
            }else{
                fprintf(stderr, "Invalid character %c. Characters must either be 'A' or 'B'.\n", c);
            }
            if (menuRoot != NULL){freeNodes(*nodeRoot); freeMenu(nodeRoot); freeMenu(&menuRoot);} /*freeMenuAndNodes(&menuRoot);*/
            break;
        }
        prevChar = c;
        }else{
            fprintf(stderr, "Error with formatting of data.\nNote that records must either be\n  -'A' followed by a 4 digit integer and a string\n  -'B' followed by 2 different 4 digit integers");
            if (menuRoot != NULL){freeNodes(*nodeRoot); freeMenu(nodeRoot); freeMenu(&menuRoot);} /*freeMenuAndNodes(&menuRoot);*/
            break;
        }
    }
    /*Frees the temporary list of nodes*/
    /*freeMenu(&nodeRoot);*/
    return menuRoot;
}

/******************************************************/
/* Function findNode
 *      Searches a linked list for a node with a given ID
 * Parameters:
 *      id - The ID of the node that si being searched for
 *      root - A pointer to the start of the linked list
 * Returns:
 *      If found pointer to the node is returned,
 *      otherwise a NULL pointer is returned. 
 ******************************************************/
NODE *findNode(unsigned short int id, MENU *root){
    NODE *p = NULL;
    while(root!=NULL){
        if (root->node->id == id){
            p = root->node;
            break;
        }
        root = root->next;
    }
    return p;
}
/******************************************************/
/* Function freeNode
 *      Frees the memory allocated to all nodes and their 
 *      description. The nodes are stored in a linked list
 *      of menu structures and each structure must point to
 *      a unique node.
 * Parameters:
 *      root - A pointer to the first menu structure in a linked list
 * Returns:
 *      (nothing)
 ******************************************************/
void freeNodes(MENU *root){
    while (root != NULL){
        MENU *t = root;
        root = root->next;
        free(t->node->desc);
        free(t->node);
        
    }
}