#include <cstdio>
#include <iostream>
#include <string>
#include <map>
#include<fstream>
#include <locale.h>
using namespace std;

//���������� �������
string clear_and_write_bufer(string bufer, char input_symble, ofstream& fout, ifstream& fin);
string add_in_bufer(string bufer, char input_symble, ofstream& fout, ifstream& fin);
string ungetch(string bufer, char input_symble, ofstream& fout, ifstream& fin);
string write_act_pre(string bufer, char input_symble, ofstream& fout, ifstream& fin);
string write_scene_pre(string bufer, char input_symble, ofstream& fout, ifstream& fin);
void add_list_to_rules(char* symbols, std::map<std::pair<char, int>, std::pair<string(*)(string bufer, char   input_symble, ofstream& fout, ifstream& fin), int>>& rules, int start, int end, string(*func)(string bufer, char   input_symble, ofstream& fout, ifstream& fin));
string skip(string bufer, char input_symble, ofstream& fout, ifstream& fin);



//������� ������� �������� ����, ������� ����������(����������), ������� �������� �����\���� (�����)
std::map<std::string, std::string> key_words{};
std::map<std::string, int> personality_table{}, act_scene_table{};
//� personality_table � act_scene_table ������ ����� �������������� ����� - ���������� ����� ����������, ������� ���������� � �������������� ����������




string skip(string bufer, char input_symble, ofstream& fout, ifstream& fin)
{
    return bufer;
}


//��������� ��������� ������ � �����
string add(string bufer, char input_symble, ofstream& fout, ifstream& fin)
{
    bufer += input_symble; //��������� ��������� ������ � �����
    return bufer;
}


