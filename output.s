.LFE0:
	.size   main, .-main
	.ident  "pasc"
.LC0:
	.string "%d\n"
	.text
	.globl  main
	.type   main, @function
main:
.LFB0:
	pushq   %rbp
	movq    %rsp, %rbp
	movl 	$4, %ebx
	movl 	%ebx, -12(%rbp)
	movl 	$6, %ebx
	movl 	%ebx, -16(%rbp)
	movl 	$0, %ebx
	subl 	$1, %ebx
	movl 	%ebx, -20(%rbp)
	movl 	-20(%rbp), %ebx
	movl    %ebx, %esi
	movl    $.LC0, %edi
	call    printf
	movl    $0, %eax
	popq    %rbp
	ret
