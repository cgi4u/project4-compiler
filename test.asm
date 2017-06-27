	.data
glob:	.space 28
input:	.asciiz "Input Integer:"
output:	.asciiz "Output:"
	.text
	.globl main
main:
	move	$fp, $sp
	addiu	$sp, $sp, -44
	la	$t0, -4($fp)
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	la	$t0, -8($fp)
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	lw	$t0, 0($sp)
	addiu	$sp, $sp, 4
	move	$t1, t0
	lw	$t0, 0($t1)
	move	$t2, $t0
	lw	$t0, 0($sp)
	addiu	$sp, $sp, 4
	move	$t1, t0
	lw	$t0, 0($t1)
	move	$t1, $t0
	add	$t0, $t1, $t2
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	lw	$t0, 0($sp)
	addiu	$sp, $sp, 4
	beqz	$t0, SL0
	la	$t0, -4($fp)
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	la	$t0, -4($fp)
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	la	$t0, -8($fp)
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	lw	$t0, 0($sp)
	addiu	$sp, $sp, 4
	move	$t1, t0
	lw	$t0, 0($t1)
	move	$t2, $t0
	lw	$t0, 0($sp)
	addiu	$sp, $sp, 4
	move	$t1, t0
	lw	$t0, 0($t1)
	move	$t1, $t0
	add	$t0, $t1, $t2
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	lw	$t0, 0($sp)
	addiu	$sp, $sp, 4
	move	$t1, $t0
	lw	$t0, 0($sp)
	addiu	$sp, $sp, 4
	sw	$t1, 0($t0)
SL0:
	li	$t0, 1
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	lw	$t0, 0($sp)
	addiu	$sp, $sp, 4
	move	$t1, $t0
	li	$t2, 4
	mul	$t1, $t1, $t2
	la	$t0, -44($fp)
	add	$t0, $t0, $t1
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	la	$t0, -4($fp)
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	lw	$t0, 0($sp)
	addiu	$sp, $sp, 4
	move	$t1, t0
	lw	$t0, 0($t1)
	move	$t1, $t0
	lw	$t0, 0($sp)
	addiu	$sp, $sp, 4
	sw	$t1, 0($t0)
	addiu	$sp, $sp, -4
	sw	$t1, 0($sp)
	li	$v0, 4
	la	$a0, input
	syscall
	li	$v0, 4
	la	$a0, output
	syscall
IL0:
	li	$t0, 1
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	lw	$t0, 0($sp)
	addiu	$sp, $sp, 4
	move	$t1, $t0
	li	$t2, 4
	mul	$t1, $t1, $t2
	la	$t0, -44($fp)
	add	$t0, $t0, $t1
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	lw	$t0, 0($sp)
	addiu	$sp, $sp, 4
	move	$t1, t0
	lw	$t0, 0($t1)
	beqz	$t0, IL0_done
	li	$t0, 1
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	lw	$t0, 0($sp)
	addiu	$sp, $sp, 4
	move	$t1, $t0
	li	$t2, 4
	mul	$t1, $t1, $t2
	la	$t0, -44($fp)
	add	$t0, $t0, $t1
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	li	$t0, 1
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	lw	$t0, 0($sp)
	addiu	$sp, $sp, 4
	move	$t1, $t0
	li	$t2, 4
	mul	$t1, $t1, $t2
	la	$t0, -44($fp)
	add	$t0, $t0, $t1
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	li	$t0, 1
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	lw	$t0, 0($sp)
	addiu	$sp, $sp, 4
	move	$t2, $t0
	lw	$t0, 0($sp)
	addiu	$sp, $sp, 4
	move	$t1, t0
	lw	$t0, 0($t1)
	move	$t1, $t0
	sub	$t0, $t1, $t2
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	lw	$t0, 0($sp)
	addiu	$sp, $sp, 4
	move	$t1, $t0
	lw	$t0, 0($sp)
	addiu	$sp, $sp, 4
	sw	$t1, 0($t0)
	j	IL0
IL0_done:
	li $v0, 10
	syscall
