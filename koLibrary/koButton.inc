;Buttons
;������
;Version 0.1: May 15, 2017	(�� 15.05.2017)

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


;��������� ������:
;+0    view    dd 0 ;��������� �� ������ ������
;+4    text    dd 0 ;��������� �� ����� ������ (� ������� ����� �������������� ��������� �� ��������� � ����������� �������� ����� ��������� ������, ��� ����� ���� ��������� � �� ����������� ������)
;+8    status  dd 0 ;������� ��������� ������
;+12   handler dd 0 ;��������� �� ������ ������������ ���������
;+16   draw    dd 0 ;��������� �� ������� �������� ��������� ������ � ������� ��������� (��. koStyle)

;������ ������������ ��������� (�� ���� ������������ koObjectList):
;   count   dd 2 ;���-�� �������������� ���������
;   hs0     dd 0 ; ��������� �� ���������� ��������� 0
;   hs1     dd 1 ; ��������� �� ���������� ��������� 1
;   � ��� ����� (� ����������� �� count)
;���������� � ������� call ������� ������� ��������� ret

;�������� ��������� ������:
;   0 - �����������, ������ �������
;   1 - ������� ������
;   2 - ������ ������
;   3 - ���������� ������
;   4 - ����� ������ (�������� TAB)
;   5 - ����������� ������� ���� ��� �������
;   6 - ������ ���������

