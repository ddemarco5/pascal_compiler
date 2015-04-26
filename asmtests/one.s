.LFE0:
	.size	main, .-main
	.ident	"pasc"
main:
.LFB0:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$10, %esi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	popq	%rbp
	ret

.LC0:
	.string	"%d"
	.text
	.globl	main
	.type	main, @function

