# Интерпретатор языка Shakespeare

---

Shakespeare — язык программирования, разработанный Джоном Аслаудом и Карлом Хассельстромом. Язык Shakespeare призван замаскировать исходный код программы под пьесы Шекспира.

## 1. Лексический анализ
Лексический анализатор реализован на основе конечного автомата в виде программы на языке с++.
Исходный код представлен в файле [Sourse.cpp](https://github.com/Vbnzan/Shakespeare-compiler/blob/master/Source.cpp).
Диаграмма конечного автомата представлена в файле [state_machine.jpg](https://github.com/Vbnzan/Shakespeare-compiler/blob/master/state_machine.jpg) и продублирована в файле [state_machine_graph.png]().

## 2. Синтаксический анализ
Синтаксический анализатор реализован в виде программы на языке Prolog.
Исходный код представлен в файле [syntax_anal_3.pl](https://github.com/Vbnzan/Shakespeare-compiler/blob/master/syntax_anal_3.pl).
Правила грамматики описаны в файле [SPL_Grammar.txt](https://github.com/Vbnzan/Shakespeare-compiler/blob/master/SPL_grammar.txt).

# Запуск

## 0. Предварительная подготовка

- Код анализизируемой программы должен находиться в файле [input.txt](https://github.com/Vbnzan/Shakespeare-compiler/blob/master/input.txt) в кодировке ASCII.
- Используемые ключевые слова и их интерпретация должны находиться в файле [keywords.txt](https://github.com/Vbnzan/Shakespeare-compiler/blob/master/keywords.txt) в кодировке ASCII.
Формат этого файла следующий: каждая нечётная строка - ключевое слово, которое может встречаться в исходной программе; следующая за ней чётная строка - соответствующая этому ключевому слову лексема.
- Установите компилятор swipl, следуя инструкциям на [странице](https://eu.swi-prolog.org/build/unix.html).

### 1. Лексический анализ

1. Откройте окно терминала и перейдите в директорию проекта.
2. Скомпилируйте файл [Source.cpp](https://github.com/Vbnzan/Shakespeare-compiler/blob/master/Source.cpp):
```bash
g++ Source.cpp
```
3. Запустите программу:
```bash
./a.out
```
4. После выполнения программы в директории проекта появится файл output_lex.txt, содержащий поток лексем.

### 2. Синтаксический анализ

1. Откройте окно терминала и перейдите в директорию проекта.
2. Запустите командную строку компилятора prolog:
```bash
swipl
```
3. Загрузите предикаты из файла syntax_anal_3.pl:
```Prolog
[syntax_anal_3].
```
4. Запустите синтаксический анализ:
```Prolog
main.
```

Программа выведет польскую форму в терминал и в файл poland.txt.
