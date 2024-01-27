#include<stdio.h>
#include "ast.h"

int funcDeclId = 0;

void printTree(ASTNode* tree, FILE* out,int INDENT) 
{
    if (tree != NULL)
    {
        // Print indentation
        fprintf(out, "%*s", INDENT*4, "");

        // Print label first
        fprintf(out, "%s", tree->label); 

        switch (tree->node_type) 
        {
            case Prog:
                fprintf(out, " sym: %p", tree->sym);
                break;
            case Stmt:
                switch (tree->kind.stmt) 
                {
                    case IfStmt:
                    case IfElseStmt:
                    case Assn:
                    case BreakStmt:
                    case ReturnStmt:
                        fprintf(out, " Line: %d", tree->line);
                        break;
                    case For:
                    case Block:
                    case Empty:
                    case ExprStmt:
                    case DeclStmt:
                    default:
                        break;
                }
                break;
            case Expr:
                switch (tree->kind.exp) 
                {
                    case Id:
                        fprintf(out, " [%s], sym: %p, Line: %d", tree->val.sval, tree->sym, tree->line);
                        break;
                    case TypeId:
                        fprintf(out, " [%s], sym: %p, Line: %d", tree->val.sval, tree->sym, tree->line);
                        break;
                    case BasicLit:
                        fprintf(out, " [%s], Line: %d", tree->val.sval, tree->line);
                        break;
                    case UnaryExp:
                    case BinaryExp:
                        fprintf(out, " %s, Line: %d, Type: %s", opToString(tree->val.op), tree->line, typeToStr(tree->type_name));
                        break;
                    case FuncCall:
                        fprintf(out, " %s, Line: %d", typeToStr(tree->children[0]->type_name), tree->line);
                        break;
                    case Actual:
                    case Actuals:
                    default:
                        break;
                }
                break;
            case Decl:
                switch (tree->kind.decl) 
                {   
                    case idNew:
                        if(funcDeclId)
                            fprintf(out, " [%s], sym: %p, Line: %d", tree->val.sval, tree->sym, tree->line);
                        else
                            fprintf(out, " [%s], Line: %d", tree->val.sval, tree->line);
                        break;
                    case FuncDecl:
                        funcDeclId = 1;
                        fprintf(out, " Line: %d, sym: %p", tree->line, tree->sym);
                        fprintf(out, "\n");
                        for(int i = 0; i < tree->children_size; i++)
                            printTree(tree->children[i], out, INDENT+1);
                        funcDeclId = 0;
                        return;
                    case VarDecl:
                        fprintf(out, " Line: %d, sym: %p", tree->line, tree->sym);
                        break;
                    case ParamDecl:
                    case Params:
                    case Signature:
                    default:
                        break;
                }
                break;
            default:
                break;
        }

        fprintf(out, "\n");
        
        INDENT++;
        for (int i = 0; i < tree->children_size; i++) 
            printTree(tree->children[i], out, INDENT);
        INDENT--;

    }
}
