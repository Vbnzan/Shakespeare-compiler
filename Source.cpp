#include <cstdio>
#include <iostream>
#include <string>
#include <map>
#include<fstream>
#include <locale.h>
using namespace std;

//обьявление функций
string clear_and_write_bufer(string bufer, char input_symble, ofstream& fout, ifstream& fin);
string add_in_bufer(string bufer, char input_symble, ofstream& fout, ifstream& fin);
string ungetch(string bufer, char input_symble, ofstream& fout, ifstream& fin);
string write_act_pre(string bufer, char input_symble, ofstream& fout, ifstream& fin);
string write_scene_pre(string bufer, char input_symble, ofstream& fout, ifstream& fin);
void add_list_to_rules(char* symbols, std::map<std::pair<char, int>, std::pair<string(*)(string bufer, char   input_symble, ofstream& fout, ifstream& fin), int>>& rules, int start, int end, string(*func)(string bufer, char   input_symble, ofstream& fout, ifstream& fin));
string skip(string bufer, char input_symble, ofstream& fout, ifstream& fin);



//создаем таблицу ключевых слов, таблицу персонажей(переменных), таблицу названий актов\сцен (метки)
std::map<std::string, std::string> key_words{};
std::map<std::string, int> personality_table{}, act_scene_table{};
//В personality_table и act_scene_table строке имени сопоставляется число - уникальный номер переменной, который передается в синтаксический анализатор




string skip(string bufer, char input_symble, ofstream& fout, ifstream& fin)
{
    return bufer;
}


//добавляет введенный символ в буфер
string add(string bufer, char input_symble, ofstream& fout, ifstream& fin)
{
    bufer += input_symble; //добавляет введенный символ в буфер
    return bufer;
}


//обработка лексемы типа имя
string write_name(string bufer, char input_symble, ofstream& fout, ifstream& fin)
{
    //fout << "Writing name:" << bufer << endl;
    if (personality_table.count(bufer) != 0)  //проверка, что имя не является ключевым словом или названием акта\сцены
    {
        cout << "Error. name " << bufer << " already exists." << endl; //Если является - выводим ошибку
        return "";
    }
    if (key_words.count(bufer) != 0) {
        cout << "Error. name " << bufer << " must not be a key word." << endl;
        return "";
    }
    if (act_scene_table.count(bufer) != 0) {
        cout << "Error. name " << bufer << " already exists." << endl;
        return "";
    }
    //имена переменных начинаются с единицы, соответствие - имя <-> символ хранится в personality_table
    int num = personality_table.size() + 1; //вычисляем номер переменной
    personality_table[bufer] = num; //добавляем запись имя <-> номер
    return "";
}

//обработка лексемы типа акт/сцена
string write_act(string bufer, char input_symble, ofstream& fout, ifstream& fin)
{
    //fout << "Writing act:" << bufer << endl;
    //проверка, что названием акта\сцены не является ключевым словом или именем
    if (personality_table.count(bufer) != 0)
    {
        cout << "Error. name " << bufer << " already exists." << endl; // //Если является - выводим ошибку
        return "";
    }
    if (key_words.count(bufer) != 0) {
        cout << "Error. name " << bufer << " must not be a key word." << endl; //Если является - выводим ошибку
        return "";
    }
    if (act_scene_table.count(bufer) != 0) { //Если имя уже существует - выводим его в файл вывода
        fout << "\"" << act_scene_table[bufer] << "\", ";
        //fout << " " << act_scene_table[bufer] << " ";
        return "";
    }
    //имена актов\сцен - отрицательные числа. нумерация начинается с минус единицы, соответствие - имя <-> символ хранится в act_scene_table
    int num = -1 - act_scene_table.size(); //вычисляем номер переменной
    act_scene_table[bufer] = num; //добавляем запись имя <-> номер
    fout << "\"" << act_scene_table[bufer] << "\", ";  //выводим в файл вывода
    //fout << " " << act_scene_table[bufer] << " ";
    return "";
}


/*
string write_scene(string bufer, char input_symble, ofstream& fout, ifstream& fin)
{
    //fout << "Writing scene:" << bufer << endl;

    if (personality_table.count(bufer) != 0)
    {
        cout << "Error. name " << bufer << " already exists." << endl;
        return "";
    }
    if (key_words.count(bufer) != 0) {
        cout << "Error. name " << bufer << " must not be a key word." << endl;
        return "";
    }
    if (act_scene_table.count(bufer) != 0) {
        fout  << "\"" << act_scene_table[bufer] << "\", ";
        //fout << " " << act_scene_table[bufer] << " ";
        return "";
    }
    int num = -1 - act_scene_table.size();
    act_scene_table[bufer] = num;
    fout  << "\"" << act_scene_table[bufer] << "\", ";
    //fout << " " << act_scene_table[bufer] << " ";
    return "";

}
*/

