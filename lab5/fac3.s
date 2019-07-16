### COMP1521 19t2 ... week 05 lab
### Compute factorials -- recursive function version

## Global data
	.data
msg1:	.asciiz "n  = "
msg2:	.asciiz "n! = "
eol:	.asciiz "\n"

### main() function
	.text
	.globl	main
main:
	# Set up stack frame
	sw	$fp, -4($sp)	# push $fp onto stack
	la	$fp, -4($sp)	# set up $fp for this function
	sw	$ra, -4($fp)	# save return address
	sw	$s0, -8($fp)	# save $s0 to use as ... int n
	addi	$sp, $sp, -12	# reset $sp to last pushed item

	# code for main()
	li	$s0, 0

	la	$a0, msg1
	li	$v0, 4
	syscall			# printf("n  = ")

	li	$v0, 5
	syscall			# scanf("%d", into $v0)
	move	$s0, $v0

	move	$a0, $s0	#         + from $s0
	jal	fac		# % = fac(n)
	nop	#[branch delay] # |
	move	$s0, $v0	# + into $s0; don't need n now

	la	$a0, msg2
	li	$v0, 4
	syscall			# printf("n! = ")

	move	$a0, $s0	# assume $s0 holds n!
	li	$v0, 1
	syscall			# printf ("%d", tmp)

	la	$a0, eol
	li	$v0, 4
	syscall			# printf("\n")

	# clean up stack frame
	lw	$s0, -8($fp)	# restore $s0 value
	lw	$ra, -4($fp)	# restore $ra for return
	la	$sp, 4($fp)	# restore $sp (remove stack frame)
	lw	$fp, ($fp)	# restore $fp (remove stack frame)

	li	$v0, 0
	jr	$ra		# return 0

	.text
	.globl	fac
fac:
	# prologue, push $ra on to stack
	addi    $sp, $sp, -4        
	sw  	$ra, ($sp)
	
	# recursive fac() body, n is $a0
	# if n <= 1
	li		$t1, 1
	bge		$t1, $a0, fac_terminate # return 1
	# if n > 1
	addi    $sp, $sp, -4        
	sw  	$a0, ($sp) # push current n onto stack
	addi 	$a0, $a0, -1
	jal 	fac 
	# now $v0 contains fac(n - 1)
	lw  	$a0, 0($sp)
	addi    $sp, $sp, 4 # restore current n
	mul 	$v0, $a0, $v0 # return n * fac (n - 1)
	j 		fac_epilogue

fac_terminate:
	li 		$v0, 1

fac_epilogue:
	# restore $ra and return
	lw  	$ra, 0($sp)
	addi    $sp, $sp, 4
	jr	$ra		
