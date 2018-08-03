;
; TinyPTC by Gaffer
; www.gaffer.org/tinyptc
; Modifications by Gladius
;

bits 32

global _mmx_memset
global _mmx_memcpy

section .data
	
align 16

section .text

align 16

_mmx_memset:
	push ebp
	mov ebp,esp

	pushad

	mov edi,[ebp+8]			; Destination
	mov eax,[ebp+12]		; Value
	mov ecx,[ebp+16]		; Bytes

    mov edx,ecx
    shr ecx,6
    mov ebx,ecx
    shl ebx,6
    sub edx,ebx				; edx holds # of overflow bytes

	mov [edi],eax
	mov [edi+4],eax
	movq mm0,[edi]

	cmp ecx,0
	je .doneloop
	.setloop

        movq [edi],mm0
        movq [edi+8],mm0
        movq [edi+16],mm0
        movq [edi+24],mm0
	movq [edi+32],mm0
        movq [edi+40],mm0
        movq [edi+48],mm0
        movq [edi+56],mm0
        add edi,8*8
        dec ecx
	jnz .setloop

	.doneloop		
	mov ecx,edx
	shr ecx,2
	rep stosd

	emms

	popad
	pop ebp

	ret

align 16

_mmx_memcpy:

    push ebp
    mov ebp,esp

    pushad

    mov edi,[ebp+8]       ; destination
    mov esi,[ebp+12]      ; source
    mov ecx,[ebp+16]      ; bytes

    mov eax,ecx
    shr ecx,6
    mov ebx,ecx
    shl ebx,6
    sub eax,ebx

	cmp ecx,0
	je .done

align 16
             
    .loop

        movq mm0,[esi]
        movq mm1,[esi+8]
        movq mm2,[esi+16]
        movq mm3,[esi+24]
        movq mm4,[esi+32]
        movq mm5,[esi+40]
        movq mm6,[esi+48]
        movq mm7,[esi+56]
        movq [edi],mm0
        movq [edi+8],mm1
        movq [edi+16],mm2
        movq [edi+24],mm3
        movq [edi+32],mm4
        movq [edi+40],mm5
        movq [edi+48],mm6
        movq [edi+56],mm7
        add esi,8*8
        add edi,8*8
        dec ecx
	jnz .loop

	.done		
    mov ecx,eax
    rep movsb

    emms

    popad
    
    pop ebp
    ret