	.file	"1_1.c"
	.text
	.section	.rodata
.LC0:
	.string	"%c"
	.text
	.globl	main
	.type	main, @function
main:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$4096, %rsp
	orq	$0, (%rsp)
	subq	$3936, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$0, -8024(%rbp)
	jmp	.L2
.L5:
	movl	$800000000, %edi
	call	malloc@PLT
	movq	%rax, %rdx
	movl	-8024(%rbp), %eax
	cltq
	movq	%rdx, -8016(%rbp,%rax,8)
	movl	$0, -8020(%rbp)
	jmp	.L3
.L4:
	movl	-8024(%rbp), %eax
	cltq
	movq	-8016(%rbp,%rax,8), %rdx
	movl	-8020(%rbp), %eax
	cltq
	salq	$2, %rax
	addq	%rax, %rdx
	movl	-8020(%rbp), %eax
	subl	-8024(%rbp), %eax
	movl	%eax, (%rdx)
	addl	$344, -8020(%rbp)
.L3:
	cmpl	$199999, -8020(%rbp)
	jle	.L4
	movl	$1, %edi
	call	sleep@PLT
	addl	$1, -8024(%rbp)
.L2:
	cmpl	$999, -8024(%rbp)
	jle	.L5
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	__isoc99_scanf@PLT
	movl	$0, %eax
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L7
	call	__stack_chk_fail@PLT
.L7:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04.2) 11.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
