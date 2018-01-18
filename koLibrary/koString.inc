;koString
;Работа со строками
;Version 0.1: May 14, 2017   (от 14.05.2017)

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

;Структура koZeroString:
;Это ASCII строка, оканчивающаяся нулём (db 0). Один символ такой строки кодируется одним байтом.


;Структура строк koString:
;+0   encoding	  dd  0 ;кодировка строки (пока поддерживается три вида: 1 cp866, 2 utf16LE, 3 utf8)
;+4   length	  dd  0 ;длина строки в символах
;+8   symbols	  dd  0 ;указатель на символы строки в нужной кодировке
;размер шрифта

koZeroString:
   ;создать пустую строку с нулём на конце
   ;входящие: нет
   ;выходные: edi - адрес со строкой
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


   ;удалить строку, оканчивающуюся нулём
   ;входящие: edi - адрес буфера со строкой
   ;выходные: нет
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


   ;изменить размер буфера строки с нулём на конце
   ;входные: edi - адрес строки, ecx - новая длина строки (в символах, без последнего завершающего нуля)
   ;выходные: edi - адрес строки с новым размером
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


   ;длина строки в символах
   ;входящие: edi - адрес начала строки
   ;выходные: ecx - кол-во символов в строке
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


   ;размер строки с байтах (учитывается последний ноль)
   ;входные: edi - указатель на строку
   ;выходные: ecx - размер в байтах
   .size:
      call koZeroString.length
      inc ecx
      ret


   ;добавить новый символ к концу строки с нулём
   ;входящие: edi - указатель на строку, eax(al) - новый символ/байт
   ;выходные: edi - указатель на строку с нулём
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


   ;удалить последний символ строки перед нулём
   ;входящие: edi - адрес строки
   ;выходные: edi - адрес строки с удалённым последним символом
   .delLastSymbol:
      push ecx
      call koZeroString.size
      dec ecx
      call koZeroString.resize
      pop ecx
      ret


   ;адрес последнего символа строки (перед завершающим нулём)
   ;входящие: edi - адрес строки
   ;выходные: esi - адрес последнего символа строки
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


   ;вставить новый символ в произвольное место строки с завершающим нулём
   ;входящие: edi - адрес строки, eax (al) - новый символ, ebx - позиция символа в строке (от нуля)
   ;выходные: edi - адрес новой строки со вставленным символом
   .insertSymbol:
      push ebx
      push ecx
      call koZeroString.size
      cmp ebx, ecx
      jbe @f
      mov ebx, ecx   ;если задали ebx больше строки, то символ добавляем в конец (на текущее место нуля)
    @@:
      inc ecx
      call koZeroString.resize
      call koZeroString.adrLastSymbol
      mov byte[esi+1], 0       ;кладём ноль в конец новой строки
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


   ;удалить символ из строки по его номеру-индексу (если индекс больше размера строки, то из конца строки)
   ;входящие: edi - адрес строки, ebx - позиция символа в строке (от нуля)
   ;выходные: edi - адрес новой строки с удалённым символом
   .removeSymbol:
      push eax
      push ebx
      push ecx
      call koZeroString.size
      dec ecx
      cmp ebx, ecx
      jbe @f
      mov ebx, ecx   ;если задали ebx больше строки, то удаляем последний символ
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


   ;получить байт символа строки
   ;входные: edi - указатель на строку, ebx - номер символа в строке от нуля  (если больше строки, то вернёт последний символ)
   ;выходные: eax (al) - байт, код символа строки
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


   ;конкатенация двух строк (сложение, объединение, добавление одной строки в конец другой)
   ;входящие: edi - первая строка, esi - вторая строка
   ;выходные: edi - результирующая строка
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


   ;реверс строки: переставить символы задом наперёд
   ;входящие: edi - строка
   ;выходные: edi - реверс-строка
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


   ;создать подстроку из строки (полная старая строка сохраняется)
   ;входящие: edi - строка, eax - начальный номер символа подстроки, ebx - конечный номер символа подстроки
   ;выходные: esi - подстрока
   .substring:
      ret


   ;создать подстроку строки (старая строка удаляется)
   ;входящие: edi - строка, eax - начальный номер символа подстроки, ebx - конечный номер символа подстроки
   ;выходные: edi - подстрока  (старая строка удаляется)
   .createSubString:
      ret


   ;быстрое сравнение
   ;входящие: edi - первая строка, esi - вторая строка
   ;выходные: значение eax = 0 - строки совпали или обе пустые
   ;			     1 - первая строка длиннее
   ;			     2 - вторая строка длиннее
   ;			     3 - строки равной длины, но разные символы
   .fastCompare:
      ret


   ;сравнение двух zero-строк и номер первого несовпадающего символа
   ;входящие: edi - первая строка, esi - вторая строка
   ;выходные: значение eax = 0 - строки совпали или обе пустые (в ebx будет 0)
   ;			     1 - первая строка длиннее (сравнение всё-равно произведено будет, чтобы заполнить ebx)
   ;			     2 - вторая строка длиннее (сравнение всё-равно произведено будет, чтобы заполнить ebx)
   ;			     3 - строки равной длины, но разные символы (см. ebx)
   ;	      значение ebx = номеру первого отличного символа (это может быть завершающий ноль, если одна из строк короче, а символы до совпали)
   .compare:
      call koZeroString.fastCompare
      mov ebx, 0
      cmp eax, 0
      je @f
