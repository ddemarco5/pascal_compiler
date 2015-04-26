.LFE0:
	.size   main, .-main
	.ident  "pasc"
.LC0:
	.string "%d"
	.text
	.globl  main
	.type   main, @function
main:
.LFB0:
	pushq   %rbp
	movq    %rsp, %rbp
	movl 	$1, %ebx
	movl    $10, %esi
	movl    $.LC0, %edi
	call    printf
	movl    $0, %eax
	popq    %rbp
	ret
