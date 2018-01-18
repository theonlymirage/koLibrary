# koLibrary

This is a GUI library for Kolibri OS.
Current status: in development.


RU:
Структура koView:
  coord  dd 0 ;координаты Х и Y (x dw 0 ; y dw 0)
  width  dd 0 ;ширина
  height dd 0 ;высота
  pixels dd 0 ;указатель на буфер с пикселями (32 бита)


Структура буфера с пикселями изображения:
  width  dd 0 ;ширина
  height dd 0 ;высота
  pixels db 0 dup(4*width*height) ; пиксели в формате AARRGGBB размер области 4*W*H


Название koView сокращение от Kolibri View.
Текущие функции koView:
; fullDrawInWindow: отрисовать Kolibri View по указателю edi целиком как есть в текущем окне
; subDrawInWindow: отрисовать только нужную часть koView в окне: edi - указатель на koView, eax - координаты внутри koView, ecx - размеры внутри koView 
; drawRect: нарисовать прямоугольник: eax - координаты, ebx - размеры по ширине и высоте, ecx - цвет, edi - указатель на koView; ничего не возвращает
; drawPoint: задать цвет(=ecx) точки/пикселя(координаты в eax) на Kolibri View (указатель в edi), внимание: нет проверки что точка внутри koView (возможен крах приложения)
; drawImage: нарисовать картинку: eax - координаты, esi - указатель на картинку, edi - указатель на koView 
; drawText: отобразить текст (требуется усовершенствовать); входные: edi - указатель на буфер koView, eax - цвет, ebx - координаты текста, edx - указатель на начало строки (оканчивается нулём, но сейчас печатает столько символов сколько влазиет по ширине в koView);ничего не возвращает 
; clear: очистить koView (указатель в edi) в цвет ecx
; resize: изменить размеры: edi - koView с уже изменёнными новыми значениями
; create: создать объект в памяти возвращает: edi - новый объект koView
; delete: удалить объект из памяти: указатель в edi
pointInView
   описание: проверяет точка внутри области koView или нет? 
             (координаты точки должны быть относительно того же объекта что и координаты koView) 
   входящие: eax - координаты точки, 
             edi - указатель на структуру koView 
   выходные: eax = 0 - да, точка во вьювере, 1 - нет, точка не над вьювером.

Вспомогательные функции для fullDrawInWindow:
; createImage: конвертировать в изображение: создаёт буфер с картинкой для текущего koView (в edi) в ebx возвращает буфер с картинкой
; deleteImage: удаляет буфер с картинкой по адресу ebx
Вспомогательные функции для subDrawInWindow:
; createSubImage: создать буфер с изображением : Внимание тут нет проверок, поэтому область строго должна быть внутри koView (все её точки); принимает: edi - koView, eax - координаты области в koView, ecx - размеры области в koView; возвращает: ebx - адрес на буфер с изображением
; deleteSubImage: удаляет буфер с картинкой по адресу ebx 

Пример использования:
      call setEncodingSymbolSize  ;устанавливаем размер символов текущим шрифтом
      call koView.create          ;создаём новый koView с размерами 0 на 0

      mov word[edi], 24           ;задаём ему координаты в окне
      mov word[edi+2], 5

      mov dword[edi+4], 30        ;задаём размеры: ширина и высота
      mov dword[edi+8], 50
      call koView.resize          ;применяем новые размеры

      mov ecx, 0xFF0000           ;AARRGGBB
      call koView.clear           ;очищаем его область - заливка единым цветом

      mov eax, 1*65536+2          ;рисуем прямоугольник зелёного цвета
      mov ebx, 60*65536+20
      mov ecx, 0x00FF00
      call koView.drawRect

      mov eax, 0x0000FF           ;цвет текста - синий
      mov ebx, 0*65536+0          ;координаты текста
      mov edx, button_refresh.utf ;cp;;указатель на текст в текущей кодировке
      call koView.drawText        ;рисуем текстовую строку (что не влезет в область - обрезается)
 
      mov eax, 4*65536+5          ;задать цвет нужной точки koView
      mov ecx, 0xFFFFFF
      call koView.drawPoint

      mov eax, 24*65536+45        ;нарисовать картинку
      mov esi, testing
      call koView.drawImage

      mov eax, 0;или 2*65536+3
      mov ecx, 30*65536+50 ;или 28*65536+47
      call koView.subDrawInWindow       ;отрисовать определённую область koView в окне приложения

      mov word[edi], 24
      mov word[edi+2], 5+60
      call koView.fullDrawInWindow      ;отрисовать всю область koView целиком в окне приложения
      
      call koView.delete                ;удалить koView и очистить память
