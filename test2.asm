	.data
save:	.space 4004
_input:	.asciiz "Input : "
_output:	.asciiz "Output : "
_newline:	.asciiz "\n"
	.text
	.globl main
fibonacci:
	sw	$ra, -4($sp)
	sw	$fp, -8($sp)
	move	$fp, $sp
	addiu	$sp, $sp, -8
	la	$t0, 0($fp)
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	li	$t0, 0
	move	$t2, $t0
	lw	$t0, 0($sp)
	addiu	$sp, $sp, 4
	lw	$t0, 0($t0)
	move	$t1, $t0
	li	$t0, 1
	ble	$t1, $t2, _CL0
	li	$t0, 0
_CL0:
	beqz	$t0, _SL0
	li	$t0, 0
	move	$sp, $fp
	lw	$ra, -4($fp)
	lw	$fp, -8($fp)
	jr	$ra
	j	_SL0_ELSE
_SL0:
	la	$t0, 0($fp)
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	li	$t0, 2
	move	$t2, $t0
	lw	$t0, 0($sp)
	addiu	$sp, $sp, 4
	lw	$t0, 0($t0)
	move	$t1, $t0
	li	$t0, 1
	ble	$t1, $t2, _CL1
	li	$t0, 0
_CL1:
	beqz	$t0, _SL1
	li	$t0, 1
	move	$sp, $fp
	lw	$ra, -4($fp)
	lw	$fp, -8($fp)
	jr	$ra
_SL1:
_SL0_ELSE:
	addiu	$sp, $sp, -4
	la	$t0, -12($fp)
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	la	$t0, 0($fp)
	lw	$t0, 0($t0)
	lw	$t1, 0($sp)
	addiu	$sp, $sp, 4
	sw	$t0, 0($t1)
	addiu	$sp, $sp, -40
	li	$t0, 5
	move	$t1, $t0
	li	$t2, 4
	mul	$t1, $t1, $t2
	la	$t0, -52($fp)
	add	$t0, $t0, $t1
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	li	$t0, 0
	lw	$t1, 0($sp)
	addiu	$sp, $sp, 4
	sw	$t0, 0($t1)
	la	$t0, -12($fp)
	lw	$t0, 0($t0)
	move	$t1, $t0
	li	$t2, 4
	mul	$t1, $t1, $t2
	la	$t0, save
	add	$t0, $t0, $t1
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	li	$t0, 5
	move	$t1, $t0
	li	$t2, 4
	mul	$t1, $t1, $t2
	la	$t0, -52($fp)
	add	$t0, $t0, $t1
	lw	$t0, 0($t0)
	move	$t2, $t0
	lw	$t0, 0($sp)
	addiu	$sp, $sp, 4
	lw	$t0, 0($t0)
	move	$t1, $t0
	li	$t0, 1
	bne	$t1, $t2, _CL2
	li	$t0, 0
_CL2:
	beqz	$t0, _SL2
	la	$t0, -12($fp)
	lw	$t0, 0($t0)
	move	$t1, $t0
	li	$t2, 4
	mul	$t1, $t1, $t2
	la	$t0, save
	add	$t0, $t0, $t1
	lw	$t0, 0($t0)
	move	$sp, $fp
	lw	$ra, -4($fp)
	lw	$fp, -8($fp)
	jr	$ra
_SL2:
	addiu	$sp, $sp, -4
	la	$t0, -56($fp)
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	addiu	$sp, $sp, -4
	la	$t0, -12($fp)
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	li	$t0, 1
	move	$t2, $t0
	lw	$t0, 0($sp)
	addiu	$sp, $sp, 4
	lw	$t0, 0($t0)
	move	$t1, $t0
	sub	$t0, $t1, $t2
	sw	$t0, 0($sp)
	jal	fibonacci
	addiu	$sp, $sp, 4
	lw	$t1, 0($sp)
	addiu	$sp, $sp, 4
	sw	$t0, 0($t1)
	addiu	$sp, $sp, -4
	la	$t0, -60($fp)
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	addiu	$sp, $sp, -4
	la	$t0, -12($fp)
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	li	$t0, 2
	move	$t2, $t0
	lw	$t0, 0($sp)
	addiu	$sp, $sp, 4
	lw	$t0, 0($t0)
	move	$t1, $t0
	sub	$t0, $t1, $t2
	sw	$t0, 0($sp)
	jal	fibonacci
	addiu	$sp, $sp, 4
	lw	$t1, 0($sp)
	addiu	$sp, $sp, 4
	sw	$t0, 0($t1)
	la	$t0, 0($fp)
	lw	$t0, 0($t0)
	move	$t1, $t0
	li	$t2, 4
	mul	$t1, $t1, $t2
	la	$t0, save
	add	$t0, $t0, $t1
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	la	$t0, -56($fp)
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	la	$t0, -60($fp)
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
	addiu	$sp, $sp, -1600
	li	$t0, 333
	move	$t1, $t0
	li	$t2, 4
	mul	$t1, $t1, $t2
	la	$t0, -1660($fp)
	add	$t0, $t0, $t1
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	la	$t0, 0($fp)
	lw	$t0, 0($t0)
	move	$t1, $t0
	li	$t2, 4
	mul	$t1, $t1, $t2
	la	$t0, save
	add	$t0, $t0, $t1
	lw	$t0, 0($t0)
	lw	$t1, 0($sp)
	addiu	$sp, $sp, 4
	sw	$t0, 0($t1)
	li	$t0, 333
	move	$t1, $t0
	li	$t2, 4
	mul	$t1, $t1, $t2
	la	$t0, -1660($fp)
	add	$t0, $t0, $t1
	lw	$t0, 0($t0)
	move	$sp, $fp
	lw	$ra, -4($fp)
	lw	$fp, -8($fp)
	jr	$ra
main:
	move	$fp, $sp
	addiu	$sp, $sp, -4
	la	$t0, -4($fp)
	addiu	$sp, $sp, -4
	sw	$t0, 0($sp)
	li	$v0, 4
	la	$a0, _input
	syscall
	li	$v0, 5
	syscall
	move	$t0, $v0
	lw	$t1, 0($sp)
	addiu	$sp, $sp, 4
	sw	$t0, 0($t1)
	li	$v0, 4
	la	$a0, _output
	syscall
	addiu	$sp, $sp, -4
	la	$t0, -4($fp)
	lw	$t0, 0($t0)
	sw	$t0, 0($sp)
	jal	fibonacci
	addiu	$sp, $sp, 4
	li	$v0, 1
	move	$a0, $t0
	syscall
	li	$v0, 4
	la	$a0, _newline
	syscall
	li	$v0, 10
	syscall
