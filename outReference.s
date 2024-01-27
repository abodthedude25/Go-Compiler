	Ltrue = 1
	Lfalse = 0
	.text
	.globl main
main:
	jal L3
	j Lhalt
L0:
	subu $sp,$sp,4
	sw $ra,0($sp)
	la $t0,S1
	move $v0,$t0
	j L7
	la $a0,S2
	j error
L7:
	lw $ra,0($sp)
	addu $sp,$sp,4
	jr $ra
L1:
	subu $sp,$sp,16
	sw $ra,0($sp)
	sw $a0,4($sp)
	sw $a1,8($sp)
	sw $a2,12($sp)
	lw $t0,8($sp)
	move $a0,$t0
	jal Lprints
	la $t0,S3
	move $a0,$t0
	jal Lprints
	lw $t0,4($sp)
	move $a0,$t0
	jal Lprints
	la $t0,S4
	move $a0,$t0
	jal Lprints
	lw $t0,4($sp)
	move $a0,$t0
	jal Llen
	move $t1,$v0
	move $a0,$t1
	jal Lprinti
	la $t0,S5
	move $a0,$t0
	jal Lprints
	lw $t1,12($sp)
	move $a0,$t1
	jal Lprinti
	la $t0,S6
	move $a0,$t0
	jal Lprints
L8:
	lw $ra,0($sp)
	addu $sp,$sp,16
	jr $ra
L2:
	subu $sp,$sp,8
	sw $ra,0($sp)
	sw $a0,4($sp)
	lw $t0,4($sp)
	li $t1,1
	seq $t2,$t0,$t1
	beqz $t2,L10
	la $t3,S7
	move $v0,$t3
	j L9
	j L11
L10:
	lw $t0,4($sp)
	li $t1,2
	seq $t2,$t0,$t1
	beqz $t2,L12
	la $t3,S8
	move $v0,$t3
	j L9
	j L13
L12:
	lw $t0,4($sp)
	li $t1,3
	seq $t2,$t0,$t1
	beqz $t2,L14
	la $t3,S9
	move $v0,$t3
	j L9
	j L15
L14:
	lw $t0,4($sp)
	li $t1,4
	seq $t2,$t0,$t1
	beqz $t2,L16
	la $t3,S10
	move $v0,$t3
	j L9
	j L17
L16:
	lw $t0,4($sp)
	li $t1,5
	seq $t2,$t0,$t1
	beqz $t2,L18
	la $t3,S11
	move $v0,$t3
	j L9
	j L19
L18:
	lw $t0,4($sp)
	li $t1,6
	seq $t2,$t0,$t1
	beqz $t2,L20
	la $t3,S12
	move $v0,$t3
	j L9
	j L21
L20:
	lw $t0,4($sp)
	li $t1,7
	seq $t2,$t0,$t1
	beqz $t2,L22
	la $t3,S13
	move $v0,$t3
	j L9
	j L23
L22:
	lw $t0,4($sp)
	li $t1,8
	seq $t2,$t0,$t1
	beqz $t2,L24
	la $t3,S14
	move $v0,$t3
	j L9
	j L25
L24:
	lw $t0,4($sp)
	li $t1,9
	seq $t2,$t0,$t1
	beqz $t2,L26
	la $t3,S15
	move $v0,$t3
	j L9
	j L27
L26:
	la $t3,S16
	move $v0,$t3
	j L9
L27:
L25:
L23:
L21:
L19:
L17:
L15:
L13:
L11:
	la $a0,S17
	j error
L9:
	lw $ra,0($sp)
	addu $sp,$sp,8
	jr $ra
L3:
	subu $sp,$sp,16
	sw $ra,0($sp)
	la $v1,S0
	sw $v1,4($sp)
	la $v1,S0
	sw $v1,8($sp)
	la $t0,S0
	sw $t0,8($sp)
	lw $t0,4($sp)
	la $t1,S18
	li $t2,0
	move $a0,$t0
	move $a1,$t1
	move $a2,$t2
	jal L1
	la $t0,S0
	la $t1,S19
	li $t2,0
	move $a0,$t0
	move $a1,$t1
	move $a2,$t2
	jal L1
	lw $t0,8($sp)
	la $t1,S20
	li $t2,0
	move $a0,$t0
	move $a1,$t1
	move $a2,$t2
	jal L1
	la $t0,S21
	sw $t0,8($sp)
	lw $t0,8($sp)
	la $t1,S22
	li $t2,13
	move $a0,$t0
	move $a1,$t1
	move $a2,$t2
	jal L1
	la $v1,S0
	sw $v1,12($sp)
	la $t0,S23
	sw $t0,12($sp)
	lw $t0,12($sp)
	la $t1,S24
	li $t2,14
	move $a0,$t0
	move $a1,$t1
	move $a2,$t2
	jal L1
	la $t0,S25
	la $t1,S26
	li $t2,16
	move $a0,$t0
	move $a1,$t1
	move $a2,$t2
	jal L1
	jal L0
	move $t0,$v0
	li $t2,3
	subu $sp,$sp,4
	sw $t0,0($sp)
	move $a0,$t2
	jal L2
	lw $t0,0($sp)
	addu $sp,$sp,4
	move $t1,$v0
	move $a0,$t0
	move $a1,$t1
	jal L5
	jal L6
	move $t0,$v0
	la $t1,S27
	move $a0,$t0
	move $a1,$t1
	jal L5
	la $t0,S28
	la $t1,S29
	move $a0,$t0
	move $a1,$t1
	jal L4
