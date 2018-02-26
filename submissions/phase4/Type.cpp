#include "Type.h"
#include "tokens.h"
#include <iostream>

Type::Type()
{
    _kind = ERROR;
}

Type::Type(int specifier)
{
    _kind = ELLIPSIS;
}

Type::Type(int specifier, unsigned indirection)
{
    _specifier = specifier;
    _indirection = indirection;
    _kind = SCALAR;

}

Type::Type(int specifier, unsigned indirection, unsigned length)
{
    _specifier = specifier;
    _indirection = indirection;
    _length = length;
    _kind = ARRAY;
}

Type::Type(int specifier, unsigned indirection, Parameters *params)
{
    _specifier = specifier;
    _indirection = indirection;
    _params = params;   
    _kind = FUNCTION;
}

bool Type::operator==(const Type &rhs) const
{
    if (_kind != rhs._kind){
        return false;
    } 
    
    if (_kind == ERROR){
        return true;
    }

    if (_specifier != rhs._specifier){
        return false;
    }

    if (_indirection != rhs._indirection){
        return false;
    }

    if (_kind == ARRAY){
        if (_length != rhs._length){
            return false;
        }
    }

    if (_kind == FUNCTION){
        return *_params == *rhs._params;
    }

    return true;
}

bool Type::operator!=(const Type &rhs) const
{
    return !operator==(rhs);
}

int Type::specifier() const
{
    return _specifier;
}

unsigned Type::indirection() const
{
    return _indirection;
}

unsigned Type::length() const
{
    return _length;
}

Parameters * Type::parameters() const
{
    return _params;
}

bool Type::isError() const
{
    if (_kind == ERROR){
        return true;
    }
    return false;
} 

bool Type::isArray() const
{
    if (_kind == ARRAY){
        return true;
    }
    return false;
}

bool Type::isFunction() const
{
    if (_kind == FUNCTION){
        return true;
    }
    return false;
}

bool Type::isScalar() const
{
    if (_kind == SCALAR){
        return true;
    }
    return false;
}

bool Type::isEllipsis() const
{
    if (_kind == ELLIPSIS) {
        return true;
    }
    return false;
}

Type Type::promote() const
{
    /* if is a char, promote to int */
    if (_specifier == CHAR && !_indirection && _kind == SCALAR) {
        return Type(INT, 0);
    }

    /* if it is an array, promote to pointer */
    if (_kind == ARRAY) {
        return Type(_specifier, _indirection + 1);
    }
    
    return *this;
}

bool Type::isPointer() const
{
    if (_kind == SCALAR && _indirection > 0) {
        return true;
    }
    return false;
}

bool Type::isInteger() const
{
    if (*this == Type(272, 0)) {
        return true;
    }
    return false;
}

bool Type::isCompatible(const Type &right) const
{
    Type new_left = promote();
    Type new_right = right.promote();

    /* 
     * values are compatible if after any promotion the yare identical
     * or one is pointer to T and the other is a pointer to void
     */
    if (new_left == new_right) {
        return true;
    } else if ((new_left.indirection() == 1) && (new_left.specifier() == VOID)
                && new_right.isPointer()) {
            /* case where left is pointer to void */
        return true;
    } else if (new_left.isPointer() && (new_right.indirection() == 1)
                && (new_right.specifier() == VOID)) {
            /* case where right is pointer to void */
        return true;
    }
    
    return false;
}

/*I DON'T KNOW IF THIS IS CORRECT */
bool Type::isValue() const
{
    Type t = promote(); //might need to use "this" ptr

    if (t.isPointer()) {   //check if t is a pointer
        return true;
    }
    if (t.isInteger()) {    //check if t is an integer
        return true;
    }
    return false;
}

std::ostream& operator<<(std::ostream &ostr, const Type &type)
{
    if (type.isError()){
        ostr << "Something fucked up in Type" << std::endl;
        return ostr;
    }
    ostr << "Specifier: " << type.specifier() << std::endl;
    ostr << "Indirection: " << type.indirection() << std::endl;
    if (type.isArray()){
    ostr << "Length: " << type.length() << std::endl;
    } else if (type.isFunction()){
        ostr << "Parameters: ";
        for (int i = 0; i < type.parameters()->size(); i++){
            ostr << (*type.parameters())[i] << ",";
        }
    }
    return ostr;
}

/*
int main()
{
    Type *t = new Type();
    Type *t2 = new Type(10, 3);
    Type *t3 = new Type(10, 3, 5);
    std::vector<Type> v = {*t2, *t3};
    Type *t4 = new Type(2, 5, &v);
    Type *t5 = new Type(3, 5, &v);
    if (*t4 != *t5)
        std::cout << "not equal works" << std::endl;
    std::cout << *t4 << std::endl;

    return 0;
}*/
