#include "symtable.hpp"
#include <iostream>

SymEntry::SymEntry(std::string name){this->name = name;}
SymEntry::~SymEntry(){}


// different entry types
StrEntry::StrEntry(std::string name, std::string lit)
    :SymEntry(name)
{
    literal = lit;
}
void StrEntry::print(){
    std::cout<<"name "<<name<<" type STRING"<<" value "<<literal<<std::endl;
}

IntEntry::IntEntry(std::string name):SymEntry(name){}
void IntEntry::print(){
    std::cout<<"name "<<name<<" type INT"<<std::endl;
}

FltEntry::FltEntry(std::string name):SymEntry(name){}
void FltEntry::print(){
    std::cout<<"name "<<name<<" type FLOAT"<<std::endl;
}


// the symtable table
Symtable::Symtable(std::string name){this->name = name;}
Symtable::~Symtable(){
    for(auto it:entrylist){
        delete it;
    }
}

void Symtable::add(SymEntry* entry){
    entrylist.push_back(entry);
    if(id_set.find(entry->name)==id_set.end()){
        id_set.insert(entry->name);
    }
    else{
        std::cout<<"DECLARATION ERROR "<<entry->name<<std::endl;
        exit(1);
    }
}

void Symtable::print(){
    std::cout << "Symbol table " << name << std::endl;
    for(int i = 0;i < entrylist.size();i++)
        entrylist[i]->print();
}