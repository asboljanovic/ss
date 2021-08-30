#ifndef RELOCATION_TABLE_H
#define RELOCATION_TABLE_H

#include <iostream>
#include <list>
#include <fstream>
#include <vector>
#include <iomanip> //setw
#include "relocation_table_entry.h"
using namespace std;

class Relocation_Table
{
private:
    list<Relocation_Table_Entry> relocation_entries;

public:
    Relocation_Table()
    {
    }
    ~Relocation_Table()
    {
        relocation_entries.clear();
    }
    list<Relocation_Table_Entry> get_relocation_entries()
    {
        return relocation_entries;
    }
    void add_entry(Relocation_Table_Entry ste)
    {
        relocation_entries.push_back(ste);
    }

    Relocation_Table_Entry get_by_id(int id)
    {
        Relocation_Table_Entry ste;
        for (Relocation_Table_Entry &st : relocation_entries)
        {
            if (st.get_id() == id)
            {
                ste = st;
            }
        }
        return ste;
    }
   

    bool is_it_added(vector<string>sections, string s) {
        for(int  i =0 ; i < sections.size(); i++) {
            if(sections[i] == s) return true;
        } 
        
        return false;
    }

    friend ostream &operator<<(ostream &it, const Relocation_Table &rt)
    {
      

        it << "#relokaciona tabela" << endl;

        it<< " " << setw(10) << "sekcija"
           << " " 
           << " " << setw(10) << "#offset"
           << " "
           << setw(10) << "tip"
           << " "
           << setw(18) << "vrednost" << endl;

        for (auto const &i : rt.relocation_entries)
        {
            it << i;
        }
        return it;
    }
};

#endif