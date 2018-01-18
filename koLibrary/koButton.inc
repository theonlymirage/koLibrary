;Buttons
;Кнопки
;Version 0.1: May 15, 2017	(от 15.05.2017)

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


;Структура кнопки:
;+0    view    dd 0 ;указатель на вьювер кнопки
;+4    text    dd 0 ;указатель на текст кнопки (в будущем может использоваться указатель на структуру с параметрами текущего стиля отрисовки кнопки, это может быть указатель и на изображение кнопки)
;+8    status  dd 0 ;текущее состояние кнопки
;+12   handler dd 0 ;указатель на список обработчиков состояний
;+16   draw    dd 0 ;указатель на функцию стилевой отрисовки кнопки в текущем состоянии (см. koStyle)

;Список обработчиков состояния (по сути используется koObjectList):
;   count   dd 2 ;кол-во обрабатываемых состояний
;   hs0     dd 0 ; указатель на обработчик состояния 0
;   hs1     dd 1 ; указатель на обработчик состояния 1
;   и так далее (в зависимости от count)
;вызываются с помощью call поэтому обязаны содержать ret

;Значения состояний кнопки:
;   0 - бездействие, кнопка активна
;   1 - нажатие кнопки
;   2 - кнопка нажата
;   3 - отпускание кнопки
;   4 - выбор кнопки (например TAB)
;   5 - перемещение курсора мыши над кнопкой
;   6 - кнопка неактивна

