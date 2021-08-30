#ifndef RELOCATION_TABLE_ENTRY_H
#define RELOCATION_TABLE_ENTRY_H

#include <iostream>
#include <string>
#include <iomanip> //setw
using namespace std;

static int id_r = 0;

class Relocation_Table_Entry
{

private:
    string section;
    int offset;
    int type; // R_386_32 = 1 , R_386_PC32 = 0
    int value;
    int id;

public:
    Relocation_Table_Entry(string sec, int offs, int v)
    {

        section = sec;
        offset = offs;
        value = v;

        id = id_r++;
    }
    Relocation_Table_Entry()
    {
    }

    friend ostream &operator<<(ostream &it, const Relocation_Table_Entry &st)
    {
        it << " " << setw(10) << st.section;
        it << " " << setw(10) << st.offset;
        if (st.type == 1)
        {
            it << " " << setw(15) << "R_386_32";
        }
        else
            it << " " << setw(15) << "R_386_PC32";
        it << " " << setw(10) << st.value << endl;

        return it;
    }

    string get_section()
    {
        return section;
    }
    void set_section(string s)
    {
        section = s;
    }
    int get_offset()
    {
        return offset;
    }
    void set_offset(int off)
    {
        offset = off;
    }
    int get_type()
    {
        return type; // R_386_32 = 1 , R_386_PC32 = 0
    }
    void set_type_R_386_32()
    {
        type = 1;
    }
    void set_type_R_386_PC32()
    {
        type = 0;
    }
    int get_id()
    {
        return id;
    }
    int get_value()
    {
        return value;
    }
    void set_value(int v)
    {
        value = v;
    }
};

#endif