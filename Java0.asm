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
	M	dw	13
	N	dw	56
	lit18	dw	18
section	.bss
	TempChar	RESB	1
	testchar	RESB	1
	ReadInt	RESW	1
	tempint	RESW	1
	negflag	RESB	1
	X	resw	1
	Y	resw	1
	Z	resw	1
	T1	resw	1
	T2	resw	1
	T3	resw	1
	global	_start
section	.text
_start:
	CALL	GetAnInteger
	mov	ax,[ReadInt]
	mov	[Y],ax
	mov	ax,[M]
	mov	bx,[N]
	mul	bx
	mov	[T1],ax
	mov	ax,[T1]
	add	ax,[lit18]
	mov	[T2],ax
	mov	ax,[T2]
	sub	ax,[Y]
	mov	[T3],ax
	mov	eax,X
	mov	bx,[T3]
	mov	[eax],bx
	mov	ax,[X]
	CALL	Print
	mov	eax,sys_write
	mov	ebx,stdout
	mov	ecx,Result
	mov	edx,ResultEnd
	int	0x80
	mov	eax,sys_exit
	xor ebx,ebx
	int	0x80
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
