# koLibrary

This is a GUI library for Kolibri OS.
Current status: in development.

[development forum for questions](http://board.kolibrios.org/viewtopic.php?f=44&t=3514)


RU:
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
