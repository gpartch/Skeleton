#include "Skeleton.hpp"

void Skeleton::initBoneAdj()
{
    // assemble bone adjacency vectors
    vector<vector<adj_bone>> adjacency;
    adjacency.resize(NUM_BONES);
    for(int i = 0; i < NUM_BONES; i++)
    {
        // get adjacency list for current bone
        vector<int> adj_list = bones_adj.at(i);
        // get directional information for bones adj to current bone
        vector<bone_dir> dir_list = bones_dir.at(i);
        // get joins for current bone
        vector<shared_ptr<join>> join_list = bones_j.at(i);

        int num_adj = bones_adj.at(i).size();
        for(int j=0; j<num_adj; j++)
        {
            // index of adjacent bone in the array of all bones
            int adj_idx = adj_list.at(j);
            // pointer to the adjacent bone
            shared_ptr<Bone> adj_ptr = bones[adj_idx];
            // direction of the adjacent bone from current bone
            bone_dir adj_dir = dir_list.at(j);
            // join for adjacent bone
            shared_ptr<join> adj_join = join_list.at(j);
            
            adj_bone new_adj = {adj_ptr,adj_dir,adj_join};
            adjacency.at(i).push_back(new_adj);
            //qDebug() << "   " << new_adj.adj_bone->getBoneName();
            if (adj_ptr == nullptr) fatal("null adj ptr");
        }
    }

    // pass bones adj info to bone objects
    for(int i=0; i<NUM_BONES; i++)
    {
        vector<adj_bone> adjacency_vec = adjacency.at(i);
        bones[i]->initAdj(adjacency_vec);
    }
}
void Skeleton::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST); //  Enable Z-buffer depth testing
    glDisable(GL_BLEND);
    setMouseTracking(true);  //  Ask for mouse events
    glClearColor(0,0,0,255);

    QSurfaceFormat format;
    format.setAlphaBufferSize(8); // Request an 8-bit alpha channel
    QSurfaceFormat::setDefaultFormat(format);

    // initialize skeleton and bones
    for (int i = 0; i < NUM_BONES; i++)
    {
        //qDebug().noquote() << "building " << bones_n[i];
        QString bone_file_path = (bones_adr!="" ? (bones_adr + "/" + bones_f[i]) : ("./" + bones_f[i]));
        shared_ptr<Bone> new_bone = std::make_shared<Bone>(this, bone_file_path,bones_n[i], bones_ang[i], bones_lim[i], bones_off[i], bones_l[i], i);
        if(new_bone == nullptr) fatal("null bone in newBone: " + bones_n[i]);
        (*new_bone).initBone();
        bones[i] = new_bone;
    }
    initBoneAdj();
}

//-------------------------------------------------------------------

