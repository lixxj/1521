### COMP1521 19t2 ... week 04 lab
### Compute factorials -- iterative function version

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

########################################################################
	# place n in $a0
	move	$a0, $v0	
	# call fac function for factorial calculation
	jal 	fac
	# get the result from $v0 and move to $t0
	move	$t0, $v0
########################################################################

	la	$a0, msg2
	li	$v0, 4
	syscall			# printf("n! = ")

	move	$a0, $t0	# assume $t0 holds n!
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


### fac() function
	.text
	.globl	fac
fac:
	# setup stack frame
	sw	$fp, -4($sp)	# push $fp onto stack
	la	$fp, -4($sp)	# set up $fp for this function
	sw	$ra, -4($fp)	# save return address
	sw	$s0, -8($fp)	# save $s0 to use as ... int i;
	sw	$s1, -12($fp)	# save $s1 to use as ... int prod;
	addi	$sp, $sp, -16	# reset $sp to last pushed item

########################################################################
	# initialise factorial, i and n
	li   	$t1, 1 # factorial
	li	$t2, 1 # i
	move	$t3, $a0 # n	

	# factorial calculation
while:
	sub 	$t4, $t3, $t2 # n - i  
	blez  	$t4, end_while    
   	addi 	$t2, $t2, 1 # i++
   	mul  	$t1, $t1, $t2 # fac = fac * i
   	j    	while
	
	# place result n! in $v0
end_while:
   	move $v0, $t1   
########################################################################

	# clean up stack frame
	lw	$s1, -12($fp)	# restore $s1 value
	lw	$s0, -8($fp)	# restore $s0 value
	lw	$ra, -4($fp)	# restore $ra for return
	la	$sp, 4($fp)	# restore $sp (remove stack frame)
	lw	$fp, ($fp)	# restore $fp (remove stack frame)
	jr	$ra		# return