;�������� ������� ������ � ��������
koButton:
   ;������� ����� ������
   ;��������: eax - ����������, ebx - ��������� �� �����, ecx - ��������� ������ ��� ��������, edx - ��������� �� ������� ��������� ���������
   ;����������: edi - ��������� �� ��������� ������
   .create:
      push eax
      push ebx
      push ecx
      mcall 68, 12, 20			   ;�������� ������ ��� ������
      mov edi, eax			   ;���������� �����
      pop ecx
      pop ebx
      pop eax

      mov dword[edi+4], ebx		   ;�������������� ����������� ����������
      mov dword[edi+8], ecx
      mov dword[edi+16], edx

      push eax
      push edi
      call koView.create		   ;������ koView ��� ������
      mov dword[edi], eax		   ;�������������� ����������� ������������
      mov eax, edi
      pop edi
      mov dword[edi], eax
      pop eax

      call koObjectList.create		   ;������ ������ ������������ ���������
      mov dword[edi+12], esi
      ret


   ;������� ������: edi - ��������� �� ��������� ������
   .delete: ;��� ����� ��������������
      push eax
      push ebx
      push ecx

      push esi
      mov esi, dword[edi+12]
      call koObjectList.delete
      pop esi
      ;call koButton.removeAllStatusHandler ;������� ��� ����������� ���������

      push edi				   ;��������� ��������� �� ��������� ������
      mov edi, dword[edi]
      call koView.delete		   ;������� koView ������
      pop edi				   ;��������������� ��������� �� ������

      mov ecx, edi
      mcall 68, 13			   ;������� ��������� ������
      pop ecx
      pop ebx
      pop eax
      ret


   ;�������� ��������� ���������� ������ � edi: ���� ���� �������, ������ ��������� � ��������
   .changeActive:
      push eax
      mov eax, dword[edi+8]
      cmp eax, 6
      je @f		    ;���� ���������, �� ������ ��������
      mov eax, 6    ;����� ������ ����������
      jmp .changeActiveEnd
   @@:
      mov eax, 0    ;������ ��������
   .changeActiveEnd:
      mov dword[esi+8], eax
      pop eax
      ret


   ;���������� ������ - ������ ������ ������ � �������
   ;��������: edi - ��������� ������
   .draw:
      call dword[edi+16]
      ret


   ;�������� ������� ������: �� ��������� ��� �������� ������� 0 �� 0, ��� ��������� ��������� �������
   ;��������: eax - ����� ������ ������, ebx - ����� ������ ������, edi - ��������� ������
   .resize:
      push eax
      push ebx
      push edi
      mov edi, dword[edi]
      mov dword[edi+4], eax
      mov dword[edi+8], ebx
      call koView.resize
      pop edi
      pop ebx
      pop eax
      ret


   ;������� ���������� �������� ��������� (���� �� ����)
   .callStatusHandler:
      push edi
      push eax
      mov eax, dword[edi+8]		  ;��������� ������� ��������� ������
      inc eax				  ;����������� ����� ��� ��� ��������� � ����
      mov edi, dword[edi+12]		  ;��������� � ������ ����������
      cmp eax, dword[edi]		  ;���������� ����� ��������� � ���������� �������������
      ja @f				  ;���� ������� ����������� ���, �� ��������� ������
      shl eax, 2			  ;����������� ����� � ��������� ������ ������������
      add edi, eax
      cmp dword[edi], 0 		  ;���� ����������� ��� (����� 0, �� ������ ��������)
      je @f				  ;�� ������ �� ������
      call dword[edi]			  ;����� �������� ���������� ��������� ������
   @@:
      pop eax
      pop edi
      ret


   ;�������� ��������� ������: edi - ��������� ������, eax - ����� ���������
   .changeStatus:
      mov dword[edi+8], eax	       ;�������� ��������� ������
      call koButton.callStatusHandler  ;� �������� ���������� ������ ���������
      ret


   ;�������� ��������� �� ���������� ��������� (���� �� ����), �� �� ������� ��� �� ������
   ;eax - ������������ ��� ��������� (�� 0)
   .clearStatusHandler:
      push edi
      push eax
      inc eax				  ;����������� ����� ��� ��� ��������� � ����
      mov edi, dword[edi+12]		  ;��������� � ������ ����������
      cmp eax, dword[edi]		  ;���������� ����� ��������� � ���������� �������������
      ja @f				  ;���� ������� ����������� ���, �� ��������� ������
      shl eax, 2			  ;����������� ����� � ��������� ������ ������������
      add edi, eax
      mov dword[edi], 0 		  ;������� ��������� �� ���������� ���������
   @@:
      pop eax
      pop edi
      ret


   ;�������� ����� ���������� ��������� - ������ (���� ���) ������������� �����
   ;��������: eax - ������������ ��� ��������� ������ (�� 0), ebx - ����� ���� �����������, edi - ��������� �� ��������� ������
   .addStatusHandler:
      push eax
      push esi
      push edi
      inc eax				  ;����������� ����� ��� ��� ��������� � ����
      mov edi, dword[edi+12]		  ;��������� � ������ ����������
      cmp eax, dword[edi]		  ;���������� ����� ��������� � ���������� �������������
      ja @f				  ;���� ������� ����������� ���, �� ����� ��������� ����� ������ � ������ �������
      shl eax, 2			  ;���� ���������� ��� ����, �� ����������� ����� � ��������� ������ ������������
      add edi, eax
      mov dword[edi], ebx		  ;����������� ����� ������ �����������
      pop edi				  ;��� �������� ��� ������� ����� � ����� ��������� ��-�� ������ ����� ������� - ����� ������������ ����
      jmp .addStatusHandlerEnd		  ;������ ������� ��������� - ��������� ������
   @@:
      sub eax, dword[edi]		  ;���� ������� ����������� ���, �� ������ ������� �� ������� ������������ �� ������� � ������
      dec eax				  ;�������� ��� ����� �� �������, ����� ����� ����� ����� �� ����� � ������� ��� ����������

      mov esi, edi			  ;������������� �������� ��� ������� ���������� � ������  ������ ��������
      mov edi, 0

   .start_cicle_add:
      cmp eax, 0			  ;��������� �������� eax
      jbe @f
      call koObjectList.addObject	  ;��������� � ����� ������ ���� ����� �������
      dec eax				  ;��������� �������
      jmp .start_cicle_add
   @@:
      mov edi, ebx
      call koObjectList.addObject	  ;��������� � ����� ������ ��� ����� ����������
      pop edi
      mov dword[edi+12], esi		  ;�� �������� ������ - ����� �������� ����� �� ����� ������
   .addStatusHandlerEnd:
      pop esi
      pop eax
      ret


   ;������� ���������� ��������� (���� �� ����)
   ;� ������� ���������� ��� �� ����� ������
   ;.removeStatusHandler:
   ;   ret


   ;������� ��� ����������� ���������
   ;��������: edi - ��������� ������
   .removeAllStatusHandler:
      push esi
      mov esi, dword[edi+12]
      call koObjectList.delete		   ;������� ������� ������ ������������ ���������
      call koObjectList.create		   ;������ ����� ������ ������������ ���������
      mov dword[edi+12], esi
      pop esi
      ret


   ;������ ������ � ������� ���� �� ���� ������: ������ ��������� � �������� ������� �� ������� ������
   ;��������: edi - ��������� �� ��������� ������
   .fullDrawInWindow:
      push edi
      mov edi, dword[edi]
      call koView.fullDrawInWindow
      pop edi
      ret


   ;������ ����� ������ � ������� ���� � ������� ��������� � ���������
   ;������ ��������� ��� ������� ������ ���� ������� �� ������� ������
   ;��������: eax - ���������� �������� ������ ������� ������, ecx - �������, edi - ��������� �� ��������� ������
   .subDrawInWindow:
      push edi
      mov edi, dword[edi]
      call koView.subDrawInWindow
      pop edi
      ret


   ;������ ������ � ������ koView-�������� �� ���� ������
   ;������ ��������� ��� ������� ������ koView (� esi) ������� �� ������� ������
   ;��������: edi - ��������� ������, esi - ��������� koView, � ������� ������
   .fullDrawInView:
      push eax
      push ebx
      push ecx
      push edx
      push edi
      push esi

      mov edi, dword[edi]	 ;����� � edi ������ ������
      call koView.createImage	 ;������������ ������ ������ � ����������� � ������ BBGGRR
      ;������ � ebx ����� ��������� �� ����� � ��������� (� ������ ������ ������� ������� ��������)

      push edi
      push ebx
      mov eax, dword[edi]	 ;eax ������ ��������� ����������
      mov edi, esi		 ;edi ������ ��������� �� koView, � ������� ������ ������
      mov esi, ebx		 ;esi ������ ��������� �� ��������
      pop ebx
      pop edi
      call koView.drawImage	 ;������������ ���������� �����������

      call koView.deleteImage	 ;������� ����� � ������������
      pop esi
      pop edi
      pop edx
      pop ecx
      pop ebx
      pop eax
      ret


   ;������ ������ � ������ koView-�������� � ������� ��������� � ���������
   ;������ ��������� ��� ������� ������ koView (� esi) ������� �� ������� ������
   ;��������: eax - ���������� �������� ������ ������� ������, ecx - �������, edi - ��������� ������, esi - ��������� koView, � ������� ������
   .subDrawInView:
      push eax
      push ebx
      push ecx
      push edx
      push edi
      push esi

      mov edi, dword[edi]	 ;����� � edi ������ ������
      call koView.createSubImage    ;������������ ������ ������ � ����������� � ������ BBGGRR
      ;������ � ebx ����� ��������� �� ����� � ���������
      ;(� ������ ������ ������� ������� �������� ������� ������ ��������������� ������� � ecx ��� ���� ������)

      push edi
      push ebx
      mov eax, dword[edi]	 ;eax ������ ��������� ����������
      mov edi, esi		 ;edi ������ ��������� �� koView, � ������� ������ ������
      mov esi, ebx		 ;esi ������ ��������� �� ��������
      pop ebx
      pop edi
      call koView.drawImage	 ;������������ ���������� �����������

      call koView.deleteSubImage    ;������� ����� � ������������
      pop esi
      pop edi
      pop edx
      pop ecx
      pop ebx
      pop eax
      ret