;иначе тут сравниваем и заносим в ebx
    @@:
      ret


   ;создать в памяти копию строки с завершающим нулём
   ;входящие: edi - адрес оригинальной строки
   ;выходные: esi - адрес строки копии
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


   ;напечатать текст в окне как есть
   ;входящие: edi - адрес строки, ebx - координаты в окне, ecx - цвет, eax - множитель размера (от значения 0=х1 до 7=х8)
   ;выходные: нет
   .fullDrawInWindow:
      push eax
      push ebx
      push ecx
      push edx
      push esi
      push edi

      shl eax, 24+5
      shr eax, 24+5	      ;отсекаем недопустимые множители размера

      shl eax, 24	      ;сдвигаем множитель размера на его позицию

      shl ecx, 8	      ;обрезаем лишний байт в цвете (мусор который туда записали)
      shr ecx, 8
      add eax, ecx	      ;цвет вместе с множителем размера
      push eax
      mov edx, edi	      ;задаём адрес строки
      call koZeroString.length
      mov esi, ecx	      ;задаём размер строки
      pop eax
      mov ecx, eax	      ;задаём цвет и множитель строки текста
      mov edi, 0
      mcall 4		      ;рисуем текст

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

   ;преобразовать строку в целое число
   .toInteger:
      ret
   ;преобразовать число в строку, текущее содержимое строки удаляется
   .fromInteger:
      ret
   ;из любого похожего на zero string в памяти
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


   ;удалить пробелы и табуляцию в начале строки
   ;входящие: edi - строка
   ;выходящие: edi - строка без пробелов и табуляции в начале
   .leftTrim:
      ret


   ;удалить пробелы и табуляцию в конце строки
   ;входящие: edi - строка
   ;выходящие: edi - строка без пробелов и табуляции в конце строки
   .rightTrim:
      ret


   ;удалить пробелы и табуляцию в начале и в конце строки
   ;входящие: edi - строка
   ;выходящие: edi - строка без пробелов и табуляции в начале и конце строки
   .trim:
      call koZeroString.leftTrim
      call koZeroString.rightTrim
      ret




koString:
   ;создать пустую строку с нулевым размером
   ;входящие: нет
   ;выходящие: edi - адрес структуры koString
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


   ;удалить строку (вместе с содержимым символов - если они есть)
   ;входящие: edi - адрес структуры koString
   ;выходящие: нет
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


   ;установить данные строки
   ;входящие: edi - адрес структуры koString, eax - кодировка строки, ebx - указатель на символы строки или 0, ecx - длина строки или 0
   ;выходящие: нет
   .setString:
      ret


   ;изменить размер строки: символу урежутся или добавятся пустые пробельные символы в конце
   ;входящие: edi - адрес структуры koString
   ;выходящие: нет
   .resize:
      ret


   ;получить подстроку koString - т.е. сделать срез
   ;входящие: eax - номер начального символа среза, ebx - номер конечного символа среза, edi - адрес структуры koString
   ;выходящие: esi - адрес структуры koString с содержимым нужного среза (с подстрокой)
   .substring:
      ret


   ;сделать копию строки
   ;входящие: edi - адрес структуры koString
   ;выходящие: esi - адрес новой структуры koString
   .copy:
      ret


   ;реверс строки (не создаёт новую строку, а работает с содержимым переданной)
   ;входящие: edi - адрес структуры koString
   ;выходящие: нет
   .revers:
      ret


   ;убрать пробельные символы в начале строки(не создаёт новую строку, а работает с содержимым переданной)
   ;входящие: edi - адрес структуры koString
   ;выходящие: нет
   .leftTrim:
      ret


   ;убрать пробельные символы в конце строки(не создаёт новую строку, а работает с содержимым переданной)
   ;входящие: edi - адрес структуры koString
   ;выходящие: нет
   .rightTrim:
      ret


   ;убрать пробельные символы в начале и конце строки(не создаёт новую строку, а работает с содержимым переданной)
   ;входящие: edi - адрес структуры koString
   ;выходящие: нет
   .trim:
      call koString.leftTrim
      call koString.rightTrim
      ret


   ;преобразовать строку в целое число
   .toInteger:
      ret
   ;преобразовать число в строку, текущее содержимое строки удаляется
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
;(с) Ефременков Сергей В., 2017