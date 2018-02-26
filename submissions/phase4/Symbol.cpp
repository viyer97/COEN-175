#include "Symbol.h"
#include <iostream>

Symbol::Symbol()
{

}

Symbol::Symbol(std::string id, Type *type, bool def)
{
    _id = id;
    _type = type;
    _def = def;
}

bool Symbol::operator==(const Symbol &rhs) const
{
    if (_id != rhs._id){
        return false;
    }
    return *_type == *rhs._type;
}

bool Symbol::operator!=(const Symbol &rhs) const
{
    return !operator==(rhs);
}

std::string Symbol::getID() const
{
    return _id;
}

Type * Symbol::getType() const
{
    return _type;
}

bool Symbol::getDef() const
{
    return _def;
}

std::ostream& operator<<(std::ostream &ostr, const Symbol &sym)
{
    ostr << *sym.getType() << std::endl;
    ostr << sym.getID() << std::endl;
    
    return ostr;
}

/*
main()
{
    Type *t1 = new Type(10, 1);
    Type *t2 = new Type(11, 1);
    std::string str1("the name");
    std::string str2("the name");
    Symbol *s1 = new Symbol(str1, t1);
    Symbol *s2 = new Symbol(str2, t2);
    if (*s1 != *s2)
        std::cout << "eql works" << std::endl;
    std::cout << *s2 << std::endl;
}*/