;��������� ������ ��� ������ (�� ��������� ������������� �� �������� �� ������� � �������)
   ;���������� ������� ���� � ����������� �� �������� ��������� ������
   ;��������: edi - ��������� ������,
   ;	      eax - ���������� ���� (������������ ���� �� ������� ������������ �������� ������ ���������� ������� ������),
   ;	      ebx - ��������� � ������� ���� (������������ �-���� 37.3)
   ;��������: ���
   .mouse:
      push eax
      push ebx
      push ecx
      push edi

      mov ecx, dword[edi+8]   ;������ ��������� ������ � ecx
      cmp ecx, 6	      ;���� ������ ���������, �� ������ �� ������
      je .mouse_end


;���� ������ �������, �� ���������� ������ ���� ��� ������� ��� ��� ?
push eax
push ebx
push ecx
push edi
mov edi, dword[edi]
call koView.pointInView
pop edi
pop ecx
pop ebx

cmp eax, 0
je @f

pop eax
.set_status_0:
;���� ���, ��
mov dword[edi+8], 0	  ; ������ ��������� ������ �� =0
			  ;(�� ��� ��������� ��������� ������ ����� ����������� ������������ ��� ������ ��� �������)
call koButton.callStatusHandler   ;��������� ��� ���������
jmp .mouse_end		  ;� ����� ������ ������ �� ������

