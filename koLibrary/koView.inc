;Kolibri View  - Basic visual component for KolibriOS
;Базовый визуальный компонент для КолибриОС
;Version 0.1: May 11, 2017   (от 11.05.2017)

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

;Структура koView:
;   coord    dd   0 ;координаты Х и Y
;   width    dd   0 ;ширина
;   height   dd   0 ;высота
;   pixels   dd   0 ;указатель на буфер с пикселями (32 бита)
;Структура буфера с пикселями изображения:
;   width    dd   0 ;ширина
;   height   dd   0 ;высота
;   pixels   db 0 dup(4*width*height)  ; пиксели в формате AARRGGBB размер области 4*W*H

koView:
   ;конвертировать в изображение: создаёт буфер с картинкой для текущего koView (в edi) в ebx возвращает буфер с картинкой
   .createImage:
      push eax
      push ecx
      push edx

      mov eax, dword[edi+4]    ;вычисляем размер
      mov ebx, dword[edi+8]
      mov edx, 0
      mul ebx
      mov edx, 0
      mov ebx, 3
      mul ebx
      mov ecx, eax

      mcall 68, 12
      mov ebx, eax


      push ebx		      ;копируем пиксели AARRGGBB -> BBGGRR
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


   ;удаляет буфер с картинкой по адресу ebx
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


   ;отрисовать Kolibri View по указателю edi целиком как есть в текущем окне
   .fullDrawInWindow:
      push eax
      push ebx
      push ecx
      push edx
      push edi
      call koView.createImage	 ;конвертируем в изображение в буфере BBGGRR
      mov ecx, 0		 ;задаём размеры
      mov ecx, dword[edi+4]
      shl ecx, 16
      add ecx, dword[edi+8]

      mov edx, dword[edi]	 ;задаём координаты
      mcall 7			 ;отрисовываем полученное изображение
      call koView.deleteImage	 ;удаляем буфер с изображением
      pop edi
      pop edx
      pop ecx
      pop ebx
      pop eax
      ret

;---
   ;создать буфер с изображением : Внимание тут нет проверок, поэтому область строго должна быть внутри koView (все её точки)
   ;принимает: edi - koView, eax - координаты области в koView, ecx - размеры области в koView
   ;возвращает: ebx - адрес на буфер с изображением
   .createSubImage:
      push eax
      push ecx
      push edi
      push esi

      push eax
      push ecx		 ;вычисляем размер для буфера
      push edx
      mov edx, 0
      mov eax, ecx
      shr eax, 16	 ;eax=Width
      shl ecx, 16
      shr ecx, 16	 ;ecx=Height
      mul ecx
      mov edx, 0
      mov ecx, 3
      mul ecx		 ;теперь в eax - размер буфера для картинки в байтах
      mov ecx, eax	 ;выделяем память под буфер
      mcall 68, 12
      mov ebx, eax	 ;перекладываем адрес буфера в ebx
      pop edx
      pop ecx
      pop eax


      push eax
      push ecx		 ;вычисляем кол-во пропускаемых пикселей в байтах в edx
      mov edx, 0
      shr ecx, 16	 ;ecx=Width
      mov eax, dword[edi+4]
      sub eax, ecx
      shl eax, 2	 ;тут нужно переписать и улучшить код
      mov edx, eax	 ;теперь в edx - кол-во пропускаемых байт
      pop ecx
      pop eax


;вычисляем начальный адрес edi
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
pop ecx 		    ;eax = кол-во пикселей в высоту

shr ecx, 16		    ;ecx = размер в ширину

;edi - указатель на начало копируемых пикселей
;eax - размер в высоту в пикселях
;ebx - указатель на начало области картинки
;ecx - размер в ширину в пикселях
;edx - пропускаемые байты после копирования одной строки пикселей

push ebx
.start_copy_sub_image:
   cmp eax, 0
   jbe @f

   push ecx
.copy_sub_image:
   cmp ecx, 0
   jbe .end_copy_sub_image

;копируем пиксели из edi в ebx
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


   ;удаляет буфер с картинкой по адресу ebx
   .deleteSubImage:
      call koView.deleteImage
      ret

   ;отрисовать только нужную часть koView в окне: edi - указатель на koView, eax - координаты внутри koView, ecx - размеры внутри koView
   .subDrawInWindow:
      push eax
      push ebx
      push ecx
      push edx
      push edi
      call koView.createSubImage    ;конвертируем в изображение в буфере BBGGRR с размерами ecx
      mov edx, dword[edi]	    ;задаём координаты
      mcall 7			    ;отрисовываем полученное изображение
      call koView.deleteSubImage    ;удаляем буфер с изображением
      pop edi
      pop edx
      pop ecx
      pop ebx
      pop eax
      ret
