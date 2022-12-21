% Синтаксический анализ программы на языке Shakespeare с построением польской формы.

% Подключение модуля, необходимого для использования файлового ввода
:- use_module(library(readutil)).

% Процедура распечатывания списка
:- set_prolog_flag(double_quotes, string).
print_x([]):- !.
print_x([X|Y]):- writeln(X), print_x(Y).

% Процедура для запуска синтаксического анализа
% На входе - набор лексем из файла output_lex.txt
% На выходе - польская форма - файл poland.txt
main :-
    read_file_to_terms('output_lex.txt', L, [dotlists(true)]),
    prog(L, PF),
    print_x(PF),
    tell('poland.txt'),
    print_x(PF),
    told.

% Процедуры для конкатенации нескольких списков в один.
% На их основе реализованы правила грамматики.
a2([], L, L).
a2([H|A], B, [H|C]) :- 
    a2(A, B, C).
a3(L1, L2, L3, L) :- 
    a2(L1, LX, L), 
    a2(L2, L3, LX).
a4(L1, L2, L3, L4, L) :- 
    a2(L1, LX, L),
    a3(L2, L3, L4, LX).
a5(L1, L2, L3, L4, L5, L) :- 
    a2(L1, LX, L),
    a4(L2, L3, L4, L5, LX).

    
%%%%% Правила грамматики %%%%%    
    
    
% Программа состоит из актов - одного или нескольких
prog(L, PF)  :- 
    acts(L, A), 
    a3(["$PROG"], A, ["$PROGEND"], PF).
    
acts(L, A)   :- 
    a2(L1, L2, L),
    act(L1, A1), 
    acts(L2, A2), 
    a2(A1, A2, A).
    
acts(L, A)   :- 
    act(L, A).

% Акт состоит из заголовка и сцен - одной или нескольких
act(L, A)    :- 
    a2(L1, L2, L), 
    titleAct(L1, A1),
    scenes(L2, A2), 
    a4(["$ACT"], A1, A2, ["$ACTEND"], A).

% Заголовок акта: <Акт>, метка, комментарий
titleAct(L, A) :- 
    a5(["act"], L2, [":"], L4, ["."], L), 
    isname(L2, A2), 
    descr(L4),
    a2(A2, ["$ACTLABEL"], A).

scenes(L, A) :- 
    a2(L1, L2, L), 
    scene(L1, A1), 
    scenes(L2, A2),
    a2(A1, A2, A).
    
scenes(L, A) :- 
    scene(L, A).

% Сцена состоит из заголовка и операторов - одного или нескольких
scene(L, A) :- a2(L1, L2, L), 
    titleScene(L1, A1),
    mop(L2, A2),
    a4(["$SCENE"], A1, A2, ["$SCENEEND"], A).

% Заголовок сцены: <Сцена>, метка, комментарий
titleScene(L, A) :- 
    a5(["scene"], L2, [":"], L4, ["."], L), 
    isname(L2, A2), 
    descr(L4), 
    a2(A2, ["$SCENELABEL"], A).

mop(L, A)    :- 
    a2(L1, L2, L), 
    op(L1, A1), 
    mop(L2, A2), 
    a2(A1, A2, A).
    
mop(L, A)    :- 
    op(L, A).

% Оператором может быть речь персонажа или вход (выход) персонажей
op(L, A)     :- 
    speech(L, A).
    
op(L, A)     :- 
    a3(["[", "enter"], L3, ["]"], L),
    isname(L3, A3),
    a2(A3, ["$ENTERONE"], A).
    
op(L, A)     :- 
    a5(["[", "enter"], L3, [","], L5, ["]"], L),
    isname(L3, A3),
    isname(L5, A5),
    a3(A3, A5, ["$ENTERTWO"], A).
    
op(L, A)     :- 
    a3(["[", "exit"], L3, ["]"], L),
    isname(L3, A3),
    a2(A3, ["$EXITONE"], A).
    
