#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

enum Clear_TKind
{
    INT,
    FLOAT,
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
    DOT,       // .
    ARROW,     // ->

    TEOF
};

typedef struct Clear_Token
{
    enum Clear_TKind kind;
    char *value;
} Token;

const char *tokenKindToString(enum Clear_TKind kind)
{
    switch (kind)
    {
    case INT:
        return "INT";
    case FLOAT:
        return "FLOAT";
    case STRING:
        return "STRING";
    case IDENTIFIER:
        return "IDENTIFIER";
    case SUM:
        return "+";
    case SUBTRACT:
        return "-";
    case MULTIPLY:
        return "*";
    case DIVIDE:
        return "/";
    case MODULO:
        return "%";
    case ASSIGN:
        return "=";
    case ASSIGN_ADD:
        return "+=";
    case ASSIGN_SUB:
        return "-=";
    case ASSIGN_MUL:
        return "*=";
    case ASSIGN_DIV:
        return "/=";
    case ASSIGN_MOD:
        return "%=";
    case EQUAL:
        return "==";
    case NOT_EQUAL:
        return "!=";
    case LESS:
        return "<";
    case LESS_EQUAL:
        return "<=";
    case GREATER:
        return ">";
    case GREATER_EQUAL:
        return ">=";
    case LOGICAL_AND:
        return "&&";
    case LOGICAL_OR:
        return "||";
    case LOGICAL_NOT:
        return "!";
    case INCREMENT:
        return "++";
    case DECREMENT:
        return "--";
    case LPAREN:
        return "(";
    case RPAREN:
        return ")";
    case LBRACKET:
        return "[";
    case RBRACKET:
        return "]";
    case LBRACE:
        return "{";
    case RBRACE:
        return "}";
    case SEMICOLON:
        return ";";
    case COMMA:
        return ",";
    case COLON:
        return ":";
    case DOT:
        return ".";
    case ARROW:
        return "->";
    case TEOF:
        return "<EOF>";
    default:
        return "<unknown>";
    }
}

void printToken(struct Clear_Token *tok)
{
    if (!tok)
        return;

    const char *kindStr = tokenKindToString(tok->kind);

    if (tok->value)
    {
        switch (tok->kind)
        {
        case INT:
        case FLOAT:
        case STRING:
        case IDENTIFIER:
            printf("%-12s  %s\n", kindStr, tok->value);
            break;
        default:
            printf("%-12s  \"%s\"\n", kindStr, tok->value);
            break;
        }
    }
    else
    {
        printf("%-12s\n", kindStr);
    }
}

typedef struct Clear_TokenList
{
    Token **tokens;
    int size;
    int capacity;
} TokenList;

typedef struct Clear_Lexer
{
    int pos;
    const char *src;
    int len;
    TokenList list;
} Lexer;

void addToken(enum Clear_TKind kind, const char *value, struct Clear_Lexer *lex)
{
    if (lex->list.size >= lex->list.capacity)
    {
        int new_cap = lex->list.capacity == 0 ? 16 : lex->list.capacity * 2;
        Token **new_arr = realloc(lex->list.tokens, new_cap * sizeof(Token *));
        if (!new_arr)
        {
            fprintf(stderr, "realloc failed for tokens\n");
            exit(1);
        }
        lex->list.tokens = new_arr;
        lex->list.capacity = new_cap;
    }

    Token *tok = malloc(sizeof(Token));
    if (!tok)
    {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }

    tok->kind = kind;
    tok->value = value ? strdup(value) : NULL;

    printToken(tok);

    lex->list.tokens[lex->list.size++] = tok;
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
        fprintf(stderr, "realloc failed in addChar\n");
        exit(1);
    }

    new_str[len] = c;
    new_str[len + 1] = '\0';
    *str_ptr = new_str;
}

