#ifndef CONST_TABLE_ENTRY_H
#define CONST_TABLE_ENTRY_H
#include <iostream>
#include <string>
using namespace std;

static int _id = 0;

class Const_Table_Entry
{

private:
    string name;
    int value;
    int id;

public:
    Const_Table_Entry(string n, int val)
    {
        name = n;
        value = val;
        id = _id++;
    }
    Const_Table_Entry()
    {
    }

    void set_name(string n) {
        name = n;
    }
    void set_val(int v) {
        value = v;
    }
    

    string get_name()
    {
        return name;
    }
    int get_val()
    {
        return value;
    }
    int get_id()
    {
        return id;
    }
};

#endif