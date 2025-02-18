#include "Skeleton.hpp"

// convert c++ str to c str char*
char * Skeleton::toCStr(std::string str)
{
    char * cstr = str.data();
    return cstr;
}