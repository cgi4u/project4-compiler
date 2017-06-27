	.data
glob:	.space 28
_input:	.asciiz "Input : "
_output:	.asciiz "Output : "
_newline:	.asciiz "\n"
	.text
	.globl main
main:
	move	$fp, $sp
	addiu	$sp, $sp, -4
	la	$t0, -4($fp)
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	li	$t0, 10
	lw	$t1, 0($sp)
	addiu	$sp, $sp, 4
	sw	$t0, 0($t1)
	li	$t0, 2
	move	$t1, $t0
	li	$t2, 4
	mul	$t1, $t1, $t2
	la	$t0, glob
	add	$t0, $t0, $t1
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	la	$t0, -4($fp)
	lw	$t0, 0($t0)
	lw	$t1, 0($sp)
	addiu	$sp, $sp, 4
	sw	$t0, 0($t1)
	li	$t0, 1
	move	$t1, $t0
	li	$t2, 4
	mul	$t1, $t1, $t2
	la	$t0, glob
	add	$t0, $t0, $t1
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	li	$t0, 1
	lw	$t1, 0($sp)
	addiu	$sp, $sp, 4
	sw	$t0, 0($t1)
	li	$t0, 3
	move	$t1, $t0
	li	$t2, 4
	mul	$t1, $t1, $t2
	la	$t0, glob
	add	$t0, $t0, $t1
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	li	$t0, 3
	lw	$t1, 0($sp)
	addiu	$sp, $sp, 4
	sw	$t0, 0($t1)
	li	$t0, 2
	move	$t1, $t0
	li	$t2, 4
	mul	$t1, $t1, $t2
	la	$t0, glob
	add	$t0, $t0, $t1
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	la	$t0, -4($fp)
	lw	$t0, 0($t0)
	move	$t2, $t0
	lw	$t0, 0($sp)
	addiu	$sp, $sp, 4
	lw	$t0, 0($t0)
	move	$t1, $t0
	sub	$t0, $t1, $t2
	beqz	$t0, _SL0
	li	$t0, 3
	move	$t1, $t0
	li	$t2, 4
	mul	$t1, $t1, $t2
	la	$t0, glob
	add	$t0, $t0, $t1
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	li	$t0, 2
	move	$t1, $t0
	li	$t2, 4
	mul	$t1, $t1, $t2
	la	$t0, glob
	add	$t0, $t0, $t1
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	la	$t0, -4($fp)
	lw	$t0, 0($t0)
	move	$t2, $t0
	lw	$t0, 0($sp)
	addiu	$sp, $sp, 4
	lw	$t0, 0($t0)
	move	$t1, $t0
	add	$t0, $t1, $t2
	lw	$t1, 0($sp)
	addiu	$sp, $sp, 4
	sw	$t0, 0($t1)
	j	_SL0_ELSE
_SL0:
	li	$t0, 1
	move	$t1, $t0
	li	$t2, 4
	mul	$t1, $t1, $t2
	la	$t0, glob
	add	$t0, $t0, $t1
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	la	$t0, -4($fp)
	lw	$t0, 0($t0)
	lw	$t1, 0($sp)
	addiu	$sp, $sp, 4
	sw	$t0, 0($t1)
_SL0_ELSE:
_IL0:
	la	$t0, -4($fp)
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	li	$t0, 0
	move	$t2, $t0
	lw	$t0, 0($sp)
	addiu	$sp, $sp, 4
	lw	$t0, 0($t0)
	move	$t1, $t0
	li	$t0, 1
	bgt	$t1, $t2, _CL0
	li	$t0, 0
_CL0:
	beqz	$t0, _IL0_done
	la	$t0, -4($fp)
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	la	$t0, -4($fp)
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	li	$t0, 3
	move	$t2, $t0
	lw	$t0, 0($sp)
	addiu	$sp, $sp, 4
	lw	$t0, 0($t0)
	move	$t1, $t0
	sub	$t0, $t1, $t2
	lw	$t1, 0($sp)
	addiu	$sp, $sp, 4
	sw	$t0, 0($t1)
	j	_IL0
_IL0_done:
	li	$v0, 4
	la	$a0, _output
	syscall
	li	$t0, 3
	move	$t1, $t0
	li	$t2, 4
	mul	$t1, $t1, $t2
	la	$t0, glob
	add	$t0, $t0, $t1
	lw	$t0, 0($t0)
	li	$v0, 1
	move	$a0, $t0
	syscall
	li	$v0, 4
	la	$a0, _newline
	syscall
	li	$v0, 4
	la	$a0, _output
	syscall
	li	$t0, 1
	move	$t1, $t0
	li	$t2, 4
	mul	$t1, $t1, $t2
	la	$t0, glob
	add	$t0, $t0, $t1
	lw	$t0, 0($t0)
	li	$v0, 1
	move	$a0, $t0
	syscall
	li	$v0, 4
	la	$a0, _newline
	syscall
	li	$v0, 4
	la	$a0, _output
	syscall
	la	$t0, -4($fp)
	lw	$t0, 0($t0)
	li	$v0, 1
	move	$a0, $t0
	syscall
	li	$v0, 4
	la	$a0, _newline
	syscall
	li $v0, 10
	syscall
