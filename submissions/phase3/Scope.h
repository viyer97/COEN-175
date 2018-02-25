#ifndef SCOPE_H
#define SCOPE_H

#include "Symbol.h"
#include <unordered_map> /* I don't knpwo if c++ 11 is supported */

typedef std::unordered_map<std::string, Symbol*> Symbols;
//typedef std::vector<Symbol*> Symbols;

class Scope {
    Scope *_enclosing;  //"next" pointer in the linked list of scopes
    Symbols _symbols;   //THIS MAY NEED TO BE A PTR

    public:
        Scope(Scope *enclosing=0);    //MAY NEED TO BE null

        void insert(Symbol *symbol);
        Symbol * find(const std::string &name) const;
        Symbol * lookup(const std::string &name) const;
        
        Scope * enclosing() const;
        const Symbols &symbols() const;
};

std::ostream& operator<<(std::ostream &ostr, const Scope &scope);

#endif
