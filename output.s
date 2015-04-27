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
	movl 	$1, %edx
	addl 	$1, %edx
	movl 	$1, %ebx
	addl 	$1, %ebx
	imull 	$1, %ebx
	addl 	%ebx, %edx
	movl    %edx, %esi
	movl    $.LC0, %edi
	call    printf
	movl    $0, %eax
	popq    %rbp
	ret
