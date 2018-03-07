#ifndef MENUTREES_H
#define MENUTREES_H

#include <menuStruct.h>

/*Function prototypes*/
MENU *processFile(FILE **, MENU **);
NODE *findNode(unsigned short int, MENU *);
void freeNodes(MENU *);

#endif /* MENUTREES_H */

