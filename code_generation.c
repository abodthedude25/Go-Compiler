#include "code_generation.h"


int allocateRegister() {
    // iterate through the array of registers to find an available one
    for (int i = 0; i < REGISTERS_AVAILABLE; i++) {
        if (registers[i] == 0) {
            registers[i] = 1;
            return i;
        }
    }
    // all registers are used
    printf("\tj too_complicated\n");
    printFileContents("../final/systemCall.s");
    exit(0);
    return -1;
}

void parameterRegisters( int num){
    for(int i = 0; i < num; i++){
        paramRegisters[i] = 1;
    }
    // paramRegisters[num] = 1;
}

void resetParamRegs(){
    for(int i = 0; i < 4; i++){
        paramRegisters[i] = 0;
    }
}

int pushParamRegs(){

    int count = 0;
    for(int i = 0; i < 4; i++){
        if(paramRegisters[i] == 1){
            // if(i == 3) count = 3;
            continue;
        }
        else{
            count = i;
            break;
        }
    }

    if(count > 0)
        printf("\tsubu $sp,$sp,%d\n", count*4);
    
    for(int i = 0; i < count; i++){
        if(paramRegisters[i] == 1){
            printf("\tsw $a%d,%d($sp)\n", i, i*4);

        }
    }
    return count;
}

void popParamRegs(int count){
    
    for(int i = 0; i < count; i++){
        printf("\tlw $a%d,%d($sp)\n", i, i*4);
    }
    if(count > 0)
        printf("\taddu $sp,$sp,%d\n", count*4);
}


void freeRegister(int reg) {
    // mark the register as available
    registers[reg] = 0;
}

void emitGlobal(ASTNode* tree){
    // 1. Emit global variable
    emit(".data");
    printf("%s: \n", tree->codeGenLabel);
    if(strToType(tree->children[1]->val.sval) == F_STRING){
        emit(".word S0");
    }
    else{
        emit(".word 0");
    }
    emit(".text");
}

void prologueFunction(ASTNode* tree){
    // 1. Push registers
    // 2. Save registers
    printf("\tsubu $sp,$sp,%d\n", tree->offset);
    emit("sw $fp,0($sp)");
    emit("move $fp,$sp");
    emit("sw $ra,4($fp)");
    int i;
    for(i = 0; i < tree->children[1]->children[1]->children_index; i++){
        printf("\tsw $a%d,%d($fp)\n", i, (i+2)*4);
    }
    // for(int x = 0; i < (tree->offset/4)-2; i++){
    //     printf("\tsw $0,%d($fp)\n", (i+x+2)*4);
    // }
    // 3. Allocate stack space
    // 4. Save frame pointer
    // 5. Set frame pointer
    // 6. Save return address
    // 7. Save arguments
    // 8. Save local variables
    // 9. Save return value
    // 10. Save return address
    // 11. Save arguments
    // 12. Save local variables
    // 13. Save return value
}

void epilogueFunction(ASTNode* tree){
    // 1. Restore registers
    emit("lw $ra,4($sp)");
    // for(int i = 0; i < tree->children[1]->children[1]->children_index; i++){
    //     printf("\tlw $a%d,%d($fp)\n", i, (i+2)*4);
    // }
    emit("lw $fp,0($sp)");
    printf("\taddu $sp,$sp,%d\n", tree->offset);
    emit("jr $ra");

    // 2. Restore frame pointer
    // 3. Restore return address
    // 4. Restore arguments
    // 5. Restore local variables
    // 6. Restore return value
    // 7. Restore return address
    // 8. Restore arguments
    // 9. Restore local variables
    // 10. Restore return value
    // 11. Restore registers
    // 12. Restore frame pointer
    // 13. Restore return address
    // 14. Restore arguments
    // 15. Restore local variables
    // 16. Restore return value
    // 17. Pop registers
    // 18. Return
}

int labelNum = 0;
char* getLabel(){
    // 1. Generate a unique label
    char* label = malloc(sizeof(char) * 10);
    sprintf(label, "L%d", labelNum);
    labelNum++;
    // 2. Return the label
    return label;
}

int stringLabelNum = 0;
char* getStringLabel(){
    // 1. Generate a unique label
    char* label = malloc(sizeof(char) * 10);
    sprintf(label, "S%d", stringLabelNum);
    stringLabelNum++;
    // 2. Return the label
    return label;
}