;Основные функции работы с кнопками
koButton:
   ;создать новую кнопку
   ;входящие: eax - координаты, ebx - указатель на текст, ecx - состояние кнопки при создании, edx - указатель на функцию отрисовки состояний
   ;возвращает: edi - указатель на структуру кнопки
   .create:
      push eax
      push ebx
      push ecx
      mcall 68, 12, 20			   ;выделяем память под кнопку
      mov edi, eax			   ;запоминаем адрес
      pop ecx
      pop ebx
      pop eax

      mov dword[edi+4], ebx		   ;инициализируем полученными значениями
      mov dword[edi+8], ecx
      mov dword[edi+16], edx

      push eax
      push edi
      call koView.create		   ;создаём koView для кнопки
      mov dword[edi], eax		   ;инициализируем полученными координатами
      mov eax, edi
      pop edi
      mov dword[edi], eax
      pop eax

      call koObjectList.create		   ;создаём список обработчиков состояния
      mov dword[edi+12], esi
      ret


   ;удалить кнопку: edi - указатель на структуру кнопки
   .delete: ;код можно оптимизировать
      push eax
      push ebx
      push ecx

      push esi
      mov esi, dword[edi+12]
      call koObjectList.delete
      pop esi
      ;call koButton.removeAllStatusHandler ;удаляем все обработчики состояний

      push edi				   ;сохраняем указатель на структуру кнопки
      mov edi, dword[edi]
      call koView.delete		   ;удаляем koView кнопки
      pop edi				   ;восстанавливаем указатель на кнопку

      mov ecx, edi
      mcall 68, 13			   ;удаляем структуру кнопки
      pop ecx
      pop ebx
      pop eax
      ret


   ;изменить состояние активности кнопки в edi: если была активна, станет неактивна и наоборот
   .changeActive:
      push eax
      mov eax, dword[edi+8]
      cmp eax, 6
      je @f		    ;если неактивна, то делаем активной
      mov eax, 6    ;иначе делаем неактивной
      jmp .changeActiveEnd
   @@:
      mov eax, 0    ;делаем активной
   .changeActiveEnd:
      mov dword[esi+8], eax
      pop eax
      ret


   ;отрисовать кнопку - рисует кнопку внутри её вьювера
   ;входящие: edi - структура кнопки
   .draw:
      call dword[edi+16]
      ret


   ;изменить размеры кнопки: по умолчанию при создании размеры 0 на 0, что исключает случайные нажатия
   ;входящие: eax - новый размер ширины, ebx - новая высота кнопки, edi - структура кнопки
   .resize:
      push eax
      push ebx
      push edi
      mov edi, dword[edi]
      mov dword[edi+4], eax
      mov dword[edi+8], ebx
      call koView.resize
      pop edi
      pop ebx
      pop eax
      ret


   ;вызвать обработчик текущего состояния (если он есть)
   .callStatusHandler:
      push edi
      push eax
      mov eax, dword[edi+8]		  ;считываем текущее состояние кнопки
      inc eax				  ;увеличиваем номер так как состояния с нуля
      mov edi, dword[edi+12]		  ;переходим в список указателей
      cmp eax, dword[edi]		  ;сравниваем номер состояния с имеющимися обработчиками
      ja @f				  ;если нужного обработчика нет, то завершаем работу
      shl eax, 2			  ;высчитываем адрес в структуре списка обработчиков
      add edi, eax
      cmp dword[edi], 0 		  ;если обработчика нет (адрес 0, то значит заглушка)
      je @f				  ;то ничего не делаем
      call dword[edi]			  ;иначе вызываем обработчик состояния кнопки
   @@:
      pop eax
      pop edi
      ret


   ;изменить состояние кнопки: edi - структура кнопки, eax - новое состояние
   .changeStatus:
      mov dword[edi+8], eax	       ;изменяем состояние кнопки
      call koButton.callStatusHandler  ;и вызываем обработчик нового состояния
      ret


   ;очистить указатель на обработчик состояния (если он есть), но не удалять его из списка
   ;eax - интересующее нас состояние (от 0)
   .clearStatusHandler:
      push edi
      push eax
      inc eax				  ;увеличиваем номер так как состояния с нуля
      mov edi, dword[edi+12]		  ;переходим в список указателей
      cmp eax, dword[edi]		  ;сравниваем номер состояния с имеющимися обработчиками
      ja @f				  ;если нужного обработчика нет, то завершаем работу
      shl eax, 2			  ;высчитываем адрес в структуре списка обработчиков
      add edi, eax
      mov dword[edi], 0 		  ;очищаем указатель на обработчик состояния
   @@:
      pop eax
      pop edi
      ret


   ;добавить новый обработчик состояния - старый (если был) перезапишется новым
   ;входящие: eax - интересующее нас состояние кнопки (от 0), ebx - адрес кода обработчика, edi - указатель на структуру кнопки
   .addStatusHandler:
      push eax
      push esi
      push edi
      inc eax				  ;увеличиваем номер так как состояния с нуля
      mov edi, dword[edi+12]		  ;переходим в список указателей
      cmp eax, dword[edi]		  ;сравниваем номер состояния с имеющимися обработчиками
      ja @f				  ;если нужного обработчика нет, то нужно создавать новый список с копией старого
      shl eax, 2			  ;если обработчик уже есть, то высчитываем адрес в структуре списка обработчиков
      add edi, eax
      mov dword[edi], ebx		  ;присваиваем адрес нового обработчика
      pop edi				  ;тут пришлось это сделать иначе в конце программы из-за второй ветки условия - будет дублирование кода
      jmp .addStatusHandlerEnd		  ;задача функции выполнена - завершаем работу
   @@:
      sub eax, dword[edi]		  ;если нужного обработчика нет, то узнаем сколько не хватает обработчиков до нужного в списке
      dec eax				  ;уменьшим это число на единицу, чтобы сразу выйти затем из цикла и создать наш обработчик

      mov esi, edi			  ;инициализация значений для функции добавления в список  нового элемента
      mov edi, 0

   .start_cicle_add:
      cmp eax, 0			  ;проверяем значение eax
      jbe @f
      call koObjectList.addObject	  ;добавляем в конец списка один новый элемент
      dec eax				  ;уменьшаем счётчик
      jmp .start_cicle_add
   @@:
      mov edi, ebx
      call koObjectList.addObject	  ;добавляем в конец списка наш новый обработчик
      pop edi
      mov dword[edi+12], esi		  ;мы обновили список - нужно обновить адрес на новый список
   .addStatusHandlerEnd:
      pop esi
      pop eax
      ret


   ;удалить обработчик состояния (если он есть)
   ;в текущей реализации это не имеет смысла
   ;.removeStatusHandler:
   ;   ret


   ;удалить все обработчики состояний
   ;входящие: edi - структура кнопки
   .removeAllStatusHandler:
      push esi
      mov esi, dword[edi+12]
      call koObjectList.delete		   ;удаляем текущий список обработчиков состояний
      call koObjectList.create		   ;создаём новый список обработчиков состояния
      mov dword[edi+12], esi
      pop esi
      ret


   ;рисует кнопку в текущем окне во весь размер: данные координат и размеров берутся из вьювера кнопки
   ;входящие: edi - указатель на структуру кнопки
   .fullDrawInWindow:
      push edi
      mov edi, dword[edi]
      call koView.fullDrawInWindow
      pop edi
      ret


   ;рисует часть кнопки в текущем окне с нужными размерами и смещением
   ;данные координат для позиции внутри окна берутся из вьювера кнопки
   ;входящие: eax - координаты смещения внутри вьювера кнопки, ecx - размеры, edi - указатель на структуру кнопки
   .subDrawInWindow:
      push edi
      mov edi, dword[edi]
      call koView.subDrawInWindow
      pop edi
      ret


   ;рисует кнопку в другом koView-элементе во весь размер
   ;данные координат для позиции внутри koView (в esi) берутся из вьювера кнопки
   ;входящие: edi - структура кнопки, esi - структура koView, в котором рисуем
   .fullDrawInView:
      push eax
      push ebx
      push ecx
      push edx
      push edi
      push esi

      mov edi, dword[edi]	 ;кладём в edi вьювер кнопки
      call koView.createImage	 ;конвертируем вьювер кнопки в изображение в буфере BBGGRR
      ;теперь в ebx лежит указатель на буфер с картинкой (в начале буфера указаны размеры картинки)

      push edi
      push ebx
      mov eax, dword[edi]	 ;eax должен содержать координаты
      mov edi, esi		 ;edi должен указывать на koView, в котором рисуем кнопку
      mov esi, ebx		 ;esi должен указывать на картинку
      pop ebx
      pop edi
      call koView.drawImage	 ;отрисовываем полученное изображение

      call koView.deleteImage	 ;удаляем буфер с изображением
      pop esi
      pop edi
      pop edx
      pop ecx
      pop ebx
      pop eax
      ret


   ;рисует кнопку в другом koView-элементе с нужными размерами и смещением
   ;данные координат для позиции внутри koView (в esi) берутся из вьювера кнопки
   ;входящие: eax - координаты смещения внутри вьювера кнопки, ecx - размеры, edi - структура кнопки, esi - структура koView, в котором рисуем
   .subDrawInView:
      push eax
      push ebx
      push ecx
      push edx
      push edi
      push esi

      mov edi, dword[edi]	 ;кладём в edi вьювер кнопки
      call koView.createSubImage    ;конвертируем вьювер кнопки в изображение в буфере BBGGRR
      ;теперь в ebx лежит указатель на буфер с картинкой
      ;(в начале буфера указаны размеры картинки которые должны соответствовать размеру в ecx или быть меньше)

      push edi
      push ebx
      mov eax, dword[edi]	 ;eax должен содержать координаты
      mov edi, esi		 ;edi должен указывать на koView, в котором рисуем кнопку
      mov esi, ebx		 ;esi должен указывать на картинку
      pop ebx
      pop edi
      call koView.drawImage	 ;отрисовываем полученное изображение

      call koView.deleteSubImage    ;удаляем буфер с изображением
      pop esi
      pop edi
      pop edx
      pop ecx
      pop ebx
      pop eax
      ret



