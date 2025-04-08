#include "Bone.hpp"

Bone::Bone(QOpenGLWidget* widget, QString file_name, QString new_name, angles new_ang, offset new_off, int new_inv_norm, int new_idx) :  main_widget(widget)
{
    main_widget = widget;

    adr = file_name;
    name = new_name;
    ang = new_ang;
    off = new_off;
    idx = new_idx;
    inv_norm = new_inv_norm;
    adj = {};
    visited = false;

    num_vert = 0;
    num_tri = 0;
    vbo = 0;
    ebo = 0;
}

//-------------------------------------------------------------------

void Bone::printBone()
{
    qDebug() << "-----------------------------------";
    qDebug() << "address: " << adr;
    qDebug() << "name: " << name;
    qDebug() << "angles: (" << ang.th << "," << ang.ph << ")";
    qDebug() << "offset: " << off.x << "," << off.y << "," << off.z;
    qDebug() << "idx: " << idx;
    qDebug() << "adjacent bones: ";
    int len = adj.size();
    for(int i = 0; i < len; i++)
    {
        QString adj_name = adj.at(i).adj_bone->name;
        qDebug() << "   [" << adj_name << "]";
    }
    qDebug() << "num vertices: " << num_vert;
    qDebug() << "num triangles: " << num_tri;
    qDebug() << "vbo: " << vbo;
    qDebug() << "ebo: " << ebo;
}
void Bone::drawBone(int mode)
{
    if(!main_widget) fatal("bone draw function accessed without main opengl context");
    
    main_widget->makeCurrent();

    glPushMatrix();
    //qDebug() << "drawing " << name;
    // bind vbo
    glBindBuffer(GL_ARRAY_BUFFER,vbo);

    // vertices
    glVertexPointer(3,GL_FLOAT,sizeof(vert),(void*)0);
    glEnableClientState(GL_VERTEX_ARRAY);

    // normals
    glNormalPointer(GL_FLOAT,sizeof(vert),(void*)(sizeof(vec3)));
    glEnableClientState(GL_NORMAL_ARRAY);

    // color
    glColorPointer(4,GL_UNSIGNED_BYTE,sizeof(vert),(void*)(sizeof(vec3)*2));
    glEnableClientState(GL_COLOR_ARRAY);

    // bind ebo
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);

    // draw model
    //  OpenGL should normalize normal vectors
    glEnable(GL_NORMALIZE);
    //  Enable lighting
    glEnable(GL_LIGHTING);
    //  Enable light 0
    glEnable(GL_LIGHT0);
    if(mode == 0)
    {
        //  white light
        float Emission[]  = {0.0,0.0,0.0,1.0};
        float Ambient[]   = {0.05, 0.05, 0.05, 1.0};
        float Specular[]  = {1.0,1.0,1.0,1.0};
        float Diffuse[]   = {1.0,1.0,1.0,1.0};
        float Position[]  = {1.0,1.0,1.0,0.0};
        float Shinyness[] = {16};
        //  Set ambient, diffuse, specular components and position of light 0
        glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT0,GL_POSITION,Position);
        //  Set materials
        glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,Shinyness);
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,Diffuse);
        glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,Diffuse);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Specular);
        glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    }
    else
    {
        //  green light
        float Emission[]  = {0.0,0.05,0.0,1.0};
        float Ambient[]   = {0.0,0.3,0.0,1.0};
        float Specular[]  = {1.0,1.0,1.0,1.0};
        float Diffuse[]   = {0.0,0.7,0.0,1.0};
        float Position[]  = {1.0,1.0,1.0,0.0};
        float Shinyness[] = {16};
        //  Set ambient, diffuse, specular components and position of light 0
        glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT0,GL_POSITION,Position);
        //  Set materials
        glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,Shinyness);
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,Diffuse);
        glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,Diffuse);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Specular);
        glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    }
    // adjust to scale to reasonable size + face along the +z axis
    glRotated(-90,0,1,0);
    glScaled(40,40,40);
    glDrawElements(GL_TRIANGLES, 3*num_tri, GL_UNSIGNED_INT, (void*)0);

    // unbind ebo, vertex, vbo
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    glPopMatrix();
}
void Bone::loadBone()
{
    // qDebug() << "loading " << name << "...";
    // open file
    QFile file(adr);
    if (!file.open(QFile::ReadOnly)) fatal("Cannot open file "+adr);

    // create text stream
    QTextStream in(&file);

    // read header
    QString line = in.readLine();
    if(line!="ply") fatal("Invalid header " + line + " in " + adr);

    // read format
    line = in.readLine();
    if (line!="format binary_little_endian 1.0") fatal("Invalid "+line+" in "+adr);

    // read vertex count
    QStringList word;
    // snarf comments
    do
    {
        line = in.readLine();
        word = line.split(" ");
    } while (word[0]=="comment");
    // must have "element vertex <num vertices>"
    if (word.size()!=3 || word[0]!="element" || word[1]!="vertex") fatal("Invalid vertex line "+line+" in "+adr);
    num_vert = word[2].toInt(); // number of vertices
    if (num_vert < 1) fatal("Invalid vertex count "+word[2]+" in "+adr);

    // read property vars
    QStringList w = {"x","y","z"};
    for (int i=0;i<w.size();i++)
    {
        line = in.readLine();
        word = line.split(" ");
        if (word.size()!=3 || word[0]!="property" || word[1]!="float" || word[2]!=w[i]) fatal("Unexpected variable line "+line+" in "+adr);
    }

    // read element line
    line = in.readLine();
    word = line.split(" ");
    if (word.size()!=3 || word[0]!="element" || word[1]!="face") fatal("Invalid face line "+line+" in "+adr);
    num_tri = word[2].toInt(); // number of triangles
    if (num_tri<1) fatal("Invalid element count "+word[2]+" in "+adr);

    //qDebug() << "num vert: " << num_vert << "\nnum tri: " << num_tri;

    // read element type
    line = in.readLine();
    word = line.split(" ");
    if (word.size()<4 || word[0]!="property" || word[1]!="list" || word[2]!="uchar" || (word[3]!="uint" && word[3]!="int")) fatal("Invalid element line "+line+" in "+adr);

    //  Read end header
    line = in.readLine();
    if (line!="end_header") fatal("Invalid ply header end "+line+" in "+adr);

    //  Allocate memory for vertex and S arrays
    vertices = new vert[num_vert];
    elements = new elem[num_tri];
    if (!vertices || !elements) fatal("Cannot allocate memory for "+adr);

    // seek to end of text and start binary stream
    file.seek(in.pos());
    QDataStream bin(&file);
    bin.setByteOrder(QDataStream::LittleEndian);
    bin.setFloatingPointPrecision(QDataStream::SinglePrecision);

    // read vertex data
    for(int i = 0; i < num_vert; i++) 
    {
        bin >> vertices[i].v.x;
        bin >> vertices[i].v.y;
        bin >> vertices[i].v.z;
    }

    // read element data
    for (int i = 0; i < num_tri; i++)
    {
        unsigned char n;
        bin >> n;
        //if (n!=3) fatal("Invalid face count "+QString::number(n));
        bin >> elements[i].a;
        bin >> elements[i].b;
        bin >> elements[i].c;
    }

    // close file
    file.close();

    // set initial color and alpha channel values
    for(int i = 0; i < num_vert; i++)
    {
        vertices[i].c = {255,255,255};
        vertices[i].a = idx;
    }

    // calculate normal and sum at vertexes
    for (int i=0; i<num_tri; i++)
    {
        vec3 N = normalize((vertices[elements[i].b].v-vertices[elements[i].a].v) * (vertices[elements[i].c].v-vertices[elements[i].b].v));
        vertices[elements[i].a].n += N;
        vertices[elements[i].b].n += N;
        vertices[elements[i].c].n += N;
    }
    // average normals
    for (int i=0; i<num_vert; i++) vertices[i].n = normalize(vertices[i].n);
    // invert normals if necessary
    if(inv_norm) for(int i = 0; i < num_vert; i++) vertices[i].n = vertices[i].n.invert();
}
void Bone::initBone()
{
    initializeOpenGLFunctions();
    loadBone();
    //  Copy vertex data to VBO
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,num_vert*sizeof(vert),vertices,GL_STATIC_DRAW);
    //  Copy element data to EBO
    glGenBuffers(1,&ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,num_tri*sizeof(elem),elements,GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}