//обрабатывает лексему типа слово, выводит в файл вывода
string write_word(string bufer, char input_symble, ofstream& fout, ifstream& fin)
{
    //Каждому слову сопоставляется лексема или из таблицы ключевых слов, или из таблицы имен, или из таблицы имен актов/сцен
    
    if (bufer == "@" or bufer == "$")  //обработка так как ниже, но с добавлением \n для удобства восприятия и отладки
    {
        fout <<"\n" << "\"" << key_words[bufer] << "\", ";
        return "";
    }
    if (personality_table.count(bufer) != 0) { //если слово - это имя переменной  - выводим в файл вывода соотвутствующую лексему
        fout << "\"" << personality_table[bufer] << "\", ";
        //fout << " " << personality_table[bufer] << " ";
        return "";
    }
    if (key_words.count(bufer) != 0) {  //если слово - это ключевое слово  - выводим в файл вывода соотвутствующую лексему
        fout << "\"" << key_words[bufer] << "\", ";
        //fout << " " << key_words[bufer] << " ";
        return "";
    }
    if (act_scene_table.count(bufer) != 0) { //если слово - это имя акта\сцены  - выводим в файл вывода соотвутствующую лексему
        fout << "\"" << act_scene_table[bufer] << "\", ";
        //fout << " " << act_scene_table[bufer] << " ";
        return "";
    }
    //Если слово ничему не соответствует - пропускаем. Программа может содержать "лишние" слова для большей художественной выразительности
    return "";
}

//Обрабатывает две лексемы, лексему типа слово из bufer и лексему типа слово из input_symble
string write_word_and_next(string bufer, char input_symble, ofstream& fout, ifstream& fin)
{
    string next = "";
    next += input_symble;
    //fout << "Writing words:" << bufer << "\t" << next << endl;
    write_word(bufer, input_symble, fout, fin); //обрабатываем буфер как слово
    write_word(next, input_symble, fout, fin); //обрабатываем input_symble как слово
    return "";
}

//Обрабатывает две лексемы, лексему типа имя акта/сцены из bufer и лексему типа слово из input_symble
string write_act_and_next(string bufer, char input_symble, ofstream& fout, ifstream& fin)
{
    write_act(bufer, input_symble, fout, fin); //обрабатываем буфер как акт\сцену
    write_word_and_next("", input_symble, fout, fin); //обрабатываем input_symble как слово
    return "";
}



