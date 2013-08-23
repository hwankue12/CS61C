	.file	"sseTest.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC5:
	.string	"|%g %g| * |%g %g| = |%g %g|\n"
.LC6:
	.string	"|%g %g|   |%g %g|   |%g %g|\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB531:
	.cfi_startproc
	subq	$104, %rsp
	.cfi_def_cfa_offset 112
	movsd	.LC0(%rip), %xmm0
	movsd	%xmm0, (%rsp)
	movl	$0, 8(%rsp)
	movl	$1073741824, 12(%rsp)
	movsd	.LC2(%rip), %xmm1
	movsd	%xmm1, 16(%rsp)
	movl	$0, 24(%rsp)
	movl	$1074790400, 28(%rsp)
	movsd	%xmm1, 32(%rsp)
	movq	$0, 40(%rsp)
	movq	$0, 48(%rsp)
	movl	$0, 56(%rsp)
	movl	$1073741824, 60(%rsp)
	movq	$0, 64(%rsp)
	movq	$0, 72(%rsp)
	movq	$0, 80(%rsp)
	movq	$0, 88(%rsp)
	movupd	64(%rsp), %xmm3
	movupd	80(%rsp), %xmm2
	movupd	(%rsp), %xmm4
	movapd	%xmm1, %xmm6
	unpcklpd	%xmm6, %xmm6
	movapd	%xmm6, %xmm5
	mulpd	%xmm4, %xmm5   # the loop multiplication
	addpd	%xmm3, %xmm5   # the add
	xorpd	%xmm6, %xmm6
	unpcklpd	%xmm6, %xmm6
	movapd	%xmm6, %xmm3
	mulpd	%xmm6, %xmm4
	addpd	%xmm2, %xmm4
	movupd	16(%rsp), %xmm2
	mulpd	%xmm2, %xmm3      ### second time
	addpd	%xmm5, %xmm3
	movsd	.LC1(%rip), %xmm6
	unpcklpd	%xmm6, %xmm6
	mulpd	%xmm6, %xmm2       ## third time
	addpd	%xmm4, %xmm2
	movupd	%xmm3, 64(%rsp)
	movupd	%xmm2, 80(%rsp)
	movsd	80(%rsp), %xmm5
	movsd	64(%rsp), %xmm4
	xorpd	%xmm3, %xmm3
	movapd	%xmm1, %xmm2
	movl	$.LC5, %esi
	movl	$1, %edi
	movl	$6, %eax
	call	__printf_chk
	movsd	88(%rsp), %xmm5
	movsd	72(%rsp), %xmm4
	movsd	.LC1(%rip), %xmm3
	xorpd	%xmm2, %xmm2
	movsd	24(%rsp), %xmm1
	movsd	8(%rsp), %xmm0
	movl	$.LC6, %esi
	movl	$1, %edi
	movl	$6, %eax
	call	__printf_chk
	movl	$0, %eax
	addq	$104, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE531:
	.size	main, .-main
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC0:
	.long	0
	.long	1072693248
	.align 8
.LC1:
	.long	0
	.long	1073741824
	.align 8
.LC2:
	.long	0
	.long	1074266112
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