void Bone::initAdj(vector<adj_bone> a)
{
    adj = a;
}
void Bone::incrementBoneAng(int th, int ph)
{
    ang.th += th;
    ang.ph += ph;

    // find any bones that move concurrently (bone_dir = neither, not up or down) and add to those as well
    visited = true;
    int len = adj.size();
    for(int i = 0; i < len; i++)
    {
        adj_bone adj_b = adj.at(i);
        shared_ptr<join> adj_join = adj_b.j;
        // if bone is concurrent, eg moves together, and hasn't been set yet, set th,ph for adj bone
        if(adj_join != nullptr && !adj_b.adj_bone->getVisited())
        {
            if(adj_join->a == theta) adj_b.adj_bone->incrementBoneAng(th*adj_join->s,0);
            else if(adj_join->a == pheta) adj_b.adj_bone->incrementBoneAng(0,ph*adj_join->s);
            else if(adj_join->a == both) adj_b.adj_bone->incrementBoneAng(th*adj_join->s,ph*adj_join->s);
            else fatal("Invalid option in incrementBoneAng");
        }
    }
}
void Bone::rotateBone(ang_dir a, int x, int y, int z)
{
    if(!main_widget) fatal("bone rotate function accessed without main opengl context");
    
    main_widget->makeCurrent();
    if(a == theta) {glRotated(ang.th,x,y,z);}
    else {glRotated(ang.ph,x,y,z);}
}
void Bone::offsetBone()
{
    if(!main_widget) fatal("bone offset function accessed without main opengl context");

    main_widget->makeCurrent();
    glTranslated(off.x, off.y, off.z);
}
int Bone::getBoneIdx()
{
    return idx;
}
QString Bone::getBoneName()
{
    return name;
}
void Bone::setFlag(bool f)
{
    visited = f;
}
bool Bone::getVisited()
{
    return visited;
}
void Bone::setVisited(bool b)
{
    visited = b;
}
void Bone::setBoneAng(int th, int ph)
{
    ang.th = th;
    ang.ph = ph;
}