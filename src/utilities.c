#include <instrumentr/utilities.h>
#include <string.h>
#include <stdlib.h>

char* instrumentr_duplicate_string(const char* original) {
    int length = strlen(original);
    char* duplicate = (char*) malloc(length * sizeof(char));
    strcpy(duplicate, original);
    return duplicate;
}
