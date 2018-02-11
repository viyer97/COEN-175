#include "../includes/Type.h"
/*
Type::Type()
{

}*/

Type::Type(int specifier, unsigned indirection)
{
    /* change to using setters */
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
    _params = params;   //make sure to call std::move when assigning    
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

    if (_length != rhs._kind){
        return false;
    }

    if (_specifier != rhs._specifier){
        return false;
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

int main()
{
    Type t = new Type();
    Type t = new Type();

    return 0;
}
