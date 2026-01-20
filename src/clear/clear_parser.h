#pragma once

enum Clear_ExprKind {
    Int_Literal,
    Float_Literal,
    String_Literal,
    Identifier_Literal,
    
    Binary_Op
};

struct Clear_Expr
{
    enum Clear_ExprKind kind;
    union
    {
        struct {
            int value;
        } Int;

        struct 
        {
            float value;
        } Float;

        struct
        {
            char * value;
        } Identifier;
        
        struct
        {
            char *op;
            struct Clear_Expr *left;
            struct Clear_Expr *right;
        } BinaryOp;
        
        
    } data;
    
};