void Skeleton::resetAng() // reset all bone angles
{
    for(int i = 0; i < NUM_BONES; i++) (*bones[i]).incrementBoneAng(0,0);
}
void Skeleton::resetFlags()
{
    for(int i = 0; i < NUM_BONES; i++) bones[i]->setFlag(0);
}
void Skeleton::setAng(int idx, int th, int ph) // increment bone motion angles
{
    resetFlags();
    bones[idx]->incrementBoneAng(th,ph);
}
void Skeleton::printSkeleton() // print bones information to terminal
{
    for(int i = 0; i < NUM_BONES; i++) (*bones[i]).printBone();
}
void Skeleton::drawSkeleton() // draw the complete skeleton
{
    glColor3f(1,1,1);

    Bone pelvis = *(bones[0]);
    Bone lumbar = *(bones[1]);
    Bone torso = *(bones[2]);
    Bone head = *(bones[3]);

    // Draw upper body
    glPushMatrix();
        // Pelvis
        pelvis.offsetBone();
        drawLeg(14,-1);
        drawLeg(19,1);
        drawBone(pelvis);
        glPushMatrix();
            // Lumbar
            lumbar.offsetBone();
            lumbar.rotateBone(pheta,1,0,0);
            glTranslated(0,5.25,2.25); // move rotation pt on lumbar to the origin
            drawBone(lumbar);
            glPushMatrix();
                // Torso
                torso.offsetBone();
                torso.rotateBone(pheta,1,0,0);
                drawBone(torso);
                glPushMatrix();
                    // Arms
                    drawArm(4,1);
                    drawArm(9,-1);
                    glPushMatrix();
                        // Head
                        head.offsetBone();
                        head.rotateBone(pheta,1,0,0);
                        head.rotateBone(theta,0,1,0);
                        drawBone(head);
                    glPopMatrix();
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}
void Skeleton::drawBone(Bone b) // draw bone at origin
{
    if(b.getBoneIdx() == selected_bone) {b.drawBone(1);} else {b.drawBone(0);}
}
void Skeleton::drawLeg(int idx, float i) // draw leg: i - +/- 1 axis rotation specifier, ch - starting char signifier
{
    if(!validIdx(idx)) fatal("Invalid index in drawLeg: " + idx);
    else if (i != 1 && i != -1) fatal("Invalid value for i in drawLeg");
    Bone femur = *bones[idx];
    Bone tibfib = *bones[idx+1];
    Bone talus = *bones[idx+2];
    Bone foot = *bones[idx+3];
    Bone toes = *bones[idx+4];

    glPushMatrix();
    // move complete leg
        // femur
        femur.offsetBone();
        femur.rotateBone(theta,0,0,i);
        femur.rotateBone(pheta,-1,0,0);
        drawBone(femur);
        glPushMatrix();
            // tibia/fibula
            // move tibfib+talus+foot+toes down the y axis the length of the femur
            tibfib.offsetBone();
            tibfib.rotateBone(pheta,-1,0,0);
            drawBone(tibfib);
            glPushMatrix();
                // talus
                //move talus+foot+toes down the y axis the length of the tibia/fibula
                talus.offsetBone();
                talus.rotateBone(theta,0,1,0);
                talus.rotateBone(pheta,-1,0,0);
                drawBone(talus);
                glPushMatrix();
                    // foot
                    // move foot+toes down along y axis the length of talus
                    foot.offsetBone();
                    drawBone(foot);
                    glPushMatrix();
                        // toes
                        // move toes forward along z axis the length of foot
                        toes.offsetBone();
                        toes.rotateBone(pheta,-1,0,0);
                        drawBone(toes);
                    glPopMatrix();
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
    ErrCheck("leg");
}
void Skeleton::drawArm(int idx, float i) // draw arm: i - +/- axis rotation specifier, ch - starting char signifier
{
    if(idx >= NUM_BONES || idx < 0) fatal("Invalid index in drawLabel: "+idx);

    Bone shoulder = *bones[idx];
    Bone humerus = *bones[idx+1];
    Bone ulna = *bones[idx+2];
    Bone radius = *bones[idx+3];
    Bone hand = *bones[idx+4];

    glPushMatrix();
        // shoulder blade
        shoulder.offsetBone();
        shoulder.rotateBone(theta,0,i,0);
        shoulder.rotateBone(pheta,0,0,i);
        drawBone(shoulder);
        glPushMatrix();
            // upper arm
            humerus.offsetBone();
            humerus.rotateBone(theta,0,i,0);
            humerus.rotateBone(pheta,0,0,i);
            drawBone(humerus);
            glPushMatrix();
                ulna.offsetBone();
                ulna.rotateBone(pheta,i,0,0);
                // axis of rotation adjustment
                glTranslated(.9,.2,-.6);
                drawBone(ulna);
                glPushMatrix();
                    radius.rotateBone(theta,0,i,0);
                    drawBone(radius);
                    glPushMatrix();
                        hand.offsetBone();
                        hand.rotateBone(pheta,i,0,0);
                        drawBone(hand);
                    glPopMatrix();
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
    ErrCheck("arm");
}

bool Skeleton::validIdx(int idx)
{
    if(idx >= min_idx && idx <= max_idx) return true;
    return false;
}
bool Skeleton::validChar(char ch)
{
    if(ch >= min_char && ch <= max_char) return true;
    return false;
}

//-------------------------------------------------------------------

// Set global x
void Skeleton::setGX(double X)
{
    gx = X;
    update();
}
// Set global y
void Skeleton::setGY(double Y)
{
    gy = Y;
    update();
}
// Set global z
void Skeleton::setGZ(double Z)
{
    gz = Z;
    update();
}
void Skeleton::setDIM(double DIM)
{
    dim = DIM;
    update();
}
void Skeleton::reset(void)
{
    gx = 0;
    gy = 0;
    gz = 0;
    th = 0;
    ph = 0;
    dim = 60;
    emit setDim(dim);
    emit setAngles("th,ph= "+QString::number(th)+","+QString::number(ph));
    emit setWidgetGX(gx);
    emit setWidgetGY(gy);
    emit setWidgetGZ(gz);
    update();
}

//-------------------------------------------------------------------

void Skeleton::mousePressEvent(QMouseEvent* e)
{
   mouse = true;
   pos = e->pos();  //  Remember mouse location
   int a = (int)getAlpha(); //  Get alpha value at mouse location
   if(a >= 0 && a < NUM_BONES)
   {
        if(selected_bone == a) setSelectedBone(-1);
        else setSelectedBone(a);
        emit resetBoneSelectedBtn(selected_bone);
   }
}