;Временные методы для кнопки (их требуется унифицировать по событиям от системы в будущем)
   ;обработчик событий мыши в зависимости от текущего состояния кнопки
   ;входящие: edi - структура кнопки,
   ;	      eax - координаты мыши (относительно того же объекта относительно которого заданы координаты вьювера кнопки),
   ;	      ebx - состояния и события мыши (определяемые ф-цией 37.3)
   ;выходные: нет
   .mouse:
      push eax
      push ebx
      push ecx
      push edi

      mov ecx, dword[edi+8]   ;читаем состояние кнопки в ecx
      cmp ecx, 6	      ;если кнопка неактивна, то ничего не делаем
      je .mouse_end


;если кнопка активна, то определяем курсор мыши над кнопкой или нет ?
push eax
push ebx
push ecx
push edi
mov edi, dword[edi]
call koView.pointInView
pop edi
pop ecx
pop ebx

cmp eax, 0
je @f

pop eax
.set_status_0:
;если нет, то
mov dword[edi+8], 0	  ; меняем состояние кнопки на =0
			  ;(тк все остальные состояния кнопки кроме неактивного предполагают что курсор над кнопкой)
call koButton.callStatusHandler   ;выполняем код состояния
jmp .mouse_end		  ;и затем ничего больше не делаем

