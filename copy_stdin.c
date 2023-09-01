#include "shell.h"

int copy_stdin(void) {
	FILE *file = fopen("output.txt", "w");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }

    char buffer[4096];
    
    printf("Entrez les données (Ctrl-D pour terminer) :\n");

    /* Lire chaque ligne depuis stdin et écrire dans le fichier */
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        fputs(buffer, file);
    }

    fclose(file); /* Fermer le fichier */

    return 0;
}