//
//  Mouse released
//
void Skeleton::mouseReleaseEvent(QMouseEvent*)
{
    mouse = false;
}

//
//  Mouse moved
//
void Skeleton::mouseMoveEvent(QMouseEvent* e)
{
   if (mouse)
   {
      QPoint d = e->pos()-pos;  //  Change in mouse location
      th = (th+d.x())%360;      //  Translate x movement to azimuth
      ph = (ph+d.y())%360;      //  Translate y movement to elevation
      pos = e->pos();           //  Remember new location
      update();                 //  Request redisplay
   }
}

//
//  Mouse wheel
//
void Skeleton::wheelEvent(QWheelEvent* e)
{
    if(selected_bone == -1)
    {
        //  Zoom out
        if (e->angleDelta().y()<0)
            setDIM(dim+5);
        //  Zoom in
        else if (dim>5)
            setDIM(dim-5);
        //  Signal to change dimension spinbox
        emit setDim(dim);
    }
    else if(selected_bone >= 0 && selected_bone < NUM_BONES)
    {
        //  Zoom out
        if (e->angleDelta().y()<0)
        {
            // increase ph
            setAng(selected_bone,0,5);
        }
        //  Zoom in
        else if (e->angleDelta().y()>0)
        {
            // decrease ph
            setAng(selected_bone,0,-5);
        }
        //  Slide Up
        if (e->angleDelta().x()<0)
        {
            // increase th
            setAng(selected_bone,5,0);
        }
        //  Slide Down
        else if (e->angleDelta().x()>0)
        {
            // decrease th
            setAng(selected_bone,-5,0);
        }
    }
    update();
}

//-------------------------------------------------------------------

void Skeleton::resizeGL(int width, int height)
{
   //  Window aspect ration
   asp = (width && height) ? width / (float)height : 1;
   //  Viewport is whole screen
   glViewport(0,0,width,height);
   //  Set projection
   project();
}

//
//  Draw the window
//
void Skeleton::paintGL()
{
    glClearColor(0,0,0,255);
    //  Erase the window and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    //  Enable Z-buffering in OpenGL
    glEnable(GL_DEPTH_TEST);
    //  Undo previous transformations
    glLoadIdentity();

    glRotatef(ph,1,0,0);
    glRotatef(th,0,1,0);

    //  Draw the model
    glPushMatrix();
    glTranslated(gx,gy,gz);
    drawSkeleton();
    ErrCheck(toCStr("skeleton"));
    glPopMatrix();
    
    //  Draw axes - no lighting from here on
    glDisable(GL_LIGHTING);
    glColor3f(1,1,1);
    if (axes)
    {
        float f=40;
        glBegin(GL_LINES);
        glVertex3f(0,0,0);
        glVertex3f(f,0,0);
        glVertex3f(0,0,0);
        glVertex3f(0,f,0);
        glVertex3f(0,0,0);
        glVertex3f(0,0,f);
        glEnd();
    }

    //  Render the scene and make it visible
    ErrCheck("display");
    project();
    glFlush();

    //  Emit signal with display angles and dimensions
    emit setAngles("th,ph= "+QString::number(th)+","+QString::number(ph));

    // GLint alphaBits;
    // glGetIntegerv(GL_ALPHA_BITS, &alphaBits);
    // qDebug() << "Alpha bits in framebuffer:" << alphaBits;

    //  Done
    glFlush();
}

//
//  Set projection
//
void Skeleton::project()
{
   //  Orthogonal projection to dim
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (asp>1)
      glOrtho(-dim*asp, +dim*asp, -dim, +dim, -3*dim, +3*dim);
   else
      glOrtho(-dim, +dim, -dim/asp, +dim/asp, -3*dim, +3*dim);

   //  Back to model view
   glMatrixMode(GL_MODELVIEW);
}

unsigned char Skeleton::getAlpha()
{
    makeCurrent();
    int x = pos.x();
    int y = height()-pos.y();
    unsigned char c[4];
    update();
    glReadPixels(x,y,1,1,GL_RGBA,GL_UNSIGNED_BYTE,c);
    return c[3];
}

//-------------------------------------------------------------------------------

// set selected bone to idx, or if idx is already selected set to unselected (-1)
void Skeleton::setSelectedBone(int idx) 
{
    if(idx != selected_bone) 
    {
        selected_bone = idx; 
        emit resetBoneSelectedBtn(idx);
        update();
    }
    else
    {
        selected_bone = -1;
        emit resetBoneSelectedBtn(-1);
        update();
    }
}

void Skeleton::resetBones(void)
{
    for(int i = 0; i < NUM_BONES; i++)
    {
        bones[i]->setBoneAng(bones_ang[i].th, bones_ang[i].ph);
    }
    update();
}