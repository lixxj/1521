### COMP1521 19t2 ... week 04 lab
### Compute factorials -- no functions (except main)

	.data
msg1:	.asciiz "n  = "
msg2:	.asciiz "n! = "
eol:	.asciiz "\n"

	.text
	.globl	main
main:
	# Set up stack frame.
	sw	$fp, -4($sp)	# push $fp onto stack
	la	$fp, -4($sp)	# set up $fp for this function
	sw	$ra, -4($fp)	# save return address
	sw	$s0, -8($fp)	# save $s0 to use as ... int n
	sw	$s1, -12($fp)	# save $s1 to use as ... int i
	sw	$s2, -16($fp)	# save $s2 to use as ... int fac
	addi	$sp, $sp, -20	# move $sp to top of stack

	# code for main()
	li	$s0, 0		# n = 0

	# printf("n  = ")
	la	$a0, msg1
	li	$v0, 4
	syscall			
	
	# scanf("%d", into $v0)
	li	$v0, 5
	syscall			
	
	# DEBUG: If an int is expected while the input is a str, $v0 will contain 0
	#move	$a0, $v0
	#li	$v0, 1
	#syscall			

########################################################################
	# initialise factorial, i and n
	li   	$t1, 1 # factorial
	li	$t2, 1 # i
	move	$t3, $v0 # n	
	
	# compute the factorial
while:
	sub 	$t4, $t3, $t2 # n - i  
	blez  	$t4, end_while    
   	addi 	$t2, $t2, 1 # i++
   	mul  	$t1, $t1, $t2 # fac = fac * i
   	j    	while
	
	# leave the result in register $s2
end_while:
   	move $s2, $t1   
########################################################################

	# printf("n! = ")
	la	$a0, msg2
	li	$v0, 4
	syscall			

	# printf("%d", fac)
	move	$a0, $s2	# assume $s2 holds n!
	li	$v0, 1
	syscall			

	# printf("\n")
	la	$a0, eol
	li	$v0, 4
	syscall			

	# clean up stack frame
	lw	$s2, -16($fp)	# restore $s2 value
	lw	$s1, -12($fp)	# restore $s1 value
	lw	$s0, -8($fp)	# restore $s0 value
	lw	$ra, -4($fp)	# restore $ra for return
	la	$sp, 4($fp)	# restore $sp (remove stack frame)
	lw	$fp, ($fp)	# restore $fp (remove stack frame)

	li	$v0, 0
	jr	$ra		# return 0

