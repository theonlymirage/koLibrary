;koTab
;Вкладка
;Version 0.1: May 17, 2017   (от 17.05.2017)

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

;Структура koTab:
;+0    view    dd  0 ;указатель на koView вкладки
;+4    text    dd  0 ;указатель на текст вкладки koString
;+8    status  dd  0 ;текущее состояние
;+12   handler dd  0 ;указатель на список обработчиков состояний
;+16   draw    dd  0 ;указатель на функцию стилевой отрисовки вкладки в текущем состоянии (см. koStyle)
;+20   data    dd  0 ;указатель на блок данных для режима работы: рабочая директория (путь), стиль отображения директории и тд


;Значения состояний вкладки:
;   0 - бездействие, вкладка не выбрана, но активна
;   1 - кладка выбрана
;   2 - наведение и выбор клавиатурой (например TAB)
;   3 - перемещение курсора мыши над вкладкой
;   4 - вкладка неактивна


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
;(с) Ефременков Сергей В., 2017