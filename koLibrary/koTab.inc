;koTab
;�������
;Version 0.1: May 17, 2017   (�� 17.05.2017)

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

;��������� koTab:
;+0    view    dd  0 ;��������� �� koView �������
;+4    text    dd  0 ;��������� �� ����� ������� koString
;+8    status  dd  0 ;������� ���������
;+12   handler dd  0 ;��������� �� ������ ������������ ���������
;+16   draw    dd  0 ;��������� �� ������� �������� ��������� ������� � ������� ��������� (��. koStyle)
;+20   data    dd  0 ;��������� �� ���� ������ ��� ������ ������: ������� ���������� (����), ����� ����������� ���������� � ��


;�������� ��������� �������:
;   0 - �����������, ������� �� �������, �� �������
;   1 - ������ �������
;   2 - ��������� � ����� ����������� (�������� TAB)
;   3 - ����������� ������� ���� ��� ��������
;   4 - ������� ���������


koTab:
   .create:
      ret
   .delete:
      ret
   .draw:
      ret
   .mouse:
      ret
   .keyboard:
      ret


;--------------------------------------------
;(�) ���������� ������ �., 2017