//��������� ������� ���� ���
string write_name(string bufer, char input_symble, ofstream& fout, ifstream& fin)
{
    //fout << "Writing name:" << bufer << endl;
    if (personality_table.count(bufer) != 0)  //��������, ��� ��� �� �������� �������� ������ ��� ��������� ����\�����
    {
        cout << "Error. name " << bufer << " already exists." << endl; //���� �������� - ������� ������
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
    //����� ���������� ���������� � �������, ������������ - ��� <-> ������ �������� � personality_table
    int num = personality_table.size() + 1; //��������� ����� ����������
    personality_table[bufer] = num; //��������� ������ ��� <-> �����
    return "";
}

//��������� ������� ���� ���/�����
string write_act(string bufer, char input_symble, ofstream& fout, ifstream& fin)
{
    //fout << "Writing act:" << bufer << endl;
    //��������, ��� ��������� ����\����� �� �������� �������� ������ ��� ������
    if (personality_table.count(bufer) != 0)
    {
        cout << "Error. name " << bufer << " already exists." << endl; // //���� �������� - ������� ������
        return "";
    }
    if (key_words.count(bufer) != 0) {
        cout << "Error. name " << bufer << " must not be a key word." << endl; //���� �������� - ������� ������
        return "";
    }
    if (act_scene_table.count(bufer) != 0) { //���� ��� ��� ���������� - ������� ��� � ���� ������
        fout << "\"" << act_scene_table[bufer] << "\", ";
        //fout << " " << act_scene_table[bufer] << " ";
        return "";
    }
    //����� �����\���� - ������������� �����. ��������� ���������� � ����� �������, ������������ - ��� <-> ������ �������� � act_scene_table
    int num = -1 - act_scene_table.size(); //��������� ����� ����������
    act_scene_table[bufer] = num; //��������� ������ ��� <-> �����
    fout << "\"" << act_scene_table[bufer] << "\", ";  //������� � ���� ������
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

//������������ ������� ���� �����, ������� � ���� ������
string write_word(string bufer, char input_symble, ofstream& fout, ifstream& fin)
{
    //������� ����� �������������� ������� ��� �� ������� �������� ����, ��� �� ������� ����, ��� �� ������� ���� �����/����
    
    if (bufer == "@" or bufer == "$")  //��������� ��� ��� ����, �� � ����������� \n ��� �������� ���������� � �������
    {
        fout <<"\n" << "\"" << key_words[bufer] << "\", ";
        return "";
    }
    if (personality_table.count(bufer) != 0) { //���� ����� - ��� ��� ����������  - ������� � ���� ������ ��������������� �������
        fout << "\"" << personality_table[bufer] << "\", ";
        //fout << " " << personality_table[bufer] << " ";
        return "";
    }
    if (key_words.count(bufer) != 0) {  //���� ����� - ��� �������� �����  - ������� � ���� ������ ��������������� �������
        fout << "\"" << key_words[bufer] << "\", ";
        //fout << " " << key_words[bufer] << " ";
        return "";
    }
    if (act_scene_table.count(bufer) != 0) { //���� ����� - ��� ��� ����\�����  - ������� � ���� ������ ��������������� �������
        fout << "\"" << act_scene_table[bufer] << "\", ";
        //fout << " " << act_scene_table[bufer] << " ";
        return "";
    }
    //���� ����� ������ �� ������������� - ����������. ��������� ����� ��������� "������" ����� ��� ������� �������������� ���������������
    return "";
}

//������������ ��� �������, ������� ���� ����� �� bufer � ������� ���� ����� �� input_symble
string write_word_and_next(string bufer, char input_symble, ofstream& fout, ifstream& fin)
{
    string next = "";
    next += input_symble;
    //fout << "Writing words:" << bufer << "\t" << next << endl;
    write_word(bufer, input_symble, fout, fin); //������������ ����� ��� �����
    write_word(next, input_symble, fout, fin); //������������ input_symble ��� �����
    return "";
}

//������������ ��� �������, ������� ���� ��� ����/����� �� bufer � ������� ���� ����� �� input_symble
string write_act_and_next(string bufer, char input_symble, ofstream& fout, ifstream& fin)
{
    write_act(bufer, input_symble, fout, fin); //������������ ����� ��� ���\�����
    write_word_and_next("", input_symble, fout, fin); //������������ input_symble ��� �����
    return "";
}



int main()
{
    setlocale(LC_ALL, "RUSSIAN");
    ifstream keyword_table("keywords.txt");
    
    
    
    //���������� ������� �������� ����
    //������ ������ - input; ������ ������ - output
    while (!keyword_table.eof())
    {
        //��������� ��� ������
        string s_input, s_output;
        getline(keyword_table, s_input);  
        getline(keyword_table, s_output);
        

        key_words.insert(make_pair(s_input, s_output)); //��������� ������ � ������� �������� ����
        //cout << s_input << "  "<< s_output <<endl;
    }
    keyword_table.close();


    //������� ���� ���������������� 
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

    //� input.txt �������� �������� ��� �������������� ���������
    ifstream fin_pre("input.txt");
    //� output_pre.txt �������� ����� �������������
    ofstream fout_pre("output_pre.txt");
    std::map<std::pair<char, int>, std::pair<string(*)(string bufer, char input_symble, ofstream& fout, ifstream& fin), int>> preprocess_rules{};
    //��� ���� ��� ��������� int � ������� ����� char ������ � ������������ ���� ���������� ��������� � �������, ��� ����� ���-�� ������

    //������� ��������� �������� ��� �������������:
    //������������ ��������� ��� ������� � ������ ������� � �������� �������� ����� "���" � "�����" �� ������� @ � $
    //��� ����� ��� ��������� ��������� ������������ ������� � �������� ������ �����������
    preprocess_rules[make_pair('�', 1)] = make_pair(&add_in_bufer, 2); //��������� ������� - �� ��������� 1 ��� ��������� input_symble = '�' 
    //�� ��������� � ��������� 2 � ��������� ��������� add_in_bufer
    preprocess_rules[make_pair('�', 2)] = make_pair(&add_in_bufer, 3);
    preprocess_rules[make_pair('�', 3)] = make_pair(&add_in_bufer, 4);
    preprocess_rules[make_pair(' ', 4)] = make_pair(&write_act_pre, 1);
    preprocess_rules[make_pair('\n', 4)] = make_pair(&write_act_pre, 1);

    preprocess_rules[make_pair('�', 1)] = make_pair(&add_in_bufer, 5);
    preprocess_rules[make_pair('�', 5)] = make_pair(&add_in_bufer, 6);
    preprocess_rules[make_pair('�', 6)] = make_pair(&add_in_bufer, 7);
    preprocess_rules[make_pair('�', 7)] = make_pair(&add_in_bufer, 8);
    preprocess_rules[make_pair('�', 8)] = make_pair(&add_in_bufer, 9);
    preprocess_rules[make_pair(' ', 9)] = make_pair(&write_scene_pre, 1);
    preprocess_rules[make_pair('\n', 9)] = make_pair(&write_scene_pre, 1);




    //������� ���������� ��������� ��������:
    
    //���������� ��������� �������
    string bufer = ""; 
    char input_symbol;
    int current_state = 1;
    while (fin_pre.get(input_symbol)) //��������� ���� ������
    {
        auto it = preprocess_rules.find(make_pair(input_symbol, current_state));
        if (it == preprocess_rules.end()) //���������, ��� ������� ����������
        {
            //���� ���:

            //cout << "skip " << current_state << "\t�����:" << bufer << "\t������� ������:" << static_cast<int> (input_symbol) << endl;
 
            //�� ������ ��� ������ - �� ������
            bufer = clear_and_write_bufer(bufer, input_symbol, fout_pre, fin_pre); //��������� ���������� ������ � ���� ������ 
            if (input_symbol == ' ' or input_symbol == '\n') //��������� � ��������� 1(����� ����) ����� �����������
                current_state = 1;
            else
                current_state = 0; //��������� � ��������� 0 (������ ������������ �����) 
            continue;
        }
        //������� ��������� � ���� � �������, �������� ���������
        auto action = (it->second).first; //�������� ��������� �� ������� ��������
        int next_state = (it->second).second; //�������� ��������� ��������� �� ������ ��������

        
        bufer = (*action)(bufer, input_symbol, fout_pre, fin_pre); //��������� ���������
        current_state = next_state; //��������� � ��������� ���������
    }
 


    fin_pre.close();
    fout_pre.close();

    //������� ��������� �������� ��� ������������ �������
    std::map<std::pair<char, int>, std::pair<string(*)(string bufer, char   input_symble, ofstream& fout, ifstream& fin), int>> lex_anal_rules{};

    //��������� �������� �� �������� �� ������
    char letters[] = "����������������������������������������������������������������ivxlcdm";
    char non_roman_letters[] = "����������������������������������������������������������������";
    char spaces[] = "\n\t ";
    char all_but_point[] = "����������������������������������������������������������������,?:! \n\tivxlcdm";
    char end_sentence[] = ".!?";
    char divinity[] = " \n\t,";
    char roman_num[] = "ivxlcdm";
    char symbols[] = ".!?,@$[]:";
    //add_list_to_rules(letters, lex_anal_rules, 0, 1, &skip);
    //preprocess_rules[make_pair('�', 1)] = make_pair(&add_in_bufer, 2);

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

     //��� ������� ������������������ ���������� � ������ �����
    add_list_to_rules(all_but_point, lex_anal_rules, 0, 0, &skip); //1
    //��� ������� ������� �� ������� all_but_point ��������� � lex_anal_rules �������: (������, ��������� 0) -> (��������� 0, ��������� skip)
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

    //������ ��������� ������ ������������� � ���� ����������
    ifstream fin_lex("output_pre.txt");
    ofstream fout_lex("output_lex.txt");


    cout << "Starting lex analysis" << endl;
    //�������� ����������� ������

    //���������� ��������� �������
    bufer = "";
    current_state = 0;
    while (fin_lex.get(input_symbol)) //��������� ���� ������
    {
        auto it = lex_anal_rules.find(make_pair(input_symbol, current_state)); 
        if (it == lex_anal_rules.end())  //���������, ��� ������� ����������
        {
            //���� ��� - ������
            cout << "Error: wrong syntaxis:\t" << current_state << "\tbufer:" << bufer << "\tcurrent symble:" << static_cast<int> (input_symbol) << endl;
            break;
        }
        //������� ��������� � ���� � �������, �������� ���������
        auto action = (it->second).first; //�������� ��������� �� ������� ��������
        int next_state = (it->second).second; //�������� ��������� ��������� �� ������ ��������

        //string out_str = "";
        //out_str += input_symbol;
        //cout << "Doing something" << current_state << "\tbufer:" << bufer << "\tcurrent symble:" << static_cast<int> (input_symbol)<< endl;

        bufer = (*action)(bufer, input_symbol, fout_lex, fin_lex); //��������� ���������
        current_state = next_state; //��������� � ��������� ���������
    }

    fin_lex.close();
    fout_lex.close();

}
















string clear_and_write_bufer(string bufer, char input_symble, ofstream& fout, ifstream& fin) //��������� ���������� ������ � ����� ������
{
    char c = tolower(input_symble); //��������� ������� ������ � ������ �������
    bufer += c; //��������� ������ � �����
    fout << bufer; //������� ����� � ���� ������
    return ""; //������� �����
}

//��������� input_symble � bufer
string add_in_bufer(string bufer, char input_symble, ofstream& fout, ifstream& fin)
{
    char c = tolower(input_symble); //��������� ������� ������ � ������ �������
    bufer += c; //��������� ������ � �����
    return bufer;
}

//���� �� �����, �� ����� �����
/*
string ungetch(string bufer, char input_symble, ofstream& fout, ifstream& fin)
{
    int cur_pos = fin.tellg();
    fin.seekg(cur_pos - 1); //��� +1 ?
    return bufer;
}*/

//������������  �������� �������� ����� "���" � "�����" �� ������� @ � $
//��� ����� ��� ��������� ��������� ������������ ������� � �������� ������ �����������
string write_act_pre(string bufer, char input_symble, ofstream& fout, ifstream& fin)
{
    cout << "writing act\n";
    string str = "@";
    str += input_symble;
    fout << str;
    return "";
}


//������������  �������� �������� ����� "���" � "�����" �� ������� @ � $
//��� ����� ��� ��������� ��������� ������������ ������� � �������� ������ �����������
string write_scene_pre(string bufer, char input_symble, ofstream& fout, ifstream& fin)
{
    cout << "writing scene";
    string str = "$";
    str += input_symble;
    fout << str;
    return "";
}




//���������� ���������� ������ �������� ��� ������� ������� �� ������
void add_list_to_rules(char* symbols, std::map<std::pair<char, int>, std::pair<string(*)(string bufer, char   input_symble, ofstream& fout, ifstream& fin), int>> &rules, int start, int end, string(*func)(string bufer, char   input_symble, ofstream& fout, ifstream& fin)) {

    char* ptr = symbols; 
    while (*ptr) { //���������� ��� �������
        //printf("%c",*ptr);
        rules[make_pair(*ptr, start)] = make_pair(func, end); // ��������� ������� �  rules
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
