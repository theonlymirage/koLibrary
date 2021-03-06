;Kolibri View  - Basic visual component for KolibriOS
;������� ���������� ��������� ��� ���������
;Version 0.1: May 11, 2017   (�� 11.05.2017)

; Copyright (c) 2017, Efremenkov Sergey aka TheOnlyMirage aka ������������ �����
; All rights reserved.
; Redistribution and use in source and binary forms, with or without modification,
; are permitted provided that the following conditions are met:
;    * Redistributions of source code must retain the above copyright notice, this
;    list of conditions and the following disclaimer.
;    * Redistributions in binary form must reproduce the above copyright  notice,
;    this list of conditions and the following disclaimer in the documentation and/or
;    other materials provided with the distribution.
;    * Neither the name of the <organization> nor the names of its contributors may
;    be used to endorse or promote products derived from this software without
;    specific prior written permission.

; THE SOFTWARE IS PROVIDED �AS IS�, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
; INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
; PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
; HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
; OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
; SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
; --------------------------------------------------------------------------------------

;��������� koView:
;   coord    dd   0 ;���������� � � Y
;   width    dd   0 ;������
;   height   dd   0 ;������
;   pixels   dd   0 ;��������� �� ����� � ��������� (32 ����)
;��������� ������ � ��������� �����������:
;   width    dd   0 ;������
;   height   dd   0 ;������
;   pixels   db 0 dup(4*width*height)  ; ������� � ������� AARRGGBB ������ ������� 4*W*H

koView:
   ;�������������� � �����������: ������ ����� � ��������� ��� �������� koView (� edi) � ebx ���������� ����� � ���������
   .createImage:
      push eax
      push ecx
      push edx

      mov eax, dword[edi+4]    ;��������� ������
      mov ebx, dword[edi+8]
      mov edx, 0
      mul ebx
      mov edx, 0
      mov ebx, 3
      mul ebx
      mov ecx, eax

      mcall 68, 12
      mov ebx, eax


      push ebx		      ;�������� ������� AARRGGBB -> BBGGRR
      mov ebx, dword[edi+12]
      add ebx, 8
      mov edx, 0

   .img_cicle:
      cmp ecx, 0
      jbe @f
      mov dl, byte[ebx]     ;B
      mov byte[eax], dl
      inc ebx
      dec ecx
      cmp ecx, 0
      jbe @f

      mov dl, byte[ebx]     ;G
      mov byte[eax+1], dl
      inc ebx
      dec ecx
      cmp ecx, 0
      jbe @f

      mov dl, byte[ebx]     ;R
      mov byte[eax+2], dl
      dec ecx
      add ebx, 2
      add eax, 3
      jmp .img_cicle
   @@:
      pop ebx

      pop edx
      pop ecx
      pop eax
      ret


   ;������� ����� � ��������� �� ������ ebx
   .deleteImage:
      push eax
      push ebx
      push ecx
      mov ecx, ebx
      mcall 68, 13
      pop ecx
      pop ebx
      pop eax
      ret


   ;���������� Kolibri View �� ��������� edi ������� ��� ���� � ������� ����
   .fullDrawInWindow:
      push eax
      push ebx
      push ecx
      push edx
      push edi
      call koView.createImage	 ;������������ � ����������� � ������ BBGGRR
      mov ecx, 0		 ;����� �������
      mov ecx, dword[edi+4]
      shl ecx, 16
      add ecx, dword[edi+8]

      mov edx, dword[edi]	 ;����� ����������
      mcall 7			 ;������������ ���������� �����������
      call koView.deleteImage	 ;������� ����� � ������������
      pop edi
      pop edx
      pop ecx
      pop ebx
      pop eax
      ret

