#ifndef _ASSEMBLER_H_
#define _ASSEMBLER_H_

#include "symbol_table.h"
#include "const_table.h"
#include "my_list.h"
#include "relocation_table.h"
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <regex>
#include <cstdint>

#include <bits/stdc++.h>
using namespace std;

const regex label_regex("(^[\\s]*[_a-zA-Z][a-zA-Z_]+:([\\s]|[\\w])*$)");
const regex global_regex("(^[\\s]*(.global)[\\s]+(([_a-zA-Z]+)|(([_a-zA-Z]+,[\\s]+)+[_a-zA-Z]+))[\\s]*$)");
const regex extern_regex("(^[\\s]*(.extern)[\\s]+(([_a-zA-Z]+)|(([_a-zA-Z]+,[\\s]+)+[_a-zA-Z]+))[\\s]*$)");
const regex section_regex("(^[\\s]*(.section)([\\s])+[a-zA-Z_.]+[\\s]*$)");
const regex word_regex("(^[\\s]*(.word)[\\s]+(([_a-zA-Z0-9]+)|(([_a-zA-Z0-9]+,[\\s]+)+[_a-zA-Z0-9]+))[\\s]*$)");
const regex skip_regex("(^[\\s]*(.skip)[\\s]+[a-zA-Z_0-9]+[\\s]*$)");
const regex equ_regex("(^[\\s]*(.equ)[\\s]+[a-zA-Z_]+,[\\s]+[a-fA-F0-9x]+[\\s]*$)");
const regex end_regex("(^(.end)([\\s])*$)");
const regex decimal_regex("(^[\\s]*[0-9]+[\\s]*$)");
const regex hexa_regex("(^[\\s]*(0x[a-fA-F0-9]+)[\\s]*$)");
const regex halt_regex("(^[\\s]*(halt)[\\s]*$)");
const regex interrupt_regex("(^[\\s]*(int)[\\s]+((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw))[\\s]*$)");
const regex iret_regex("(^[\\s]*(iret)[\\s]*$)");
const regex call_literal_regex("(^[\\s]*(call)[\\s]+([a-fA-F0-9x]+)[\\s]*$)");
const regex call_symbol_regex("(^[\\s]*(call)[\\s]+([a-zA-Z_]+)[\\s]*$)");
const regex call_symbolPC_regex("(^[\\s]*(call)[\\s]+(%[0-9a-zA-Z_]+)[\\s]*$)");
const regex call_literalMEM_regex("(^[\\s]*(call)[\\s]+(\\*[a-fA-F0-9x]+)[\\s]*$)");
const regex call_symbolMEM_regex("(^[\\s]*(call)[\\s]+(\\*[a-zA-Z_]+)[\\s]*$)");
const regex call_reg_regex("(^[\\s]*(call)[\\s]+\\*((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw))[\\s]*$)");
const regex call_regIND_regex("(^[\\s]*(call)[\\s]+\\*[\\s]*\\[[\\s]*((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw))[\\s]*\\][\\s]*$)");
const regex call_regIND_literal_regex("(^[\\s]*(call)[\\s]+\\*[\\s]*\\[[\\s]*(((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw))[\\s]+\\+[\\s]+[a-fA-F0-9x]+)[\\s]*\\][\\s]*$)");
const regex call_regIND_symbol_regex("(^[\\s]*(call)[\\s]+\\*[\\s]*\\[[\\s]*(((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw))[\\s]+\\+[\\s]+[a-zA-Z_]+)[\\s]*\\][\\s]*$)");
const regex ret_regex("(^[\\s]*(ret)[\\s]*$)");
const regex jump_literal_regex("(^[\\s]*((jmp)|(jeq)|(jne)|(jgt))[\\s]+([a-fA-F0-9x]+)[\\s]*$)");
const regex jump_symbol_regex("(^[\\s]*((jmp)|(jeq)|(jne)|(jgt))[\\s]+([a-zA-Z_]+)[\\s]*$)");
const regex jump_symbolPC_regex("(^[\\s]*((jmp)|(jeq)|(jne)|(jgt))[\\s]+(%[0-9a-zA-Z_]+)[\\s]*$)");
const regex jump_literalMEM_regex("(^[\\s]*((jmp)|(jeq)|(jne)|(jgt))[\\s]+(\\*[a-fA-F0-9x]+)[\\s]*$)");
const regex jump_symbolMEM_regex("(^[\\s]*((jmp)|(jeq)|(jne)|(jgt))[\\s]+(\\*[a-zA-Z_]+)[\\s]*$)");
const regex jump_reg_regex("(^[\\s]*((jmp)|(jeq)|(jne)|(jgt))[\\s]+\\*((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw))[\\s]*$)");
const regex jump_regIND_regex("(^[\\s]*((jmp)|(jeq)|(jne)|(jgt))[\\s]+\\*[\\s]*\\[[\\s]*((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw))[\\s]*\\][\\s]*$)");
const regex jump_regIND_literal_regex("(^[\\s]*((jmp)|(jeq)|(jne)|(jgt))[\\s]+\\*[\\s]*\\[[\\s]*(((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw))[\\s]+\\+[\\s]+[a-fA-F0-9x]+)[\\s]*\\][\\s]*$)");
const regex jump_regIND_symbol_regex("(^[\\s]*((jmp)|(jeq)|(jne)|(jgt))[\\s]+\\*[\\s]*\\[[\\s]*(((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw))[\\s]+\\+[\\s]+[a-zA-Z_]+)[\\s]*\\][\\s]*$)");
const regex push_regex("(^[\\s]*(push)[\\s]+((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw))[\\s]*$)");
const regex pop_regex("(^[\\s]*(pop)[\\s]+((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw))[\\s]*$)");
const regex xchg_regex("(^[\\s]*(xchg)[\\s]+(((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw)),)[\\s]+((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw))[\\s]*$)");
const regex aritmetic_regex("(^[\\s]*((add)|(sub)|(mul)|(div)|(cmp))[\\s]+(((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw)),)[\\s]+((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw))[\\s]*$)");
const regex logic_regex("(^[\\s]*(((not)[\\s]+((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw)))|(((and)|(or)|(xor)|(test))[\\s]+(((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw)),)[\\s]+((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw))))[\\s]*$)");
const regex shift_regex("(^[\\s]*((shl)|(shr))[\\s]+(((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw)),)[\\s]+((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw))[\\s]*$)");
const regex empty_regex("(^[\\s]*$)");
const regex ldrstr_literal_regex("(^[\\s]*((ldr|(str))[\\s]+(((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw)),)[\\s]+\\$[a-fA-F0-9x]+[\\s]*$))");
const regex ldrstr_simbol_regex("(^[\\s]*((ldr|(str))[\\s]+(((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw)),)[\\s]+\\$[a-zA-Z_]+[\\s]*$))");
const regex ldrstr_literalMEM_regex("(^[\\s]*((ldr|(str))[\\s]+(((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw)),)[\\s]+[a-fA-F0-9x]+[\\s]*$))");
const regex ldrstr_simbolMEM_regex("(^[\\s]*((ldr|(str))[\\s]+(((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw)),)[\\s]+[a-zA-Z_]+[\\s]*$))");
const regex ldrstr_simbolPC_regex("(^[\\s]*((ldr|(str))[\\s]+(((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw)),)[\\s]+\\%[a-zA-Z]+[\\s]*$))");
const regex ldrstr_reg_regex("(^[\\s]*((ldr|(str))[\\s]+(((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw)),)[\\s]+(((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw)))[\\s]*$))");
const regex ldrstr_regMEM_regex("(^[\\s]*((ldr|(str))[\\s]+(((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw)),)[\\s]+\\[[\\s]*(((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw)))[\\s]*\\][\\s]*$))");
const regex ldrstr_regMEM_literal_regex("(^[\\s]*((ldr|(str))[\\s]+(((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw)),)[\\s]+\\[[\\s]*((((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw)))[\\s]+\\+[\\s]+[a-fA-F0-9x]+)[\\s]*\\][\\s]*$))");
const regex ldrstr_regMEM_symbol_regex("(^[\\s]*((ldr|(str))[\\s]+(((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw)),)[\\s]+\\[[\\s]*((((r0)|(r1)|(r2)|(r3)|(r4)|(r5)|(r6)|(r7)|(psw)))[\\s]+\\+[\\s]+[a-zA-Z_]+)[\\s]*\\][\\s]*$))");