int main()
{
    setlocale(LC_ALL, "RUSSIAN");
    ifstream keyword_table("keywords.txt");
    
    
    
    //заполнение таблицы ключевых слов
    //первая строка - input; вторая строка - output
    while (!keyword_table.eof())
    {
        //считываем две строки
        string s_input, s_output;
        getline(keyword_table, s_input);  
        getline(keyword_table, s_output);
        

        key_words.insert(make_pair(s_input, s_output)); //добавляем запись в таблицу ключевых слов
        //cout << s_input << "  "<< s_output <<endl;
    }
    keyword_table.close();


    //простой тест функциональности 
    /*
    char c;
    while (fin >> c)
    {
        string in = "";
        in += c;
        fout << key_words[in] << endl;
        //cout << in <<endl;
    }
    fout.close();
    fin.close();

    */

    //В input.txt хратится исходный код обрабатываемой программы
    ifstream fin_pre("input.txt");
    //В output_pre.txt хратится вывод препроцессора
    ofstream fout_pre("output_pre.txt");
    std::map<std::pair<char, int>, std::pair<string(*)(string bufer, char input_symble, ofstream& fout, ifstream& fin), int>> preprocess_rules{};
    //Для пары тек состояния int и символа ввода char ставим в соответствие пару следующего состояния и функцию, что будет что-то делать

    //правила конечного автомата для препроцессора:
    //Препроцессор переводит все символы в нижний регистр и заменяет ключевые слова "Акт" и "Сцена" на символы @ и $
    //Это нужно для упрощения процедуры лексического анализа и удобного вывода результатов
    preprocess_rules[make_pair('А', 1)] = make_pair(&add_in_bufer, 2); //добавляем правило - из состояния 1 при получении input_symble = 'А' 
    //мы переходим в состояние 2 и выполняем процедуру add_in_bufer
    preprocess_rules[make_pair('к', 2)] = make_pair(&add_in_bufer, 3);
    preprocess_rules[make_pair('т', 3)] = make_pair(&add_in_bufer, 4);
    preprocess_rules[make_pair(' ', 4)] = make_pair(&write_act_pre, 1);
    preprocess_rules[make_pair('\n', 4)] = make_pair(&write_act_pre, 1);

    preprocess_rules[make_pair('С', 1)] = make_pair(&add_in_bufer, 5);
    preprocess_rules[make_pair('ц', 5)] = make_pair(&add_in_bufer, 6);
    preprocess_rules[make_pair('е', 6)] = make_pair(&add_in_bufer, 7);
    preprocess_rules[make_pair('н', 7)] = make_pair(&add_in_bufer, 8);
    preprocess_rules[make_pair('а', 8)] = make_pair(&add_in_bufer, 9);
    preprocess_rules[make_pair(' ', 9)] = make_pair(&write_scene_pre, 1);
    preprocess_rules[make_pair('\n', 9)] = make_pair(&write_scene_pre, 1);




    //процесс выполнения конечного автомата:
    
    //определяем начальные условия
    string bufer = ""; 
    char input_symbol;
    int current_state = 1;
    while (fin_pre.get(input_symbol)) //считываем один символ
    {
        auto it = preprocess_rules.find(make_pair(input_symbol, current_state));
        if (it == preprocess_rules.end()) //проверяем, что правило существует
        {
            //если нет:

            //cout << "skip " << current_state << "\tбуфер:" << bufer << "\tТекущий символ:" << static_cast<int> (input_symbol) << endl;
 
            //не знаешь что делать - не трогай
            bufer = clear_and_write_bufer(bufer, input_symbol, fout_pre, fin_pre); //переносим содержимое буферв в файл вывода 
            if (input_symbol == ' ' or input_symbol == '\n') //переходим в состояние 1(между слов) после разделителя
                current_state = 1;
            else
                current_state = 0; //переходим в состояние 0 (внутри неизвестного слова) 
            continue;
        }
        //загнали состояние и ввод в правила, получили результат
        auto action = (it->second).first; //получаем процедуру из правила перехода
        int next_state = (it->second).second; //получаем следующее состояние из правил перехода

        
        bufer = (*action)(bufer, input_symbol, fout_pre, fin_pre); //выполняем процедуру
        current_state = next_state; //переходим в следующее состояние
    }
 


    fin_pre.close();
    fout_pre.close();

    //Правила конечного автомата для лексического анализа
    std::map<std::pair<char, int>, std::pair<string(*)(string bufer, char   input_symble, ofstream& fout, ifstream& fin), int>> lex_anal_rules{};

    //Разбиение символов из алфавита на группы
    char letters[] = "ЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮйцукенгшщзхъфывапролджэячсмитьбюivxlcdm";
    char non_roman_letters[] = "ЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮйцукенгшщзхъфывапролджэячсмитьбю";
    char spaces[] = "\n\t ";
    char all_but_point[] = "ЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮйцукенгшщзхъфывапролджэячсмитьбю,?:! \n\tivxlcdm";
    char end_sentence[] = ".!?";
    char divinity[] = " \n\t,";
    char roman_num[] = "ivxlcdm";
    char symbols[] = ".!?,@$[]:";
    //add_list_to_rules(letters, lex_anal_rules, 0, 1, &skip);
    //preprocess_rules[make_pair('А', 1)] = make_pair(&add_in_bufer, 2);

    /*
    add_list_to_rules(all_but_point, lex_anal_rules, 0, 0, &skip); //1
    lex_anal_rules[make_pair('.', 0)] = make_pair(&skip, 1); //2
    add_list_to_rules(spaces, lex_anal_rules, 1, 1, &skip);
    add_list_to_rules(letters, lex_anal_rules, 1, 2, &add);
    add_list_to_rules(letters, lex_anal_rules, 2, 2, &add); //5
    lex_anal_rules[make_pair(',', 2)] = make_pair(&write_name, 3);
    add_list_to_rules(all_but_point, lex_anal_rules, 3, 3, &skip);
    lex_anal_rules[make_pair('.', 3)] = make_pair(&skip, 1);
    lex_anal_rules[make_pair('@', 1)] = make_pair(&write_word_and_next, 4);
    add_list_to_rules(spaces, lex_anal_rules, 4, 4, &skip); //10
    add_list_to_rules(roman_num, lex_anal_rules, 4, 5, &add);
    add_list_to_rules(roman_num, lex_anal_rules, 5, 5, &add);
    lex_anal_rules[make_pair(':', 5)] = make_pair(&write_act, 6);
    add_list_to_rules(all_but_point, lex_anal_rules, 6, 6, &skip);
    lex_anal_rules[make_pair('.', 6)] = make_pair(&skip, 7); //15
    add_list_to_rules(spaces, lex_anal_rules, 7, 7, &skip);
    lex_anal_rules[make_pair('@', 7)] = make_pair(&write_word_and_next, 4);
    lex_anal_rules[make_pair('$', 7)] = make_pair(&write_word_and_next, 8);
    add_list_to_rules(spaces, lex_anal_rules, 8, 8, &skip);
    add_list_to_rules(roman_num, lex_anal_rules, 8, 9, &add);  //20
    add_list_to_rules(roman_num, lex_anal_rules, 9, 9, &add);
    lex_anal_rules[make_pair(':', 9)] = make_pair(&write_scene, 10);
    add_list_to_rules(all_but_point, lex_anal_rules, 10, 10, &skip);
    lex_anal_rules[make_pair('.', 10)] = make_pair(&skip, 11);
    lex_anal_rules[make_pair('@', 11)] = make_pair(&write_word_and_next, 4); //25
    lex_anal_rules[make_pair('$', 11)] = make_pair(&write_word_and_next, 8);
    add_list_to_rules(letters, lex_anal_rules, 11, 12, &add);
    add_list_to_rules(letters, lex_anal_rules, 12, 12, &add);
    add_list_to_rules(end_sentence, lex_anal_rules, 12, 11, &write_word_and_next);
    lex_anal_rules[make_pair(':', 12)] = make_pair(&write_word_and_next, 11); //30
    add_list_to_rules(divinity, lex_anal_rules, 12, 13, &write_word);
    add_list_to_rules(divinity, lex_anal_rules, 13, 13, &skip);
    add_list_to_rules(end_sentence, lex_anal_rules, 13, 11, &write_word_and_next);
    add_list_to_rules(letters, lex_anal_rules, 13, 14, &add);
    add_list_to_rules(letters, lex_anal_rules, 14, 14, &add); //35
    add_list_to_rules(divinity, lex_anal_rules, 14, 13, &write_word);
    add_list_to_rules(end_sentence, lex_anal_rules, 14, 11, &write_word_and_next);
    lex_anal_rules[make_pair('[', 11)] = make_pair(&write_word_and_next, 15);
    add_list_to_rules(letters, lex_anal_rules, 15, 15, &add);
    lex_anal_rules[make_pair(']', 15)] = make_pair(&write_word_and_next, 11); //40
    lex_anal_rules[make_pair(' ', 15)] = make_pair(&write_word, 16);
    add_list_to_rules(letters, lex_anal_rules, 16, 16, &add);
    lex_anal_rules[make_pair(',', 16)] = make_pair(&write_word_and_next, 17);
    lex_anal_rules[make_pair(']', 16)] = make_pair(&write_word_and_next, 11);
    lex_anal_rules[make_pair(' ', 17)] = make_pair(&skip, 17);  //45
    add_list_to_rules(letters, lex_anal_rules, 17, 18, &add);
    add_list_to_rules(letters, lex_anal_rules, 18, 18, &add);
    lex_anal_rules[make_pair(']', 18)] = make_pair(&write_word_and_next, 11);
    add_list_to_rules(spaces, lex_anal_rules, 11, 11, &skip);
    */

     //Эти правила продемонстрированы графически в другом файле
    add_list_to_rules(all_but_point, lex_anal_rules, 0, 0, &skip); //1
    //для каждого символа из массива all_but_point добавляем в lex_anal_rules правило: (символ, состояние 0) -> (состояние 0, процедура skip)
    lex_anal_rules[make_pair('.', 0)] = make_pair(&skip, 1); //2
    add_list_to_rules(spaces, lex_anal_rules, 1, 1, &skip);
    add_list_to_rules(letters, lex_anal_rules, 1, 2, &add);
    add_list_to_rules(letters, lex_anal_rules, 2, 2, &add); //5
    lex_anal_rules[make_pair(',', 2)] = make_pair(&write_name, 3);
    add_list_to_rules(all_but_point, lex_anal_rules, 3, 3, &skip);
    lex_anal_rules[make_pair('.', 3)] = make_pair(&skip, 1);
    lex_anal_rules[make_pair('@', 1)] = make_pair(&write_word_and_next, 4);


    add_list_to_rules(spaces, lex_anal_rules, 4, 4, &skip);
    add_list_to_rules(non_roman_letters, lex_anal_rules, 4, 5, &add);
    add_list_to_rules(non_roman_letters, lex_anal_rules, 5, 5, &add);
    add_list_to_rules(symbols, lex_anal_rules, 5, 4, &write_word_and_next);
    add_list_to_rules(spaces, lex_anal_rules, 5, 4, &write_word);

    add_list_to_rules(roman_num, lex_anal_rules, 4, 6, &add);
    add_list_to_rules(roman_num, lex_anal_rules, 6, 6, &add);
    add_list_to_rules(symbols, lex_anal_rules, 6, 4, &write_act_and_next);
    add_list_to_rules(spaces, lex_anal_rules, 6, 4, &write_act);

    add_list_to_rules(symbols, lex_anal_rules, 4, 4, &write_word_and_next);

    //подаем результат работы препроцессора в лекс анализатор
    ifstream fin_lex("output_pre.txt");
    ofstream fout_lex("output_lex.txt");


    cout << "Starting lex analysis" << endl;
    //Начинаем лексический анализ

    //определяем начальные условия
    bufer = "";
    current_state = 0;
    while (fin_lex.get(input_symbol)) //считываем один символ
    {
        auto it = lex_anal_rules.find(make_pair(input_symbol, current_state)); 
        if (it == lex_anal_rules.end())  //проверяем, что правило существует
        {
            //Если нет - ошибка
            cout << "Error: wrong syntaxis:\t" << current_state << "\tbufer:" << bufer << "\tcurrent symble:" << static_cast<int> (input_symbol) << endl;
            break;
        }
        //загнали состояние и ввод в правила, получили результат
        auto action = (it->second).first; //получаем процедуру из правила перехода
        int next_state = (it->second).second; //получаем следующее состояние из правил перехода

        //string out_str = "";
        //out_str += input_symbol;
        //cout << "Doing something" << current_state << "\tbufer:" << bufer << "\tcurrent symble:" << static_cast<int> (input_symbol)<< endl;

        bufer = (*action)(bufer, input_symbol, fout_lex, fin_lex); //выполняем процедуру
        current_state = next_state; //переходим в следующее состояние
    }

    fin_lex.close();
    fout_lex.close();

}
















