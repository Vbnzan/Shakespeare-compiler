:- set_prolog_flag(double_quotes, string).
print_x([]):- !.
print_x([X|Y]):- writeln(X), print_x(Y).

a2([], L, L).
a2([H|A], B, [H|C])   :- a2(A, B, C).
a3(L1, L2, L3, L)     :- a2(L1, LX, L), 
                         a2(L2, L3, LX).
a4(L1, L2, L3, L4, L) :- a2(L1, LX, L),
                         a3(L2, L3, L4, LX).
a5(L1, L2, L3, L4, L5, L)         :- a2(L1, LX, L),
                                     a4(L2, L3, L4, L5, LX).
a6(L1, L2, L3, L4, L5, L6, L)     :- a2(L1, LX, L), 
                                     a5(L2, L3, L4, L5, L6, LX).
a7(L1, L2, L3, L4, L5, L6, L7, L) :- a2(L1, LX, L),
                                     a6(L2, L3, L4, L5, L6, L7, LX).

% Программа: акты
%prog(L)     :- acts(L).
prog(L, PF)  :- acts(L, A), a3(["$PROG"], A, ["$PROGEND"], PF).

% Акты: один или несколько актов
%acts(L)     :- a2(L1, L2, L), act(L1), acts(L2).
%acts(L)     :- act(L).
acts(L, A)   :- a2(L1, L2, L), act(L1, A1), acts(L2, A2), a2(A1, A2, A).
acts(L, A)   :- act(L, A).

% Акт: название, сцены
%act(L)      :- a2(L1, L2, L), titleAct(L1), scenes(L2).
act(L, A)    :- a2(L1, L2, L), titleAct(L1, A1), scenes(L2, A2), a4(["$ACT"], A1, A2, ["$ACTEND"], A).

% Название акта: Акт, его номер, комментарий
%titleAct(L)   :- a5(["act"], L2, [":"], L4, ["."], L), isname(L2), descr(L4).
titleAct(L, A) :- a5(["act"], L2, [":"], L4, ["."], L), isname(L2, A2), descr(L4), a2(A2, ["$ACTLABEL"], A).

% Сцены: одна или несколько сцен
%scenes(L)   :- a2(L1, L2, L), scene(L1), scenes(L2).
%scenes(L)   :- scene(L).
scenes(L, A) :- a2(L1, L2, L), scene(L1, A1), scenes(L2, A2), a2(A1, A2, A).
scenes(L, A) :- scene(L, A).

% Сцена: название, операторы
%scene(L)    :- a2(L1, L2, L), titleScene(L1), mop(L2).
scene(L, A)  :- a2(L1, L2, L), titleScene(L1, A1), mop(L2, A2), a4(["$SCENE"], A1, A2, ["$SCENEEND"], A).

% Название сцены: Сцена, её номер, комментарий
%titleScene(L)   :- a5(["scene"], L2, [":"], L4, ["."], L), isname(L2), descr(L4).
titleScene(L, A) :- a5(["scene"], L2, [":"], L4, ["."], L), isname(L2, A2), descr(L4), a2(A2, ["$SCENELABEL"], A).

% Операторы: один или несколько операторов
mop(L, A)    :- a2(L1, L2, L), op(L1, A1), mop(L2, A2), a2(A1, A2, A).
mop(L, A)    :- op(L, A).

% Оператор: речь персонажа, вход и выход персонажей
op(L, A)     :- speech(L, A).
op(L, A)     :- a3(["[", "enter"], L3, ["]"], L), isname(L3, A3), a2(A3, ["$ENTERONE"], A).
op(L, A)     :- a5(["[", "enter"], L3, [","], L5, ["]"], L), isname(L3, A3), isname(L5, A5), a3(A3, A5,  ["$ENTERTWO"], A).
op(L, A)     :- a3(["[", "exit"], L3, ["]"], L), isname(L3, A3), a2(A3, ["$EXITONE"], A).
op(L, A)     :- a5(["[", "exeunt"], L3, [","], L5, ["]"], L), isname(L3, A3), isname(L5, A5), a3(A3, A5,  ["$EXITTWO"], A).
op(["[", "exeunt", "]"], A) :- a2([], ["$EXITALL"], A).

