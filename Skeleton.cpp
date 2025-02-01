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
    double len = bones_l[idx];
    // get bone char signifier
    char ch = 'A' + idx;
    // get angles for degrees of motion
    angles ang = bones_ang[idx];
    // get bone xyz offset
    offset off = bones_off[idx]; 
    // get bone name
    string name = bones_n[idx];

    shared_ptr<bone> new_bone(new bone{vbo,len,ch,ang,off,name});
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
    "length: " << b.len << "\n" <<
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
    printBone(13,0);
    // cout << "drawing skeleton...\n";
    // Torso
    //glPushMatrix();
    drawLeg(13,-1,'N');
    //glPopMatrix;
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
void Skeleton::drawLeg(int idx, float i, char ch) // draw leg: i - +/- axis rotation specifier, ch - starting char signifier
{
    const bone femur = *bones[idx];
    const bone tibfib = *bones[idx+1];
    const bone talus = *bones[idx+2];
    const bone foot = *bones[idx+3];
    const bone toes = *bones[idx+4];
    cout << "building leg [" << idx << "-" << idx+4 << "]\n";

    glPushMatrix();
    // move complete leg
        // femur
        glTranslated(femur.off.x,femur.off.y,femur.off.z);
        drawLabel(ch);
        glRotated(femur.ang.ph,-1,0,0);
        glRotated(femur.ang.th,0,0,i);
        drawBone(femur);
        glPushMatrix();
            // tibia/fibula
            // move tibfib+talus+foot+toes down the y axis the length of the femur
            glTranslated(tibfib.off.x,tibfib.off.y-femur.len,tibfib.off.z);
            drawLabel(ch+1);
            glRotated(femur.ang.ph,-1,0,0);
            drawBone(tibfib);
            glPushMatrix();
                // talus
                //move talus+foot+toes down the y axis the length of the tibia/fibula
                glTranslated(talus.off.x,talus.off.y-tibfib.len,talus.off.z);
                drawLabel(ch+2);
                glRotated(talus.ang.ph,0,1,0);
                glRotated(talus.ang.th,-1,0,0);
                drawBone(talus);
                glPushMatrix();
                    // foot
                    // move foot+toes down along y axis the length of talus
                    glTranslated(foot.off.x,foot.off.y-talus.len,foot.off.z);
                    drawLabel(ch+3);
                    drawBone(foot);
                    glPushMatrix();
                        // toes
                        // move toes forward along z axis the length of foot
                        glTranslated(toes.off.x,toes.off.y,toes.off.z+foot.len);
                        drawLabel(ch+4);
                        glRotated(toes.ang.ph,-1,0,0);
                        drawBone(toes);
                    glPopMatrix();
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}
void Skeleton::drawArm(int idx, float i, char ch) // draw arm: i - +/- axis rotation specifier, ch - starting char signifier
{
    // glPushMatrix();

    // glPopMatrix;
}
void Skeleton::drawLabel(char ch) // draw char signifier labels at each bone
{

}