;koStyle
;������ � ����������� � ����������� ���������, � ��� �� �������� ��������� ��������� ������� ��������


;----------------------------------------------------------------------------------------------------
; ��� ��� ������ koButton.inc
;----------------------------------------------------------------------------------------------------

;�������� ��������� ������:
;   0 - �����������, ������ �������
;   1 - ������� ������
;   2 - ������ ������
;   3 - ���������� ������
;   4 - ����� ������ (�������� TAB)
;   5 - ����������� ������� ���� ��� �������
;   6 - ������ ���������


koButtonBackgroundColors:
   .state_0   dd  0xF0E6ED ;���� ������� ������
   .state_1   dd  0x663E5A ;0x663E5A
   .state_2   dd  0x663E5A
   .state_3   dd  0xF0E6ED ;0x663E5A
   .state_4   dd  0xB784A7
   .state_5   dd  0xB784A7
   .state_6   dd  0xFFFFFF ;���� ���������� ������
koButtonTextColors:
   .state_0   dd  0x000000   ;���� ������ � �������� ���������
   .state_1   dd  0x000000
   .state_2   dd  0x000000
   .state_3   dd  0x000000
   .state_4   dd  0x000000
   .state_5   dd  0x000000
   .state_6   dd  0xB3B3B3   ;���� ������ � ���������� ��������� ������


;��������� ������
koButtonSettings:   ;������� ����� ��������� � koObjectList
   .unpress_color    dd 0xF0E6ED ;���� ������� ������
   .press_color      dd 0x663E5A ;���� ������� ������
   .select_color     dd 0xB784A7 ;���� ����������/��������� ������
   .deactive_button  dd 0xFFFFFF ;���� ���������� ������
   .text_active_color	dd 0x000000 ;���� ������ � �������� ���������
   .text_deactive_color dd 0xB3B3B3 ;���� ������ � ���������� ��������� ������


;������� ��������� ������ ������� ������: � ������ ��� ������� ������ ��������� ������� ������� ������ � � ������������ � ���� ��������
;��������: edi - ��������� �� ��������� ������, esi - ��������� �� ���� ������ ������ (�� ������������ � ���� ������)
koButtonDraw:
   push esi
   push edi
;   cmp esi, 0	     ;���� 0, �� ������ ������ � ������������ ����������� ������
;   jne @f	     ;�������� ��������� �� �������������� - �������

   ;��������� � ����������� �������
   push eax
   push ebx
   push ecx
   mov eax, dword[edi+8]				 ;����� ��������� ������
   ;� ����������� �� ��������� ������������� �����

   ;������ ������:
   push esi
   mov esi, dword[edi+4]				 ;�����
   mov edi, dword[edi]					 ;������(koView) ������

   ;mov ecx, dword[koButtonSettings.unpress_color]
   call koButtonSetBackgroundColor			 ;���� ����
   call koView.clear					 ;����������� ���

   ;mov eax, dword[koButtonSettings.text_active_color]
   call koButtonSetTextColor				 ;���� ������
   ;����� ������� ���� �  ebx �������� ���-�� �������� � ������
   mov ebx, 7 ;����� refresh
   call setCoordInCenterForView
  ; mov ebx, 0*65536+0					 ;���������� ������
   mov edx, esi 					 ;��������� �� ����� � ������� ���������
   call koView.drawText 				 ;������ �����

   pop esi

   pop ecx
   pop ebx
   pop eax
   jmp .koButtonDrawEnd
@@:		    ;��� ������ ���� ��������� ����� � ��������� � ��� �������

.koButtonDrawEnd:
   pop edi
   pop esi
   ret


;���������� � eax ���� ������ ��� �������� ��������� ������
;�������: eax - ��������� ������
;��������: eax - ����
koButtonSetTextColor:
   push ebx
   shl eax, 2
   mov ebx, koButtonTextColors
   add ebx, eax
   mov eax, dword[ebx]
   pop ebx
   ret


;���������� � ecx ���� ���� ��� �������� ��������� ������
;��������: eax - ��������� ������
;��������: ecx - ����
koButtonSetBackgroundColor:
    push eax
    shl eax, 2	;�������� �� 4
    mov ecx, koButtonBackgroundColors
    add ecx, eax
    mov ecx, dword[ecx]
    pop eax
    ret

;----------------------------------------------------------------------------------------------------




;----------------------------------------------------------------------------------------------------
; ��� ��� ������ .inc
;----------------------------------------------------------------------------------------------------

;----------------------------------------------------------------------------------------------------