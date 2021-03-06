;koObjectList
;������ �������� (������ ���������� �� �������)
;Version 0.1: May 13, 2017	(�� 13.05.2017)

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


;��������� ������ ��������:
;+0   dd     ���-�� ��������
;+4   dd     ��������� �� ������� ������ 1-��� � ������ �������
;+8   dd     ��������� �� ������� ������ 2-��� � ������ �������
;� ��� �����


koObjectList:
   ;������� ������ ������ ��������: esi - ��������� �� ��������� ������
   .create:
      push eax
      push ebx
      push ecx
      mcall 68, 12, 4
      mov esi, eax
      mov dword[esi], 0
      pop ecx
      pop ebx
      pop eax
      ret
   ;������� ������ �������� �� ������ � esi
   .delete:
      push eax
      push ebx
      push ecx
      mov ecx, esi
      mcall 68, 13
      pop ecx
      pop ebx
      pop eax
      ret
   ;�������� ������ � ����� ������,��������: � edi - ����� ����������� ������, � esi - ������� ������; ����������: esi - ����� ������
   .addObject:
      push eax
      push ebx
      push ecx
      push edx
      push edi
      mov edx, edi	     ;��������� ��������� �� ����� ������

      mov ecx, dword[esi]
      inc ecx
      push ecx
      inc ecx
      shl ecx, 2	     ;�������� �� 4, ����� �������� ������ � ������
      mov ebx, ecx
      push ebx
      mcall 68, 12	     ;������ ����� ������ ������� �������
      mov edi, eax
      pop ebx
      pop ecx

      mov eax, dword[esi]
      inc eax
      call copyESIinEDI      ;�������� ������ ������ � ����� ������ (������ �� ������ ���-���)
      mov dword[edi], eax    ;���������� ����� ������ ������

      push edi
      add edi, ebx	   ;��������� ����� ������
      sub edi, 4
      mov dword[edi], edx
      pop edi

      call koObjectList.delete ;������� ������ ������
      mov esi, edi ;���������� ��������� �� ����� ������
      pop edi
      pop edx
      pop ecx
      pop ebx
      pop eax
      ret
   ;������� ������ �� ������: edi - ��������� �� ��������� ������, � esi - ������� ������; ����������: esi - ����� ������ ��� ���������� �������
   .removeObject:
      push eax
      push ebx
      push ecx
      push edx
      push edi

      mov ecx, dword[esi]   ;������ ����� = ���-�� ���������
      mov eax, 0	    ;��������� ����� ���������� ������� - �������� ��������� ��������
   .find_object:
      cmp ecx, 0	    ;���� � ������ ��� ������ ������� ��� ������ ����, �� �������
      jbe .end_find
      cmp esi, dword[edi]
      je .end_find
      add edi, 4
      dec ecx
      inc eax
   .end_find:		    ;������ � eax - ����� ������� � ������ (���� �� 1) ��� ���� (���� ������� ��� � ������)
      cmp eax, 0
      jbe @f		    ;���� ������ ������� � ������ ���, �� �� ������ �� ������ �� �������

      push eax
      mov ecx, dword[esi]
      dec ecx
      mov edx, ecx
      inc ecx		    ;����� ��� ���� ���-�� �������� � ������
      shl ecx, 2	    ;�������� �� 4, ����� �������� ������ � ������
      mcall 68, 12	    ;������ ����� ������ ������� �������
      mov edi, eax
      mov dword[edi], edx   ;���������� ����� ������ ������
      pop eax

      push esi
      push edi
      add edi, 4	    ;��������� � ����������� �������
      add esi, 4

      dec eax
      call copyESIinEDI     ;�������� ������ ������ �� ���������� �������
      inc eax

      sub edx, eax
      shl eax, 2
      add esi, eax
      add edi, eax
      sub edi, 4
      mov eax, edx
      call copyESIinEDI     ;�������� ������ ������ ����� ���������� �������

      pop edi
      pop esi

      call koObjectList.delete ;������� ������ ������
      mov esi, edi	       ;���������� ��������� �� ����� ������
   @@:
      pop edi
      pop edx
      pop ecx
      pop ebx
      pop eax
      ret
   ;�������� ������ � ����������� ������� ������
   ;.insertobject:
   ;   ret


;��������������� ������� ��� ����������� ���������: �������� n=eax ��������� �� 4 ����� �� ESI �� ������ EDI
copyESIinEDI:
   push eax
   push ebx
   push ecx
   push esi
   push edi
   cmp eax, 0
   jbe .endcopy
   mov ecx, 0
.copy:
   add esi, ecx
   add edi, ecx
   add ecx, 4
   mov ebx, dword[esi]
   mov dword[edi], ebx
   dec eax
   cmp eax, 0
   ja .copy
.endcopy:
   pop edi
   pop esi
   pop ecx
   pop ebx
   pop eax
   ret

;--------------------------------------------
;(�) ���������� ������ �., 2017