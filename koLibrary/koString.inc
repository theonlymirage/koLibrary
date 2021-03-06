;koString
;������ �� ��������
;Version 0.1: May 14, 2017   (�� 14.05.2017)

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

;��������� koZeroString:
;��� ASCII ������, �������������� ���� (db 0). ���� ������ ����� ������ ���������� ����� ������.


;��������� ����� koString:
;+0   encoding	  dd  0 ;��������� ������ (���� �������������� ��� ����: 1 cp866, 2 utf16LE, 3 utf8)
;+4   length	  dd  0 ;����� ������ � ��������
;+8   symbols	  dd  0 ;��������� �� ������� ������ � ������ ���������
;������ ������

koZeroString:
   ;������� ������ ������ � ���� �� �����
   ;��������: ���
   ;��������: edi - ����� �� �������
   .create:
      push eax
      push ebx
      push ecx
      mcall 68, 12, 1
      mov edi, eax
      mov byte[eax], 0
      pop ecx
      pop ebx
      pop eax
      ret


   ;������� ������, �������������� ����
   ;��������: edi - ����� ������ �� �������
   ;��������: ���
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


   ;�������� ������ ������ ������ � ���� �� �����
   ;�������: edi - ����� ������, ecx - ����� ����� ������ (� ��������, ��� ���������� ������������ ����)
   ;��������: edi - ����� ������ � ����� ��������
   .resize:
      push eax
      push ebx
      push ecx
      push esi
      inc ecx
      mcall 68, 12
      mov esi, eax
      mov ebx, 0
   @@:
      cmp ecx, 1
      jbe @f
      mov bl, byte[edi]
      cmp bl, 0
      je .resizeAddSpace
      mov byte[eax], bl
      inc edi
      inc eax
      dec ecx
      jmp @b
   .resizeAddSpace:
      mov byte[eax], ' '
      inc eax
      dec ecx
      jmp @b
   @@:
      mov byte[eax], 0
      mov edi, esi
      pop esi
      pop ecx
      pop ebx
      pop eax
      ret


   ;����� ������ � ��������
   ;��������: edi - ����� ������ ������
   ;��������: ecx - ���-�� �������� � ������
   .length:
      push edi
      mov ecx, 0
   @@:
      cmp byte[edi], 0
      je @f
      inc ecx
      inc edi
      jmp @b
   @@:
      pop edi
      ret


   ;������ ������ � ������ (����������� ��������� ����)
   ;�������: edi - ��������� �� ������
   ;��������: ecx - ������ � ������
   .size:
      call koZeroString.length
      inc ecx
      ret


   ;�������� ����� ������ � ����� ������ � ����
   ;��������: edi - ��������� �� ������, eax(al) - ����� ������/����
   ;��������: edi - ��������� �� ������ � ����
   .addLastSymbol:
      push eax
      push ecx
      call koZeroString.size
      inc ecx
      call koZeroString.resize
      mov byte[edi+ecx], al
      pop ecx
      pop eax
      ret


   ;������� ��������� ������ ������ ����� ����
   ;��������: edi - ����� ������
   ;��������: edi - ����� ������ � �������� ��������� ��������
   .delLastSymbol:
      push ecx
      call koZeroString.size
      dec ecx
      call koZeroString.resize
      pop ecx
      ret


   ;����� ���������� ������� ������ (����� ����������� ����)
   ;��������: edi - ����� ������
   ;��������: esi - ����� ���������� ������� ������
   .adrLastSymbol:
      cmp byte[edi], 0
      jne @f
      mov esi, edi
      ret
   @@:
      mov esi, edi
   @@:
      cmp byte[esi], 0
      je @f
      inc esi
      jmp @b
   @@:
      dec esi
      ret


   ;�������� ����� ������ � ������������ ����� ������ � ����������� ����
   ;��������: edi - ����� ������, eax (al) - ����� ������, ebx - ������� ������� � ������ (�� ����)
   ;��������: edi - ����� ����� ������ �� ����������� ��������
   .insertSymbol:
      push ebx
      push ecx
      call koZeroString.size
      cmp ebx, ecx
      jbe @f
      mov ebx, ecx   ;���� ������ ebx ������ ������, �� ������ ��������� � ����� (�� ������� ����� ����)
    @@:
      inc ecx
      call koZeroString.resize
      call koZeroString.adrLastSymbol
      mov byte[esi+1], 0       ;����� ���� � ����� ����� ������
      dec esi

      mov ecx, edi
      add ecx, ebx

      push eax
      mov eax, 0
   @@:
      cmp esi, ecx
      jb @f
      mov al, byte[esi]
      mov byte[esi+1], al
      dec esi
      jmp @b
   @@:
      pop eax
      mov byte[esi], al
      pop ecx
      pop ebx
      ret


   ;������� ������ �� ������ �� ��� ������-������� (���� ������ ������ ������� ������, �� �� ����� ������)
   ;��������: edi - ����� ������, ebx - ������� ������� � ������ (�� ����)
   ;��������: edi - ����� ����� ������ � �������� ��������
   .removeSymbol:
      push eax
      push ebx
      push ecx
      call koZeroString.size
      dec ecx
      cmp ebx, ecx
      jbe @f
      mov ebx, ecx   ;���� ������ ebx ������ ������, �� ������� ��������� ������
    @@:
      add ebx, edi
      push esi
      call koZeroString.adrLastSymbol
      mov eax, 0
    @@:
      cmp ebx, esi
      ja @f
      mov al, byte[ebx+1]
      mov byte[ebx], al
      inc ebx
      jmp @b
    @@:
      pop esi
      call koZeroString.resize
      pop ecx
      pop ebx
      pop eax
      ret


   ;�������� ���� ������� ������
   ;�������: edi - ��������� �� ������, ebx - ����� ������� � ������ �� ����  (���� ������ ������, �� ������ ��������� ������)
   ;��������: eax (al) - ����, ��� ������� ������
   .getChar:
      push ebx
      push ecx
      call koZeroString.length
      cmp ebx, ecx
      jbe @f
      mov ebx, ecx
    @@:
      add ebx, edi
      mov eax, 0
      mov al, byte[ebx]
      pop ecx
      pop ebx
      ret


   ;������������ ���� ����� (��������, �����������, ���������� ����� ������ � ����� ������)
   ;��������: edi - ������ ������, esi - ������ ������
   ;��������: edi - �������������� ������
   .concat:
      push eax
      push ebx
      push ecx
      push edx
      push esi

      call koZeroString.size
      mov eax, ecx
      mov ecx, 0
      push edi
      mov edi, esi
      call koZeroString.length
      mov edx, ecx
      inc edx
      pop edi
      add ecx, eax
      call koZeroString.resize
      dec eax
      add edi, eax
      mov eax, 0
      push edi
   @@:
      cmp edx, 0
      jbe @f
      mov al, byte[esi]
      mov byte[edi], al
      inc edi
      inc esi
      dec edx
      jmp @b
   @@:
      pop edi

      pop esi
      pop edx
      pop ecx
      pop ebx
      pop eax
      ret


   ;������ ������: ����������� ������� ����� ������
   ;��������: edi - ������
   ;��������: edi - ������-������
   .revers:
      push eax
      push ebx
      push esi
      mov eax, 0
      mov ebx, 0
      call koZeroString.adrLastSymbol
      push edi
   @@:
      cmp esi, edi
      jbe @f
      mov al, byte[esi]
      mov bl, byte[edi]
      mov byte[esi], bl
      mov byte[edi], al
      dec esi
      inc edi
      jmp @b
   @@:
      pop edi
      pop esi
      pop ebx
      pop eax
      ret


   ;������� ��������� �� ������ (������ ������ ������ �����������)
   ;��������: edi - ������, eax - ��������� ����� ������� ���������, ebx - �������� ����� ������� ���������
   ;��������: esi - ���������
   .substring:
      ret


   ;������� ��������� ������ (������ ������ ���������)
   ;��������: edi - ������, eax - ��������� ����� ������� ���������, ebx - �������� ����� ������� ���������
   ;��������: edi - ���������  (������ ������ ���������)
   .createSubString:
      ret


   ;������� ���������
   ;��������: edi - ������ ������, esi - ������ ������
   ;��������: �������� eax = 0 - ������ ������� ��� ��� ������
   ;			     1 - ������ ������ �������
   ;			     2 - ������ ������ �������
   ;			     3 - ������ ������ �����, �� ������ �������
   .fastCompare:
      ret


   ;��������� ���� zero-����� � ����� ������� �������������� �������
   ;��������: edi - ������ ������, esi - ������ ������
   ;��������: �������� eax = 0 - ������ ������� ��� ��� ������ (� ebx ����� 0)
   ;			     1 - ������ ������ ������� (��������� ��-����� ����������� �����, ����� ��������� ebx)
   ;			     2 - ������ ������ ������� (��������� ��-����� ����������� �����, ����� ��������� ebx)
   ;			     3 - ������ ������ �����, �� ������ ������� (��. ebx)
   ;	      �������� ebx = ������ ������� ��������� ������� (��� ����� ���� ����������� ����, ���� ���� �� ����� ������, � ������� �� �������)
   .compare:
      call koZeroString.fastCompare
      mov ebx, 0
      cmp eax, 0
      je @f
