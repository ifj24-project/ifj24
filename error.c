#include <stdio.h>
#include <stdlib.h>

void ThrowError(int errorCode){
    fprintf(stderr, "\033[0;31m");
    fprintf(stderr, "%s", "[ERROR]");
    fprintf(stderr, "\033[0m");

    switch (errorCode) {
        case 1: //Lexikalni chyba
            fprintf(stderr, "Chyba v programu v rámci lexikální analýzy.\n");
            break;
        
        case 2: //Syntaticka chyba
            fprintf(stderr, "Chyba v programu v rámci syntaktické analýzy.\n");
            break;
        
        case 3: //Semanticka chyba
            fprintf(stderr, "Sémantická chyba v programu - nedefinovaná funkce či proměnná.\n");
            break;
        
        case 4: // Semanticka chyba
            fprintf(stderr, "Sémantická chyba v programu– špatný počet/typ parametrů u volání funkce; špatný typ či nepovolené zahození návratové hodnoty z funkce.\n");
            break;

        case 5: // Semanticka chyba
            fprintf(stderr, "Sémantická chyba v programu– redefinice proměnné nebo funkce; přiřazení do nemodifikovatelné proměnné.\n");
            break;
            
        case 6: // Semanticka chyba
            fprintf(stderr, "Sémantická chyba v programu– chybějící/přebývající výraz v příkazu návratu z funkce.\n");
            break;

        case 7: // Semanticka chyba
            fprintf(stderr, "Sémantická chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech; nekompatibilní typ výrazu.\n");
            break;

        case 8: // Semanticka chyba
            fprintf(stderr, "Sémantická chyba odvození typu– typ proměnné není uveden a nelze odvodit od použitého výrazu.\n");
            break;
        
        case 9: // Semanticka chyba
            fprintf(stderr, "Sémantická chyba nevyužité proměnné v jejím rozsahu platnosti; modifikovatelná proměnná bez možnosti změny po její inicializaci.\n");
            break;
        case 10: // Semanticka chyba
            fprintf(stderr, "Ostatní sémantické chyby.\n");
            break;

        case 99: // INTERNAL ERROR
            fprintf(stderr, "Interní chyba překladače tj. neovlivněná vstupním programem (např. chyba alokace paměti atd.).\n");
            break;
    }

    //printf("Error Code: %d\n", errorCode);
    exit(errorCode);
}

