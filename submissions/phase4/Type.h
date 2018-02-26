#ifndef TYPE_H
#define TYPE_H
#include <ostream>
#include <vector>
//#include <memory>

typedef std::vector<class Type> Parameters;

class Type{
    int _specifier;
    unsigned _indirection;
    enum {ARRAY, SCALAR, FUNCTION, ELLIPSIS, ERROR} _kind;
    unsigned _length;
    Parameters *_params;
    //std::unique_ptr<Parameters> _params;

    public:
        Type();
        Type(int specifier);    // for handling ellipsis
        Type(int specifier, unsigned indirection);
        Type(int specifier, unsigned indirection, unsigned length);
        Type(int specifier, unsigned indirection, 
                Parameters *params);

        bool operator==(const Type &rhs) const;
        bool operator!=(const Type &rhs) const;
        int specifier() const;
        unsigned indirection() const;
        unsigned length() const;
        Parameters *parameters() const;
        bool isError() const;   //do I even need this?
        bool isArray() const;
        bool isFunction() const;
        bool isScalar() const;
        bool isEllipsis() const;

        /* added in phae 4 */
        bool isInteger() const;
        bool isPointer() const;
        bool isValue() const;
        bool isCompatible(const Type &right) const;   //might have to change this
        Type promote() const;
}; 

std::ostream& operator<<(std::ostream &ostr, const Type &type);

#endif 
