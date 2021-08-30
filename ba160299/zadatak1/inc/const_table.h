#ifndef CONST_TABLE_H
#define CONST_TABLE_H
#include <iostream>
#include <list>
#include "const_table_entry.h"
using namespace std;

class Const_Table
{
private:
    list<Const_Table_Entry> const_entries;

public:
    Const_Table()
    {
    }
    ~Const_Table()
    {
        const_entries.clear();
    }
    list<Const_Table_Entry> get_const_entries()
    {
        return const_entries;
    }
    void add_entry(Const_Table_Entry cte)
    {
        const_entries.push_back(cte);
    }
    Const_Table_Entry get_by_name(string n)
    {
        Const_Table_Entry ste;
        for (Const_Table_Entry &st : const_entries)
        {
            if (st.get_name() == n)
            {
                ste = st;
            }
        }
        return ste;
    }
    Const_Table_Entry get_by_id(int id)
    {
        Const_Table_Entry ste;
        for (Const_Table_Entry &st : const_entries)
        {
            if (st.get_id() == id)
            {
                ste = st;
            }
        }
        return ste;
    }
       int get_value(string name)
    {
        for (Const_Table_Entry &st : const_entries)
        {
            if (st.get_name() == name)
            {
                return st.get_val();
            }
        }
        return 0;
    }


    void update_const_entry(string name, int val)
    {
        if (name_exists(name))
        {
            for (Const_Table_Entry &st : const_entries)
            {
                if (st.get_name() == name)
                {
                    st.set_name(name);

                    st.set_val(val);
                }
            }
        }
    }
    bool name_exists(string n)
    {
        for (Const_Table_Entry &st : const_entries)
        {
            if (st.get_name() == n)
            {
                return true;
            }
        }
        return false;
    }
  
};

#endif