L28:
	lw $ra,0($sp)
	addu $sp,$sp,16
	jr $ra
L4:
	subu $sp,$sp,16
	sw $ra,0($sp)
	sw $a0,4($sp)
	sw $a1,8($sp)
	la $v1,S0
	sw $v1,12($sp)
	la $t0,S0
	lw $t1,12($sp)
	seq $t2,$t0,$t1
	beqz $t2,L30
	la $t0,S30
	move $a0,$t0
	jal Lprints
	j L31
L30:
	la $t0,S31
	move $a0,$t0
	jal Lprints
L31:
	la $t0,S0
	la $t1,S0
	sne $t2,$t0,$t1
	beqz $t2,L32
	la $t0,S31
	move $a0,$t0
	jal Lprints
	j L33
L32:
	la $t0,S30
	move $a0,$t0
	jal Lprints
L33:
	jal L6
	move $t0,$v0
	la $t1,S32
	seq $t2,$t0,$t1
	beqz $t2,L34
	la $t0,S31
	move $a0,$t0
	jal Lprints
	j L35
L34:
	la $t0,S30
	move $a0,$t0
	jal Lprints
L35:
	la $t0,S33
	subu $sp,$sp,4
	sw $t0,0($sp)
	jal L6
	lw $t0,0($sp)
	addu $sp,$sp,4
	move $t1,$v0
	seq $t2,$t0,$t1
	beqz $t2,L36
	la $t0,S30
	move $a0,$t0
	jal Lprints
	j L37
L36:
	la $t0,S31
	move $a0,$t0
	jal Lprints
L37:
	lw $t0,8($sp)
	lw $t1,4($sp)
	slt $t2,$t0,$t1
	beqz $t2,L38
	la $t0,S30
	move $a0,$t0
	jal Lprints
	j L39
L38:
	la $t0,S31
	move $a0,$t0
	jal Lprints
L39:
	lw $t0,4($sp)
	lw $t1,8($sp)
	sgt $t2,$t0,$t1
	beqz $t2,L40
	la $t0,S30
	move $a0,$t0
	jal Lprints
	j L41
L40:
	la $t0,S31
	move $a0,$t0
	jal Lprints
L41:
	lw $t0,8($sp)
	la $t1,S34
	slt $t2,$t0,$t1
	beqz $t2,L42
	la $t0,S30
	move $a0,$t0
	jal Lprints
	j L43
L42:
	la $t0,S31
	move $a0,$t0
	jal Lprints
L43:
	lw $t0,8($sp)
	la $t1,S35
	sle $t2,$t0,$t1
	beqz $t2,L44
	la $t0,S30
	move $a0,$t0
	jal Lprints
	j L45
L44:
	la $t0,S31
	move $a0,$t0
	jal Lprints
L45:
	lw $t0,8($sp)
	lw $t1,8($sp)
	sle $t2,$t0,$t1
	beqz $t2,L46
	la $t0,S30
	move $a0,$t0
	jal Lprints
	j L47
L46:
	la $t0,S31
	move $a0,$t0
	jal Lprints
L47:
	lw $t0,4($sp)
	lw $t1,4($sp)
	sge $t2,$t0,$t1
	beqz $t2,L48
	la $t0,S30
	move $a0,$t0
	jal Lprints
	j L49
L48:
	la $t0,S31
	move $a0,$t0
	jal Lprints
L49:
	la $t0,S36
	lw $t1,4($sp)
	sge $t2,$t0,$t1
	beqz $t2,L50
	la $t0,S31
	move $a0,$t0
	jal Lprints
	j L51