class Assembler
{
private:
    string input_file;
    string output_file;
    string current_section;
    ifstream input;
    ofstream output;
    Symbol_Table symbol_table;
    Const_Table const_table;
    Relocation_Table relocation_table;
    int location_counter;

public:
    Assembler(string in_file, string out_file)
    {
        this->input_file = in_file;
        this->output_file = out_file;
        location_counter = 0;
        current_section = "";
    }
    ~Assembler()
    {
    }

    vector<string> split_line(string in)
    { //izdeli string na delove
        vector<string> split_return;
        stringstream input_stringstream(in);

        do
        {
            string element;
            input_stringstream >> element;
            split_return.push_back(element);

        } while (input_stringstream);

        split_return.pop_back();
        return split_return;
    }
    int instruction_size = 0;
    int num_of_lines = 0;
    bool first_pass_succes = true;
    void first_pass()
    {
        input.open(input_file);

        int last_line = -1;

        if (input.is_open())
        {

            vector<string> split;
            string code_line = " ";
            //add UNDEFINED

            Symbol_Table_Entry ste("", "UND", location_counter);
            ste.set_locality_local();
            symbol_table.add_entry(ste);

            while (getline(input, code_line))
            {

                num_of_lines++;
                split = split_line(code_line);
                if (split.size() == 0)
                { //prazan red

                    continue;
                }
                if (num_of_lines == last_line)
                {
                    cout << "Greska! .end mora biti poslednja!" << endl;
                    first_pass_succes = false;
                }

                string first_element = split[0];
                // KOMENTARI

                size_t comment_start = code_line.find("#", 0);
                if (comment_start != string::npos)
                {
                    code_line = code_line.substr(0, comment_start);
                    regex empty_regex("(^[\\s]*$)");
                    if (regex_match(code_line, empty_regex))
                        continue;
                }

                //                    ***********SEKCIJA******************
                if (is_it_section(code_line))
                {
                    string section_name = split[1];
                    // section_name.erase(0, 1);
                    current_section = section_name;
                    location_counter = 0;
                    add_to_symbol_table(section_name);
                    continue;
                }
                //                             ********LABELA***********
                if (is_it_label(code_line))
                { //labela
                    if (current_section == "")
                    {
                        cout << "Greska! Nijedna sekcija nije definisana!" << endl;
                        first_pass_succes = false;
                        break;
                    }
                    string label_name = first_element;

                    label_name.pop_back();
                    if (symbol_table.label_exists(label_name))
                    {
                        symbol_table.update_symbol_entry(label_name, current_section, location_counter);
                    }
                    else
                        add_to_symbol_table(label_name);
                    int i;
                    continue;
                }
                /// **** GLOBAL I EXTERN****
                if (is_it_externORglobal(code_line))
                {

                    vector<string> split = split_line(code_line);
                    split.erase(split.begin());
                    for (int i = 0; i < split.size(); i++)
                    {
                        size_t comma_start = split[i].find(",", 0);
                        if (comma_start != string::npos)
                        {
                            split[i] = split[i].substr(0, comma_start);
                        }
                        add_to_const_table(split[i], -1);
                    }

                    continue;
                }
                /// ****EQU********
                if (is_it_equ(code_line))
                {
                    vector<string> split = split_line(code_line);
                    split.erase(split.begin());
                    size_t comma_start = split[0].find(",", 0); //ime
                    if (comma_start != string::npos)
                    {
                        split[0] = split[0].substr(0, comma_start);
                    }
                    //vrednost
                    int val = 0;
                    if (regex_match(split[1], decimal_regex))
                    {
                        stringstream dec(split[1]);

                        dec >> val;
                    }
                    else if (regex_match(split[1], hexa_regex))
                    {
                        split[1].erase(0, 2); // erase 0 and  x
                        val = stoi(split[1], 0, 16);
                    }
                    add_to_const_table(split[0], val);

                    continue;
                }
                // *** WORD ******

                if (is_it_word(code_line))
                {

                    continue;
                }

                // ** SKIP ***
                if (is_it_skip(code_line))
                {

                    continue;
                }

                /// *** END ****
                if (is_it_end(code_line))
                {
                    last_line = num_of_lines;
                    continue;
                }

                /// **** INSTRUKCIJE ******
                if (!is_it_instruction(code_line))
                {

                    cout << "Greska na liniji " << num_of_lines << endl;
                    first_pass_succes = false;
                }
                else
                {
                    if (current_section == "")
                    {
                        cout << "Greska! Nijedna sekcija nije definisana! Na liniji : " << num_of_lines << endl;
                        first_pass_succes = false;
                        break;
                    }
                }
            }
        }

        input.close();
    }

