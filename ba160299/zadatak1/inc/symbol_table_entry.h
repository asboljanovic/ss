#ifndef _SYMBOL_TABLE_ENTRY_H
#define _SYMBOL_TABLE_ENTRY_H

#include <iostream>
#include <string>
#include <iomanip> //setw
using namespace std;

static int id_ = 0;

class Symbol_Table_Entry
{

private:
    string label;
    string section;
    int offset;
    int local; // local = 1 , global = 0
    int id;

public:
    Symbol_Table_Entry(string lb, string sec, int offs) {
         label = lb;
    section = sec;
    offset = offs;
   
    id = id_++;
    }
    Symbol_Table_Entry() {

    }

      friend ostream &operator<<(ostream &it, const Symbol_Table_Entry &st) {
        
         it << " " << setw(10) << st.label;
           it << " " << setw(10) << st.section;
           it << " " << setw(8) <<st.offset ;
           if(st.local == 1) {    
                it << " " << setw(10) <<"Lokalna";
           }else  it << " " << setw(10) <<"Globalna";
          it  <<" " << setw(7) <<st.id << endl;
          return it;
      }

    string get_label() {
         return label;
    }
    void set_label(string l) {
        label = l;
    }
    string get_section() {
          return section;
    }
    void set_section(string s) {
          section = s;
    }
    int get_offset() {
         return offset;
    }
    void set_offset(int off) {
         offset = off;
    }
    int get_locality() {
         return local; // 1- local , 0-global
    }
    void set_locality_local() {
           local = 1;
    }
     void set_locality_global() {
          local = 0;
     }
    int get_id() {
        return id;
    }
};

#endif