L50:
	la $t0,S30
	move $a0,$t0
	jal Lprints
L51:
	la $t0,S37
	la $t1,S37
	sne $t2,$t0,$t1
	beqz $t2,L52
	la $t0,S31
	move $a0,$t0
	jal Lprints
	j L53
L52:
	la $t0,S30
	move $a0,$t0
	jal Lprints
L53:
L29:
	lw $ra,0($sp)
	addu $sp,$sp,16
	jr $ra
L5:
	subu $sp,$sp,12
	sw $ra,0($sp)
	sw $a0,4($sp)
	sw $a1,8($sp)
	lw $t0,4($sp)
	move $a0,$t0
	jal Lprints
	lw $t0,8($sp)
	move $a0,$t0
	jal Lprints
	la $t0,S6
	move $a0,$t0
	jal Lprints
L54:
	lw $ra,0($sp)
	addu $sp,$sp,12
	jr $ra
L6:
	subu $sp,$sp,4
	sw $ra,0($sp)
	la $t0,S33
	move $v0,$t0
	j L55
	la $a0,S38
	j error
L55:
	lw $ra,0($sp)
	addu $sp,$sp,4
	jr $ra
	.data
S0:
	.byte 0
S37:
	.byte 9
	.byte 0
S6:
	.byte 10
	.byte 0
S4:
	.byte 34
	.byte 41
	.byte 32
	.byte 61
	.byte 32
	.byte 0
S5:
	.byte 44
	.byte 32
	.byte 99
	.byte 111
	.byte 114
	.byte 114
	.byte 101
	.byte 99
	.byte 116
	.byte 32
	.byte 97
	.byte 110
	.byte 115
	.byte 119
	.byte 101
	.byte 114
	.byte 32
	.byte 105
	.byte 115
	.byte 32
	.byte 0
S27:
	.byte 44
	.byte 32
	.byte 119
	.byte 111
	.byte 114
	.byte 108
	.byte 100
	.byte 33
	.byte 0
S3:
	.byte 58
	.byte 32
	.byte 108
	.byte 101
	.byte 110
	.byte 40
	.byte 34
	.byte 0
S9:
	.byte 69
	.byte 97
	.byte 114
	.byte 116
	.byte 104
	.byte 0
S36:
	.byte 70
	.byte 79
	.byte 79
	.byte 0
S1:
	.byte 72
	.byte 0
S33:
	.byte 72
	.byte 101
	.byte 108
	.byte 108
	.byte 111
	.byte 0
S23:
	.byte 72
	.byte 101
	.byte 108
	.byte 108
	.byte 111
	.byte 44
	.byte 32
	.byte 119
	.byte 111
	.byte 114
	.byte 108
	.byte 100
	.byte 33
	.byte 10
	.byte 0
S11:
	.byte 74
	.byte 117
	.byte 112
	.byte 105
	.byte 116
	.byte 101
	.byte 114
	.byte 0
S10:
	.byte 77
	.byte 97
	.byte 114
	.byte 115
	.byte 0
S7:
	.byte 77
	.byte 101
	.byte 114
	.byte 99
	.byte 117
	.byte 114
	.byte 121
	.byte 0
S14:
	.byte 78
	.byte 101
	.byte 112
	.byte 116
	.byte 117
	.byte 110
	.byte 101
	.byte 0
S15:
	.byte 80
	.byte 108
	.byte 97
	.byte 116
	.byte 111
	.byte 0
S12:
	.byte 83
	.byte 97
	.byte 116
	.byte 117
	.byte 114
	.byte 110
	.byte 0
S13:
	.byte 85
	.byte 114
	.byte 97
	.byte 110
	.byte 117
	.byte 115
	.byte 0
S8:
	.byte 86
	.byte 101
	.byte 110
	.byte 117
	.byte 115
	.byte 0
S16:
	.byte 88
	.byte 0
S25:
	.byte 92
	.byte 34
	.byte 8
	.byte 13
	.byte 101
	.byte 115
	.byte 99
	.byte 97
	.byte 112
	.byte 101
	.byte 32
	.byte 116
	.byte 104
	.byte 9
	.byte 110
	.byte 40
	.byte 0
S29:
	.byte 98
	.byte 97
	.byte 114
	.byte 0
S35:
	.byte 98
	.byte 97
	.byte 114
	.byte 100
	.byte 0
S34:
	.byte 98
	.byte 97
	.byte 114
	.byte 102
	.byte 0
S21:
	.byte 98
	.byte 111
	.byte 114
	.byte 105
	.byte 110
	.byte 103
	.byte 32
	.byte 115
	.byte 116
	.byte 114
	.byte 105
	.byte 110
	.byte 103
	.byte 0
