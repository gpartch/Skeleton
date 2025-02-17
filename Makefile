# Example 30
EXE=skel

# Main target
all: $(EXE)

#  Msys/MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall -DUSEGLEW -ggdb
LIBS=-lfreeglut -lglew32 -lglu32 -lopengl32 -lm
CLEAN=rm -f *.exe *.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
RES=$(shell uname -r|sed -E 's/(.).*/\1/'|tr 12 21)
CFLG=-O3 -Wall -Wno-deprecated-declarations -DRES=$(RES) -ggdb
LIBS=-framework GLUT -framework OpenGL
#  Linux/Unix/Solaris
else
CFLG=-g -Wall -ggdb
LIBS=-lglut -lGLU -lGL -lm
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f $(EXE) *.o *.a
endif

# Dependencies
fatal.o: fatal.cpp Skeleton.hpp
errcheck.o: errcheck.cpp Skeleton.hpp
print.o: print.cpp Skeleton.hpp
loadobj.o: loadobj.cpp Skeleton.hpp
projection.o: projection.cpp Skeleton.hpp
printVBO.o: printVBO.cpp Skeleton.hpp
Skeleton.o: Skeleton.cpp Skeleton.hpp
skel.o: skel.cpp Skeleton.hpp
cstr.o: cstr.cpp Skeleton.hpp

#  Create archive
Skeleton.a:fatal.o errcheck.o print.o read.o projection.o loadply.o loadmodel.o printVBO.o cstr.o Skeleton.o
	ar -rcs $@ $^

# Compile rules
.cpp.o:
	g++ -c $(CFLG)  $<

#  Link
skel:skel.o Skeleton.a
	g++ $(CFLG) -o $@ $^  $(LIBS)

#  Clean
clean:
	$(CLEAN)
