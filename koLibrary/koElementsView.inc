;koElementsView
;������ ��������� ���������

;��������� koElementsView:
;+0    view	   dd	0 ;��������� �� ������ koView, � ������� ������ ����
;+4    elems	   dd	0 ;������ ���������� �� �������� �������� (koObjectList)  -��� ���� � ���� ����� �������� � ���������� � ���� ���������
;+8    elem_types  dd	0 ;������ ����� �������� ���������	  (koObjectList)
;+12   count	   dd	0 ;���-�� �������� ���������
;+16		   dd	0 ;������ �������������� �������� ��������� ����� ����� (�� ������������)


koElementsView:
   .create:
      ret


   .delete:
      ret


   .fullDrawInWindow:
      ret


   .fullDrawInView:
      ret


   .addElement:
      ret


   .delElement:
      ret


   .hideElement:
      ret


   .showElement:
      ret


   ;��������� ������� �� �������� ����� ���� ���-�� �������� ������ ���������� - ����� ������ ���� � ���������� - ����� ��� � �����
   .mouse:
      ret


   .keyboard:
      ret