op(L, A)     :-
    a5(["[", "exeunt"], L3, [","], L5, ["]"], L),
    isname(L3, A3),
    isname(L5, A5),
    a3(A3, A5, ["$EXITTWO"], A).
    
op(["[", "exeunt", "]"], A) :-
    a2([], ["$EXITALL"], A).

% Речь персонажа состоит из имени персонажа и фраз - одной или нескольких
speech(L, A) :- 
    a3(L1, [":"], L3, L),
    isname(L1, A1),
    lines(L3, A3),
    a3(A1, ["$ISTALKING"], A3, A).

lines(L, A)    :-
    a2(L1, L2, L), 
    line(L1, A1),
    lines(L2, A2),
    a2(A1, A2, A).
    
lines(L, A)    :- 
    line(L, A).

% Строка
line(L, A)     :- quest(L, A). % вопрос
line(L, A)     :- statm(L, A). % утверждение
line(L, A)     :- cond(L, A).  % условный переход
line(L, A)     :- jump(L, A).  % безусловный переход
line(L, A)     :- rec(L, A).   % удаление элемента из стека
line(L, A)     :- rem(L, A).   % добавление элемента в стек
line(L, A)     :- getn(L, A).  % ввод числа
line(L, A)     :- putn(L, A).  % вывод числа
line(L, A)     :- getc(L, A).  % ввод символа
line(L, A)     :- putc(L, A).  % вывод символа

% Ввод и вывод
getn(["listen", "to", "your", "heart", "."], A) :- a2(["$YOURNAME"], ["$SCANNUM"], A).
putn(["open", "your", "heart", "."], A)         :- a2(["$YOURVAL"], ["$PRINTNUM"], A).
getc(["open", "your", "mind", "."], A)          :- a2(["$YOURNAME"], ["$SCANCHAR"], A).
putc(["speak", "your", "mind", "."], A)         :- a2(["$YOURVAL"], ["$PRINTCHAR"], A).

% Операции со стеком
rec(L, A) :- 
    a3(["recall"], L2, ["."], L),
    descr(L2),
    a2(["$YOURNAME"], ["$POP"], A).
    
rem(["remember", "me", "."], A) :- a3(["$MYVAL"], ["$YOURNAME"], ["$PUSH"], A).
rem(["remember", "yourself", "."], A)   :- a3(["$YOURVAL"], ["$YOURNAME"], ["$PUSH"], A).

% Условный переход
cond(L, A) :- 
    a3(["if", "so", ",", "let's", "proceed", "to", "scene"], L2, ["."], L),
    isname(L2, A2), 
    a2(A2, ["$TOSCENE", "$BRNZ"], A).
    
cond(L, A) :- 
    a3(["if", "not", ",", "let's", "proceed", "to", "scene"], L2, ["."], L), 
    isname(L2, A2),
    a2(A2, ["$TOSCENE", "$BRZ"], A).
    
cond(L, A) :-
    a3(["if", "so", ",", "let's", "proceed", "to", "act"], L2, ["."], L), 
    isname(L2, A2), 
    a2(A2, ["$TOACT", "$BRNZ"], A).
    
cond(L, A) :-
    a3(["if", "not", ",", "let's", "proceed", "to", "act"], L2, ["."], L), 
    isname(L2, A2),
    a2(A2, ["$TOACT", "$BRZ"], A).

% Безусловный переход
jump(L, A) :-
    a3(["let's", "proceed", "to", "scene"], L2, ["."], L), 
    isname(L2, A2),
    a2(A2, ["$TOSCENE", "$BR"], A).
    
jump(L, A) :- 
    a3(["let's", "proceed", "to", "act"], L2, ["."], L), 
    isname(L2, A2), 
    a2(A2, ["$TOACT", "$BR"], A).

% Вопрос
quest(L, A) :-
    a3(L1, L2, ["?"], L),
    e(L1, A1),
    comp(L2, A2),
    a2(A1, A2, A).

% Утверждение
statm(L, A) :-
    a5(["you", "as"], L4, ["as"], L6, ["."], L),   
    adj(L4), 
    e(L6, A6), 
    a3(["$YOURNAME"], A6, [":="], A).
    