;---
   ;������� ����� � ������������ : �������� ��� ��� ��������, ������� ������� ������ ������ ���� ������ koView (��� � �����)
   ;���������: edi - koView, eax - ���������� ������� � koView, ecx - ������� ������� � koView
   ;����������: ebx - ����� �� ����� � ������������
   .createSubImage:
      push eax
      push ecx
      push edi
      push esi

      push eax
      push ecx		 ;��������� ������ ��� ������
      push edx
      mov edx, 0
      mov eax, ecx
      shr eax, 16	 ;eax=Width
      shl ecx, 16
      shr ecx, 16	 ;ecx=Height
      mul ecx
      mov edx, 0
      mov ecx, 3
      mul ecx		 ;������ � eax - ������ ������ ��� �������� � ������
      mov ecx, eax	 ;�������� ������ ��� �����
      mcall 68, 12
      mov ebx, eax	 ;������������� ����� ������ � ebx
      pop edx
      pop ecx
      pop eax


      push eax
      push ecx		 ;��������� ���-�� ������������ �������� � ������ � edx
      mov edx, 0
      shr ecx, 16	 ;ecx=Width
      mov eax, dword[edi+4]
      sub eax, ecx
      shl eax, 2	 ;��� ����� ���������� � �������� ���
      mov edx, eax	 ;������ � edx - ���-�� ������������ ����
      pop ecx
      pop eax


;��������� ��������� ����� edi
; (X + Y * W) * 4
push eax
push edx
push ebx
push ecx
mov ebx, eax
shr ebx, 16	    ;ebx = X

shl eax, 16
shr eax, 16	    ;eax = Y

mov ecx, dword[edi+4]	;ecx = W

mov edx, 0
mul ecx 	    ;Y * W

add eax, ebx	    ;(X + Y * W)
shl eax, 2

mov edi, dword[edi+12]
add edi, 8
add edi, eax

pop ecx
pop ebx
pop edx
pop eax



push ecx
;mov eax, 0
;mov ax, cx
mov eax, ecx
shl eax, 16
shr eax, 16
pop ecx 		    ;eax = ���-�� �������� � ������

shr ecx, 16		    ;ecx = ������ � ������

;edi - ��������� �� ������ ���������� ��������
;eax - ������ � ������ � ��������
;ebx - ��������� �� ������ ������� ��������
;ecx - ������ � ������ � ��������
;edx - ������������ ����� ����� ����������� ����� ������ ��������

push ebx
.start_copy_sub_image:
   cmp eax, 0
   jbe @f

   push ecx
.copy_sub_image:
   cmp ecx, 0
   jbe .end_copy_sub_image

;�������� ������� �� edi � ebx
push edx
      mov dl, byte[edi]     ;B
      mov byte[ebx], dl
      inc edi
      inc ebx

      mov dl, byte[edi]     ;G
      mov byte[ebx], dl
      inc edi
      inc ebx

      mov dl, byte[edi]     ;R
      mov byte[ebx], dl
      add edi, 2
      inc ebx
pop edx

   dec ecx
   jmp .copy_sub_image
.end_copy_sub_image:
   pop ecx
   add edi, edx

   dec eax
   jmp .start_copy_sub_image
@@:
pop ebx

      pop esi
      pop edi
      pop ecx
      pop eax
      ret


   ;������� ����� � ��������� �� ������ ebx
   .deleteSubImage:
      call koView.deleteImage
      ret

   ;���������� ������ ������ ����� koView � ����: edi - ��������� �� koView, eax - ���������� ������ koView, ecx - ������� ������ koView
   .subDrawInWindow:
      push eax
      push ebx
      push ecx
      push edx
      push edi
      call koView.createSubImage    ;������������ � ����������� � ������ BBGGRR � ��������� ecx
      mov edx, dword[edi]	    ;����� ����������
      mcall 7			    ;������������ ���������� �����������
      call koView.deleteSubImage    ;������� ����� � ������������
      pop edi
      pop edx
      pop ecx
      pop ebx
      pop eax
      ret
;---


   ;���������� �������������:
   .drawRect:  ;eax - ����������, ebx - ������� �� ������ � ������, ecx - ����, edi - ��������� �� koView; ������ �� ����������
      push eax
      push ebx
      push ecx
      push edx
      push edi
      push esi
push eax


