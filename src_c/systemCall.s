
    .align 2
    .text
no_value_returned:
    la  $a0, return_error_msg
    jal Lprints
    jal Lexit

    .data
return_error_msg:
    .asciiz "\nRuntime error: No value returned from function\n"

    .align 2
    .text
too_complicated:
    la  $a0, too_complicated_msg
    jal Lprints
    jal Lexit

    .data
too_complicated_msg:
    .asciiz "\nRuntime error: Too complicated\n"

   .align 2
    .text
Lgetchar:
    li $a0, 0           # read from stdin [loads the immediate value 0 to the register $a0]
    addi $sp, $sp, -4   # allocate a word on the stack [set $sp to $sp - 4]
    move $a1, $sp       # buffer to write to [copy $sp to $a1]
    li $a2, 1           # number of bytes to write
    li $v0 14
    syscall
    beq $v0, $zero, LgetcharEOF # reading `man read` tells us that if we read 0 bytes, we've reached
                               # EOF, so just return -1
                               # [if $v0 and $zero are the same, jump to the label `getcharEOF`]
    lbu $v0, 0($sp)     # load the byte from the address $sp + 0 [without sign extend] i.e., the char from the user

    addi $sp, $sp, 4    # deallocate the word on the stack
    jr $ra              # jump to the address stored in the register $ra

LgetcharEOF:
    li $v0, -1
    addi $sp, $sp, 4
    jr $ra

#####################################################################################
    .align 2
    .text
Lprints:
    move $s2, $a0
LprintsLoop:
    lb $s3, 0($s2)
    addu $s2, $s2, 1    # increment the pointer
    beq $s3, $zero, LprintsEnd
    beq $s3, 92, handle_special_char
    move $a0, $s3
    li $v0, 11 
    syscall
    j LprintsLoop

handle_special_char:
    lb $s3, 0($s2)       # load the next byte
    li $s4, 0           # initialize the special character code

    beq $s3, 92, handle_backslash   # check for \\
    beq $s3, 98, handle_backspace   # check for \b
    beq $s3, 116, handle_tab         # check for \t
    beq $s3, 110, handle_newline     # check for \n
    beq $s3, 102, handle_formfeed    # check for \f
    beq $s3, 114, handle_carriagbreak # check for \r
    beq $s3, 34, handle_doublequote # check for \"
    j LprintsLoop              # not a special character, continue looping

handle_backspace:
    li $s4, 8           # set the special character code to backspace
    j print_special_char

handle_tab:
    li $s4, 9           # set the special character code to tab
    j print_special_char

handle_newline:
    li $s4, 10          # set the special character code to newline
    j print_special_char

handle_formfeed:
    li $s4, 12          # set the special character code to form feed
    j print_special_char

handle_carriagbreak:
    li $s4, 13          # set the special character code to carriage return
    j print_special_char

handle_doublequote:
    li $s4, 34          # set the special character code to double quote
    j print_special_char

handle_backslash:
    li $s4, 92          # set the special character code to backslash
    j print_special_char

print_special_char:
    move $a0, $s4       # load the special character code
    li $v0, 11          # syscall to print a character
    syscall             # print the special character
    addu $s2, $s2, 1    # increment the pointer
    j LprintsLoop             # continue looping

LprintsEnd:
    jr $ra              # return from the routine

#####################################################################################


    .align 2
    .text
Lprinti:
    li $v0 1
    syscall
    jr $ra  # return to the caller

    .align 2
    .text
Lprintb:
    li $v0, 4        
    beqz $a0, print_false  
    la $a0, true_str    
    syscall            
    j print_end       
print_false:
    la $a0, false_str   
    syscall           
print_end:
    jr $ra            

    .data
true_str: 
    .asciiz "true"
    .data
false_str: 
    .asciiz "false"


    .align 2
    .text
Lprintc:
    #lb $s2, 0($a0)
    #la $s2, 0($a0)
    #li $s3, 127        
    blt $a0, 0, int_error 
    bgt $a0, 127, int_error  
    li $v0, 11        
    syscall            
    jr $ra            
int_error:
    li $v0, 4          
    la $a0, int_error_msg 
    syscall           
    jal Lexit  
           

    .data
int_error_msg:
    .asciiz "\nError: integer value outside the range of 7-bit ASCII.\n"

    .align 2
    .text