    void print_symbol_table()
    {

        ofstream output(output_file);
        output << symbol_table;

        output.close();
    }

    void add_to_const_table(string name, int val)
    {
        if (const_table.name_exists(name))
        {
            const_table.update_const_entry(name, val);
        }
        else
        {
            Const_Table_Entry cte(name, val);
            const_table.add_entry(cte);
        }
    }

    bool is_it_label(string s)
    {
        if (regex_search(s, label_regex))
        {

            string inst;
            size_t start = s.find(":", 0);
            if (start != string::npos)
            {
                inst = s.erase(0, start + 1);
            }
            if (!regex_match(inst, empty_regex))
            {
                if (!is_it_instruction(inst))
                {
                    first_pass_succes = false;
                    cout << "Greska! Na liniji: " << num_of_lines << " Nakon labele mora ili prazan red ili instrukcija!" << endl;
                }
            }
            return true;
        }
        return false;
    }

    bool is_it_section(string s)
    {
        if (regex_search(s, section_regex))
        {
            return true;
        }
        return false;
    }

    bool is_it_instruction(string s)
    {

        // **********HALT********

        if (regex_match(s, halt_regex))
        {
            instruction_size = 1;
            location_counter += 1;
            return true;
        }
        // ************ SOFT INT **********

        if (regex_match(s, interrupt_regex))
        {
            instruction_size = 2;
            location_counter += 2;
            return true;
        }

        //************IRET**********

        if (regex_match(s, iret_regex))
        {
            instruction_size = 1;
            location_counter += 1;
            return true;
        }

        // ***********CALL OPERAND***********

        if (regex_match(s, call_regIND_symbol_regex))
        {
            instruction_size = 5;
            location_counter += 5;
            return true;
        }

        else if (regex_match(s, call_symbol_regex))
        {
            instruction_size = 5;
            location_counter += 5;
            return true;
        }
        else if (regex_match(s, call_symbolPC_regex))
        {
            instruction_size = 5;
            location_counter += 5;
            return true;
        }

        else if (regex_match(s, call_symbolMEM_regex))
        {
            instruction_size = 5;
            location_counter += 5;
            return true;
        }
        else if (regex_match(s, call_reg_regex))
        {
            instruction_size = 3;
            location_counter += 3;
            return true;
        }
        else if (regex_match(s, call_regIND_regex))
        {
            instruction_size = 3;
            location_counter += 3;
            return true;
        }
        else if (regex_match(s, call_regIND_literal_regex))
        {
            instruction_size = 5;
            location_counter += 5;
            return true;
        }
        else if (regex_match(s, call_literalMEM_regex))
        {
            instruction_size = 5;
            location_counter += 5;
            return true;
        }

        else if (regex_match(s, call_literal_regex))
        {
            instruction_size = 5;
            location_counter += 5;
            return true;
        }

        /////      **********RET********

        if (regex_match(s, ret_regex))
        {
            instruction_size = 1;
            location_counter += 1;
            return true;
        }

        ///       ********JUMPS***********

        if (regex_match(s, jump_regIND_symbol_regex))
        {
            instruction_size = 5;
            location_counter += 5;
            return true;
        }

        else if (regex_match(s, jump_symbol_regex))
        {
            instruction_size = 5;
            location_counter += 5;
            return true;
        }
        else if (regex_match(s, jump_symbolPC_regex))
        {
            instruction_size = 5;
            location_counter += 5;
            return true;
        }

        else if (regex_match(s, jump_symbolMEM_regex))
        {
            instruction_size = 5;
            location_counter += 5;
            return true;
        }
        else if (regex_match(s, jump_reg_regex))
        {
            instruction_size = 3;
            location_counter += 3;
            return true;
        }
        else if (regex_match(s, jump_regIND_regex))
        {
            instruction_size = 3;
            location_counter += 3;
            return true;
        }
        else if (regex_match(s, jump_regIND_literal_regex))
        {
            instruction_size = 5;
            location_counter += 5;
            return true;
        }
        else if (regex_match(s, jump_literalMEM_regex))
        {
            instruction_size = 5;
            location_counter += 5;
            return true;
        }

        else if (regex_match(s, jump_literal_regex))
        {
            instruction_size = 5;
            location_counter += 5;
            return true;
        }

        // ***********PUSH***********

        if (regex_match(s, push_regex))
        {
            instruction_size = 3;
            location_counter += 3;
            return true;
        }
        // ***********POP***********

        if (regex_match(s, pop_regex))
        {
            instruction_size = 3;
            location_counter += 3;
            return true;
        }
        // ************XCHANGE***********

        if (regex_match(s, xchg_regex))
        {
            instruction_size = 2;
            location_counter += 2;
            return true;
        }

        // ***********ARITMETIC***************

        if (regex_match(s, aritmetic_regex))
        {
            instruction_size = 2;
            location_counter += 2;
            return true;
        }
        // ***********LOGICAL***************

        if (regex_match(s, logic_regex))
        {
            instruction_size = 2;
            location_counter += 2;
            return true;
        }
        // ***********SHIFT***************
        if (regex_match(s, shift_regex))
        {
            instruction_size = 2;
            location_counter += 2;
            return true;
        }

        //*********LOAD AND STORE**************

        if (regex_match(s, ldrstr_simbol_regex))
        {
            instruction_size = 5;
            location_counter += 5;
            return true;
        }
        else if (regex_match(s, ldrstr_simbolMEM_regex))
        {
            instruction_size = 5;
            location_counter += 5;
            return true;
        }
        else if (regex_match(s, ldrstr_simbolPC_regex))
        {
            instruction_size = 5;
            location_counter += 5;
            return true;
        }
        else if (regex_match(s, ldrstr_reg_regex))
        {
            instruction_size = 3;
            location_counter += 3;
            return true;
        }
        else if (regex_match(s, ldrstr_regMEM_regex))
        {
            instruction_size = 3;
            location_counter += 3;
            return true;
        }
        else if (regex_match(s, ldrstr_regMEM_symbol_regex))
        {
            instruction_size = 5;
            location_counter += 5;
            return true;
        }
        else if (regex_match(s, ldrstr_regMEM_literal_regex))
        {
            instruction_size = 5;
            location_counter += 5;
            return true;
        }
        else if (regex_match(s, ldrstr_literalMEM_regex))
        {
            instruction_size = 5;
            location_counter += 5;
            return true;
        }
        else if (regex_match(s, ldrstr_literal_regex))
        {
            instruction_size = 5;
            location_counter += 5;
            return true;
        }

        return false;
    }