;����� ��� ���������� � ������� � ebx
    @@:
      ret


   ;������� � ������ ����� ������ � ����������� ����
   ;��������: edi - ����� ������������ ������
   ;��������: esi - ����� ������ �����
   .copy:
      push eax
      push ebx
      push edi
      mov eax, edi
      call koZeroString.size

      call koZeroString.create
      call koZeroString.resize
      mov esi, edi
   @@:
      cmp ecx, 0
      jbe @f
      mov bl, byte[eax]
      mov byte[edi], bl
      inc eax
      inc edi
      dec ecx
      jmp @b
   @@:
      pop edi
      pop ebx
      pop eax
      ret


   ;���������� ����� � ���� ��� ����
   ;��������: edi - ����� ������, ebx - ���������� � ����, ecx - ����, eax - ��������� ������� (�� �������� 0=�1 �� 7=�8)
   ;��������: ���
   .fullDrawInWindow:
      push eax
      push ebx
      push ecx
      push edx
      push esi
      push edi

      shl eax, 24+5
      shr eax, 24+5	      ;�������� ������������ ��������� �������

      shl eax, 24	      ;�������� ��������� ������� �� ��� �������

      shl ecx, 8	      ;�������� ������ ���� � ����� (����� ������� ���� ��������)
      shr ecx, 8
      add eax, ecx	      ;���� ������ � ���������� �������
      push eax
      mov edx, edi	      ;����� ����� ������
      call koZeroString.length
      mov esi, ecx	      ;����� ������ ������
      pop eax
      mov ecx, eax	      ;����� ���� � ��������� ������ ������
      mov edi, 0
      mcall 4		      ;������ �����

      pop edi
      pop esi
      pop edx
      pop ecx
      pop ebx
      pop eax
      ret


   .subDrawInWindow:
      ret
   .fullDrawInView:
      ret
   .subDrawInView:
      ret

   ;������������� ������ � ����� �����
   .toInteger:
      ret
   ;������������� ����� � ������, ������� ���������� ������ ���������
   .fromInteger:
      ret
   ;�� ������ �������� �� zero string � ������
   .fromZeroString:
      ret
   .split:
      ret
   .width:
      ret
   .height:
      ret
   .findSymbol:
      ret
   .findSubString:
      ret
   .replaceSymbol:
      ret
   .replaceSubString:
      ret


   ;������� ������� � ��������� � ������ ������
   ;��������: edi - ������
   ;���������: edi - ������ ��� �������� � ��������� � ������
   .leftTrim:
      ret


   ;������� ������� � ��������� � ����� ������
   ;��������: edi - ������
   ;���������: edi - ������ ��� �������� � ��������� � ����� ������
   .rightTrim:
      ret


   ;������� ������� � ��������� � ������ � � ����� ������
   ;��������: edi - ������
   ;���������: edi - ������ ��� �������� � ��������� � ������ � ����� ������
   .trim:
      call koZeroString.leftTrim
      call koZeroString.rightTrim
      ret




