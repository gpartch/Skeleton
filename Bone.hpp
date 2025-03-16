#ifndef BONE_HPP
#define BONE_HPP

#include <QOpenGLWidget>
#include <QString>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QFile>
#include <QTextStream>
#include <QDataStream>

#include <memory>
#include <iostream>
#include <vector>

#include "Fatal.hpp"
#include "Vec3.hpp"

using std::cout;
using std::string;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;

struct angles{ int th,ph; }; // angles/degrees of freedom for bone
struct offset { double x,y,z; }; // offset to the position of a bone
enum bone_dir
{
   up = 1, // adj bone is upstream
   down = -1, // adj bone is downstream
   neither = 0 // adj bone is neither upstream nor downstream
};
class Bone;
struct adj_bone
{
   shared_ptr<Bone> adj_bone;
   bone_dir dir;
};
struct rgb {unsigned char r,g,b;};
typedef struct {vec3 v,n;rgb c;unsigned char a;} vert;
struct elem {int a,b,c;};
struct pixel {unsigned char r,g,b,a; vec3 p;};

enum ang_dir
{
    t = 0, // theta
    p = 1 // pheta
};

class Bone : protected QOpenGLFunctions
{
    public:
        Bone(QOpenGLWidget* widget, QString file_name, QString name, angles ang, offset off, int inv_norm, int idx);
        void printBone();
        void drawBone(int mode);
        void loadBone();
        void initBone();
        void initAdj(vector<adj_bone> a);
        void rotateBone(ang_dir a, int x, int y, int z);
        void offsetBone();
        void incrementBoneAng(int th, int ph);
        int getBoneIdx();
        QString getBoneName();
        void setFlag(bool f);
        bool getVisited();
        void setBoneAng(int th, int ph);

    private:
        QString adr; // address of the bone object file
        QString name; // name of the bone
        angles ang; // rotation angles th,ph
        offset off; // xyz offset
        int idx; // index/ model number for bone
        vector<adj_bone> adj; // adjacent bones
        int inv_norm;
        bool visited;

        int num_vert; // number of vertices
        int num_tri; // number of triangles
        vert* vertices; 
        elem* elements; 
        unsigned int vbo,ebo; // Vertex and Element buffers

        QOpenGLWidget* main_widget = nullptr; // Pointer to main qt widget
};

#endif