S30:
	.byte 99
	.byte 111
	.byte 114
	.byte 114
	.byte 101
	.byte 99
	.byte 116
	.byte 10
	.byte 0
S19:
	.byte 101
	.byte 109
	.byte 112
	.byte 116
	.byte 121
	.byte 32
	.byte 115
	.byte 116
	.byte 114
	.byte 105
	.byte 110
	.byte 103
	.byte 0
S38:
	.byte 101
	.byte 114
	.byte 114
	.byte 111
	.byte 114
	.byte 58
	.byte 32
	.byte 102
	.byte 117
	.byte 110
	.byte 99
	.byte 116
	.byte 105
	.byte 111
	.byte 110
	.byte 32
	.byte 39
	.byte 98
	.byte 121
	.byte 101
	.byte 39
	.byte 32
	.byte 109
	.byte 117
	.byte 115
	.byte 116
	.byte 32
	.byte 114
	.byte 101
	.byte 116
	.byte 117
	.byte 114
	.byte 110
	.byte 32
	.byte 97
	.byte 32
	.byte 118
	.byte 97
	.byte 108
	.byte 117
	.byte 101
	.byte 10
	.byte 0
S2:
	.byte 101
	.byte 114
	.byte 114
	.byte 111
	.byte 114
	.byte 58
	.byte 32
	.byte 102
	.byte 117
	.byte 110
	.byte 99
	.byte 116
	.byte 105
	.byte 111
	.byte 110
	.byte 32
	.byte 39
	.byte 104
	.byte 105
	.byte 39
	.byte 32
	.byte 109
	.byte 117
	.byte 115
	.byte 116
	.byte 32
	.byte 114
	.byte 101
	.byte 116
	.byte 117
	.byte 114
	.byte 110
	.byte 32
	.byte 97
	.byte 32
	.byte 118
	.byte 97
	.byte 108
	.byte 117
	.byte 101
	.byte 10
	.byte 0
S17:
	.byte 101
	.byte 114
	.byte 114
	.byte 111
	.byte 114
	.byte 58
	.byte 32
	.byte 102
	.byte 117
	.byte 110
	.byte 99
	.byte 116
	.byte 105
	.byte 111
	.byte 110
	.byte 32
	.byte 39
	.byte 112
	.byte 108
	.byte 97
	.byte 110
	.byte 101
	.byte 116
	.byte 39
	.byte 32
	.byte 109
	.byte 117
	.byte 115
	.byte 116
	.byte 32
	.byte 114
	.byte 101
	.byte 116
	.byte 117
	.byte 114
	.byte 110
	.byte 32
	.byte 97
	.byte 32
	.byte 118
	.byte 97
	.byte 108
	.byte 117
	.byte 101
	.byte 10
	.byte 0
S28:
	.byte 102
	.byte 111
	.byte 111
	.byte 0
S32:
	.byte 104
	.byte 101
	.byte 108
	.byte 108
	.byte 111
	.byte 0
S31:
	.byte 105
	.byte 110
	.byte 99
	.byte 111
	.byte 114
	.byte 114
	.byte 101
	.byte 99
	.byte 116
	.byte 10
	.byte 0
S26:
	.byte 115
	.byte 105
	.byte 108
	.byte 108
	.byte 121
	.byte 32
	.byte 115
	.byte 116
	.byte 114
	.byte 105
	.byte 110
	.byte 103
	.byte 0
S24:
	.byte 115
	.byte 116
	.byte 114
	.byte 105
	.byte 110
	.byte 103
	.byte 32
	.byte 119
	.byte 105
	.byte 116
	.byte 104
	.byte 32
	.byte 92
	.byte 92
	.byte 32
	.byte 0
S20:
	.byte 118
	.byte 97
	.byte 114
	.byte 32
	.byte 97
	.byte 115
	.byte 115
	.byte 105
	.byte 103
	.byte 110
	.byte 101
	.byte 100
	.byte 32
	.byte 101
	.byte 109
	.byte 112
	.byte 116
	.byte 121
	.byte 32
	.byte 115
	.byte 116
	.byte 114
	.byte 105
	.byte 110
	.byte 103
	.byte 0
S22:
	.byte 121
	.byte 97
	.byte 119
	.byte 110
	.byte 0
S18:
	.byte 122
	.byte 101
	.byte 114
	.byte 111
	.byte 32
	.byte 118
	.byte 97
	.byte 108
	.byte 117
	.byte 101
	.byte 0
	.align 2
	.text
