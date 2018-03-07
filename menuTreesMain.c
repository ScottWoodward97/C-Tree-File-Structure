#include <menuTrees.h>

int main(int argc, char** argv) {
    
   FILE *fp;
   MENU *root; 
   MENU *nodes = NULL;
   
   fprintf(stderr, "Parameter listing:\n");
   fprintf(stderr, "1: input file name: %s\n", argv[1]);
   
   fp = fopen(argv[1], "r");
    if (fp==NULL){
        fprintf(stderr, "Error opening file %s\n", argv[1]);
        exit(1);
    }

    root = processFile(&fp, &nodes);
    fclose(fp);
    
    printMenu(root, NULL);
    fflush(stdout);
    
    if (root!=NULL){
        freeNodes(nodes);
        freeMenu(&nodes);
        freeMenu(&root);
    }
    
    return (EXIT_SUCCESS);
}