push ebx		;�������� ���������� �� �������������
push eax
shr eax, 16
mov ebx, 0
mov bx, word[edi]
add bx, ax
shl ebx, 16
mov eax, 0
mov ax, word[edi+2]
add bx, ax
pop eax
add bx, ax
mov eax, ebx
pop ebx


call koView.pointInView ;���� ���������� �� ��������� �������, �� ������ ��������
cmp eax, 0
jne .end_draw_rect
pop eax
      push eax


push ecx			  ;�������� �� ������
mov ecx, dword[edi+8]
sub cx, ax
cmp cx, bx
jae .height_full
mov bx, cx
.height_full:
pop ecx


push eax
push edx			      ;�������� �� ������
push ecx
mov edx, eax
shr edx, 16
mov ecx, dword[edi+4]
sub cx, dx
mov eax, ebx
shr eax, 16
cmp cx, ax
jae .width_full
  mov eax, 0
  mov ax, cx
  shl eax, 16
  add ax, bx
  mov ebx, eax
.width_full:
pop ecx
pop edx
pop eax


      push ecx

      push ebx
      mov ecx, eax
      shr ecx, 16	    ;ecx - ���������� �

      shl eax, 16
      shr eax, 16	    ;eax - ���������� Y
      mov ebx, dword[edi+4] ;ebx - ������ koView
      mov edx, ebx
      shl edx, 2	    ;edx - ������ ������ koView � ������
      push edx
      mov edx, 0
      mul ebx
      pop edx
      add eax, ecx	    ;eax = (Xpoint + Ypoint * Width)
      shl eax, 2	    ; �������� �� 4

      mov edi, dword[edi+12]
      add edi, 8

      add edi, eax	    ;edi - ������ ��������� ��������������
      pop ebx
      ;edx - ������ koView, ebx - ������ � ������ ��������������
      mov eax, ebx
      shr eax, 16	    ;eax - ������ ��������������
      shl ebx, 16
      shr ebx, 16	    ;ebx - ������ ��������������

      pop ecx
  .draw_rect_start:
      cmp ebx, 0
      jbe .end_draw_rect
      dec ebx
      push edi

      push eax
   .rect_cicle:
      cmp eax, 0
      jbe .rect_cicle_end
      mov dword[edi], ecx
      add edi, 4
      dec eax
      jmp .rect_cicle
   .rect_cicle_end:
      pop eax

      pop edi
      add edi, edx
      jmp .draw_rect_start

   .end_draw_rect:
      pop eax
      pop esi
      pop edi
      pop edx
      pop ecx
      pop ebx
      pop eax
      ret


   ;������ ����(=ecx) �����/�������(���������� � eax) �� Kolibri View (��������� � edi)
   .drawPoint:
      ;�������� �������� - ����� ������ koView?
      ;���� ��, �� ������ ; ����� ������ �� ����� ������

      ;��������� �� ����� � ������ ����������� koView: (Xpoint + Ypoint * Width) * 4 + 8
      push eax
      push ebx
      push edx
      push edi
      mov edx, eax

      mov ebx, 0
      mov bx, ax
      mov eax, dword[edi+4]
      push edx
      mov edx, 0
      mul ebx
      pop edx
      shr edx, 16
      add eax, edx
      shl eax, 2      ;�������� �� 4

      mov edi, dword[edi+12]
      add edi, 8
      add edi, eax
      mov dword[edi], ecx

      pop edi
      pop edx
      pop ebx
      pop eax
      ret


   ;���������� ��������: eax - ����������, esi - ��������� �� ��������, edi - ��������� �� koView
   .drawImage:
      push eax
      push edx
      push esi
      push edi

