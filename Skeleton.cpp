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

vbo_t Skeleton::readBoneFile(string file_name)
{
    
    // If a separate file location is specified in bones_adr, append to the start of file location path
    string bone_file_path = bones_adr!="" ? (bones_adr + "/" + file_name) : ("./" + file_name);
    // generate a new VBO for object
    // cout << "Reading bone from " << bone_file_path << "\n";
    // cout << "Running LoadModel(" << bone_file_path << ")\n";
    vbo_t new_vbo = LoadModel(toCStr(bone_file_path));
    // cout << "\nVBO in readBoneFile:\n";
    // printVBO(new_vbo);
    return new_vbo;
}
shared_ptr<bone> Skeleton::newBone(int idx)
{
    if(idx >= NUM_BONES || idx < 0) Fatal("invalid bone index in newBone: %d\n",idx);
    // get VBO for bone
    vbo_t vbo = readBoneFile(bones_f[idx]);
    // cout << "\nVBO in newBone:\n";
    // printVBO(vbo);
    // get length of bone
    //double len = bones_l[idx];
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
        shared_ptr<bone> b = bones[i];
        // get adjacency list
        vector<int> adj_list = bones_adj.at(i);
        // get length of adjacency list
        int len = adj_list.size();
        for(int j = 0; j < len; j++)
        {
            // get adj bone pointer using int index stored in adj list 'a'
            shared_ptr<bone> adj_bone = bones[adj_list.at(j)];
            b->adj.push_back(adj_bone);
        }
    }
}

//-------------------------------------------------------------------

void Skeleton::resetAng() // reset all bone angles
{
    for(int i = 0; i < NUM_BONES; i++) {updateAng(i,0,0);}
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
        string adj_name = b.adj.at(j)->name;
        cout << " [" << adj_name << "]";
    }
    cout << "\n";
    if(v == true) {printVBO(b.vbo,1);}
    else {printVBO(b.vbo,0);}
}
const void Skeleton::printBones(bool v) // print bones information to terminal
{
    for(int i = 0; i < NUM_BONES; i++) {printBone(i,v);}
}
void Skeleton::updateAng(int idx, int th, int ph) // update bone motion angles
{
    bones[idx]->ang.th = th;
    bones[idx]->ang.ph = ph;
}
void Skeleton::drawSkeleton() // draw the complete skeleton
{
    const bone pelvis = *bones[0];
    const bone torso = *bones[1];
    const bone head = *bones[2];

    // Draw upper body
    glPushMatrix();
        // Pelvis
        drawBone(pelvis);
        drawLabel(pelvis.ch);
        glPushMatrix();
            // Torso
            glTranslated(torso.off.x,torso.off.y,torso.off.z);
            drawLabel(torso.ch);
            glRotated(torso.ang.ph,1,0,0);
            drawBone(torso);
            glPushMatrix();
                // Arms
                drawArm(3,-1);
                drawArm(8,1);
            glPopMatrix();
            glPushMatrix();
                // Head
                glTranslated(head.off.x,head.off.y,head.off.z);
                drawLabel(head.ch);
                glRotated(head.ang.ph,1,0,0);
                glRotated(head.ang.th,0,1,0);
                drawBone(head);
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
    drawLeg(13,-1);
    drawLeg(18,1);
}
void Skeleton::drawBone(int idx) // draw bone at origin
{
    cout << "drawing " << bones[idx]->name << "\n";
    vbo_t vbo = bones[idx]->vbo;
    DrawModel(vbo);
}
void Skeleton::drawBone(bone b) // draw bone at origin
{
    glPushMatrix();
        // adjust to scale to reasonable size + face along the +z axis
        glRotated(-90,0,1,0);
        glScaled(40,40,40);
        cout << "drawing " << b.name << "\n";
        vbo_t vbo = b.vbo;
        DrawModel(vbo);
    glPopMatrix();
}
void Skeleton::drawLeg(int idx, float i) // draw leg: i - +/- axis rotation specifier, ch - starting char signifier
{
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
            glRotated(femur.ang.ph,-1,0,0);
            drawBone(tibfib);
            glPushMatrix();
                // talus
                //move talus+foot+toes down the y axis the length of the tibia/fibula
                glTranslated(talus.off.x,talus.off.y,talus.off.z);
                drawLabel(talus.ch);
                glRotated(talus.ang.ph,0,1,0);
                glRotated(talus.ang.th,-1,0,0);
                drawBone(talus);
                glPushMatrix();
                    // foot
                    // move foot+toes down along y axis the length of talus
                    glTranslated(foot.off.x,foot.off.y,foot.off.z);
                    drawLabel(foot.ch);
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
        glRotated(humerus.ang.ph,1,0,0);
        glRotated(humerus.ang.th,0,0,i);
        drawBone(humerus);
        

        glPushMatrix();
            // lower arm - radius + ulna
            // radius
            glPushMatrix();
                glTranslated(radius.off.x,radius.off.y,radius.off.z);
                glRotated(radius.ang.ph,1,0,0);
                drawBone(radius);
            glPopMatrix();
            // ulna
            glPushMatrix();
                glTranslated(ulna.off.x,ulna.off.y,ulna.off.z);
                drawLabel(ulna.ch);
                glRotated(ulna.ang.ph,1,0,0);
                drawBone(ulna);
                glPushMatrix();
                    // hand
                    glTranslated(hand.off.x,hand.off.y,hand.off.z);
                    drawLabel(hand.ch);
                    glRotated(hand.ang.ph,1,0,0);
                    glRotated(hand.ang.th,0,i,0);
                    drawBone(hand);
                glPopMatrix();    
            glPopMatrix();  
        glPopMatrix();
    glPopMatrix();
}
void Skeleton::drawLabel(char ch) // draw char signifier labels at each bone
{
    glColor3f(1,1,1);
    glRasterPos3d(0,0,0);
    Print("%c",ch);
}