% Речь персонажа: имя, строки
speech(L, A) :- a3(L1, [":"], L3, L), isname(L1, A1), lines(L3, A3), a3(A1, ["$ISTALKING"], A3, A).

% Строки: одна или несколько строк
lines(L, A)    :- a2(L1, L2, L), line(L1, A1), lines(L2, A2), a2(A1, A2, A).
lines(L, A)    :- line(L, A).

% Строка: вопрос, или утверждение, или условный переход, или безусловный переход, или добавление-удаление элемента в стек
line(L, A)     :- quest(L, A).
line(L, A)     :- statm(L, A).
line(L, A)     :- cond(L, A).
line(L, A)     :- jump(L, A).
line(L, A)     :- rec(L, A).
line(L, A)     :- rem(L, A).
% или ввод-вывод числа или символа
line(L, A)     :- getn(L, A).     
line(L, A)     :- putn(L, A).     
line(L, A)     :- getc(L, A).     
line(L, A)     :- putc(L, A). 

getn(["listen", "to", "your", "heart", "."], A) :- a2(["$YOURNAME"], ["$SCANNUM"], A).
putn(["open", "your", "heart", "."], A)         :- a2(["$YOURVAL"], ["$PRINTNUM"], A).
getc(["open", "your", "mind", "."], A)          :- a2(["$YOURNAME"], ["$SCANCHAR"], A).
putc(["speak", "your", "mind", "."], A)         :- a2(["$YOURVAL"], ["$PRINTCHAR"], A).

%Добавление и удаление элемента в стек
rec(L, A)                               :- a3(["recall"], L2, ["."], L), descr(L2), a2(["$YOURNAME"], ["$POP"], A).
rem(["remember", "me", "."], A)         :- a3(["$MYVAL"], ["$YOURNAME"], ["$PUSH"], A).
rem(["remember", "yourself", "."], A)   :- a3(["$YOURVAL"], ["$YOURNAME"], ["$PUSH"], A).

% Условный переход
% Вопрос и переход могут находиться в разных сценах, поэтому мы не объединяем их пару в ещё одну категорию
cond(L, A) :- a3(["if", "so", ",", "let's", "proceed", "to", "scene"], L2, ["."], L), isname(L2, A2), a2(A2, ["$TOSCENE", "$BRNZ"], A).
cond(L, A) :- a3(["if", "not", ",", "let's", "proceed", "to", "scene"], L2, ["."], L), isname(L2, A2), a2(A2, ["$TOSCENE", "$BRZ"], A).
cond(L, A) :- a3(["if", "so", ",", "let's", "proceed", "to", "act"], L2, ["."], L), isname(L2, A2), a2(A2, ["$TOACT", "$BRNZ"], A).
cond(L, A) :- a3(["if", "not", ",", "let's", "proceed", "to", "act"], L2, ["."], L), isname(L2, A2), a2(A2, ["$TOACT", "$BRZ"], A).

% Безусловный переход
jump(L, A)  :- a3(["let's", "proceed", "to", "scene"], L2, ["."], L), isname(L2, A2), a2(A2, ["$TOSCENE", "$BR"], A).
jump(L, A)  :- a3(["let's", "proceed", "to", "act"], L2, ["."], L), isname(L2, A2), a2(A2, ["$TOACT", "$BR"], A).

%lets - gone
%goto - gone

% Вопрос
%quest(L, A) :- a3(["am", "i"], L3, ["?"], L), comp(L3, A3), a2(["$MYVAL"], A3, A).
%quest(L, A) :- a3(["are", "you"], L3, ["?"], L), comp(L3, A3), a2(["$YOURVAL"], A3, A).
%quest(L, A) :- a4(["is"], L2, L3, ["?"], L), e(L2, A2), comp(L3, A3), a2(A2, A3, A).
quest(L, A) :- a3(L1, L2, ["?"], L), e(L1, A1), comp(L2, A2), a2(A1, A2, A).

