#ifndef SKELETON_HPP
#define SKELETON_HPP

#include "CFunctions.h"

#include <iostream>

char * toCStr(std::string str);

class Skeleton{
   public:
      Skeleton();
      ~Skeleton();
   private:
      void createDisplayLists();
};

#endif

/*
TO-DO:
- add code to compile display lists for each skeleton component
- add code to skeleton class to call display functions appropriately to draw skeleton
- add better controls with qt
*/