    void add_to_symbol_table(string label)
    {
        Symbol_Table_Entry ste(label, current_section, location_counter);
        ste.set_locality_local();
        symbol_table.add_entry(ste);
    }

    bool is_it_skip(string s)
    {
        if (regex_match(s, skip_regex))
        {
            vector<string> split = split_line(s);
            split.erase(split.begin());

            if (regex_match(split[0], decimal_regex))
            {
                stringstream dec(split[0]);
                int add = 0;
                dec >> add;
                location_counter += add;
                return true;
            }
            else if (regex_match(split[0], hexa_regex))
            {
                split[0].erase(0, 2); // erase 0 and  x
                int add = stoi(split[0], 0, 16);
                location_counter += add;
                return true;
            }
            else

                return false;
        }
        return false;
    }

    bool is_it_word(string s)
    {
        if (regex_match(s, word_regex))
        {
            vector<string> split = split_line(s);
            split.erase(split.begin());

            for (int i = 0; i < split.size(); i++)
            { //brisi zarez

                size_t comma = split[i].find(",", 0);
                if (comma != string::npos)
                {
                    split[i] = split[i].substr(0, comma);
                }
            }
            location_counter += 2 * split.size();
            return true;
        }
        return false;
    }

    bool is_it_equ(string s)
    {
        if (regex_match(s, equ_regex))
        {
            return true;
        }
        return false;
    }

    bool is_it_end(string s)
    {
        if (regex_match(s, end_regex))
        {
            return true;
        }
        return false;
    }

    bool is_it_externORglobal(string s)
    {
        if (regex_match(s, global_regex))
        {
            return true;
        }
        else if (regex_match(s, extern_regex))
        {
            return true;
        }
        else
            return false;
    }

    // *** SECOND PASS *********

    string out_code = "";

    void second_pass()
    {
        if (first_pass_succes)
        {
            input.open(input_file);
            int num_of_lines = 0;
            int last_line = -1;

            if (input.is_open())
            {

                vector<string> split;
                string code_line = " ";
                while (getline(input, code_line))
                {

                    num_of_lines++;
                    split = split_line(code_line);
                    if (split.size() == 0)
                    { //prazan red

                        continue;
                    }

                    string first_element = split[0];
                    // KOMENTARI

                    size_t comment_start = code_line.find("#", 0);
                    if (comment_start != string::npos)
                    {
                        code_line = code_line.substr(0, comment_start);

                        if (regex_match(code_line, empty_regex))
                            continue;
                    }

                    //                    ***********SEKCIJA******************
                    if (is_it_section(code_line))
                    {
                        string section_name = split[1];
                        current_section = section_name;
                        location_counter = 0;
                        continue;
                    }
                    //                             ********LABELA***********
                    if (is_it_label(code_line))
                    { //labela

                        string label_name = first_element;

                        continue;
                    }
                    /// **** GLOBAL I EXTERN****
                    if (is_it_externORglobal(code_line))
                    {
                        vector<string> split = split_line(code_line);
                        split.erase(split.begin());
                        for (int i = 0; i < split.size(); i++)
                        {
                            size_t comma_start = split[i].find(",", 0);
                            if (comma_start != string::npos)
                            {
                                split[i] = split[i].substr(0, comma_start);
                            }
                            if (symbol_table.label_exists(split[i]))
                            {

                                symbol_table.set_global(split[i]);
                            }
                        }
                        continue;
                    }
                    /// ****EQU********
                    if (is_it_equ(code_line))
                    {

                        continue;
                    }
                    // *** WORD ******

                    if (is_it_word(code_line))
                    {
                        vector<string> split = split_line(code_line);
                        split.erase(split.begin());

                        for (int i = 0; i < split.size(); i++)
                        { //brisi zarez

                            size_t comma = split[i].find(",", 0);
                            if (comma != string::npos)
                            {
                                split[i] = split[i].substr(0, comma);
                            }
                            int code = symbol_table.get_offset(split[i]);
                            out_code += decToHexa(code, false);
                        }

                        continue;
                    }

                    // ** SKIP ***
                    if (is_it_skip(code_line)) // skip 9
                    {
                        vector<string> split = split_line(code_line);

                        if (regex_match(split[1], decimal_regex))
                        {
                            stringstream dec(split[1]);
                            int add = 0;
                            dec >> add;
                            location_counter += add;
                            for (int i = 0; i < add; i++)
                                out_code += "00";
                        }
                        else if (regex_match(split[1], hexa_regex)) // 0xabcd
                        {
                            split[0].erase(0, 2); // erase 0 and  x
                            int add = stoi(split[1], 0, 16);
                            location_counter += add;
                            for (int i = 0; i < add; i++)
                                out_code += "00";
                        }

                        continue;
                    }

                    /// *** END ****
                    if (is_it_end(code_line))
                    {

                        continue;
                    }

                    /// **** INSTRUKCIJE ******
                    if (is_it_instruction(code_line))
                    {
                        out_code += instruction_code(code_line);
                    }
                }
            }
        }
        // cout << out_code << endl;
        // cout << symbol_table << endl
        //      << relocation_table;
        print_to_out_file();
    }

