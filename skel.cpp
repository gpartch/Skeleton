#include <QApplication>

#include "Skeleton.hpp"
#include "Viewer.hpp"

int main(int argc,char* argv[])
{
   //  Create the application
   QApplication app(argc,argv);
   //  Create and show Viewer widget
   Viewer viewer;
   viewer.show();
   // Create skeleton
   Skeleton skel; // Skeleton object
   //  Main loop for application
   return app.exec();
   return 0;
}