% Утверждение
statm(L, A) :- a5(["you", "as"], L4, ["as"], L6, ["."], L), adj(L4), e(L6, A6), a3(["$YOURNAME"], A6, [":="], A).
statm(L, A) :- a3(["you"], L2, ["."], L), e(L2, A2), a3(["$YOURNAME"], A2, [":="], A).

statm(L, A) :- a5(["i", "as"], L4, ["as"], L6, ["."], L), adj(L4), e(L6, A6), a3(["$MYNAME"], A6, [":="], A).
statm(L, A) :- a3(["i"], L2, ["."], L), e(L2, A2), a3(["$MYNAME"], A2, [":="], A).


% Сравнение
comp(L, A)  :- a4(["as"], L2, ["as"], L4, L), adj(L2), e(L4, A4), a2(A4, ["=="], A).
comp(L, A)  :- a4(["not", "as"], L3, ["as"], L5, L), adj(L3), e(L5, A5), a2(A5, ["!="], A).
comp(L, A)  :- a2(["better", "than"], L3, L), e(L3, A3), a2(A3, [">"], A).
comp(L, A)  :- a2(["not", "better", "than"], L4, L), e(L4, A4), a2(A4, ["<="], A).
comp(L, A)  :- a2(["worse", "than"], L3, L), e(L3, A3), a2(A3, ["<"], A).
comp(L, A)  :- a2(["not", "worse", "than"], L4, L), e(L4, A4), a2(A4, [">="], A).

e(L, A) :- isname(L, A).
e(L, A) :- t(L, A).
e(L, A) :- a2(["square"], L3, L), e(L3, A3), a2(A3, ["$SQUARE"], A).
e(L, A) :- a2(["cube"], L3, L), e(L3, A3), a2(A3, ["$CUBE"], A).
e(L, A) :- a2(["square", "root"], L4, L), e(L4, A4), a2(A4, ["$SQROOT"], A).
e(L, A) :- a4(["sum"], L3, ["and"], L5, L), e(L3, A3), e(L5, A5), a3(A3, A5, ["+"], A).
e(L, A) :- a4(["difference"], L3, ["and"], L5, L), e(L3, A3), e(L5, A5), a3(A3, A5, ["-"], A).
e(L, A) :- a4(["product"], L3, ["and"], L5, L), e(L3, A3), e(L5, A5), a3(A3, A5, ["*"], A).

t(L, A) :- a2(L1, L2, L), adj(L1), t(L2, A2), a2(A2, ["<<"], A).
t(L, A) :- noun(L, A).

noun(L, A) :- a2([], ["+noun"], L), a2([], ["$ONE"], A).
noun(L, A) :- a2([], ["=noun"], L), a2([], ["$ONE"], A).
noun(L, A) :- a2([], ["-noun"], L), a2([], ["$MINUSONE"], A).

adj(L) :- a2([], ["-adj"], L).
adj(L) :- a2([], ["=adj"], L).
adj(L) :- a2([], ["+adj"], L).
adj(L) :- a2([], ["adj"], L).

isname(["Mitya"]).
isname(["Anya"]).

isname(["I"]).
isname(["II"]).
isname(["III"]).

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

descr(L) :- L = [].
descr([H|T]) :- H \= ".", descr(T).
% run run run
% t(["adj", "adj", "adj", "+noun"], X), print_x(X).
% prog(["act", "-1", ":", ".", "scene", "-1", ":", ".", "[", "enter", "1", ",", "2", "]", "1", ":", "i", "+noun", ".", "you", "adj", "=noun", ".", "2", ":", "i", "+noun", ".", "scene", "-2", ":", ".", "1", ":", "you", "adj", "adj", "adj", "adj", "=noun", ".", "i", "better", "than", "you", "?", "2", ":", "if", "so", ",", "let's", "proceed", "to", "scene", "-3", ".", "scene", "-4", ":", ".", "2", ":", "speak", "your", "mind", ".", "you", "as", "adj", "as", "sum", "you", "and", "=noun", ".", "1", ":", "let's", "proceed", "to", "scene", "-2", ".", "scene", "-3", ":", ".", "1", ":", "i", "adj", "+noun", ".", "2", ":", "i", "+noun", ".", "[", "exeunt", "]"], X), print_x(X).
