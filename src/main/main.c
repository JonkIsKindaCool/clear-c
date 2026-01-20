#include<stdio.h>
#include "clear_lexer.h"

int main(int argc, char const *argv[])
{   
    struct Clear_TokenList tokens = tokenify("var damn = 2; damn++;");
    printf("Tokenify done, final size %i\n", tokens.size);
    
    freeTokenList(&tokens);
    return 0;
}
