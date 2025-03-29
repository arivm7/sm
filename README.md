# SM - Simple Menu

© RI-Network Technical Union. 2025

> Программа написана на C++ 
  c использованием библиотеки nlohmann/json.hpp 

> Предполагалась для использования библитека ncurses, но пока без неё. 
Реализация с помощью ncurses запланирована на следующий выпуск. 

> Программу можно использовать **|как есть|**. 
Никакая гарантия ни на что не предоставляется в пределах, допускаемых законом.

![SM -- Консольное многоуровневое широкое меню.](/img/sm-social-preview.png)

##
## Начать использовать

> В папке [output] находится скомпилированная программа и демонстрационный файл конфигурации:

>| [output]
>>| sm
>>| sm.conf.json



##
## Как работает

Программа представляет собой простое консольное меню,
в котором для каждой задачи можно назначить несколько действий 
с использованием разных инструментов

К примеру, на сервер можно зайти с помощью ssh, вэб-интерфейса, 
или сразу выполнить на нём нужную команду обслуживания 
без отдельных действий входа-выхода 
(перезапуск, обновление, антивирусная проверка или другое обслуживание).

В этом случае пункт меню можно сделать "Сервер", а к нему назначить действия, 
соответствующие тем действиям, которые можно совершить с этим объектом
(пример такой конфигкрации приведён в демонстрационном конфиге).

Для выполнения типичных команд предусмотрен списко таких комманд,
он предстваляет собой название комманды и возвращает полную комманду с шаблоном подстановки параметра

Элемент меню может быть другим меню, тогда он содержит все элементы, которые положено иметь для мню и его элементов.
Вложенности меню не ограничены.

Элемент меню может быть просто разделителем, тогда он отображается в виде не активной строки-разделителя 
служит для логического разделения одноранговых пунктов меню.

Для каждого пункта меню должны быть описаны действия, 
в действиях есть название, для группировки действий в раблицу, 
строка-подсказка по короткой клавише исполнения этого пункта,
и строка с символом этой самой короткой клавиши.

Если для действия не заданы короткие клавиши, то после заполнения меню 
все незаполненные горячие клавиши заполнятся не занятыми маленькими буквами латинского алфавита


Конфигурационный файл меню в формате json с возможностью игнорирования комментариев.



##
## Использование программы

Для использования достаточно просто запустить программу.
При запуске программа ищет конфигурационный файл в той папке где она запущена,
если файл не найден, то ищет его в той папке, где находится исполняемый файл.
Если не находит, то завершается аварийно.

Навигация по пунктам меню происходит с помощью клавиш управления курсором 
(клавишами-стрелками) и клавишами Home, End.

Поскольку действий для каждого пункта менгю может быть несколько, 
то меню представляет собой таблицу, в которой перемещаться можно не только вверх-вниз, но и вправо-влево.

Для выполнения действия нужно нажать <ввод>.

Или, для мгновенного выполнения действия можно нажать короткую клавишу закрепленную за этим действием.

Другой способ, указать короткую клавишу при запуске программы 
в качестве параметра и программа при запуске сразу выполнит это дейтвие.
Если указать несколько символов, то программа будет нажимать эти кнопки 
в том порядке как они указаны в параметре запуска. Один символ 
рассматривается как одна горячая клавиша.
    
    sm
    sm <a...z0...9>
    sm e        --- Это встроенная комманда редактирования конфигурационного файла 
                с помощью внешней программы. 
                Программа задана в списке шаблонов комманд под именем [%edit%]
                
                Редактировать конфигурационный файл можно любой программой, на ваш вкус и цвет.
                
    Имя файла конфигурации меню [sm.conf.json]. 
    json-файл поддерживает комментарии вида:
    
    // Комментарий до конца строки
    /* Комментарий внутри строки */
    
    При сохранении файла конфигурации из программы коментарии не сохраняются.
    

Для понимания структуры меню проще всего посмотреть демонстрационное меню,
оно подробно прокоментировано и в нём все понятно.

