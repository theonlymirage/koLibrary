;koProgressBar
;
;Version 0.1: May 19, 2017   (от 19.05.2017)

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




;Структура koProgressBar:
;+0    view	   dd	 0 ;вьювер (koView) для данного koProgressBar
;+4    minValue    dd	 0 ;минимальное значение шкалы
;+8    maxValue    dd	 0 ;максимальное значение шкалы
;+12   value	   dd	 0 ;текущее значение шкалы
;+16   status	   dd	 0 ;текущее состояние
;+20   handler	   dd	 0 ;указатель на список обработчиков состояний (я не использую, кто хочет может дописать аналогично кнопкам koButton)
;+24   draw	   dd	 0 ;указатель на функцию стилевой отрисовки в текущем состоянии (см. koStyle)

;Состояния koProgressBar:
; 0 - активен
; 1 - достигло финиша (Value == maxValue)
; 2 - не активен


koProgressBar:
   .create:  ;возвращает: edi - структура нового koProgressBar
      push eax
      push ebx
      push ecx
      mcall 68, 12, 28			   ;выделяем память под прогресс бар
      mov edi, eax			   ;запоминаем адрес
      pop ecx
      pop ebx
      pop eax
      ret

   .delete:  ;удаляет структуру koProgressBar по адресу edi
      push eax
      push ebx
      push ecx
      mov ecx, edi
      mcall 68, 13			   ;удаляем структуру koProgressBar
      pop ecx
      pop ebx
      pop eax
      ret

   .draw: ;вызывает метод отрисовки для структуры koProgressBar в edi
      cmp dword[edi+24], 0
      je @f
      call [edi+24]
   @@:
      ret


   ;проверить и скорректировать текущее значение шкалы
   ;изменить состояние
   ;задать значение шкалы в процентах от возможного
   ;получить текущее значение шкалы в процентах (может понадобиться стилевой отрисовке для вывода текста поверх)

;сейчас вообще не нужны, но потом как и другие элементы придумать как лучше обрабатывать (если кому-то надо):
   .mouse:     ret
   .keyboard:  ret


;--------------------------------------------
;(с) Ефременков Сергей В., 2017