statm(L, A) :- 
    a3(["you"], L2, ["."], L),
    e(L2, A2),
    a3(["$YOURNAME"], A2, [":="], A).
    
statm(L, A) :-
    a5(["i", "as"], L4, ["as"], L6, ["."], L),
    adj(L4), 
    e(L6, A6), 
    a3(["$MYNAME"], A6, [":="], A).
    
statm(L, A) :- 
    a3(["i"], L2, ["."], L),
    e(L2, A2),
    a3(["$MYNAME"], A2, [":="], A).

% Сравнение
comp(L, A)  :-
    a4(["as"], L2, ["as"], L4, L), 
    adj(L2),
    e(L4, A4),
    a2(A4, ["=="], A).
    
comp(L, A) :-
    a4(["not", "as"], L3, ["as"], L5, L),
    adj(L3), 
    e(L5, A5),
    a2(A5, ["!="], A).
    
comp(L, A) :-
    a2(["better", "than"], L3, L),
    e(L3, A3),
    a2(A3, [">"], A).
    
comp(L, A) :- 
    a2(["not", "better", "than"], L4, L),
    e(L4, A4),
    a2(A4, ["<="], A).
    
comp(L, A) :-
    a2(["worse", "than"], L3, L),
    e(L3, A3),
    a2(A3, ["<"], A).
    
comp(L, A) :-
    a2(["not", "worse", "than"], L4, L),
    e(L4, A4),
    a2(A4, [">="], A).

% Выражение
e(L, A) :- isname(L, A).
e(L, A) :- t(L, A).

e(L, A) :-
    a2(["square"], L3, L),
    e(L3, A3),
    a2(A3, ["$SQUARE"], A).
    
e(L, A) :-
    a2(["cube"], L3, L),
    e(L3, A3),
    a2(A3, ["$CUBE"], A).
    
e(L, A) :-
    a2(["square", "root"], L4, L),
    e(L4, A4),
    a2(A4, ["$SQROOT"], A).
    
e(L, A) :- 
    a4(["sum"], L3, ["and"], L5, L),
    e(L3, A3),
    e(L5, A5),
    a3(A3, A5, ["+"], A).
    
e(L, A) :- 
    a4(["difference"], L3, ["and"], L5, L),
    e(L3, A3),
    e(L5, A5),
    a3(A3, A5, ["-"], A).
    
e(L, A) :-
    a4(["product"], L3, ["and"], L5, L),
    e(L3, A3),
    e(L5, A5),
    a3(A3, A5, ["*"], A).

% Число
t(L, A) :- 
    a2(L1, L2, L),
    adj(L1),
    t(L2, A2),
    a2(A2, ["<<"], A).
    
t(L, A) :- noun(L, A).

% Существительное
noun(L, A) :- a2([], ["+noun"], L), a2([], ["$ONE"], A).
noun(L, A) :- a2([], ["=noun"], L), a2([], ["$ONE"], A).
noun(L, A) :- a2([], ["-noun"], L), a2([], ["$MINUSONE"], A).

% Прилагательное
adj(L) :- a2([], ["-adj"], L).
adj(L) :- a2([], ["=adj"], L).
adj(L) :- a2([], ["+adj"], L).
adj(L) :- a2([], ["adj"], L).

% Соответствие между именами и их польской формой
isname(["you"], ["$YOURVAL"]).
isname(["i"], ["$MYVAL"]).
isname(["1"], ["1"]).
isname(["2"], ["2"]).
isname(["3"], ["3"]).
isname(["4"], ["4"]).
isname(["5"], ["5"]).
isname(["6"], ["6"]).
isname(["-1"], ["-1"]).
isname(["-2"], ["-2"]).
isname(["-3"], ["-3"]).
isname(["-4"], ["-4"]).
isname(["-5"], ["-5"]).
isname(["-6"], ["-6"]).

% Комментарий - любая последовательность лексем, не содержащая точку.
descr(L) :- L = [].
descr([H|T]) :- H \= ".", descr(T).
