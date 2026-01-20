#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

enum TKind
{
    NUMBER,
    STRING,

    IDENTIFIER,

    SUM,      // +
    SUBTRACT, // -
    MULTIPLY, // *
    DIVIDE,   // /
    MODULO,   // %

    ASSIGN,     // =
    ASSIGN_ADD, // +=
    ASSIGN_SUB, // -=
    ASSIGN_MUL, // *=
    ASSIGN_DIV, // /=
    ASSIGN_MOD, // %=

    EQUAL,         // ==
    NOT_EQUAL,     // !=
    LESS,          // <
    LESS_EQUAL,    // <=
    GREATER,       // >
    GREATER_EQUAL, // >=

    LOGICAL_AND, // &&
    LOGICAL_OR,  // ||
    LOGICAL_NOT, // !

    BIT_AND,     // &
    BIT_OR,      // |
    BIT_XOR,     // ^
    BIT_NOT,     // ~
    LEFT_SHIFT,  // <<
    RIGHT_SHIFT, // >>

    INCREMENT, // ++
    DECREMENT, // --

    LPAREN,    // (
    RPAREN,    // )
    LBRACKET,  // [
    RBRACKET,  // ]
    LBRACE,    // {
    RBRACE,    // }
    SEMICOLON, // ;
    COMMA,     // ,
    COLON,     // :
    DOT,
    ARROW,

    TEOF
};

struct Token
{
    enum TKind kind;
    char *value;
} Token;

struct TokenList
{
    struct Token **tokens;
    int size;
} TokenList;

struct Lexer
{
    int pos;
    char *src;
    int len;
    struct TokenList list;
} Lexer;

void addToken(enum TKind kind, const char *value, struct TokenList *list)
{
    struct Token *tok = malloc(sizeof(struct Token));
    if (!tok)
    {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }

    tok->kind = kind;
    tok->value = value ? strdup(value) : NULL;

    struct Token **new_tokens = realloc(list->tokens, (list->size + 1) * sizeof(struct Token *));
    if (!new_tokens)
    {
        fprintf(stderr, "realloc failed\n");
        free(tok->value);
        free(tok);
        exit(1);
    }

    printf("Added Token, Kind: %i, Value: %s\n", kind, value);

    list->tokens = new_tokens;
    list->tokens[list->size] = tok;
    list->size++;
}

void addChar(char c, char **str_ptr)
{
    if (!str_ptr)
        return;

    char *str = *str_ptr;
    size_t len = str ? strlen(str) : 0;

    char *new_str = realloc(str, len + 2);
    if (!new_str)
    {
        fprintf(stderr, "realloc failed\n");
        exit(1);
    }

    new_str[len] = c;
    new_str[len + 1] = '\0';

    *str_ptr = new_str;
}
struct TokenList initTokenList(void)
{
    struct TokenList list = {0};
    return list;
}

struct TokenList tokenify(char *src)
{
    struct Lexer lex = {
        .pos = 0,
        .src = src,
        .list = initTokenList(),
        .len = strlen(lex.src)};

    while (lex.pos < lex.len)
    {
        char character = lex.src[lex.pos];

        switch (character)
        {
        case '+':
            addToken(SUM, NULL, &lex.list);
            lex.pos++;
            break;

        case '-':
            addToken(SUBTRACT, NULL, &lex.list);
            lex.pos++;
            break;
        case '*':
            addToken(MULTIPLY, NULL, &lex.list);
            lex.pos++;
            break;
        case '/':
            addToken(DIVIDE, NULL, &lex.list);
            lex.pos++;
            break;
        case '.':
            addToken(DOT, NULL, &lex.list);
            lex.pos++;
            break;

        case '(':
            addToken(LPAREN, NULL, &lex.list);
            lex.pos++;
            break;

        case ')':
            addToken(RPAREN, NULL, &lex.list);
            lex.pos++;
            break;

        case '[':
            addToken(LBRACKET, NULL, &lex.list);
            lex.pos++;
            break;

        case ']':
            addToken(RBRACKET, NULL, &lex.list);
            lex.pos++;
            break;

        case '{':
            addToken(LBRACE, NULL, &lex.list);
            lex.pos++;
            break;

        case '}':
            addToken(RBRACE, NULL, &lex.list);
            lex.pos++;
            break;

        case ':':
            addToken(COLON, NULL, &lex.list);
            lex.pos++;
            break;

        case ';':
            addToken(SEMICOLON, NULL, &lex.list);
            lex.pos++;
            break;

        case ',':
            addToken(COMMA, NULL, &lex.list);
            lex.pos++;
            break;

        case '\n':
        case '\r':
        case '\t':
        case ' ':
            lex.pos++;
            break;

        default:
            if (isdigit(character))
            {
                char *num = malloc(0);
                while (lex.pos < lex.len && isdigit(lex.src[lex.pos]))
                {
                    addChar(lex.src[lex.pos++], &num);
                }
                addToken(NUMBER, num, &lex.list);
            }
            else if (isalnum(character))
            {
                char *result = malloc(0);
                while (lex.pos < lex.len && isalnum(lex.src[lex.pos]))
                {
                    addChar(lex.src[lex.pos++], &result);
                }

                addToken(IDENTIFIER, result, &lex.list);
            }
            else
            {
                printf("Unexpected token at %i (%c)\n", lex.pos, character);
                exit(1);
            }

            break;
        }
    }

    return lex.list;
}

void freeList(struct TokenList list)
{
    free(list.tokens);
}
