#include "Skeleton.hpp"

Skeleton::Skeleton()
{
    cout << "Initializing skeleton...\n";
    cout << "Reading bone object files from " << bones_adr << "\n";
    for (int i = 0; i < NUM_BONES; i++)
    {
        string bone_name = bones_n[i];
        cout << "Building " << bone_name << "... ";
        bones[i] = newBone(i);
        cout << "complete\n";
    }
    // once all bones are allocated, initialize bone adjacency vectors
    initBoneAdj();
}
Skeleton::~Skeleton()
{
    for(int i = 0; i < NUM_BONES; i++) {bones[i].reset();}
}

//-------------------------------------------------------------------

vbo_t Skeleton::readBoneFile(string file_name, int idx)
{
    // If a separate file location is specified in bones_adr, append to the start of file location path
    string bone_file_path = bones_adr!="" ? (bones_adr + "/" + file_name) : ("./" + file_name);
    // generate a new VBO for object
    vbo_t new_vbo = LoadModel(toCStr(bone_file_path),bones_l[idx]);
    return new_vbo;
}
shared_ptr<bone> Skeleton::newBone(int idx)
{
    if(idx >= NUM_BONES || idx < 0) Fatal("invalid bone index in newBone: %d\n",idx);
    // get VBO for bone
    vbo_t vbo = readBoneFile(bones_f[idx],idx);
    // get bone char signifier
    char ch = 'A' + idx;
    // get angles for degrees of motion
    angles ang = bones_ang[idx];
    // get bone xyz offset
    offset off = bones_off[idx]; 
    // get bone name
    string name = bones_n[idx];

    shared_ptr<bone> new_bone(new bone{vbo,ch,ang,off,name});
    return new_bone;
}
void Skeleton::initBoneAdj()
{
    // adjacency information is stored as a vector of int indices corresponding to elements in bones array
    for(int i = 0; i < NUM_BONES; i++)
    {
        // retrieve current bone
        shared_ptr<bone> b = bones[i];
        // get adjacency list for current bone
        vector<int> adj_list = bones_adj.at(i);
        // get directional information for bones adj to current bone
        vector<bone_dir> dir_list = bones_dir.at(i);

        // get length of adjacency list
        int len = adj_list.size();
        // for each bone in adjacency list, append an adj_bone with ptr to that bone and the direction to the current bone's adj list
        for(int j = 0; j < len; j++)
        {
            // get adj bone pointer using int index stored in adj_list corresponding to a bone in bones array
            shared_ptr<bone> new_adj_ptr = bones[adj_list.at(j)];
            // get direction of the adj bone from the current bone
            bone_dir new_bone_dir = dir_list.at(j);
            // create a new adj_bone
            adj_bone new_adj_bone = {new_adj_ptr,new_bone_dir};
            // add to current bone's adjacency list
            b->adj.push_back(new_adj_bone);
        }
    }
}

//-------------------------------------------------------------------

