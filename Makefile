# Example 30
EXE=sv

# Main target
all: $(EXE)

#  Msys/MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall -DUSEGLEW
LIBS=-lfreeglut -lglew32 -lglu32 -lopengl32 -lm
CLEAN=rm -f *.exe *.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
RES=$(shell uname -r|sed -E 's/(.).*/\1/'|tr 12 21)
CFLG=-O3 -Wall -Wno-deprecated-declarations -DRES=$(RES)
LIBS=-framework GLUT -framework OpenGL
#  Linux/Unix/Solaris
else
CFLG=-g -Wall
LIBS=-lglut -lGLU -lGL -lm
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f $(EXE) *.o *.a
endif

# Dependencies
sv.o: sv.c Skeleton.h
fatal.o: fatal.c Skeleton.h
errcheck.o: errcheck.c Skeleton.h
print.o: print.c Skeleton.h
loadtexbmp.o: loadtexbmp.c Skeleton.h
loadobj.o: loadobj.c Skeleton.h
projection.o: projection.c Skeleton.h

#  Create archive
Skeleton.a:fatal.o errcheck.o print.o loadtexbmp.o read.o projection.o loadply.o loadmodel.o
	ar -rcs $@ $^

# Compile rules
.c.o:
	gcc -c $(CFLG)  $<
.cpp.o:
	g++ -c $(CFLG)  $<

#  Link
sv:sv.o Skeleton.a
	gcc $(CFLG) -o $@ $^  $(LIBS)

#  Clean
clean:
	$(CLEAN)
