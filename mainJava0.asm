sys_exit	equ	1
sys_read	equ	3
sys_write	equ	4
stdin	equ	0
stdout	equ	1
stderr	equ	3
section	.data
	userMsg	db	'Enter an integer(less than 32,765): '
	lenUserMsg	equ	$-userMsg
	displayMsg	db	'You entered: '
	lenDisplayMsg	equ	$-displayMsg
	printTempchar	db	'Tempchar=: '
	lenprintTempchar	equ	$-printTempchar
	Result	db	'Ans='
	ResultValue	db	'aaaaa'
	db	0xA
	ResultEnd	equ	$-Result
	num	times	6	db	'ABCDEF'
	numEnd	equ	$-num
	M	dw	7
	N	dw	85
	lit0	dw	0
	lit2	dw	2
	lit1	dw	1
	lit25	dw	25
	lit3	dw	3
section	.bss
	TempChar	RESB	1
	testchar	RESB	1
	ReadInt	RESW	1
	tempint	RESW	1
	negflag	RESB	1
	X	resw	1
	Y	resw	1
	Z	resw	1
	Q	resw	1
	R	resw	1
	A	resw	1
	B	resw	1
	W	resw	1
	F	resw	1
	G	resw	1
	T1	resw	1
	global	_start
section	.text
_start:
	mov	eax,X
	mov	bx,[M]
	mov	[eax],bx
	mov	eax,Y
	mov	bx,[N]
	mov	[eax],bx
	CALL	Multiply
	CALL	GetAnInteger
	mov	ax,[ReadInt]
	mov	[X],ax
	CALL	GetAnInteger
	mov	ax,[ReadInt]
	mov	[Y],ax
	CALL	Divide
	mov	eax,X
	mov	bx,[lit25]
	mov	[eax],bx
	mov	eax,Y
	mov	bx,[lit3]
	mov	[eax],bx
	mov	ax,[X]
	CALL	Print
	mov	eax,sys_write
	mov	ebx,stdout
	mov	ecx,Result
	mov	edx,ResultEnd
	int	0x80
	mov	ax,[Y]
	CALL	Print
	mov	eax,sys_write
	mov	ebx,stdout
	mov	ecx,Result
	mov	edx,ResultEnd
	int	0x80
	CALL	Divide
	CALL	GetAnInteger
	mov	ax,[ReadInt]
	mov	[X],ax
	CALL	GetAnInteger
	mov	ax,[ReadInt]
	mov	[Y],ax
	CALL	GCD
	mov	eax,sys_exit
	xor ebx,ebx
	int	0x80
Multiply:
	mov	eax,A
	mov	bx,[X]
	mov	[eax],bx
	mov	eax,B
	mov	bx,[Y]
	mov	[eax],bx
	mov	eax,Z
	mov	bx,[lit0]
	mov	[eax],bx
wbe1:NOP
	mov	ax,[B]
	cmp	ax,[lit0]
	jng	be1
	mov	ax,[B]
	and	ax,0x0001
	cmp	ax,0x0001
	jne	be2
	mov	ax,[Z]
	add	ax,[A]
	mov	[T1],ax
	mov	eax,Z
	mov	bx,[T1]
	mov	[eax],bx
be2:NOP
	mov	ax,[lit2]
	mov	bx,[A]
	mul	bx
	mov	[T1],ax
	mov	eax,A
	mov	bx,[T1]
	mov	[eax],bx
	mov	ax,[B]
	mov	bx,[lit2]
	div	bx
	mov	[T1],ax
	mov	eax,B
	mov	bx,[T1]
	mov	[eax],bx
	mov	ax,[A]
	CALL	Print
	mov	eax,sys_write
	mov	ebx,stdout
	mov	ecx,Result
	mov	edx,ResultEnd
	int	0x80
	mov	ax,[B]
	CALL	Print
	mov	eax,sys_write
	mov	ebx,stdout
	mov	ecx,Result
	mov	edx,ResultEnd
	int	0x80
	jmp	wbe1
be1:NOP
	ret
Divide:
	mov	eax,R
	mov	bx,[X]
	mov	[eax],bx
	mov	eax,Q
	mov	bx,[lit0]
	mov	[eax],bx
	mov	eax,W
	mov	bx,[Y]
	mov	[eax],bx