int globalLabelNum = 0;
char* getLabelGlobal(){
    // 1. Generate a unique label
    char* label = malloc(sizeof(char) * 10);
    sprintf(label, "G%d", globalLabelNum);
    globalLabelNum++;
    // 2. Return the label
    return label;
}

void emitLabel(char* label){
    // 1. Emit label
    printf("%s: \n", label);
}

void AssociateString(ASTNode* tree){
    // 1. Associate string
    char* label = getStringLabel();
    tree->codeGenLabel = label;
}

void emitString(char* string, ASTNode* tree){
    // 1. Emit string
    emit(".data");
    emitLabel(tree->codeGenLabel);
    for(int i  = 1; i < strlen(string)-1; i++){
        printf("\t.byte %d\n", string[i]);
    }
    printf("\t.byte 0\n");
    // printf("\t.asciiz %s \n", string);
}

void emitError(char* string, ASTNode* tree){
    // 1. Emit string
    char* label = getStringLabel();
    emit(".data");
    emitLabel(label);
    printf("\t.asciiz \"%s\" \n", string);
    tree->returnError = label;
}

void emit(char* instruction){
    // 1. Emit assembly code
    printf("\t%s\n", instruction);
}

// char* id2Reg(char* id){
//     // 1. Find the register that contains the value of id
//     // 2. Return the register name
// }