@@: ;����� ���� ������ ��� �������, �������� ������������� ������� ������
   pop eax

   cmp ecx, 0 ;������ ����� ���� ��������� ������� ��������� ������
   jne @f     ;���� ��� �� ����� ���� �� ��������� ������

;����� ���� ������� ��������� ������ =0 ��:
mov dword[edi+8], 5 ;������������� ��������� ������ =5
call koButton.callStatusHandler ;� ��������� ��� ���������

@@: ;� ��������� ������ ������ �� ������ � ��������� ������
;����� �������� ��������� ������� ����

;shl ebx, 8	   ;������� ����� ������� �� ������������ - ����������
;shr ebx, 8
;shl ebx, 16+7
;shr ebx, 31

;call myTestPrint

cmp ebx, 0x101 ;257
jne @f
;���� ������ ����� ������, ��
cmp ecx, 4 ;	���� ������� ��������� ������ =4 ��� =5 ����� ������ ��������� �� =1 � ��������� ��� ���������
je .set_status_1
cmp ecx, 5
je .set_status_1
jmp .set_status_0 ;    ����� ���������� ��������� ������ �� =0

.set_status_1:
mov dword[edi+8], 1
call koButton.callStatusHandler
jmp .mouse_end

@@:
cmp ebx, 1
jne @f
;���� ������������ ����� ������, ��
cmp ecx, 1   ;	  ���� ������� ��������� ������ =1 ����� ������ ��������� �� =2 � ��������� ��� ���������
je .set_status_2

cmp ecx, 2;    ���� ������� ��������� ������ =2 �� ������ �� ������ (��������� ��� ��������� � ������)
;je .mouse_end
je .set_status_2

jmp .set_status_0 ;    ����� ���������� ��������� ������ �� =0

.set_status_2:
mov dword[edi+8], 2
call koButton.callStatusHandler
jmp .mouse_end

@@:
cmp ebx, 0x10000
jne .mouse_end
;���� �������� ����� ������, ��
;    ���� ������� ��������� ������ =2 ��� =1 ����� ������ ��������� �� =3 � ��������� ��� ���������
cmp ecx, 1
je .set_status_3
cmp ecx, 2
je .set_status_3

jmp .set_status_0 ;    ����� ���������� ��������� ������ �� =0

.set_status_3:
mov dword[edi+8], 3
call koButton.callStatusHandler
;jmp .mouse_end
call start_application
   .mouse_end:
      pop edi
      pop ecx
      pop ebx
      pop eax
      ret
;��������
;���� ������ �������, �� ���������� ������ ���� ��� ������� ��� ��� ?
;���� ���, �� ������ ��������� ������ �� =0 (�� ��� ��������� ��������� ������ ����� ����������� ������������ ��� ������ ��� �������)
;��������� ��� ��������� � ����� ������ ������ �� ������

;����� ���� ������ ��� �������, �������� ������������� ������� ������
;   ������ ����� ���� ���� ������� ��������� ������ =0 �� ������������� ��������� ������ =5 � ��������� ��� ���������

;���� ������ ����� ������, ��
;    ���� ������� ��������� ������ =4 ��� =5 ����� ������ ��������� �� =1 � ��������� ��� ���������
;    ����� ���������� ��������� ������ �� =0
;���� ������������ ����� ������, ��
;    ���� ������� ��������� ������ =1 ����� ������ ��������� �� =2 � ��������� ��� ���������
;    ���� ������� ��������� ������ =2 �� ������ �� ������ (��������� ��� ��������� � ������)
;    ����� ���������� ��������� ������ �� =0
;���� �������� ����� ������, ��
;    ���� ������� ��������� ������ =2 ��� =1 ����� ������ ��������� �� =3 � ��������� ��� ���������
;    ����� ���������� ��������� ������ �� =0








   ;���������� ������� ����������
   ;��������: edi - ��������� ������, ...
   ;��������: ���
   .keyboard:
      ret


    ;������������ � ����������� ���������� ���������� ����� ��� ������
    ;��������: edi - ��������� ������, ...
;   .touch:
;      ret

;--------------------------------------------
;(�) ���������� ������ �., 2017
