#include "Fatal.hpp"

void fatal(QString message)
{
   QMessageBox::critical(NULL,"Error",message);
   exit(1);
}