@@: ;иначе если курсор над кнопкой, начинаем анализировать события кнопки
   pop eax

   cmp ecx, 0 ;причём перед этим проверяем текущее состояние кнопки
   jne @f     ;если оно не равно нулю то переходим дальше

;иначе если текущее состояние кнопки =0 то:
mov dword[edi+8], 5 ;устанавливаем состояние кнопки =5
call koButton.callStatusHandler ;и выполняем код состояния

@@: ;в противном случае ничего не делаем и переходим дальше
;здесь начинаем обработку событий мыши

;shl ebx, 8	   ;битовую маску которую не обрабатываем - игнорируем
;shr ebx, 8
;shl ebx, 16+7
;shr ebx, 31

;call myTestPrint

cmp ebx, 0x101 ;257
jne @f
;если нажата левая кнопка, то
cmp ecx, 4 ;	если текущее состояние кнопки =4 или =5 тогда меняем состояние на =1 и выполняем код состояния
je .set_status_1
cmp ecx, 5
je .set_status_1
jmp .set_status_0 ;    иначе сбрасываем состояние кнопки на =0

.set_status_1:
mov dword[edi+8], 1
call koButton.callStatusHandler
jmp .mouse_end

@@:
cmp ebx, 1
jne @f
;если удерживается левая кнопка, то
cmp ecx, 1   ;	  если текущее состояние кнопки =1 тогда меняем состояние на =2 и выполняем код состояния
je .set_status_2

cmp ecx, 2;    если текущее состояние кнопки =2 то ничего не делаем (сохраняем это состояние и дальше)
;je .mouse_end
je .set_status_2

jmp .set_status_0 ;    иначе сбрасываем состояние кнопки на =0

.set_status_2:
mov dword[edi+8], 2
call koButton.callStatusHandler
jmp .mouse_end

@@:
cmp ebx, 0x10000
jne .mouse_end
;если отпущена левая кнопка, то
;    если текущее состояние кнопки =2 или =1 тогда меняем состояние на =3 и выполняем код состояния
cmp ecx, 1
je .set_status_3
cmp ecx, 2
je .set_status_3

jmp .set_status_0 ;    иначе сбрасываем состояние кнопки на =0

.set_status_3:
mov dword[edi+8], 3
call koButton.callStatusHandler
;jmp .mouse_end
call start_application
   .mouse_end:
      pop edi
      pop ecx
      pop ebx
      pop eax
      ret
;АЛГОРИТМ
;если кнопка активна, то определяем курсор мыши над кнопкой или нет ?
;если нет, то меняем состояние кнопки на =0 (тк все остальные состояния кнопки кроме неактивного предполагают что курсор над кнопкой)
;выполняем код состояния и затем ничего больше не делаем

;иначе если курсор над кнопкой, начинаем анализировать события кнопки
;   причём перед этим если текущее состояние кнопки =0 то устанавливаем состояние кнопки =5 и выполняем код состояния

;если нажата левая кнопка, то
;    если текущее состояние кнопки =4 или =5 тогда меняем состояние на =1 и выполняем код состояния
;    иначе сбрасываем состояние кнопки на =0
;если удерживается левая кнопка, то
;    если текущее состояние кнопки =1 тогда меняем состояние на =2 и выполняем код состояния
;    если текущее состояние кнопки =2 то ничего не делаем (сохраняем это состояние и дальше)
;    иначе сбрасываем состояние кнопки на =0
;если отпущена левая кнопка, то
;    если текущее состояние кнопки =2 или =1 тогда меняем состояние на =3 и выполняем код состояния
;    иначе сбрасываем состояние кнопки на =0








   ;обработчик событий клавиатуры
   ;входящие: edi - структура кнопки, ...
   ;выходные: нет
   .keyboard:
      ret


    ;классический и стандартный обработчик сенсорного ввода для кнопки
    ;входящие: edi - структура кнопки, ...
;   .touch:
;      ret

;--------------------------------------------
;(с) Ефременков Сергей В., 2017
