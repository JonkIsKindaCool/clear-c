#include<stdio.h>
#include "lexer.h"

int main(int argc, char const *argv[])
{   
    struct TokenList tokens = tokenify("damn + 1243.24");
    printf("Tokenify done, final size %i\n", tokens.size);
    
    freeList(tokens);
    return 0;
}
