#ifndef SYMBOL_H
#define SYMBOL_H

#include "Type.h"
#include <string>
class Symbol {
    std::string _id;
    Type *_type;
    bool _def;
    
    public:
        Symbol();
        Symbol(std::string id, Type *type, bool def);

        bool operator==(const Symbol &rhs) const;
        bool operator!=(const Symbol &rhs) const;
        std::string getID() const;
        Type * getType() const;
        bool getDef() const;
};

std::ostream& operator<<(std::ostream &ostr, const Symbol &sym);
#endif