;��� ���� ��������� � �������� �������� ������ ���� ����� ������ koView

      push eax	 ;eax = (Xpoint + Ypoint * Width) * 4
      push ebx
      push ecx

      mov ebx, eax
      shr ebx, 16	;ebx = Xpoint
      mov ecx, eax
      shl ecx, 16
      shr ecx, 16	;ecx = Ypoint
      mov edx, 0
      mov eax, dword[edi+4]
      mul ecx
      add eax, ebx
      shl eax, 2
      pop ecx
      pop ebx

      mov edx, dword[edi+4]
      shl edx, 2	 ;edx = ���-�� ���� � ������ �������� koView

      mov ebx, dword[edi+8]    ;������ � �������� koView - ����������� ������ �� ����

      mov edi, dword[edi+12]   ;��������� ��������� ������� koView
      add edi, 8
      add edi, eax
      pop eax


      push eax
      shl eax, 16
      shr eax, 16
      sub ebx, eax
      pop eax
      cmp ebx, dword[esi+4]
      jb .setHeightImage    ;���� ������, �� ������������� ������ �� �������� � ebx
      mov ebx, dword[esi+4] ;����� ������ ������ ������ ��������
   .setHeightImage:; ������ ebx = ������ � �������� ��������


      mov ecx, dword[esi]   ;ecx = ������ � �������� ��������
      add esi, 8	  ;��������� ��������� ������� ��������

      push edx
      push ebx
      shr edx, 2     ;���-�� �������� � ������ koView
      shr eax, 16    ;Xpoint
      sub edx, eax
      mov eax, 0
      cmp edx, ecx; dword[esi]
      jae @f
      mov eax, ecx ;dword[esi]
      sub eax, edx
      mov ecx, edx     ;��������� ������ ��������� ��������
      mov edx, 0
      mov ebx, 3
      mul ebx	       ;eax = ���-�� ������������ ���� � �������� ����� ������ �������� ������ ��������
   @@:
      pop ebx
      pop edx

.start_draw_image_in_view:
   cmp ebx, 0
   jbe .end_draw_image_in_view
push edi
;push esi

   push eax
   push ecx
   mov eax, 0
.next_pixel:
   cmp ecx, 0
   jbe @f
   mov al, byte[esi]	  ;B
   mov byte[edi], al

   mov al, byte[esi+1]	  ;G
   mov byte[edi+1], al

   mov al, byte[esi+2]	  ;R
   mov byte[edi+2], al

   dec ecx
   add esi, 3
   add edi, 4
   jmp .next_pixel
@@:
   pop ecx
   pop eax
   add esi, eax

