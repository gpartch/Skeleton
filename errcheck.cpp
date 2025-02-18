//  CSCIx239 library
//  Willem A. (Vlakkies) Schreuder
#include "Skeleton.hpp"

//
//  Check for OpenGL errors and print to stderr
//
void Skeleton::ErrCheck(const char* where)
{
   int err = glGetError();
   if (err) qDebug() << "ERROR: " << qt_error_string(err) << " [" << where << "]\n";
}

void Skeleton::ErrCheck(string where)
{
   int err = glGetError();
   if (err) qDebug() << "ERROR: " << qt_error_string(err) << " [" << toCStr(where) << "]\n";
}