koString:
   ;������� ������ ������ � ������� ��������
   ;��������: ���
   ;���������: edi - ����� ��������� koString
   .create:
      push eax
      push ebx
      push ecx
      mcall 68, 12, 12
      mov edi, eax
      mov dword[edi], 0
      mov dword[edi+4], 0
      mov dword[edi+8], 0
      pop ecx
      pop ebx
      pop eax
      ret


   ;������� ������ (������ � ���������� �������� - ���� ��� ����)
   ;��������: edi - ����� ��������� koString
   ;���������: ���
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


   ;���������� ������ ������
   ;��������: edi - ����� ��������� koString, eax - ��������� ������, ebx - ��������� �� ������� ������ ��� 0, ecx - ����� ������ ��� 0
   ;���������: ���
   .setString:
      ret


   ;�������� ������ ������: ������� �������� ��� ��������� ������ ���������� ������� � �����
   ;��������: edi - ����� ��������� koString
   ;���������: ���
   .resize:
      ret


   ;�������� ��������� koString - �.�. ������� ����
   ;��������: eax - ����� ���������� ������� �����, ebx - ����� ��������� ������� �����, edi - ����� ��������� koString
   ;���������: esi - ����� ��������� koString � ���������� ������� ����� (� ����������)
   .substring:
      ret


   ;������� ����� ������
   ;��������: edi - ����� ��������� koString
   ;���������: esi - ����� ����� ��������� koString
   .copy:
      ret


   ;������ ������ (�� ������ ����� ������, � �������� � ���������� ����������)
   ;��������: edi - ����� ��������� koString
   ;���������: ���
   .revers:
      ret


   ;������ ���������� ������� � ������ ������(�� ������ ����� ������, � �������� � ���������� ����������)
   ;��������: edi - ����� ��������� koString
   ;���������: ���
   .leftTrim:
      ret


   ;������ ���������� ������� � ����� ������(�� ������ ����� ������, � �������� � ���������� ����������)
   ;��������: edi - ����� ��������� koString
   ;���������: ���
   .rightTrim:
      ret


   ;������ ���������� ������� � ������ � ����� ������(�� ������ ����� ������, � �������� � ���������� ����������)
   ;��������: edi - ����� ��������� koString
   ;���������: ���
   .trim:
      call koString.leftTrim
      call koString.rightTrim
      ret


   ;������������� ������ � ����� �����
   .toInteger:
      ret
   ;������������� ����� � ������, ������� ���������� ������ ���������
   .fromInteger:
      ret
   .split:
      ret
   .replace:
      ret
   .concat:
      ret
   .find:
      ret
   .changeEncoding:
      ret
   .print:
      ret
   .addSymbol:
      ret
   .addString:
      ret
   .loadFromZeroString:
      ret
   .saveInZeroString:
      ret
   .height:
      ret
   .width:
      ret


;--------------------------------------------
;(�) ���������� ������ �., 2017