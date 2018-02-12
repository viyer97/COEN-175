#include "../includes/Scope.h"
#include <iostream>

Scope::Scope(Scope *enclosing)
{
    _enclosing = enclosing;
}

void Scope::insert(Symbol *symbol)
{
    /* add symbol to the map with the id as the name*/
    _symbols[symbol->getID()] = symbol;
}

Symbol * Scope::find(const std::string &name) const
{
    std::unordered_map<std::string, Symbol*>::const_iterator got = _symbols.find(name);
    if (got == _symbols.end()){
        //std::cout << "not found";
        return NULL;
    }

    return got->second;
}

Symbol * Scope::lookup(const std::string &name) const
{
    Scope *curr;
    Symbol *sym;
    /* while there are still scope to go through 
     * curr != NULL
     */
    curr = _enclosing;
    sym = curr->find(name);
    if (sym){
        if (sym->getID() == name){
            return sym;
        }
    }
    
    return NULL;
}

Scope * Scope::enclosing() const
{
    return _enclosing;
}

const Symbols & Scope::symbols() const
{
    return _symbols;
}

std::ostream& operator<<(std::ostream &ostr, const Scope &scope)
{
    std::unordered_map<std::string, Symbol*>::const_iterator itr;
    for (itr = scope.symbols().begin(); itr != scope.symbols().end(); itr++){
        ostr << *itr->second << std::endl;
    }
    return ostr;
}

/*
main()
{
    Type *t1 = new Type(11, 1);
    Type *t2 = new Type(12, 2);
    Type *t3 = new Type(13, 3);
    std::string str1("the name");
    std::string str2("the name2");
    std::string str3("the name3");
    Symbol *s1 = new Symbol(str1, t1);
    Symbol *s2 = new Symbol(str2, t2);
    Symbol *s3 = new Symbol(str3, t3);

    Scope *sc1 = new Scope;
    sc1->insert(s1);
    Scope *sc2 = new Scope(sc1);
    sc2->insert(s2);
    Scope *sc3 = new Scope(sc2);
    sc2->insert(s3);
    std::cout << *sc3->lookup("the name2") << std::endl;
}*/