TokenList tokenify(const char *src)
{
    struct Clear_Lexer lex = {
        .pos = 0,
        .src = src,
        .len = (int)strlen(src),
        .list = {.tokens = NULL, .size = 0, .capacity = 0}};

    while (lex.pos < lex.len)
    {
        char c = lex.src[lex.pos];
        char next = (lex.pos + 1 < lex.len) ? lex.src[lex.pos + 1] : '\0';

        if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
        {
            lex.pos++;
            continue;
        }

        if (c == '#')
        {
            lex.pos += 2;
            while (lex.pos < lex.len && lex.src[lex.pos] != '\n')
                lex.pos++;
            continue;
        }

        if (c == '+' && next == '+')
        {
            addToken(INCREMENT, NULL, &lex);
            lex.pos += 2;
            continue;
        }
        if (c == '+' && next == '=')
        {
            addToken(ASSIGN_ADD, NULL, &lex);
            lex.pos += 2;
            continue;
        }
        if (c == '-' && next == '-')
        {
            addToken(DECREMENT, NULL, &lex);
            lex.pos += 2;
            continue;
        }
        if (c == '-' && next == '=')
        {
            addToken(ASSIGN_SUB, NULL, &lex);
            lex.pos += 2;
            continue;
        }
        if (c == '-' && next == '>')
        {
            addToken(ARROW, NULL, &lex);
            lex.pos += 2;
            continue;
        }
        if (c == '*' && next == '=')
        {
            addToken(ASSIGN_MUL, NULL, &lex);
            lex.pos += 2;
            continue;
        }
        if (c == '/' && next == '=')
        {
            addToken(ASSIGN_DIV, NULL, &lex);
            lex.pos += 2;
            continue;
        }
        if (c == '%' && next == '=')
        {
            addToken(ASSIGN_MOD, NULL, &lex);
            lex.pos += 2;
            continue;
        }
        if (c == '=' && next == '=')
        {
            addToken(EQUAL, NULL, &lex);
            lex.pos += 2;
            continue;
        }
        if (c == '!' && next == '=')
        {
            addToken(NOT_EQUAL, NULL, &lex);
            lex.pos += 2;
            continue;
        }

        switch (c)
        {
        case '+':
            addToken(SUM, NULL, &lex);
            lex.pos++;
            continue;
        case '-':
            addToken(SUBTRACT, NULL, &lex);
            lex.pos++;
            continue;
        case '*':
            addToken(MULTIPLY, NULL, &lex);
            lex.pos++;
            continue;
        case '/':
            addToken(DIVIDE, NULL, &lex);
            lex.pos++;
            continue;
        case '%':
            addToken(MODULO, NULL, &lex);
            lex.pos++;
            continue;
        case '=':
            addToken(ASSIGN, NULL, &lex);
            lex.pos++;
            continue;
        case '!':
            addToken(LOGICAL_NOT, NULL, &lex);
            lex.pos++;
            continue;
        case '<':
            addToken(LESS, NULL, &lex);
            lex.pos++;
            continue;
        case '>':
            addToken(GREATER, NULL, &lex);
            lex.pos++;
            continue;
        case '&':
            addToken(BIT_AND, NULL, &lex);
            lex.pos++;
            continue;
        case '|':
            addToken(BIT_OR, NULL, &lex);
            lex.pos++;
            continue;
        case '^':
            addToken(BIT_XOR, NULL, &lex);
            lex.pos++;
            continue;
        case '~':
            addToken(BIT_NOT, NULL, &lex);
            lex.pos++;
            continue;
        case '(':
            addToken(LPAREN, NULL, &lex);
            lex.pos++;
            continue;
        case ')':
            addToken(RPAREN, NULL, &lex);
            lex.pos++;
            continue;
        case '[':
            addToken(LBRACKET, NULL, &lex);
            lex.pos++;
            continue;
        case ']':
            addToken(RBRACKET, NULL, &lex);
            lex.pos++;
            continue;
        case '{':
            addToken(LBRACE, NULL, &lex);
            lex.pos++;
            continue;
        case '}':
            addToken(RBRACE, NULL, &lex);
            lex.pos++;
            continue;
        case ';':
            addToken(SEMICOLON, NULL, &lex);
            lex.pos++;
            continue;
        case ',':
            addToken(COMMA, NULL, &lex);
            lex.pos++;
            continue;
        case ':':
            addToken(COLON, NULL, &lex);
            lex.pos++;
            continue;
        case '.':
            addToken(DOT, NULL, &lex);
            lex.pos++;
            continue;
        }

        if (isdigit(c))
        {
            char *num = NULL;
            while (lex.pos < lex.len && isdigit(lex.src[lex.pos]))
                addChar(lex.src[lex.pos++], &num);
            addToken(INT, num, &lex);
            free(num);
            continue;
        }

        if (isalpha(c) || c == '_')
        {
            char *id = NULL;
            while (lex.pos < lex.len && (isalnum(lex.src[lex.pos]) || lex.src[lex.pos] == '_'))
                addChar(lex.src[lex.pos++], &id);
            addToken(IDENTIFIER, id, &lex);
            free(id);
            continue;
        }

        if (c == '"')
        {
            char *str = NULL;
            lex.pos++;

            while (lex.pos < lex.len && lex.src[lex.pos] != '"')
            {
                if (lex.src[lex.pos] == '\\' && lex.pos + 1 < lex.len)
                {
                    lex.pos++;
                    switch (lex.src[lex.pos])
                    {
                    case 'n':
                        addChar('\n', &str);
                        break;
                    case 't':
                        addChar('\t', &str);
                        break;
                    case '\\':
                        addChar('\\', &str);
                        break;
                    case '"':
                        addChar('"', &str);
                        break;
                    default:
                        addChar(lex.src[lex.pos], &str);
                        break;
                    }
                }
                else
                {
                    addChar(lex.src[lex.pos], &str);
                }
                lex.pos++;
            }

            if (lex.pos < lex.len && lex.src[lex.pos] == '"')
                lex.pos++;
            else
                fprintf(stderr, "Unclosed string in position %d\n", lex.pos);

            addToken(STRING, str ? str : "", &lex);
            free(str);
            continue;
        }
        fprintf(stderr, "Unexpected character '%c' (pos %d)\n", c, lex.pos);
        exit(1);
    }

    addToken(TEOF, NULL, &lex);
    return lex.list;
}

void freeTokenList(struct Clear_TokenList *list)
{
    if (!list)
        return;
    for (int i = 0; i < list->size; i++)
    {
        if (list->tokens[i])
        {
            free(list->tokens[i]->value);
            free(list->tokens[i]);
        }
    }
    free(list->tokens);
    list->tokens = NULL;
    list->size = 0;
    list->capacity = 0;
}