;---


   ;нарисовать прямоугольник:
   .drawRect:  ;eax - координаты, ebx - размеры по ширине и высоте, ecx - цвет, edi - указатель на koView; ничего не возвращает
      push eax
      push ebx
      push ecx
      push edx
      push edi
      push esi
push eax


push ebx		;изменяем координаты на относительные
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


call koView.pointInView ;если координата за пределами области, то нечего рисовать
cmp eax, 0
jne .end_draw_rect
pop eax
      push eax


push ecx			  ;обрезаем по высоте
mov ecx, dword[edi+8]
sub cx, ax
cmp cx, bx
jae .height_full
mov bx, cx
.height_full:
pop ecx


push eax
push edx			      ;обрезаем по ширине
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
      shr ecx, 16	    ;ecx - координата Х

      shl eax, 16
      shr eax, 16	    ;eax - координата Y
      mov ebx, dword[edi+4] ;ebx - размер koView
      mov edx, ebx
      shl edx, 2	    ;edx - размер ширины koView в байтах
      push edx
      mov edx, 0
      mul ebx
      pop edx
      add eax, ecx	    ;eax = (Xpoint + Ypoint * Width)
      shl eax, 2	    ; умножаем на 4

      mov edi, dword[edi+12]
      add edi, 8

      add edi, eax	    ;edi - начало отрисовки прямоугольника
      pop ebx
      ;edx - ширина koView, ebx - ширина и высота прямоугольника
      mov eax, ebx
      shr eax, 16	    ;eax - ширина прямоугольника
      shl ebx, 16
      shr ebx, 16	    ;ebx - высота прямоугольника

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


   ;задать цвет(=ecx) точки/пикселя(координаты в eax) на Kolibri View (указатель в edi)
   .drawPoint:
      ;ДОБАВИТЬ ПРОВЕРКУ - точка внутри koView?
      ;если да, то рисуем ; иначе ничего не нужно делать

      ;Указатель на точку в буфере изображения koView: (Xpoint + Ypoint * Width) * 4 + 8
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
      shl eax, 2      ;умножаем на 4

      mov edi, dword[edi+12]
      add edi, 8
      add edi, eax
      mov dword[edi], ecx

      pop edi
      pop edx
      pop ebx
      pop eax
      ret


   ;нарисовать картинку: eax - координаты, esi - указатель на картинку, edi - указатель на koView
   .drawImage:
      push eax
      push edx
      push esi
      push edi

