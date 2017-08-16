	.file	"code.c"
	.text
	.globl	simple_1
	.type	simple_1, @function
simple_1:
.LFB24:
	.cfi_startproc
	movq	%rsi, %rax
	addq	(%rdi), %rax
	movq	%rax, (%rdi)
	ret
	.cfi_endproc
.LFE24:
	.size	simple_1, .-simple_1
	.globl	call_simple_1
	.type	call_simple_1, @function
call_simple_1:
.LFB25:
	.cfi_startproc
	movq	gval1(%rip), %rax
	addq	$12, %rax
	movq	%rax, gval1(%rip)
	addq	gval2(%rip), %rax
	ret
	.cfi_endproc
.LFE25:
	.size	call_simple_1, .-call_simple_1
	.globl	gval2
	.data
	.align 8
	.type	gval2, @object
	.size	gval2, 8
gval2:
	.quad	763
	.globl	gval1
	.align 8
	.type	gval1, @object
	.size	gval1, 8
gval1:
	.quad	567
	.ident	"GCC: (Ubuntu 4.8.4-2ubuntu1~14.04.3) 4.8.4"
	.section	.note.GNU-stack,"",@progbits