;pop esi
pop edi
add edi, edx
dec ebx
jmp .start_draw_image_in_view
.end_draw_image_in_view:

      pop edi
      pop esi
      pop edx
      pop eax
      ret


   ;���������� ����� (�����������������)
   .drawText:  ;�������: edi - ��������� �� ����� koView, eax - ����, ebx - ���������� ������, edx - ��������� �� ������ ������ (������������ ����)
      push eax ;������ �� ����������
      push ecx
      push edi
      push esi

      shl eax, 8   ;������� ������ � ������������ ����� (�� ��� �� ��������������)
      shr eax, 8
      push eax

      mov ecx, 0;1     ;��������� ��� ��������� ������ ����
      shl ecx, 2
      mov eax, dword[fonts.number]	   ;��������� ����� � ���������
      add cl, al ;cx, ax
      shl ecx, 1;2
      inc cx	     ;������������� ��� ��������� � �����
      shl ecx, 3
      shl ecx, 24
      pop eax
      add ecx, eax   ;��������� ����

      mov eax, dword[edi+4]
      push ebx
      shr ebx, 16
      sub eax, ebx
      pop ebx
      call getMaxSymbolsCount
      mov esi, eax   ;����� ������ (������� ������ �������� ������� � ��������)

      mov edi, dword[edi+12]
      mcall 4

      pop esi
      pop edi
      pop ecx
      pop eax
      ret


   ;�������� koView (��������� � edi) � ���� ecx
   .clear:
      push eax
      push ebx
      push ecx
      push esi
      mov eax, dword[edi+4]    ;��������� ������ �������� ������ � ��������
      mov ebx, dword[edi+8]
      mov edx, 0
      mul ebx

      mov esi, dword[edi+12]
      add esi, 8	       ;���������� ���� ������ � ������ � ������

   .clear_cicle:	       ;� �����
      cmp eax, 0
      jbe .end_clear_cicle
      mov dword[esi], ecx      ;���������� ����
      add esi, 4	       ;��������� �� ��������� �������
      dec eax
      jmp .clear_cicle
   .end_clear_cicle:

      pop esi
      pop ecx
      pop ebx
      pop eax
      ret


   ;�������� �������: edi - koView � ��� ����������� ������ ����������
   .resize:
      push eax
      push ebx
      push ecx
      push edx
      mov ecx, dword[edi+12]
      mcall 68, 13	       ;������� ������� �����
      mov eax, dword[edi+4]    ;��������� ������ ������ ������
      mov ebx, dword[edi+8]
      mov edx, 0
      mul ebx
      shl eax, 2	       ;�������� �� 4
      add eax, 8	       ;� ������ ������ ���� ���� ������ � ������
      mov ecx, eax
      mcall 68, 12	       ;������ ����� �����
      mov dword[edi+12], eax   ;���������� � ��������� �����-��������� �� ����� �����

      mov edx, dword[edi+4]    ;���������� � ���� ������ � ������ ������ ��������
      mov dword[eax], edx
      mov edx, dword[edi+8]
      mov dword[eax+4], edx
      pop edx
      pop ecx
      pop ebx
      pop eax
      ret


   ;������� ������ � ������ ����������: edi - ����� ������ koView
   .create:
      push eax
      push ebx
      push ecx
      mcall 68, 12, 32
      mov edi, eax
      call koView.resize ;�������������� � ������������� ������� ������
      pop ecx
      pop ebx
      pop eax
      ret


   ;������� ������ �� ������: ��������� � edi
   .delete:
      push eax
      push ebx
      push ecx
      mov ecx, edi
      mcall 68, 13
      pop ecx
      pop ebx
      pop eax
      ret



   .pointInView: ;����� ������ ������� koView ��� ���?	  (���������� ����� ������ ���� ������������ ���� �� ������� ��� � koView)
      ;��������: eax - ���������� �����, edi - ��������� �� ��������� koView
      ;��������: eax = 0 - ����� ��� ��������, 1 - ���, ����� �� ��� ��������
      push ebx
      push ecx
      push edx
      mov ebx, 0;ebx - X�����
      mov ecx, 0;ecx - ��������
      mov edx, 0;edx - WidthView

      push eax
      shr eax, 16
      mov bx, ax	  ;X�����
      mov cx, word[edi]   ;��������
      mov edx, dword[edi+4] ;WidthView
      pop eax

      cmp bx, cx;������ >= ��������
      jb @f ;���� ��, �� ��������� ������, ����� ����� �� �� ������� - ������� �� �����

      sub bx, cx
      cmp bx, dx;������-��������   <= Width �������
      ja @f ; ���� ��, �� ��������� ������, ����� ����� �� �� �������

      push eax
      shl eax, 16
      shr eax, 16
      mov bx, ax	   ;Y�����
      mov cx, word[edi+2]  ;Y�������
      mov edx, dword[edi+8]  ;Height �������
      pop eax

      cmp bx, cx;Y����� >= Y�������
      jb @f; ���� ��, �� ��������� ������, ����� ����� �� �� �������

      sub bx, cx
      cmp bx, dx ;Y����� - Y������� <= Height �������
      ja @f; ���� ��, �� ����� ������ ������� �������, ����� ����� �� �� �������
      mov eax, 0
    @@:
      pop edx
      pop ecx
      pop ebx
      ret


; ���������� ��� ��������:  ---- ----
    ;�������� ������� ����������� koView � edi �� eax �����, �� ebx ������
;    .movePixels:
;	ret
    ;����������� ����������� ������� koView (edi) � ������ koView (esi) eax - ����������, ebx - �������
;    .copySubView:
;	ret
   ;���������� �����
;   .drawLine:
;      ret
   ;���������� ����������
;   .drawCircle:
;      ret
   ;���������� koView ������ �������� koView
;   .drawKoView:
;      ret
;---- ---- ---- ---- ----

;--------------------------------------------
;(�) ���������� ������ �., 2017