;тут надо ПРОВЕРИТЬ И НАЧИНАТЬ РИСОВАТЬ ТОЛЬКО если точка внутри koView

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
      shl edx, 2	 ;edx = кол-во байт в строке пикселей koView

      mov ebx, dword[edi+8]    ;высота в пикселях koView - потребуется дальше по коду

      mov edi, dword[edi+12]   ;вычисляем стартовые позиции koView
      add edi, 8
      add edi, eax
      pop eax


      push eax
      shl eax, 16
      shr eax, 16
      sub ebx, eax
      pop eax
      cmp ebx, dword[esi+4]
      jb .setHeightImage    ;если меньше, то устанавливаем высоту по значению в ebx
      mov ebx, dword[esi+4] ;иначе ставим полную высоту картинки
   .setHeightImage:; теперь ebx = высота в пикселях картинки


      mov ecx, dword[esi]   ;ecx = ширина в пикселях картинки
      add esi, 8	  ;вычисляем стартовые позиции картинки

      push edx
      push ebx
      shr edx, 2     ;кол-во пикселей в ширину koView
      shr eax, 16    ;Xpoint
      sub edx, eax
      mov eax, 0
      cmp edx, ecx; dword[esi]
      jae @f
      mov eax, ecx ;dword[esi]
      sub eax, edx
      mov ecx, edx     ;уменьшаем ширину выводимых пикселей
      mov edx, 0
      mov ebx, 3
      mul ebx	       ;eax = кол-во пропускаемых байт в картинке после каждой рисуемой строки пикселей
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


   ;отобразить текст (усовершенствовать)
   .drawText:  ;входные: edi - указатель на буфер koView, eax - цвет, ebx - координаты текста, edx - указатель на начало строки (оканчивается нулём)
      push eax ;ничего не возвращает
      push ecx
      push edi
      push esi

      shl eax, 8   ;очищаем данные о прозрачности цвета (тк она не поддерживается)
      shr eax, 8
      push eax

      mov ecx, 0;1     ;добавляем бит окончания строки нулём
      shl ecx, 2
      mov eax, dword[fonts.number]	   ;добавляем шрифт и кодировку
      add cl, al ;cx, ax
      shl ecx, 1;2
      inc cx	     ;устанавливаем бит отрисовки в буфер
      shl ecx, 3
      shl ecx, 24
      pop eax
      add ecx, eax   ;добавляем цвет

      mov eax, dword[edi+4]
      push ebx
      shr ebx, 16
      sub eax, ebx
      pop ebx
      call getMaxSymbolsCount
      mov esi, eax   ;длина строки (сколько влезет символов столько и печатаем)

      mov edi, dword[edi+12]
      mcall 4

      pop esi
      pop edi
      pop ecx
      pop eax
      ret


   ;очистить koView (указатель в edi) в цвет ecx
   .clear:
      push eax
      push ebx
      push ecx
      push esi
      mov eax, dword[edi+4]    ;вычисляем размер текущего буфера в пикселях
      mov ebx, dword[edi+8]
      mov edx, 0
      mul ebx

      mov esi, dword[edi+12]
      add esi, 8	       ;пропускаем поля ширины и высоты в буфере

   .clear_cicle:	       ;в цикле
      cmp eax, 0
      jbe .end_clear_cicle
      mov dword[esi], ecx      ;записываем цвет
      add esi, 4	       ;переходим на следующий пиксель
      dec eax
      jmp .clear_cicle
   .end_clear_cicle:

      pop esi
      pop ecx
      pop ebx
      pop eax
      ret


   ;изменить размеры: edi - koView с уже изменёнными новыми значениями
   .resize:
      push eax
      push ebx
      push ecx
      push edx
      mov ecx, dword[edi+12]
      mcall 68, 13	       ;удаляем текущий буфер
      mov eax, dword[edi+4]    ;вычисляем размер нового буфера
      mov ebx, dword[edi+8]
      mov edx, 0
      mul ebx
      shl eax, 2	       ;умножаем на 4
      add eax, 8	       ;в начале буфера есть поля ширины и высоты
      mov ecx, eax
      mcall 68, 12	       ;создаём новый буфер
      mov dword[edi+12], eax   ;записываем в структуру адрес-указатель на новый буфер

      mov edx, dword[edi+4]    ;записываем в поля ширины и высоты буфера значения
      mov dword[eax], edx
      mov edx, dword[edi+8]
      mov dword[eax+4], edx
      pop edx
      pop ecx
      pop ebx
      pop eax
      ret


   ;создать объект в памяти возвращает: edi - новый объект koView
   .create:
      push eax
      push ebx
      push ecx
      mcall 68, 12, 32
      mov edi, eax
      call koView.resize ;инициализируем и устанавливаем нулевой размер
      pop ecx
      pop ebx
      pop eax
      ret


   ;удалить объект из памяти: указатель в edi
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



   .pointInView: ;точка внутри области koView или нет?	  (координаты точки должны быть относительно того же объекта что и koView)
      ;входящие: eax - координаты точки, edi - указатель на структуру koView
      ;выходные: eax = 0 - точка над вьювером, 1 - нет, точка не над вьювером
      push ebx
      push ecx
      push edx
      mov ebx, 0;ebx - Xточки
      mov ecx, 0;ecx - Хвьювера
      mov edx, 0;edx - WidthView

      push eax
      shr eax, 16
      mov bx, ax	  ;Xточки
      mov cx, word[edi]   ;Хвьювера
      mov edx, dword[edi+4] ;WidthView
      pop eax

      cmp bx, cx;Хточки >= Хвьювера
      jb @f ;если да, то проверяем дальше, иначе точка не во вьювере - переход на конец

      sub bx, cx
      cmp bx, dx;Хточки-Хвьювера   <= Width вьювера
      ja @f ; если да, то проверяем дальше, иначе точка не во вьювере

      push eax
      shl eax, 16
      shr eax, 16
      mov bx, ax	   ;Yточки
      mov cx, word[edi+2]  ;Yвьювера
      mov edx, dword[edi+8]  ;Height вьювера
      pop eax

      cmp bx, cx;Yточки >= Yвьювера
      jb @f; если да, то проверяем дальше, иначе точка не во вьювере

      sub bx, cx
      cmp bx, dx ;Yточки - Yвьювера <= Height вьювера
      ja @f; если да, то точка внутри области вьювера, иначе точка не во вьювере
      mov eax, 0
    @@:
      pop edx
      pop ecx
      pop ebx
      ret


; реализация под вопросом:  ---- ----
    ;сдвинуть пиксели изображения koView в edi на eax влево, на ebx вправо
;    .movePixels:
;	ret
    ;скопировать определённую область koView (edi) в другой koView (esi) eax - координаты, ebx - размеры
;    .copySubView:
;	ret
   ;нарисовать линию
;   .drawLine:
;      ret
   ;нарисовать окружность
;   .drawCircle:
;      ret
   ;нарисовать koView внутри текущего koView
;   .drawKoView:
;      ret
;---- ---- ---- ---- ----

;--------------------------------------------
;(с) Ефременков Сергей В., 2017