    void print_to_out_file()
    {
        output.open(output_file);
        output << out_code << endl;
        output << symbol_table << endl;
        output << relocation_table << endl;
        output.close();
    }

    string register_code(string reg)
    {
        if (reg == "r0")
            return "0";
        else if (reg == "r1")
            return "1";
        else if (reg == "r2")
            return "2";
        else if (reg == "r3")
            return "3";
        else if (reg == "r4")
            return "4";
        else if (reg == "r5")
            return "5";
        else if (reg == "r6")
            return "6";
        else if (reg == "r7")
            return "7";
        else
            return "8";
    }

    string instruction_code(string inst)
    {
        if (regex_match(inst, halt_regex)) //HALT
        {
            return "00";
        }
        else if (regex_match(inst, interrupt_regex)) //INT REG
        {
            vector<string> split;
            split = split_line(inst);
            string int_ret = "10";
            int_ret += register_code(split[1]);
            int_ret += "f";
            return int_ret;
        }
        else if (regex_match(inst, iret_regex)) // IRET
        {
            return "20";
        }
        else if (regex_match(inst, call_regIND_symbol_regex)) //call *[r0 + simbol]
        {
            vector<string> split;
            split = split_line(inst);
            string ret = "30f";
            string i;
            //NADJI U KODU REGISTAR
            size_t start = inst.find("[", 0);
            if (start != string::npos)
            {
                i = inst.substr(start + 1, inst.size());
            }

            split = split_line(i);
            split[2].pop_back();
            ret += register_code(split[0]);
            ret += "05";
            int offset = location_counter - 2;
            ret += r_386_PC32_code(split[2], offset);
            return ret;
        }

        else if (regex_match(inst, call_symbol_regex)) // call simbol
        {
            vector<string> split;
            split = split_line(inst);
            string ret = "30f000";
            int offset = location_counter - 2;
            ret += r_386_PC32_code(split[1], offset);
            return ret;
        }
        else if (regex_match(inst, call_symbolPC_regex)) // call %simbol
        {
            vector<string> split;
            split = split_line(inst);
            string ret = "30f";
            ret += register_code("r7"); //pc
            ret += "03";
            int offset = location_counter - 2;
            split[1].erase(0, 1);
            ret += r_386_PC32_code(split[1], offset);
            return ret;
        }

        else if (regex_match(inst, call_symbolMEM_regex)) //call *simbol
        {
            vector<string> split;
            split = split_line(inst);
            string ret = "30f004";
            int offset = location_counter - 2;
            split[1].erase(0, 1);
            ret += r_386_PC32_code(split[1], offset);
            return ret;
        }
        else if (regex_match(inst, call_reg_regex)) // CALL *R0
        {
            vector<string> split;
            split = split_line(inst);
            string ret = "30f";
            split[1].erase(0, 1);
            ret += register_code(split[1]);
            ret += "01";

            return ret;
        }
        else if (regex_match(inst, call_regIND_regex)) // CALL *[R0]
        {
            vector<string> split;
            split = split_line(inst);
            string ret = "30f";
            split[1].erase(0, 1);
            ret += register_code(split[1]);
            ret += "02";
            return ret;
        }
        else if (regex_match(inst, call_regIND_literal_regex)) //CALL *[R0 + 2]
        {
            vector<string> split;
            split = split_line(inst);
            string ret = "30f";
            string i;
            //NADJI U KODU REGISTAR
            size_t start = inst.find("[", 0);
            if (start != string::npos)
            {
                i = inst.substr(start + 1, inst.size());
            }

            split = split_line(i);
            split[2].pop_back();
            ret += register_code(split[0]);
            ret += "05";
            if (regex_match(split[2], decimal_regex))
            {
                ret += "000";
                ret += split[2];
            }
            else if (regex_match(split[2], hexa_regex))
            {
                split[2].erase(0, 2);
                int hexa = stoi(split[2], 0, 16);
                ret += decToHexa(hexa, false);
            }
            return ret;
        }
        else if (regex_match(inst, call_literalMEM_regex)) //CALL *1
        {
            vector<string> split;
            split = split_line(inst);
            string ret = "30f004";
            split[1].erase(0, 1);
            if (regex_match(split[1], decimal_regex))
            {
                ret += "000";
                ret += split[1];
            }
            else if (regex_match(split[1], hexa_regex))
            {
                split[1].erase(0, 2);
                int hexa = stoi(split[1], 0, 16);
                ret += decToHexa(hexa, false);
            }
            return ret;
        }

        else if (regex_match(inst, call_literal_regex)) //CALL 5
        {
            vector<string> split;
            split = split_line(inst);
            string ret = "30f000";

            if (regex_match(split[1], decimal_regex))
            {
                ret += "000";
                ret += split[1];
            }
            else if (regex_match(split[1], hexa_regex))
            {
                split[1].erase(0, 2); //Ox

                int hexa = stoi(split[1], 0, 16);
                ret += decToHexa(hexa, false);
            }
            return ret;
        }

        else if (regex_match(inst, ret_regex)) // RET
        {
            return "40";
        }

        else if (regex_match(inst, jump_regIND_symbol_regex)) // jgt *[r0 + 1]
        {
            vector<string> split;
            split = split_line(inst);
            string ret = "5";
            ret += jump_code(split[0]);
            ret += "f";
            string i;
            size_t start = inst.find("[", 0);
            if (start != string::npos)
            {
                i = inst.substr(start + 1, inst.size());
            }
            // R0 + 1
            split = split_line(i);
            split[2].pop_back();
            ret += register_code(split[0]);
            ret += "05";
            int offset = location_counter - 2;
            ret += r_386_PC32_code(split[2], offset);
            return ret;
        }

        else if (regex_match(inst, jump_symbol_regex)) //jmp simbol
        {
            vector<string> split;
            split = split_line(inst);
            string ret = "5";
            ret += jump_code(split[0]);
            ret += "f000";
            int offset = location_counter - 2;
            ret += r_386_PC32_code(split[1], offset);
            return ret;
        }
        else if (regex_match(inst, jump_symbolPC_regex)) //jmp %simbol
        {
            vector<string> split;
            split = split_line(inst);
            string ret = "5";
            ret += jump_code(split[0]);
            ret += "f";
            ret += register_code("r7"); //pc
            ret += "05";
            int offset = location_counter - 2;
            split[1].erase(0, 1);
            ret += r_386_PC32_code(split[1], offset);
            return ret;
        }

        else if (regex_match(inst, jump_symbolMEM_regex)) //jmp *simbol
        {
            vector<string> split;
            split = split_line(inst);
            string ret = "5";
            ret += jump_code(split[0]);
            ret += "f004";
            split[1].erase(0, 1);
            int offset = location_counter - 2;
            ret += r_386_PC32_code(split[1], offset);
            return ret;
        }
        else if (regex_match(inst, jump_reg_regex)) // jmp *r0
        {
            vector<string> split;
            split = split_line(inst);
            string ret = "5";
            ret += jump_code(split[0]);
            ret += "f";
            split[1].erase(0, 1);
            ret += register_code(split[1]);
            ret += "01";
            return ret;
        }
        else if (regex_match(inst, jump_regIND_regex)) // jmp *[r0]
        {
            vector<string> split;
            split = split_line(inst);
            string ret = "5";
            ret += jump_code(split[0]);
            ret += "f";
            split[1].erase(0, 2);
            split[1].pop_back();
            ret += register_code(split[1]);
            ret += "04";
            return ret;
        }
        else if (regex_match(inst, jump_regIND_literal_regex)) // jgt *[r0 + 1]
        {
            vector<string> split;
            split = split_line(inst);
            string ret = "5";
            ret += jump_code(split[0]);
            ret += "f";
            string i;
            size_t start = inst.find("[", 0);
            if (start != string::npos)
            {
                i = inst.substr(start + 1, inst.size());
            }
            // R0 + 1
            split = split_line(i);
            split[2].pop_back();
            ret += register_code(split[0]);
            ret += "05";
            if (regex_match(split[2], decimal_regex))
            {
                ret += "000";
                ret += split[2];
            }
            else if (regex_match(split[2], hexa_regex))
            {
                split[2].erase(0, 2); //Ox

                int hexa = stoi(split[2], 0, 16);
                ret += decToHexa(hexa, false);
            }
            return ret;
        }
        else if (regex_match(inst, jump_literalMEM_regex)) // jmp *2
        {
            vector<string> split;
            split = split_line(inst);
            string ret = "5";
            ret += jump_code(split[0]);
            ret += "f004";
            split[1].erase(0, 1);
            if (regex_match(split[1], decimal_regex))
            {
                ret += "000";
                ret += split[1];
            }
            else if (regex_match(split[1], hexa_regex))
            {
                split[1].erase(0, 2);
                int hexa = stoi(split[1], 0, 16);
                ret += decToHexa(hexa, false);
            }
            return ret;
        }

        else if (regex_match(inst, jump_literal_regex)) // jmp 2
        {
            vector<string> split;
            split = split_line(inst);
            string ret = "5";
            ret += jump_code(split[0]);
            ret += "f000";

            if (regex_match(split[1], decimal_regex))
            {
                ret += "000";
                ret += split[1];
            }
            else if (regex_match(split[1], hexa_regex))
            {
                split[1].erase(0, 2);
                int hexa = stoi(split[1], 0, 16);
                ret += decToHexa(hexa, false);
            }
            return ret;
        }

        else if (regex_match(inst, push_regex)) // PUSH r0 / r6-sp store
        {
            vector<string> split;
            split = split_line(inst);
            string ret = "b0"; //store code
            ret += register_code(split[1]);
            ret += register_code("r6");
            ret += "1"; // smanji sp za 2
            ret += "2"; // reg ind
            return ret;
        }

        else if (regex_match(inst, pop_regex)) // POP r0 / r6-sp load
        {
            vector<string> split;
            split = split_line(inst);
            string ret = "a0"; //load code
            ret += register_code(split[1]);
            ret += register_code("r6");
            ret += "4"; // uvecaj sp za 2 nakon
            ret += "2"; // reg ind
            return ret;
        }

        else if (regex_match(inst, xchg_regex)) // XCHG REG, REG
        {
            vector<string> split;
            split = split_line(inst);
            string ret = "60";
            split[1].pop_back(); // brisi zarez

            ret += register_code(split[1]);
            ret += register_code(split[2]);
            return ret;
        }

        else if (regex_match(inst, aritmetic_regex)) // ADD R0, R1
        {
            vector<string> split;
            split = split_line(inst);
            string ret = "7";

            if (split[0] == "add")
            {
                ret += "0";
            }
            else if (split[0] == "sub")
            {
                ret += "1";
            }
            else if (split[0] == "mul")
            {
                ret += "2";
            }
            else if (split[0] == "div")
            {
                ret += "3";
            }
            else if (split[0] == "cmp")
            {
                ret += "4";
            }

            split[1].pop_back(); // brisi zarez

            ret += register_code(split[1]);
            ret += register_code(split[2]);
            return ret;
        }

        else if (regex_match(inst, logic_regex)) // AND REG, REG
        {
            vector<string> split;
            split = split_line(inst);
            string ret = "8";
            if (split[0] == "not")
            {
                ret += "0";
                ret += register_code(split[1]);
                ret += "0";
            }
            else
            {
                if (split[0] == "and")
                {
                    ret += "1";
                }
                else if (split[0] == "or")
                {
                    ret += "2";
                }
                else if (split[0] == "xor")
                {
                    ret += "3";
                }
                else if (split[0] == "test")
                {
                    ret += "4";
                }

                split[1].pop_back(); // brisi zarez

                ret += register_code(split[1]);
                ret += register_code(split[2]);
                return ret;
            }
        }

        else if (regex_match(inst, shift_regex)) // SHL REG, REG
        {
            vector<string> split;
            split = split_line(inst);
            string ret = "8";
            if (split[0] == "shl")
            {
                ret += "0";
            }
            else
                ret += "1";

            split[1].pop_back(); // brisi zarez

            ret += register_code(split[1]);
            ret += register_code(split[2]);
            return ret;
        }

        else if (regex_match(inst, ldrstr_simbol_regex)) //ldr r0, $a
        {
            vector<string> split;
            split = split_line(inst);
            string ret = ldr_or_str(split[0]);
            split[1].pop_back();
            split[2].erase(0, 1);
            ret += register_code(split[1]);
            ret += "000";
            int offset = location_counter - 2;

            ret += r_386_32_code(split[2], offset);
            return ret;
        }
        else if (regex_match(inst, ldrstr_simbolMEM_regex)) // ldr r0, sale
        {
            vector<string> split;
            split = split_line(inst);
            string ret = ldr_or_str(split[0]);
            split[1].pop_back();
            ret += register_code(split[1]);
            ret += "004";
            int offset = location_counter - 2;
            ret += r_386_32_code(split[2], offset);
            return ret;
        }
        else if (regex_match(inst, ldrstr_simbolPC_regex)) // ldr r0, %sale
        {
            vector<string> split;
            split = split_line(inst);
            string ret = ldr_or_str(split[0]);
            split[1].pop_back();
            split[2].erase(0, 1);
            ret += register_code(split[1]);
            ret += register_code("r7"); // pc
            ret += "03";
            int offset = location_counter - 2;

            ret += r_386_32_code(split[2], offset);
            return ret;
        }
        else if (regex_match(inst, ldrstr_regMEM_symbol_regex)) // ldr r0, [r1 + sale]
        {
            vector<string> split;
            split = split_line(inst);
            string ret = ldr_or_str(split[0]);
            split[1].pop_back();
            ret += register_code(split[1]);

            string i;
            size_t start = inst.find("[", 0);
            if (start != string::npos)
            {
                i = inst.substr(start + 1, inst.size());
            }

            split = split_line(i);
            ret += register_code(split[0]);
            ret += "03";
            int offset = location_counter - 2;
            split[2].pop_back();
            ret += r_386_32_code(split[2], offset);
            return ret;
        }
        else if (regex_match(inst, ldrstr_reg_regex)) // ldr r0, r0
        {
            vector<string> split;
            split = split_line(inst);
            string ret = ldr_or_str(split[0]);

            split[1].pop_back();
            ret += register_code(split[1]);
            ret += register_code(split[2]);
            ret += "01";
            return ret;
        }
        else if (regex_match(inst, ldrstr_regMEM_regex)) // ldr r0, [r0]
        {
            vector<string> split;
            split = split_line(inst);
            string ret = ldr_or_str(split[0]);
            split[1].pop_back();
            ret += register_code(split[1]);
            split[2].erase(0, 1);
            split[2].pop_back();
            ret += register_code(split[2]);
            ret += "02";
            return ret;
        }

        else if (regex_match(inst, ldrstr_regMEM_literal_regex)) // str r0, [r1 + 2]
        {
            vector<string> split;
            split = split_line(inst);
            string ret = ldr_or_str(split[0]);
            split[1].pop_back();
            ret += register_code(split[1]);
            string i;
            size_t start = inst.find("[", 0);
            if (start != string::npos)
            {
                i = inst.substr(start + 1, inst.size());
            }

            split = split_line(i);
            ret += register_code(split[0]);
            ret += "03";
            split[2].pop_back();
            if (regex_match(split[2], decimal_regex))
            {
                ret += "000";
                ret += split[2];
            }
            else if (regex_match(split[2], hexa_regex))
            {
                split[2].erase(0, 2); //Ox

                int hexa = stoi(split[2], 0, 16);
                ret += decToHexa(hexa, false);
            }

            return ret;
        }
        else if (regex_match(inst, ldrstr_literalMEM_regex)) // ldr r0, 1
        {
            vector<string> split;
            split = split_line(inst);
            string ret = ldr_or_str(split[0]);
            split[1].pop_back();
            ret += register_code(split[1]);
            ret += "005";

            if (regex_match(split[2], decimal_regex))
            {
                ret += "000";
                ret += split[2];
            }
            else if (regex_match(split[2], hexa_regex))
            {
                split[2].erase(0, 2); //Ox
                int hexa = stoi(split[2], 0, 16);
                ret += decToHexa(hexa, false);
            }
            return ret;
        }
        else if (regex_match(inst, ldrstr_literal_regex)) // ldr r0, $1
        {
            vector<string> split;
            split = split_line(inst);
            string ret = ldr_or_str(split[0]);
            split[1].pop_back();
            ret += register_code(split[1]);
            ret += "000";

            split[2].erase(0, 1);

            if (regex_match(split[2], decimal_regex))
            {
                ret += "000";
                ret += split[2];
            }
            else if (regex_match(split[2], hexa_regex))
            {
                split[2].erase(0, 2); //Ox

                int hexa = stoi(split[2], 0, 16);
                ret += decToHexa(hexa, false);
            }
            return ret;
        }
        string s = "s";
        return s;
    }