string clear_and_write_bufer(string bufer, char input_symble, ofstream& fout, ifstream& fin) //переносит содержимое буфера в поток вывода
{
    char c = tolower(input_symble); //переводим входной символ в нижний регистр
    bufer += c; //добавляем символ в буфер
    fout << bufer; //выводим буфер в файл вывода
    return ""; //очищаем буфер
}

//добавляет input_symble в bufer
string add_in_bufer(string bufer, char input_symble, ofstream& fout, ifstream& fin)
{
    char c = tolower(input_symble); //переводим входной символ в нижний регистр
    bufer += c; //добавляем символ в буфер
    return bufer;
}

//пока не нужно, но пусть будет
/*
string ungetch(string bufer, char input_symble, ofstream& fout, ifstream& fin)
{
    int cur_pos = fin.tellg();
    fin.seekg(cur_pos - 1); //или +1 ?
    return bufer;
}*/

//Препроцессор  заменяет ключевые слова "Акт" и "Сцена" на символы @ и $
//Это нужно для упрощения процедуры лексического анализа и удобного вывода результатов
string write_act_pre(string bufer, char input_symble, ofstream& fout, ifstream& fin)
{
    cout << "writing act\n";
    string str = "@";
    str += input_symble;
    fout << str;
    return "";
}


