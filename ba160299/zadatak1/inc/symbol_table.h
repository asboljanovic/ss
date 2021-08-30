#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include <iostream>
#include <list>
#include <fstream>
#include <iomanip> //setw
#include "symbol_table_entry.h"
using namespace std;

class Symbol_Table
{
private:
    list<Symbol_Table_Entry> symbol_entries;
    

public:
    Symbol_Table()
    {
    }
    ~Symbol_Table()
    {
        symbol_entries.clear();
    }
    list<Symbol_Table_Entry> get_symbol_entries()
    {
        return symbol_entries;
    }
    void add_entry(Symbol_Table_Entry ste)
    {
        symbol_entries.push_back(ste);
    }
    Symbol_Table_Entry get_by_label(string label)
    {
        Symbol_Table_Entry ste;
        for (Symbol_Table_Entry &st : symbol_entries)
        {
            if (st.get_label() == label)
            {
                ste = st;
            }
        }
        return ste;
    }
    Symbol_Table_Entry get_by_id(int id)
    {
        Symbol_Table_Entry ste;
        for (Symbol_Table_Entry &st : symbol_entries)
        {
            if (st.get_id() == id)
            {
                ste = st;
            }
        }
        return ste;
    }
     void set_global(string label){
         if (label_exists(label))
        {
            for (Symbol_Table_Entry &st : symbol_entries)
            {
                if (st.get_label() == label)
                {
                    st.set_locality_global();
                    
                   
                }
            }
        }

     
    }

    void update_symbol_entry(string label, string sect, int off)
    {
        if (label_exists(label))
        {
            for (Symbol_Table_Entry &st : symbol_entries)
            {
                if (st.get_label() == label)
                {
                    st.set_label(label);
                    st.set_section(sect);
                    st.set_offset(off);
                    
                   
                }
            }
        }
    }
    bool label_exists(string label)
    {
        for (Symbol_Table_Entry &st : symbol_entries)
        {
            if (st.get_label() == label)
            {
                return true;
            }
        }
        return false;
    }

   int get_locality(string label)
    {
        for (Symbol_Table_Entry &st : symbol_entries)
        {
            if (st.get_label() == label)
            {
                return st.get_locality();
            }
        }
        return 0;
    }
    int get_id_section(string sec)
    {
        for (Symbol_Table_Entry &st : symbol_entries)
        {
            if (st.get_section() == sec)
            {
                return st.get_id();
            }
        }
        return 0;
    }
     int get_id_label(string l)
    {
        for (Symbol_Table_Entry &st : symbol_entries)
        {
            if (st.get_label() == l)
            {
                return st.get_id();
            }
        }
        return 0;
    }
     int get_offset(string label)
    {
        for (Symbol_Table_Entry &st : symbol_entries)
        {
            if (st.get_label() == label)
            {
                return st.get_offset();
            }
        }
        return 0;
    }
    string get_section(string label)
    {
        for (Symbol_Table_Entry &st : symbol_entries)
        {
            if (st.get_label() == label)
            {
                return st.get_section();
            }
        }
        return "";
    }
    
    bool section_exists(string section)
    {
        for (Symbol_Table_Entry &st : symbol_entries)
        {
            if (st.get_section() == section)
            {
                return true;
            }
        }
        return false;
    }
    friend ostream &operator<<(ostream &it, const Symbol_Table &st)
    {
        it << "#tabela simbola" << endl;
        it <<" " << setw(10) << "#ime" << " "
           <<setw(10) << "sekcija"<< " "
           << setw(8) <<"vrednost"<< " "
           << setw(10) <<"vidljivost"<< " "
           << setw(7) <<"redni broj" << endl;
    
        for (auto const& i : st.symbol_entries) {
        it << i;
}
        return it;
    }
};

#endif