wbe3:NOP
	mov	ax,[W]
	cmp	ax,[R]
	jnle	be3
	mov	ax,[lit2]
	mov	bx,[W]
	mul	bx
	mov	[T1],ax
	mov	eax,W
	mov	bx,[T1]
	mov	[eax],bx
	jmp	wbe3
be3:NOP
wbe4:NOP
	mov	ax,[W]
	cmp	ax,[Y]
	jng	be4
	mov	ax,[lit2]
	mov	bx,[Q]
	mul	bx
	mov	[T1],ax
	mov	eax,Q
	mov	bx,[T1]
	mov	[eax],bx
	mov	ax,[W]
	mov	bx,[lit2]
	div	bx
	mov	[T1],ax
	mov	eax,W
	mov	bx,[T1]
	mov	[eax],bx
	mov	ax,[W]
	cmp	ax,[R]
	jnle	be5
	mov	ax,[R]
	sub	ax,[W]
	mov	[T1],ax
	mov	eax,R
	mov	bx,[T1]
	mov	[eax],bx
	mov	ax,[Q]
	add	ax,[lit1]
	mov	[T1],ax
	mov	eax,Q
	mov	bx,[T1]
	mov	[eax],bx
be5:NOP
	mov	ax,[W]
	CALL	Print
	mov	eax,sys_write
	mov	ebx,stdout
	mov	ecx,Result
	mov	edx,ResultEnd
	int	0x80
	mov	ax,[Q]
	CALL	Print
	mov	eax,sys_write
	mov	ebx,stdout
	mov	ecx,Result
	mov	edx,ResultEnd
	int	0x80
	mov	ax,[R]
	CALL	Print
	mov	eax,sys_write
	mov	ebx,stdout
	mov	ecx,Result
	mov	edx,ResultEnd
	int	0x80
	jmp	wbe4
be4:NOP
	ret
GCD:
	mov	eax,F
	mov	bx,[X]
	mov	[eax],bx
	mov	eax,G
	mov	bx,[Y]
	mov	[eax],bx
wbe6:NOP
	mov	ax,[F]
	cmp	ax,[G]
	je	be6
	mov	ax,[F]
	cmp	ax,[G]
	jnl	be7
	mov	ax,[G]
	sub	ax,[F]
	mov	[T1],ax
	mov	eax,G
	mov	bx,[T1]
	mov	[eax],bx
be7:NOP
	mov	ax,[G]
	cmp	ax,[F]
	jnl	be8
	mov	ax,[F]
	sub	ax,[G]
	mov	[T1],ax
	mov	eax,F
	mov	bx,[T1]
	mov	[eax],bx
be8:NOP
	jmp	wbe6
be6:NOP
	mov	eax,Z
	mov	bx,[F]
	mov	[eax],bx
	mov	ax,[Z]
	CALL	Print
	mov	eax,sys_write
	mov	ebx,stdout
	mov	ecx,Result
	mov	edx,ResultEnd
	int	0x80
	mov	ax,[F]
	CALL	Print
	mov	eax,sys_write
	mov	ebx,stdout
	mov	ecx,Result
	mov	edx,ResultEnd
	int	0x80
	mov	ax,[G]
	CALL	Print
	mov	eax,sys_write
	mov	ebx,stdout
	mov	ecx,Result
	mov	edx,ResultEnd
	int	0x80
	ret
GetAnInteger:
	mov	eax,sys_write
	mov	ebx,stdout
	mov	ecx,userMsg
	mov	edx,lenUserMsg
	int	0x80
	mov	eax,sys_read
	mov	ebx,stdin
	mov	ecx,num
	mov	edx,6
	int	0x80
CSTI:NOP
	mov	ax,0
	mov	[ReadInt],ax
	mov	ecx, num
	mov	bx,0
	mov	bl,byte[ecx]
Next:NOP
	sub	bl,'0'
	mov	ax,[ReadInt]
	mov	dx,10
	mul	dx
	add	ax,bx
	mov	[ReadInt], ax
	mov	bx,0
	add	ecx,1
	mov	bl,byte[ecx]
	cmp	bl,0xA
	jne	Next
	ret
Print:NOP
	mov	ebx,ResultValue+4
ConvertLoop:NOP
	sub	dx,dx
	mov	cx,10
	div	cx
	add	dl,'0'
	mov	[ebx],dl
	dec	ebx
	cmp	ebx,ResultValue
	jge	ConvertLoop
	ret