    string r_386_PC32_code(string name, int off)
    {
        string ret = "";
        if (symbol_table.label_exists(name)) // u tabeli simbola
        {
            int locality = symbol_table.get_locality(name);
            if (locality == 0)
            { //global
                int val = symbol_table.get_id_label(name);
                add_to_relocation_table_r_386_pc32(off, val);
                ret += decToHexa(-2, true);
            }
            else
            { // local
                string section = symbol_table.get_section(name);
                if (section == current_section)
                { //ista sekcija
                    int offset = symbol_table.get_offset(name);
                    int return_offset = offset - location_counter;
                    ret += decToHexa(return_offset, false);
                }
                else
                {
                    int val = symbol_table.get_id_section(section);
                    add_to_relocation_table_r_386_pc32(off, val);
                    int offset = symbol_table.get_offset(name);
                    int sub = offset - 2;
                    ret += decToHexa(sub, true);
                }
            }
        }
        else if (const_table.name_exists(name) && const_table.get_value(name) != -1) // u tabeli konstanti(ili extern ili global ili equ)
        {

            ret += decToHexa(const_table.get_value(name), false);
        }
        else if (const_table.name_exists(name)) // u tabeli konstanti(ili extern ili global)
        {
            Symbol_Table_Entry ste(name, "UND", 0);
            ste.set_locality_global();
            symbol_table.add_entry(ste);
            int val = symbol_table.get_id_label(name);
            add_to_relocation_table_r_386_pc32(off, val);
            ret += decToHexa(-2, true);
        }
        else // ni u ts ni u tconst
        {
            Symbol_Table_Entry ste(name, "UND", 0);
            ste.set_locality_global();
            symbol_table.add_entry(ste);
            int val = symbol_table.get_id_label(name);
            add_to_relocation_table_r_386_pc32(off, val);
            ret += decToHexa(-2, true);
        }

        return ret;
    }

