#include <../includes/Type.h>

void Type::Type()
{

}


void Type::Type(int specifier, unsigned indirection)
{
    /* change to using setters */
    _specifier = specifier;
    _indirection = indrection;
    _kind = SCALAR;

}

void Type::Type(int specifier, unsigned indirection, unsigned length)
{
    _specifier = specifier;
    _indirection = indrection;
    _length = length;
    _kind = ARRAY;
    
}

void Type::Type(int specifier, unsigned indirection, Parameters *params)
{
    _specifier = specifier;
    _indirection = indrection;
    _params = SOMETHING;    //copy constructor for vector calss
    _kind = FUNCTION;

}