Llen:
    move $s2, $a0
    li $s3, 0
LlenLoop:
    lb $s4, 0($s2)
    beq $s4, $zero, LlenEnd
    addu $s2, $s2, 1
    beq $s4, 92, LlenLoop2
    addu $s3, $s3, 1
    j LlenLoop

LlenLoop2:
    lb $s4, 0($s2)
    beq $s4, $zero, LlenEnd
    addu $s2, $s2, 1
    beq $s4, 110, LlenLoop3
    beq $s4, 116, LlenLoop3
    beq $s4, 92, LlenLoop3
    addu $s3, $s3, 1
    j LlenLoop

LlenLoop3:
    addu $s3, $s3, 1
    j LlenLoop
LlenEnd:
    move $v0, $s3
    jr $ra

    .align 2
    .text
Lstrcmp:
    move $s2, $a0
    move $s3, $a1
    li $s4, 0
LstrcmpLoop:
    lb $s5, 0($s2)
    lb $s6, 0($s3)
    beq $s5, $zero, LstrcmpEnd1 
    beq $s6, $zero, LstrcmpEnd2
    bne $s5, $s6, LstrcmpEnd3
    addu $s2, $s2, 1
    addu $s3, $s3, 1
    j LstrcmpLoop
LstrcmpEnd1:
    beq $s6, $zero, LstrcmpEnd
    li $s4, -1
    j LstrcmpEnd
LstrcmpEnd2:
    li $s4, 1
    j LstrcmpEnd
LstrcmpEnd3:
    slt $s4, $s5, $s6
    bne $s4, $zero, LstrcmpEnd4
    li $s4, 1
    j LstrcmpEnd
LstrcmpEnd4:
    li $s4, -1
    j LstrcmpEnd
LstrcmpEnd:
    move $v0, $s4
    jr $ra

    .align 2
    .text
Lintcmp:
    move $s2, $a0
    move $s3, $a1
    slt $s4, $s2, $s3  
    bne $s4, $zero, less_than 
    slt $s4, $s3, $s2 
    bne $s4, $zero, greater_than 
    li $v0, 0 
    jr $ra 
less_than:
    li $v0, -1
    jr $ra 
greater_than:
    li $v0, 1 
    jr $ra 

    .align 2
    .text
Ldivides:
    addu $sp, $sp, -4 
    sw   $ra, 0($sp)
    move $s2, $a0
    move $s3, $a1
    beq  $s3, $zero, div_error
    li  $s4, -2147483648
    li  $s5, -1
    seq $s4, $s2, -2147483648
    seq $s5, $s3, -1
    and $s6, $s4, $s5
    li $s7, 1
    beq $s6, $s7, Ldivides3
    div  $s2, $s3
    mflo $v0
    mfhi $v1
    j    div_done
Ldivides3:
    li $v0, -2147483648
    j div_done
div_error:
    la  $a0, div_error_msg
    jal Lprints
    jal Lexit
div_done:
    lw   $ra, 0($sp)
    addu $sp, $sp, 4 
    jr   $ra 
    
.data
div_error_msg: .asciiz "error: division by zero\n"

    .align 2
    .text
Lmod:
    addu $sp, $sp, -4 
    sw   $ra, 0($sp)
    move $s2, $a0
    move $s3, $a1
    beq  $s3, $zero, mod_error
    seq $s4, $s2, -2147483648
    seq $s5, $s3, -1
    and $s6, $s4, $s5
    li $s7, 1
    beq $s6, $s7, Lmod1
    div  $s2, $s3
    mflo $v0
    mfhi $v1
    j    mod_done
Lmod1:
    li $v0, 0
    j mod_done
mod_error:
    la  $a0, mod_error_msg
    jal Lprints
    jal Lexit
    li $v0, 1
mod_done:
    lw   $ra, 0($sp)
    addu $sp, $sp, 4 
    jr   $ra 

    .data
mod_error_msg: .asciiz "\nRuntime error: Division by zero\n"


    .align 2
    .text
error:
    jal Lprints
    jal Lexit
    

    .align 2
    .text
Lhalt:
    li $v0 10
    syscall

    .align 2
    .text
Lexit:
    li $a0 1
    li $v0 17
    syscall

