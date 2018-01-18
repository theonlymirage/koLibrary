;koStyle
;Модуль с настройками и параметрами элементов, а так же базовыми функциями отрисовки каждого элемента


;----------------------------------------------------------------------------------------------------
; Код для модуля koButton.inc
;----------------------------------------------------------------------------------------------------

;Значения состояний кнопки:
;   0 - бездействие, кнопка активна
;   1 - нажатие кнопки
;   2 - кнопка нажата
;   3 - отпускание кнопки
;   4 - выбор кнопки (например TAB)
;   5 - перемещение курсора мыши над кнопкой
;   6 - кнопка неактивна


koButtonBackgroundColors:
   .state_0   dd  0xF0E6ED ;цвет обычной кнопки
   .state_1   dd  0x663E5A ;0x663E5A
   .state_2   dd  0x663E5A
   .state_3   dd  0xF0E6ED ;0x663E5A
   .state_4   dd  0xB784A7
   .state_5   dd  0xB784A7
   .state_6   dd  0xFFFFFF ;цвет неактивной кнопки
koButtonTextColors:
   .state_0   dd  0x000000   ;цвет текста в активном состоянии
   .state_1   dd  0x000000
   .state_2   dd  0x000000
   .state_3   dd  0x000000
   .state_4   dd  0x000000
   .state_5   dd  0x000000
   .state_6   dd  0xB3B3B3   ;цвет текста в неактивном состоянии кнопки


;Настройки кнопок
koButtonSettings:   ;вынести цвета состояний в koObjectList
   .unpress_color    dd 0xF0E6ED ;цвет обычной кнопки
   .press_color      dd 0x663E5A ;цвет нажатой кнопки
   .select_color     dd 0xB784A7 ;цвет выделенной/выбранной кнопки
   .deactive_button  dd 0xFFFFFF ;цвет неактивной кнопки
   .text_active_color	dd 0x000000 ;цвет текста в активном состоянии
   .text_deactive_color dd 0xB3B3B3 ;цвет текста в неактивном состоянии кнопки


;Функция отрисовки кнопки текущим стилем: в идеале эта функция должна учитывать размеры вьювера кнопки и в соответствии с ними рисовать
;входящие: edi - указатель на структуру кнопки, esi - указатель на файл стилей кнопок (не используется в этой версии)
koButtonDraw:
   push esi
   push edi
;   cmp esi, 0	     ;если 0, то рисуем кнопку в классической стандартной модели
;   jne @f	     ;стилевая отрисовка не поддерживается - выходим

   ;отрисовка в стандартном дизайне
   push eax
   push ebx
   push ecx
   mov eax, dword[edi+8]				 ;узнаём состояние кнопки
   ;в зависимости от состояния устанавливаем цвета

   ;рисуем кнопку:
   push esi
   mov esi, dword[edi+4]				 ;текст
   mov edi, dword[edi]					 ;вьювер(koView) кнопки

   ;mov ecx, dword[koButtonSettings.unpress_color]
   call koButtonSetBackgroundColor			 ;цвет фона
   call koView.clear					 ;закрашиваем фон

   ;mov eax, dword[koButtonSettings.text_active_color]
   call koButtonSetTextColor				 ;цвет текста
   ;перед вызовом надо в  ebx положить кол-во символов в строке
   mov ebx, 7 ;текст refresh
   call setCoordInCenterForView
  ; mov ebx, 0*65536+0					 ;координаты текста
   mov edx, esi 					 ;указатель на текст в текущей кодировке
   call koView.drawText 				 ;рисуем текст

   pop esi

   pop ecx
   pop ebx
   pop eax
   jmp .koButtonDrawEnd
@@:		    ;тут должна быть прогрузка стиля и отрисовка в его дизайне

.koButtonDrawEnd:
   pop edi
   pop esi
   ret


;Установить в eax цвет текста для текущего состояния кнопки
;входные: eax - состояние кнопки
;выходные: eax - цвет
koButtonSetTextColor:
   push ebx
   shl eax, 2
   mov ebx, koButtonTextColors
   add ebx, eax
   mov eax, dword[ebx]
   pop ebx
   ret


;Установить в ecx цвет фона для текущего состояния кнопки
;входящие: eax - состояние кнопки
;выходные: ecx - цвет
koButtonSetBackgroundColor:
    push eax
    shl eax, 2	;умножаем на 4
    mov ecx, koButtonBackgroundColors
    add ecx, eax
    mov ecx, dword[ecx]
    pop eax
    ret

;----------------------------------------------------------------------------------------------------




;----------------------------------------------------------------------------------------------------
; Код для модуля .inc
;----------------------------------------------------------------------------------------------------

;----------------------------------------------------------------------------------------------------