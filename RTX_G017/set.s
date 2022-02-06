	.seg	"text"
	.proc	4
	.global	_set_sp
_set_sp:
	sub	%o0,0x60,%o0	! Make room for frame-clobbering
	mov	%o0,%sp				! Move first argument to stack pointer
	retl					    ! Return to caller
	nop						    ! ..delayed-branch slot will do nothing
