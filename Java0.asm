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
	lit10	dw	10
	lit2	dw	2
	lit4	dw	4
	lit1	dw	1
	lit0	dw	0
section	.bss
	TempChar	RESB	1
	testchar	RESB	1
	ReadInt	RESW	1
	tempint	RESW	1
	negflag	RESB	1
	input	resw	1
	ans	resw	1
	a	resw	1
	Bob	resw	1
	Jane	resw	1
	b	resw	1
	c	resw	1
	numA	resw	1
	numB	resw	1
	num1	resw	1
	num2	resw	1
	num3	resw	1
	M	resw	1
	N	resw	1
	RM	resw	1
	RN	resw	1
	T1	resw	1
	T2	resw	1
	T3	resw	1
	T4	resw	1
	T5	resw	1
	T6	resw	1
	T7	resw	1
	global	_start
section	.text
_start:
	CALL	procC1
	CALL	GreatestNumber
	CALL	GreatestNumber2
	mov	eax,M
	mov	bx,[lit1]
	mov	[eax],bx
wbe11:NOP
	mov	ax,[M]
	cmp	ax,[lit0]
	jng	be11
	CALL	factorial
	jmp	wbe11
be11:NOP
	mov	eax,RM
	mov	bx,[lit1]
	mov	[eax],bx
wbe12:NOP
	mov	ax,[RM]
	cmp	ax,[lit0]
	jng	be12
	mov	eax,RN
	mov	bx,[lit1]
	mov	[eax],bx
	CALL	recursfactorial
	mov	ax,[RN]
	CALL	Print
	mov	eax,sys_write
	mov	ebx,stdout
	mov	ecx,Result
	mov	edx,ResultEnd
	int	0x80
	CALL	GetAnInteger
	mov	ax,[ReadInt]
	mov	[RM],ax
	jmp	wbe12
be12:NOP
	mov	eax,sys_exit
	xor ebx,ebx
	int	0x80
procC1:
	CALL	GetAnInteger
	mov	ax,[ReadInt]
	mov	[a],ax
	CALL	GetAnInteger
	mov	ax,[ReadInt]
	mov	[Bob],ax
	CALL	GetAnInteger
	mov	ax,[ReadInt]
	mov	[Jane],ax
	CALL	GetAnInteger
	mov	ax,[ReadInt]
	mov	[b],ax
	CALL	GetAnInteger
	mov	ax,[ReadInt]
	mov	[c],ax
	mov	ax,[Bob]
	add	ax,[Jane]
	mov	[T1],ax
	mov	ax,[T1]
	sub	ax,[lit10]
	mov	[T2],ax
	mov	ax,[T2]
	mov	bx,[lit2]
	div	bx
	mov	[T3],ax
	mov	ax,[T3]
	mov	bx,[lit4]
	mul	bx
	mov	[T4],ax
	mov	ax,[a]
	mov	bx,[T4]
	mul	bx
	mov	[T5],ax
	mov	ax,[b]
	add	ax,[c]
	mov	[T6],ax
	mov	ax,[T5]
	mov	bx,[T6]
	div	bx
	mov	[T7],ax
	mov	eax,ans
	mov	bx,[T7]
	mov	[eax],bx
	mov	ax,[ans]
	CALL	Print
	mov	eax,sys_write
	mov	ebx,stdout
	mov	ecx,Result
	mov	edx,ResultEnd
	int	0x80
	ret
GreatestNumber:
	CALL	GetAnInteger
	mov	ax,[ReadInt]
	mov	[numA],ax
	CALL	GetAnInteger
	mov	ax,[ReadInt]
	mov	[numB],ax
	mov	ax,[numA]
	cmp	ax,[numB]
	jng	be1
	mov	ax,[numA]
	CALL	Print
	mov	eax,sys_write
	mov	ebx,stdout
	mov	ecx,Result
	mov	edx,ResultEnd
	int	0x80
be1:NOP
	mov	ax,[numA]
	cmp	ax,[numB]
	jnl	be2
	mov	ax,[numB]
	CALL	Print
	mov	eax,sys_write
	mov	ebx,stdout
	mov	ecx,Result
	mov	edx,ResultEnd
	int	0x80
be2:NOP
	ret
GreatestNumber2:
	CALL	GetAnInteger
	mov	ax,[ReadInt]
	mov	[num1],ax
	CALL	GetAnInteger
	mov	ax,[ReadInt]
	mov	[num2],ax
	CALL	GetAnInteger
	mov	ax,[ReadInt]
	mov	[num3],ax
	mov	ax,[num1]
	cmp	ax,[num2]
	jng	be3
	mov	ax,[num1]
	cmp	ax,[num3]
	jng	be4
	mov	ax,[num1]
	CALL	Print
	mov	eax,sys_write
	mov	ebx,stdout
	mov	ecx,Result
	mov	edx,ResultEnd
	int	0x80
be4:NOP
	mov	ax,[num1]
	cmp	ax,[num3]
	jnl	be5
	mov	ax,[num3]
	CALL	Print
	mov	eax,sys_write
	mov	ebx,stdout
	mov	ecx,Result
	mov	edx,ResultEnd
	int	0x80
be5:NOP
be3:NOP
	mov	ax,[num1]
	cmp	ax,[num2]
	jnl	be6
	mov	ax,[num2]
	cmp	ax,[num3]
	jng	be7
	mov	ax,[num2]
	CALL	Print
	mov	eax,sys_write
	mov	ebx,stdout
	mov	ecx,Result
	mov	edx,ResultEnd
	int	0x80
be7:NOP
	mov	ax,[num2]
	cmp	ax,[num3]
	jnl	be8
	mov	ax,[num3]
	CALL	Print
	mov	eax,sys_write
	mov	ebx,stdout
	mov	ecx,Result
	mov	edx,ResultEnd
	int	0x80
be8:NOP
be6:NOP
	ret
factorial:
	CALL	GetAnInteger
	mov	ax,[ReadInt]
	mov	[M],ax
	mov	eax,N
	mov	bx,[lit1]
	mov	[eax],bx
wbe9:NOP
	mov	ax,[M]
	cmp	ax,[lit0]
	jng	be9
	mov	ax,[N]
	mov	bx,[M]
	mul	bx
	mov	[T1],ax
	mov	eax,N
	mov	bx,[T1]
	mov	[eax],bx
	mov	ax,[M]
	sub	ax,[lit1]
	mov	[T1],ax
	mov	eax,M
	mov	bx,[T1]
	mov	[eax],bx
	jmp	wbe9
be9:NOP
	mov	ax,[N]
	CALL	Print
	mov	eax,sys_write
	mov	ebx,stdout
	mov	ecx,Result
	mov	edx,ResultEnd
	int	0x80
	ret
recursfactorial:
	mov	ax,[RM]
	cmp	ax,[lit0]
	jng	be10
	mov	ax,[RN]
	mov	bx,[RM]
	mul	bx
	mov	[T1],ax
	mov	eax,RN
	mov	bx,[T1]
	mov	[eax],bx
	mov	ax,[RM]
	sub	ax,[lit1]
	mov	[T1],ax
	mov	eax,RM
	mov	bx,[T1]
	mov	[eax],bx
	CALL	recursfactorial
be10:NOP
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
