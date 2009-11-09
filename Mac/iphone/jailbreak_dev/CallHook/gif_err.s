.globl __GifError
	.data
	.align 2
__GifError:
	.space 4
	.text
.globl _GifLastError
_GifLastError:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	call	L3
"L00000000001$pb":
L3:
	popl	%ecx
	leal	__GifError-"L00000000001$pb"(%ecx), %eax
	movl	(%eax), %eax
	movl	%eax, -12(%ebp)
	leal	__GifError-"L00000000001$pb"(%ecx), %eax
	movl	$0, (%eax)
	movl	-12(%ebp), %eax
	leave
	ret
	.subsections_via_symbols
