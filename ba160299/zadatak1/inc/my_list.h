#ifndef MY_LIST_H
#define MY_LIST_H
#include <string>
#include <iostream>
#include <vector>
using namespace std;


string get(vector<string> _list, int _i){
    vector<string>::iterator it = _list.begin();
    for(int i=0; i<_i; i++){
        ++it;
    }
    return *it;
}

bool is_string_in_vector(vector<string> _list, string s){
  
   for(int i = 0; i < _list.size(); i++) {
       if(s == get(_list, i)) {
           return true;
       }
    }
    return false;
}

void delete_element_from_vector(vector<string> _list, string s) {
    if(is_string_in_vector(_list, s)) {
        for(int i = 0; i < _list.size(); i++) {
            if(_list[i] == s) {
                _list.erase(_list.begin()+ i);
            }
        }
    }
}


#endif