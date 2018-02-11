#ifndef SYMBOL_H
#define SYMBOL_H

#include "Type.h"
#include <string>
class Symbol {
    std::string _id;
    Type *_type;
    
    public:
        Symbol();
        Symbol(std::string id, Type *type);
        

    
}

#endif