//Препроцессор  заменяет ключевые слова "Акт" и "Сцена" на символы @ и $
//Это нужно для упрощения процедуры лексического анализа и удобного вывода результатов
string write_scene_pre(string bufer, char input_symble, ofstream& fout, ifstream& fin)
{
    cout << "writing scene";
    string str = "$";
    str += input_symble;
    fout << str;
    return "";
}




//Добавление однотипных правил перехода для каждого символа из строки
void add_list_to_rules(char* symbols, std::map<std::pair<char, int>, std::pair<string(*)(string bufer, char   input_symble, ofstream& fout, ifstream& fin), int>> &rules, int start, int end, string(*func)(string bufer, char   input_symble, ofstream& fout, ifstream& fin)) {

    char* ptr = symbols; 
    while (*ptr) { //перебираем все символы
        //printf("%c",*ptr);
        rules[make_pair(*ptr, start)] = make_pair(func, end); // добавляем правило в  rules
        //preprocess_rules[make_pair('A', 1)] = make_pair(&add_in_bufer, 2);
        ++ptr;
    }
}


/*
int calcilate_pf(string pf[]) {
    string stack[];
    i = 0
    while(i < size.pf) :
        if i = operator :
            int n_args = i.args
            args  = []
            for j in range n_args:
                args.append(stack[-1])
                stack->pop - 1;
            execute(i, args)
            i++
        else:
            stack.append(pf[i])
            i++
}*/