// this function recurses through our entire AST and associates each node with its symbol table entry
char* breakLabel;
ASTNode* returnNode;
int didReturn = 0;
char* s0 = NULL;
int unaryPolarity = 1;
void codeGen(ASTNode* tree) {
    //
    if (tree != NULL){
        switch (tree->node_type) {
            case Prog:
                codeGenConstFolding(tree);
                codeGenGlobal(tree);
                for(int i = 0; i < tree->children_index; i++){
                    codeGen(tree->children[i]);
                    if(tree->children[i]->reg != -1)
                        freeRegister(tree->children[i]->reg);
                }
                emit(".data");
                emitLabel(s0);
                printf("\t.byte 0\n");
                codeGenStrings(tree);
                return;
                break;
            case Stmt:
                switch (tree->kind.stmt) 
                {
                    case Block:
                        break;
                    case For:
                        {
                            char* prevbreak = breakLabel;
                            int returnFor = didReturn;
                            char* label1 = getLabel();
                            char* label2 = getLabel();
                            emitLabel(label1);
                            codeGen(tree->children[0]);
                            printf("\tbeqz $t%d, %s\n", tree->children[0]->reg, label2);
                            freeRegister(tree->children[0]->reg);
                            breakLabel = label2;
                            codeGen(tree->children[1]);
                            if(tree->children[1]->reg != -1) //quick fix
                                freeRegister(tree->children[1]->reg);
                            printf("\tj %s \n", label1);
                            emitLabel(label2);
                            breakLabel = prevbreak;
                            didReturn = returnFor;
                            return;
                        }
                        break;
                    case IfStmt:
                        {
                            int returnIf = didReturn;
                            codeGen(tree->children[0]);
                            char* label = getLabel();
                            printf("\tbeqz $t%d, %s\n", tree->children[0]->reg, label);
                            freeRegister(tree->children[0]->reg);
                            codeGen(tree->children[1]);
                            if(tree->children[1]->reg != -1) //quick fix
                                freeRegister(tree->children[1]->reg);
                            emitLabel(label);
                            didReturn = returnIf;
                            return;
                        }
                        break;
                    case IfElseStmt:
                        int returnIf = didReturn;
                        char* label1 = getLabel();
                        char* label2 = getLabel();

                        codeGen(tree->children[0]);
                        printf("\tbeqz $t%d, %s\n", tree->children[0]->reg, label1);
                        freeRegister(tree->children[0]->reg);
                        codeGen(tree->children[1]);
                        if(tree->children[1]->reg != -1) //quick fix
                            freeRegister(tree->children[1]->reg);
                        
                        printf("\tj %s \n", label2);
                        emitLabel(label1);
                        codeGen(tree->children[2]);
                        if(tree->children[2]->reg != -1) //quick fix
                            freeRegister(tree->children[2]->reg);
                        emitLabel(label2);
                        didReturn = returnIf;
                        return;
                    case Assn:
                        codeGen(tree->children[1]);
                         if(tree->children[0]->sym->isGlobal)
                            printf("\tsw $t%d, %s\n", tree->children[1]->reg, tree->children[0]->sym->codeGenLabel);
                        else
                            printf("\tsw $t%d, %d($fp)\n", tree->children[1]->reg, tree->children[0]->sym->offset);
                        freeRegister(tree->children[1]->reg);
                        return;
                    case BreakStmt:
                        printf("\tj %s\n", breakLabel);
                        break;
                    case ReturnStmt: 
                        didReturn = 0;
                        if(tree->children[0] != NULL){
                            codeGen(tree->children[0]);
                            printf("\tmove $v0, $t%d\n", tree->children[0]->reg);
                            freeRegister(tree->children[0]->reg);
                        }
                        printf("\tj %s \n", returnNode->codeGenLabelEpi);
                        // printf("\tj %s \n", tree->sym->codeGenLabel);
                        return;
                    case Empty:
                        break;
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
                        tree->reg = allocateRegister();
                        // if(tree->reg == -1) return;

                        if(tree->sym->isGlobal)
                            printf("\tlw $t%d, %s\n", tree->reg, tree->sym->codeGenLabel);
                        else if (strcmp(tree->val.sval, "true") == 0 && tree->type_name == F_BOOL)
                            printf("\tli $t%d, Ltrue\n", tree->reg);
                        else if (strcmp(tree->val.sval, "false") == 0 && tree->type_name == F_BOOL)
                            printf("\tli $t%d, Lfalse\n", tree->reg);
                        else
                            printf("\tlw $t%d, %d($fp)\n", tree->reg, tree->sym->offset);
                        break;
                    case TypeId:
                        break;
                    case BinaryExp:

                        if(tree->constFolded == 1){
                            tree->reg = allocateRegister();
                            // codeGen(tree->children[0]);
                            // freeRegister(tree->children[0]->reg);
                            // codeGen(tree->children[1]);
                            // freeRegister(tree->children[1]->reg);
                            printf("\tli $t%d, %d\n", tree->reg, tree->ival);
                            return;
                        }
                        if(tree->val.op == F_AND ){
                            char* label1 = getLabel();
                            codeGen(tree->children[0]);
                            freeRegister(tree->children[0]->reg);
                            tree->reg = allocateRegister();

                            printf("\tmove $t%d, $t%d\n", tree->reg, tree->children[0]->reg);
                            printf("\tbeqz $t%d, %s\n", tree->reg, label1);

                            codeGen(tree->children[1]);
                            printf("\tmove $t%d, $t%d\n", tree->reg, tree->children[1]->reg);
                            
                            freeRegister(tree->children[1]->reg);
                            emitLabel(label1);
                            return;
                        }
                        else if( tree->val.op == F_OR){
                            char* label2 = getLabel();

                            codeGen(tree->children[0]);
                            freeRegister(tree->children[0]->reg);
                            tree->reg = allocateRegister();

                            printf("\tmove $t%d, $t%d\n", tree->reg, tree->children[0]->reg);
                            
                            printf("\tbnez $t%d, %s\n", tree->reg, label2);
                            
                            codeGen(tree->children[1]);
                            if(tree->children[1]->reg != -1) //quick fix
                                freeRegister(tree->children[1]->reg);
                            printf("\tmove $t%d, $t%d\n", tree->reg, tree->children[1]->reg);
                            freeRegister(tree->children[1]->reg);
                            emitLabel(label2);
                            return;
                        }

                        codeGen(tree->children[0]);
                        codeGen(tree->children[1]);
                        freeRegister(tree->children[0]->reg);
                        freeRegister(tree->children[1]->reg);
                        tree->reg = allocateRegister();
                        // if(tree->reg == -1) return;
                        switch (tree->val.op) {
                            case F_ADD:
                                printf("\taddu $t%d, $t%d, $t%d\n", tree->reg, tree->children[0]->reg, tree->children[1]->reg);
                                break;
                            case F_SUB:
                                printf("\tsubu $t%d, $t%d, $t%d\n", tree->reg, tree->children[0]->reg, tree->children[1]->reg);
                                break;
                            case F_MUL:
                                printf("\tmul $t%d, $t%d, $t%d\n", tree->reg, tree->children[0]->reg, tree->children[1]->reg);
                                break;
                            case F_DIV:
                                printf("\tmove $a0, $t%d\n", tree->children[0]->reg);
                                printf("\tmove $a1, $t%d\n", tree->children[1]->reg);
                                printf("\tjal Ldivides\n");
                                printf("\tmove $t%d, $v0\n", tree->reg);
                                break;
                            case F_MODULO:
                                printf("\tmove $a0, $t%d\n", tree->children[0]->reg);
                                printf("\tmove $a1, $t%d\n", tree->children[1]->reg);
                                printf("\tjal Lmod\n");
                                printf("\tmove $t%d, $v1\n", tree->reg);
                                break;
                            case F_LT:
                                if(tree->children[0]->type_name == F_STRING || tree->children[1]->type_name == F_STRING){
                                    printf("\tmove $a0, $t%d\n", tree->children[0]->reg);
                                    printf("\tmove $a1, $t%d\n", tree->children[1]->reg);
                                    printf("\tjal Lstrcmp\n");
                                    printf("\tslt $t%d, $v0, $zero\n", tree->reg);
                                }
                                else{
                                    printf("\tslt $t%d, $t%d, $t%d\n", tree->reg, tree->children[0]->reg, tree->children[1]->reg);
                                }
                                break;
                            case F_GT:
                                if(tree->children[0]->type_name == F_STRING || tree->children[1]->type_name == F_STRING ){
                                    printf("\tmove $a0, $t%d\n", tree->children[0]->reg);
                                    printf("\tmove $a1, $t%d\n", tree->children[1]->reg);
                                    printf("\tjal Lstrcmp\n");
                                    printf("\tsgt $t%d, $v0, $zero\n", tree->reg);
                                }
                                else{
                                    printf("\tsgt $t%d, $t%d, $t%d\n", tree->reg, tree->children[0]->reg, tree->children[1]->reg);
                                }
                                break;
                            case F_LE:
                                if(tree->children[0]->type_name == F_STRING || tree->children[1]->type_name == F_STRING){
                                    printf("\tmove $a0, $t%d\n", tree->children[0]->reg);
                                    printf("\tmove $a1, $t%d\n", tree->children[1]->reg);
                                    printf("\tjal Lstrcmp\n");
                                    printf("\tsle $t%d, $v0, $zero\n", tree->reg);
                                }
                                else{
                                    printf("\tsle $t%d, $t%d, $t%d\n", tree->reg, tree->children[0]->reg, tree->children[1]->reg);
                                }
                                break;
                            case F_GE:
                                if(tree->children[0]->type_name == F_STRING || tree->children[1]->type_name == F_STRING){
                                    printf("\tmove $a0, $t%d\n", tree->children[0]->reg);
                                    printf("\tmove $a1, $t%d\n", tree->children[1]->reg);
                                    printf("\tjal Lstrcmp\n");
                                    printf("\tsge $t%d, $v0, $zero\n", tree->reg);
                                }
                                else{
                                    printf("\tsge $t%d, $t%d, $t%d\n", tree->reg, tree->children[0]->reg, tree->children[1]->reg);
                                }
                                break;
                            case F_EQUALITY:
                                if(tree->children[0]->type_name == F_STRING || tree->children[1]->type_name == F_STRING){
                                    printf("\tmove $a0, $t%d\n", tree->children[0]->reg);
                                    printf("\tmove $a1, $t%d\n", tree->children[1]->reg);
                                    printf("\tjal Lstrcmp\n");
                                    printf("\tseq $t%d, $v0, $zero\n", tree->reg);
                                    
                                }
                                else{
                                    printf("\tseq $t%d, $t%d, $t%d\n", tree->reg, tree->children[0]->reg, tree->children[1]->reg);
                                }
                                break;
                            case F_UNEQUAL:
                                if(tree->children[0]->type_name == F_STRING || tree->children[1]->type_name == F_STRING){
                                    printf("\tmove $a0, $t%d\n", tree->children[0]->reg);
                                    printf("\tmove $a1, $t%d\n", tree->children[1]->reg);
                                    printf("\tjal Lstrcmp\n");
                                    printf("\tsne $t%d, $v0, $zero\n", tree->reg);
                                }
                                else{
                                    printf("\tsne $t%d, $t%d, $t%d\n", tree->reg, tree->children[0]->reg, tree->children[1]->reg);
                                }
                                break;
                            default:
                                break;
                        }
                        
                        return;
                    case FuncCall:
                        int pushParam = 0;

                        if(tree->children[1] != NULL){
                            pushParam = pushParamRegs();
                            // printf("TEST------------------------%d\n", pushParam);
                            resetParamRegs();
                            for(int i = 0; i < tree->children[1]->children_index; i++){
                                parameterRegisters(i);
                                codeGen(tree->children[1]->children[i]);
                                printf("\tmove $a%d, $t%d\n", i, tree->children[1]->children[i]->reg);
                                freeRegister(tree->children[1]->children[i]->reg);
                            }
                        }
                        // returnNode = tree->children[0]->sym;

                        // push register values to stack before function call (if any) and pop them after function call (if any) 
                        // push $ra to stack
                        for(int i = 0; i < 8; i++){
                            if(registers[i] == 1){
                                printf("\tsubu $sp, $sp, 4\n");
                                printf("\tsw $t%d, 0($sp)\n", i);
                            }
                        }

                        printf("\tjal %s \n", tree->children[0]->sym->codeGenLabel);

                        //pull register values from stack after function call (if any) and pop them after function call (if any) 
                        for(int i = 7; i >= 0; i--){
                            if(registers[i] == 1){
                                printf("\tlw $t%d, 0($sp)\n", i);
                                printf("\taddu $sp, $sp, 4\n");
                            }
                        }

                        popParamRegs(pushParam);
                        if(strToType(tree->children[0]->sym->children[1]->children[0]->val.sval) != F_VOID && strToType(tree->children[0]->sym->children[1]->children[0]->val.sval) != F_ERROR){
                            tree->reg = allocateRegister();
                            // if(tree->reg == -1) return;

                            printf("\tmove $t%d, $v0\n", tree->reg);
                        }
                        return;
                    case BasicLit:
                        tree->reg = allocateRegister();
                        if(tree->type_name == F_STRING){
                            printf("\tla $t%d, %s\n", tree->reg, tree->codeGenLabel);
                        }
                        else{
                            printf("\tli $t%d, %s\n", tree->reg, tree->val.sval);
                        }
                        break;
                    case UnaryExp:
                        switch (tree->val.op) {
                            case F_SUB:
                                if(tree->children[0]->node_type == Expr && tree->children[0]->kind.exp == BasicLit){
                                    tree->reg = allocateRegister();

                                    if(strcmp(tree->children[0]->val.sval, "2147483648")){
                                        printf("\tli $t%d, -%s\n", tree->reg, tree->children[0]->val.sval);
                                        unaryPolarity = 0;
                                    }             
                                    else{
                                        printf("\tli $t%d, -%s\n", tree->reg, tree->children[0]->val.sval);
                                    }
                                    
                                    // unaryPolarity = 1;
                                }
                                else{
                                    codeGen(tree->children[0]);
                                    tree->reg = tree->children[0]->reg;
                                    if(unaryPolarity == 1){
                                        printf("\tneg $t%d, $t%d\n", tree->reg, tree->reg);
                                        // freeRegister(tree->children[0]->reg);
                                    }
                                }
                                return;
                                break;
                            case F_NOT:
                                codeGen(tree->children[0]);
                                tree->reg = tree->children[0]->reg;
                                printf("\txori $t%d, $t%d, 1\n", tree->reg, tree->reg);
                                freeRegister(tree->children[0]->reg);
                                break;
                            default:
                                break;
                        }
                        return;
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
                    case ParamDecl:
                        break;
                    case VarDecl:
                        if(tree->isGlobal == 0){
                            if(strToType(tree->children[1]->val.sval) == F_STRING){
                                printf("\tla $v1, S0 \t\n");
                                printf("\tsw $v1,%d($fp)\n", tree->sym->offset);
                            }
                            else{
                                printf("\tsw $0,%d($fp)\n", tree->sym->offset);
                            }
                        }
                        break;
                    case FuncDecl:
                        // tree->codeGenLabelEpi = getLabel();
                        resetParamRegs();
                        tree->codeGenLabelEpi = getLabel();
                        emitLabel(tree->codeGenLabel);
                        returnNode = tree;
                        // printf("\t%s", tree->codeGenLabelEpi);
                        // char* epiLabel = getLabel();
                        prologueFunction(tree);
                        codeGen(tree->children[2]);
                        // printf("\tj %s  #jump to epilogue   \n", epiLabel);
                        // emitLabel(epiLabel);
                        if(strToType(tree->children[1]->children[0]->val.sval) != F_VOID && strToType(tree->children[1]->children[0]->val.sval) != F_ERROR){
                            didReturn = 1;
                        }
                        else{
                            didReturn = 0;
                        }
                        if(didReturn == 1){
                            printf("\tla $a0, %s\n", tree->returnError);
                            printf("\tj error\n");
                        }
                        freeRegister(tree->children[2]->reg);
                        emitLabel(tree->codeGenLabelEpi);
                        //brute force fix for returning from main/////////////////////////////////////////////////
                        // if(strcmp(tree->children[0]->val.sval, "main") == 0)
                        // {
                        //     printf("\tli $v0, 10\n");
                        //     printf("\tsyscall\n");
                        // }
                        // else
                        // {
                        //     epilogueFunction(tree);
                        // }
                        //////////////////////////////////////////////////////////////
                        epilogueFunction(tree);
                        return;
                    case Params:
                    case Signature:
                    default:
                        break;
                }
                break;
            default:
                break;
        }
        //recursively runs through the tree
        for (int i = 0; i < tree->children_size; i++) {
            codeGen(tree->children[i]);
            if(tree->children[i] != NULL){
                freeRegister(tree->children[i]->reg);
            }
            
        }
    }
}

