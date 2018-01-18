;koElementsView
;Вьювер различных элементов

;Структура koElementsView:
;+0    view	   dd	0 ;указатель на вьювер koView, в котором рисует себя
;+4    elems	   dd	0 ;список указателей на дочернии элементы (koObjectList)  -это поле и след нужно заменить и объединить в одну структуру
;+8    elem_types  dd	0 ;список типов дочерних элементов	  (koObjectList)
;+12   count	   dd	0 ;кол-во дочерних элементов
;+16		   dd	0 ;список взаимодействий дочерних элементов между собой (не используется)


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


   ;обработку событий от устройст потом надо чем-то заменить единым механизмом - апока только мышь и клавиатура - пусть так и будут
   .mouse:
      ret


   .keyboard:
      ret