    string r_386_32_code(string name, int off)
    {
        string ret = "";
        if (symbol_table.label_exists(name)) // u tabeli simbola
        {
            int locality = symbol_table.get_locality(name);
            if (locality == 0)
            { //global
                ret += "0000";
                add_to_relocation_table_r_386_32(off, symbol_table.get_id_label(name));
            }
            else
            { // local
                string sec = symbol_table.get_section(name);
                int val = symbol_table.get_id_section(sec);
                add_to_relocation_table_r_386_32(off, val);
                //decimal
                ret += decToHexa(symbol_table.get_offset(name), true);
            }
        }
        else if (const_table.name_exists(name) && const_table.get_value(name) != -1) // u tabeli konstanti(ili extern ili global ili equ)
        {

            ret += decToHexa(const_table.get_value(name), false);
        }
        else if (const_table.name_exists(name)) // u tabeli konstanti(ili extern ili global ili equ)
        {
            Symbol_Table_Entry ste(name, "UND", 0);
            ste.set_locality_global();
            symbol_table.add_entry(ste);
            ret += "0000";
            add_to_relocation_table_r_386_32(off, symbol_table.get_id_label(name));
        }
        else // ni u ts ni u tconst
        {
            Symbol_Table_Entry ste(name, "UND", 0);
            ste.set_locality_global();
            symbol_table.add_entry(ste);
            ret += "0000";
            add_to_relocation_table_r_386_32(off, symbol_table.get_id_label(name));
        }

        return ret;
    }
    void add_to_relocation_table_r_386_32(int off, int val)
    {
        Relocation_Table_Entry ret(current_section, off, val);
        ret.set_type_R_386_32();
        relocation_table.add_entry(ret);
    }
    void add_to_relocation_table_r_386_pc32(int off, int val)
    {
        Relocation_Table_Entry ret(current_section, off, val);
        ret.set_type_R_386_PC32();
        relocation_table.add_entry(ret);
    }

