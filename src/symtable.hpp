#ifndef SYMTABLE_HPP
#define SYMTABLE_HPP
#include <vector>
#include <string>
#include <iostream>

class SymEntry{
protected:
    std::string name;
public:
    SymEntry(std::string name);
    virtual ~SymEntry();
    virtual void print()=0;
    //{std::cout << "This should not happen!" << std::endl;};
};

class StrEntry: public SymEntry{
    std::string literal;
public:
    StrEntry(std::string name, std::string lit);
    virtual ~StrEntry(){};
    void print();
};

class IntEntry: public SymEntry{
public:
    IntEntry(std::string name);
    virtual ~IntEntry(){};
    void print();
};

class FltEntry: public SymEntry{
public:
    FltEntry(std::string name);
    virtual ~FltEntry(){};
    void print();
};


class Symtable{

    std::vector<SymEntry*> entrylist;
    std::string name;
public:
    Symtable(std::string name);
    virtual ~Symtable();
    virtual void add(SymEntry* SymEntry); 
    virtual void print();
};

#endif
