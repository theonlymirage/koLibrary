;koObjectList
;Список объектов (список указателей на объекты)
;Version 0.1: May 13, 2017	(от 13.05.2017)

; Copyright (c) 2017, Efremenkov Sergey aka TheOnlyMirage aka Единственный Мираж
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

; THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
; INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
; PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
; HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
; OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
; SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
; --------------------------------------------------------------------------------------


;Структура списка объектов:
;+0   dd     кол-во объектов
;+4   dd     указатель на область памяти 1-ого в списке объекта
;+8   dd     указатель на область памяти 2-ого в списке объекта
;и так далее


koObjectList:
   ;создать пустой список объектов: esi - указатель на созданный список
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
   ;удалить список объектов по адресу в esi
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
   ;добавить объект в конец списка,входящие: в edi - новый добавляемый объект, в esi - текущий список; возвращает: esi - новый список
   .addObject:
      push eax
      push ebx
      push ecx
      push edx
      push edi
      mov edx, edi	     ;сохраняем указатель на новый объект

      mov ecx, dword[esi]
      inc ecx
      push ecx
      inc ecx
      shl ecx, 2	     ;умножаем на 4, чтобы получить размер в байтах
      mov ebx, ecx
      push ebx
      mcall 68, 12	     ;создаём новый объект нужного размера
      mov edi, eax
      pop ebx
      pop ecx

      mov eax, dword[esi]
      inc eax
      call copyESIinEDI      ;копируем старые данные в новый список (вместе со старым кол-вом)
      mov dword[edi], eax    ;записываем новый размер списка

      push edi
      add edi, ebx	   ;добавляем новые данные
      sub edi, 4
      mov dword[edi], edx
      pop edi

      call koObjectList.delete ;удаляем старый список
      mov esi, edi ;возвращаем указатель на новый список
      pop edi
      pop edx
      pop ecx
      pop ebx
      pop eax
      ret
   ;удалить объект из списка: edi - указатель на удаляемый объект, в esi - текущий список; возвращает: esi - новый список без удаляемого объекта
   .removeObject:
      push eax
      push ebx
      push ecx
      push edx
      push edi

      mov ecx, dword[esi]   ;размер спика = кол-во элементов
      mov eax, 0	    ;вычисляем номер удаляемого объекта - заменить отдельной функцией
   .find_object:
      cmp ecx, 0	    ;если в списке нет такого объекта или список пуст, то выходим
      jbe .end_find
      cmp esi, dword[edi]
      je .end_find
      add edi, 4
      dec ecx
      inc eax
   .end_find:		    ;теперь в eax - номер объекта в списке (счёт от 1) или ноль (если объекта нет в списке)
      cmp eax, 0
      jbe @f		    ;если такого объекта в списке нет, то мы ничего не делаем со списком

      push eax
      mov ecx, dword[esi]
      dec ecx
      mov edx, ecx
      inc ecx		    ;место под поле кол-ва объектов в списке
      shl ecx, 2	    ;умножаем на 4, чтобы получить размер в байтах
      mcall 68, 12	    ;создаём новый объект нужного размера
      mov edi, eax
      mov dword[edi], edx   ;записываем новый размер списка
      pop eax

      push esi
      push edi
      add edi, 4	    ;переходим к содержимому списков
      add esi, 4

      dec eax
      call copyESIinEDI     ;копируем старые данные до удаляемого объекта
      inc eax

      sub edx, eax
      shl eax, 2
      add esi, eax
      add edi, eax
      sub edi, 4
      mov eax, edx
      call copyESIinEDI     ;копируем старые данные после удаляемого объекта

      pop edi
      pop esi

      call koObjectList.delete ;удаляем старый список
      mov esi, edi	       ;возвращаем указатель на новый список
   @@:
      pop edi
      pop edx
      pop ecx
      pop ebx
      pop eax
      ret
   ;добавить объект в определённую позицию списка
   ;.insertobject:
   ;   ret


;Вспомогательная функция для копирования элементов: копирует n=eax элементов по 4 байта из ESI на адресс EDI
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
;(с) Ефременков Сергей В., 2017