    string ldr_or_str(string i)
    {
        if (i == "ldr")
        {
            return "a0";
        }
        else
            return "b0";
    }

    string jump_code(string j)
    {
        if (j == "jmp")
        {
            return "0";
        }
        else if (j == "jeq")
        {
            return "1";
        }
        else if (j == "jne")
        {
            return "2";
        }
        else
        {
            return "3";
        }
    }

    string decToHexa(int num, bool obrni)
    {

        map<int, char> m;

        char digit = '0';
        char c = 'a';

        for (int i = 0; i <= 15; i++)
        {
            if (i < 10)
            {
                m[i] = digit++;
            }
            else
            {
                m[i] = c++;
            }
        }

        // string to be returned
        string res = "";

        // check if num is 0 and directly return "0"
        if (!num)
        {
            return "0000";
        }
        // if num>0, use normal technique as
        // discussed in other post
        if (num > 0)
        {
            while (num)
            {
                res = m[num % 16] + res;
                num /= 16;
            }
        }
        // if num<0, we need to use the elaborated
        // trick above, lets see this
        else
        {
            // store num in a u_int, size of u_it is greater,
            // it will be positive since msb is 0
            uint32_t n = num;

            // use the same remainder technique.
            while (n)
            {
                res = m[n % 16] + res;
                n /= 16;
            }
            res.erase(0, 4);
        }
        if (res.size() - 1 != 4)
        {
            int k = 4 - (res.size());
            while (k != 0)
            {
                res.insert(0, "0");
                k--;
            }
        }
        if (obrni)
        {
            string rett;
            rett += res[2];
            rett += res[3];
            rett += res[0];
            rett += res[1];

            return rett;
        }
        else
            return res;
    }
};

#endif