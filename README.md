# Бинарный транслятор
Перед мной была поставлена задача написать транслятор из бинарного кода моего языка в исполняемый файл ELF.
Давайте разберёмся с его структурой
# ELF file
Что он из себя представляет? \
![](https://upload.wikimedia.org/wikipedia/commons/7/77/Elf-layout--en.svg) \
Это минимальное представление, которое можно дать. Коротко о  секциях: 
- ELF header \
Здесь находится основная информация о вашей машине, о входных точках исполняемого кода, заголовках секций и программы, их размеров.
Подробнее можно всё узнать по ссылки в использованной литературе.
- Program header \
Размеры блоков, адреса в файле и в виртуальной памяти.
- Section header \
Размеры секций, адреса в файле, в виртуальной памяти, флаги (writable, executable, etc...)
- Сами секции \
Находится нужная информация. Например: \
В секции data находится проиницилизированные данные, в секции text исполняемый код
# Мой эльф файл
Во избежании трудностей с адресами, мне захотелось зафиксировать места, где находятся все блоки программы,
поэтому я переставил блоки программы следующим образом 
1. ELF header
2. Program header
3. Section header
4. Sectioт Data (фиксированного размера)
5. Section Text

Как вы видите, исполняемый код (Section Text) и память (Sectioт Data) имеют фиксированные адреса в файле, поэтому без труда 
я смогу сгенировать первые 4 секции. Единственное, потом придётся подправить 
размер исполняемого кода. \
Вроде все проблемы с эльф файлом решены, перейдём к генерации исполняемого кода.
# Исполняемый код
Мануалы Интела по структуре команд процессора меня очень сильно выручили. Оказывается, команды процессора x86
имеют следующую структуру: \
![](https://sun1-28.userapi.com/QZquOpYGIRgAiPBcULxOAKIgcmm02MuPX-5F5Q/EGAgdNGqFmo.jpg)
Чтобы поддержать этот формат мне пришлось завести свою библиотеку (const_lib.h), в которой поддерживаются актуальные
префиксы, номера функций, используемые R/M байты и SIB байты. Изначально я построил изоморфизмы между командами своего 
бинарного кода (очевидно, что команд моего процессора с прошлого семестра) и конструкциями x86, некоторые вышли 
не совсем маленькими. Передо мной встал вопрос как мне из этого сделать исполняемый код. \
Я решил использовать Variadic Temlates. Функция set_val: \
![](https://sun1-19.userapi.com/hFm4i6k8QgMe4F7e-9kR4uEonFOYazjoVl1gAA/TeUiqpM4X48.jpg) \
Проблема решена. Осталось только реализовать функции перевода из моих команд в x86 и транслятор готов!

# Использованная литература
- Elf file
1. Executable and Linkable Format (Wikipedia) https://en.wikipedia.org/wiki/Executable_and_Linkable_Format#/media/File:ELF_Executable_and_Linkable_Format_diagram_by_Ange_Albertini.png
2. https://cirosantilli.com/elf-hello-world#program-header-table
- Команды x86
1. Intel manuals https://software.intel.com/content/www/us/en/develop/articles/intel-sdm.html \
P.S. Особенно этот - https://software.intel.com/content/www/us/en/develop/download/intel-64-and-ia-32-architectures-sdm-combined-volumes-2a-2b-2c-and-2d-instruction-set-reference-a-z.html