void codeGenGlobal(ASTNode* tree) {
    //runs through the tree and adds the global variables to the symbol table
    emit("Ltrue = 1");
    emit("Lfalse = 0");
    // emit(".align 2");
    s0 = getStringLabel();
    codeGenTraverse(tree);
    for (int i = 0; i < tree->children_size; i++) {
        ASTNode* child = tree->children[i];
        if (child != NULL)
        {
            switch (child->node_type) {
                case Decl:
                    switch (child->kind.decl) 
                    {
                        case VarDecl:
                            break;
                        case FuncDecl:
                            if(strcmp(child->children[0]->val.sval, "main") == 0){
                                //main function
                                // emit(".align 2");
                                emit(".text");
                                emit(".globl main");
                                emitLabel("main");
                                printf("\tjal %s\n", child->codeGenLabel);
                                // printf("\tli $a0, 10\n");
                                // printf("\tjal Lprintc\n");
                                emit("j Lhalt");
                                resetParamRegs();
                                
                                // emit(".data");
                                // emit(".text");
                                // codeGen(child);
                                returnNode = child;
                            }
                            else{
                                //global function
                                // emit(".text");
                                // codeGen(child);
                            }
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
    }
    codeGenGlobalVar(tree);
}

void codeGenStrings(ASTNode* tree){
    if (tree != NULL)
    {
        switch (tree->node_type) {
            case Expr:
                switch (tree->kind.exp) 
                {
                    case BasicLit:
                        if(tree->type_name == F_STRING){
                            //global string
                            emitString(tree->val.sval, tree);
                        }
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }

        //recursively runs through the tree
        for (int i = 0; i < tree->children_size; i++) {
            codeGenStrings(tree->children[i]);
        }
    
    }
}

void codeGenGlobalVar(ASTNode* tree){
    if(tree != NULL){
        switch (tree->node_type) {
            case Decl:
                switch (tree->kind.decl) 
                {
                    case VarDecl:
                        if(tree->isGlobal == 1){
                            if(tree->codeGenLabel != NULL)
                                emitGlobal(tree);
                            break;
                        }
                    default:
                        break;
                }
                break;
            default:
                break;
        }
        //recursively runs through the tree
        for (int i = 0; i < tree->children_size; i++) {
            codeGenGlobalVar(tree->children[i]);
        }
    }

}

int offset = 8;
int maxOffset = 0;
void codeGenTraverse(ASTNode* tree) {
    //runs through the tree and adds the global variables to the symbol table
    if (tree != NULL)
    {
        switch (tree->node_type) {
            case Expr:
                switch (tree->kind.exp) 
                {
                    case BasicLit:
                        if(tree->type_name == F_STRING){
                            //global string
                            if(tree->codeGenLabel == NULL)
                                AssociateString(tree);
                        }
                        break;
                    default:
                        break;
                }
                break;
            case Decl:
                switch (tree->kind.decl) 
                {   
                    case VarDecl:
                        if(tree->isGlobal == 1){
                            if(tree->codeGenLabel == NULL){
                                tree->codeGenLabel = getLabelGlobal();
                            }
                            break;
                        }
                        
                    case ParamDecl:
                        if(tree->isGlobal == 1 )
                            break;
                        tree->offset = offset;
                        offset += 4;
                        // printf("%s  offset: %d\n", tree->children[0]->val.sval, offset);
                        if( offset > maxOffset)
                            maxOffset = offset;
                        break;
                    case FuncDecl:
                        tree->codeGenLabel = getLabel(); 
                        if(strToType(tree->children[1]->children[0]->val.sval) != F_VOID && strToType(tree->children[1]->children[0]->val.sval) != F_ERROR){
                            char* error[100];
                            sprintf(error, "error: function '%s' must return a value\n", tree->children[0]->val.sval);
                            emitError(error, tree);
                        }
                        for (int i = 0; i < tree->children_size; i++) {
                            codeGenTraverse(tree->children[i]);
                        }
                        if(offset > maxOffset)
                            maxOffset = offset;
                        tree->offset = maxOffset;
                        // printf("%s  maxOffset: %d\n", tree->children[0]->val.sval, maxOffset+4);
                        maxOffset = 0;
                        offset = 8;
                        return;
                    default:
                        break;
                }
                break;
            default:
                break;
        }

        //recursively runs through the tree
        for (int i = 0; i < tree->children_size; i++) {
            codeGenTraverse(tree->children[i]);
        }
    }

}

void codeGenConstFolding(ASTNode* tree){
    if(tree != NULL){
        switch (tree->node_type) {
            case Expr:
                switch (tree->kind.exp) {
                    case BinaryExp:
                        if(tree->children[0]->node_type == Expr && tree->children[0]->kind.exp == BasicLit && tree->children[1]->node_type == Expr && tree->children[1]->kind.exp == BasicLit){
                            evalConst(atoi(tree->children[0]->val.sval), atoi(tree->children[1]->val.sval), tree->val.op, tree);
                        }
                        else if(tree->children[0]->node_type == Expr && tree->children[0]->kind.exp == BinaryExp &&  tree->children[1]->node_type == Expr && tree->children[1]->kind.exp == BasicLit){
                            codeGenConstFolding(tree->children[0]);
                            if(tree->children[0]->constFolded != 0)
                                evalConst(tree->children[0]->ival, atoi(tree->children[1]->val.sval), tree->val.op, tree);
                        }
                        else if(tree->children[0]->node_type == Expr && tree->children[0]->kind.exp == BasicLit && tree->children[1]->node_type == Expr && tree->children[1]->kind.exp == BinaryExp){
                            codeGenConstFolding(tree->children[1]);
                            if(tree->children[1]->constFolded != 0)
                                evalConst(atoi(tree->children[0]->val.sval), tree->children[1]->ival, tree->val.op, tree);
                        }
                        else if(tree->children[0]->node_type == Expr && tree->children[0]->kind.exp == BinaryExp && tree->children[1]->node_type == Expr && tree->children[1]->kind.exp == BinaryExp){
                            codeGenConstFolding(tree->children[0]);
                            codeGenConstFolding(tree->children[1]);
                            if(tree->children[0]->constFolded != 0 && tree->children[1]->constFolded != 0)
                                evalConst(tree->children[0]->ival, tree->children[1]->ival, tree->val.op, tree);
                        }
                        return;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
        //recursively runs through the tree
        for (int i = 0; i < tree->children_size; i++) {
            codeGenConstFolding(tree->children[i]);
        }
    }

}

void evalConst(int left, int right, Oper op, ASTNode* tree){
    tree->constFolded = 1;
    switch (op) {       
        case F_ADD:
            tree->ival = left + right;
            break;        
        case F_SUB:
            tree->ival = left - right;
            break;        
        case F_MUL:
            tree->ival = left * right;
            break;        
        case F_DIV:
            // tree->constFolded = 0;
            if(right != 0)
                tree->ival = left / right;
            else{
                tree->constFolded = 0;
            }
            break;        
        case F_MODULO:
            // tree->constFolded = 0;
            tree->ival = left % right;
            break;        
        case F_LT:
            tree->ival = left < right;
            break;        
        case F_GT:
            tree->ival = left > right;
            break;        
        case F_LE:
            tree->ival = left <= right;
            break;        
        case F_GE:
            tree->ival = left >= right;
            break;        
        case F_EQUALITY:
            tree->ival = left == right;
            break;        
        case F_UNEQUAL:
            tree->ival = left != right;
            break;        
        case F_AND:
            tree->ival = left && right;
            break;        
        case F_OR:
            tree->ival = left || right;
        default:
            tree->constFolded = 0;
            break;
    }
}

void printFileContents(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file != NULL) {
        int c;
        while ((c = fgetc(file)) != EOF) {
            putchar(c);
        }
        fclose(file);
    }
}