void Skeleton::resetAng() // reset all bone angles
{
    for(int i = 0; i < NUM_BONES; i++) {bones[i]->ang.th=0; bones[i]->ang.ph=0;}
}
const void Skeleton::printBone(int idx, bool v)
{
    cout << "------------------------------\n";
    bone b = *bones[idx];
    cout << "[" << idx << "|" << (char)(65+idx) << "] " << bones_n[idx] << "\n" <<
    "angles: th=" << b.ang.th << " ph=" << b.ang.ph << "\n" << 
    "offset: (" << b.off.x << "," << b.off.y << "," << b.off.z << ")\n" << 
    "adjacent bones:";
    int len = b.adj.size();
    for(int j = 0; j < len; j++)
    {
        string adj_name = b.adj.at(j).adj_bone->name;
        cout << " [" << adj_name << "]";
    }
    cout << "\n";
    if(v == true) {printVBO(b.vbo,1);}
    else {printVBO(b.vbo,0);}
}
const void Skeleton::printSkeleton(bool v) // print bones information to terminal
{
    for(int i = 0; i < NUM_BONES; i++) {printBone(i,v);}
}
void Skeleton::setAng(int idx, int th, int ph) // update bone motion angles
{
    bones[idx]->ang.th += th;
    bones[idx]->ang.ph += ph;
}
void Skeleton::drawSkeleton() // draw the complete skeleton
{
    glColor3f(1,1,1);

    const bone pelvis = *bones[0];
    const bone lumbar = *bones[1];
    const bone torso = *bones[2];
    const bone head = *bones[3];



    // Draw upper body
    glPushMatrix();
        // Pelvis
        glTranslated(pelvis.off.x,pelvis.off.y,pelvis.off.z);
        drawLeg(14,-1);
        drawLeg(19,1);
        drawBone(pelvis);
        drawLabel(pelvis.ch);
        glPushMatrix();
            // Lumbar
            glPushMatrix();
                glTranslated(0,5,0); // move lumbar label up so its not overlapping pelvis label
                drawLabel(lumbar.ch);
            glPopMatrix();
            glTranslated(lumbar.off.x,lumbar.off.y,lumbar.off.z);
            glRotated(lumbar.ang.ph,1,0,0);
            glTranslated(0,5.25,2.25); // move rotation pt on lumbar to the origin
            drawBone(lumbar);
            glPushMatrix();
                // Torso
                glPushMatrix();
                    glTranslated(0,5,0);
                    drawLabel(torso.ch);
                glPopMatrix();
                glRotated(torso.ang.ph,1,0,0);
                glTranslated(torso.off.x,torso.off.y,torso.off.z);
                drawBone(torso);
                glPushMatrix();
                    // Arms
                    drawArm(4,1);
                    drawArm(9,-1);
                    glPushMatrix();
                        // Head
                        drawLabel(head.ch);
                        glRotated(head.ang.ph,1,0,0);
                        glRotated(head.ang.th,0,1,0);
                        glTranslated(head.off.x,head.off.y,head.off.z);
                        drawBone(head);
                    glPopMatrix();
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}
void Skeleton::drawBone(int idx) // draw bone at origin
{
    vbo_t vbo = bones[idx]->vbo;
    DrawModel(vbo);
}
void Skeleton::drawBone(bone b) // draw bone at origin
{
    glPushMatrix();
        // adjust to scale to reasonable size + face along the +z axis
        glRotated(-90,0,1,0);
        glScaled(40,40,40);
        vbo_t vbo = b.vbo;
        DrawModel(vbo);
    glPopMatrix();
}
void Skeleton::drawLeg(int idx, float i) // draw leg: i - +/- 1 axis rotation specifier, ch - starting char signifier
{
    if(idx >= NUM_BONES || idx < 0) Fatal("Invalid index in drawLeg: %c\n",idx);
    else if (i != 1 && i != -1) Fatal("Invalid value for i in drawLeg: %c\n",i);
    const bone femur = *bones[idx];
    const bone tibfib = *bones[idx+1];
    const bone talus = *bones[idx+2];
    const bone foot = *bones[idx+3];
    const bone toes = *bones[idx+4];

    glPushMatrix();
    // move complete leg
        // femur
        glTranslated(femur.off.x,femur.off.y,femur.off.z);
        drawLabel(femur.ch);
        glRotated(femur.ang.ph,-1,0,0);
        glRotated(femur.ang.th,0,0,i);
        drawBone(femur);
        glPushMatrix();
            // tibia/fibula
            // move tibfib+talus+foot+toes down the y axis the length of the femur
            glTranslated(tibfib.off.x,tibfib.off.y,tibfib.off.z);
            drawLabel(tibfib.ch);
            glRotated(tibfib.ang.ph,-1,0,0);
            drawBone(tibfib);
            glPushMatrix();
                // talus
                //move talus+foot+toes down the y axis the length of the tibia/fibula
                glTranslated(talus.off.x,talus.off.y,talus.off.z);
                drawLabel(talus.ch);
                glRotated(talus.ang.th,0,1,0);
                glRotated(talus.ang.ph,-1,0,0);
                drawBone(talus);
                glPushMatrix();
                    // foot
                    // move foot+toes down along y axis the length of talus
                    glTranslated(foot.off.x,foot.off.y,foot.off.z);
                    //drawLabel(foot.ch);
                    drawBone(foot);
                    glPushMatrix();
                        // toes
                        // move toes forward along z axis the length of foot
                        glTranslated(toes.off.x,toes.off.y,toes.off.z);
                        drawLabel(toes.ch);
                        glRotated(toes.ang.ph,-1,0,0);
                        drawBone(toes);
                    glPopMatrix();
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}
void Skeleton::drawArm(int idx, float i) // draw arm: i - +/- axis rotation specifier, ch - starting char signifier
{
    if(idx >= NUM_BONES || idx < 0) Fatal("Invalid index in drawLabel: %c\n",idx);

    const bone shoulder = *bones[idx];
    const bone humerus = *bones[idx+1];
    const bone ulna = *bones[idx+2];
    const bone radius = *bones[idx+3];
    const bone hand = *bones[idx+4];

    glPushMatrix();
        // shoulder blade
        glPushMatrix();
            glTranslated(shoulder.off.x,shoulder.off.y,shoulder.off.z);
            drawBone(shoulder);
        glPopMatrix();
        // upper arm
        glTranslated(humerus.off.x,humerus.off.y,humerus.off.z);
        drawLabel(humerus.ch);
        glRotated(humerus.ang.th,0,-1,0);
        glRotated(humerus.ang.ph,0,0,i);
        drawBone(humerus);
        glPushMatrix();
            // lower arm - radius + ulna
            // radius
            glPushMatrix();
                glTranslated(radius.off.x,radius.off.y,radius.off.z);
                drawLabel(radius.ch);
                glRotated(radius.ang.ph,0,0,i);
                drawBone(radius);
                drawBone(ulna);
                glPushMatrix();
                    // hand
                    glTranslated(hand.off.x,hand.off.y,hand.off.z);
                    drawLabel(hand.ch);
                    glRotated(hand.ang.th,-i,0,0);
                    glRotated(hand.ang.ph,0,0,i);
                    drawBone(hand);
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}
void Skeleton::drawLabel(char ch) // draw char signifier labels at each bone
{
    if(!validChar(ch)) Fatal("Invalid char in drawLabel: %c\n",ch);
    glDisable(GL_LIGHTING);
    // if the label being drawn is the one currently selected by the user, draw in green instead of red
    if(ch == selected_label) glColor3f(0,1,0);
    else glColor3f(1,0,0);
    glRasterPos3d(0,1,2);
    Print("%c",ch);
    glEnable(GL_LIGHTING);
}
void Skeleton::setLabel(char ch)
{
    if(!validChar(ch)) Fatal("Invalid char in setLabel: %c\n",ch);
    selected_label = ch;
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