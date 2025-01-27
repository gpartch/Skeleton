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
    for(int i = 0; i < NUM_BONES; i++) {delete bones[i];}
}

//-------------------------------------------------------------------

vbo_t Skeleton::readBoneFile(string file_name)
{
    
    // If a separate file location is specified in bones_adr, append to the start of file location path
    string bone_file_path = str2bool(bones_adr) ? (bones_adr + "/" + file_name) : ("./" + file_name);
    // generate a new VBO for object
    vbo_t new_vbo = LoadModel(toCStr(bone_file_path));
    return new_vbo;
}
bone* Skeleton::newBone(int idx)
{
    if(idx >= NUM_BONES || idx < 0) Fatal("invalid bone index in newBone: %d\n",idx);
    // get VBO for bone
    vbo_t vbo = readBoneFile(bones_f[idx]);
    // get length of bone
    unsigned int len = bones_l[idx];
    // get bone char signifier
    char ch = 'A' + idx;
    // get angles for degrees of motion
    angles ang = bones_ang[idx];
    // get bone xyz offset
    offset off = bones_off[idx]; 

    bone * new_bone = new bone{vbo,len,ch,ang,off};
    bones[idx] = new_bone;
}
void Skeleton::initBoneAdj()
{
    for(int i = 0; i < NUM_BONES; i++)
    {
        // get length of adjacency list for the current bone in bones_adj
        int len = bones_adj.at(i).size();
        for(int j = 0; j < len; j++)
        {

        }
    }
}

//-------------------------------------------------------------------

void Skeleton::resetAng() // reset all bone angles
{
    for(int i = 0; i < NUM_BONES; i++) {updateAng(i,0,0);}
}
void Skeleton::printBones() // print bones information to terminal
{

}
void Skeleton::updateAng(int idx, int th, int ph) // update bone motion angles
{
    bones[idx]->ang.th = th;
    bones[idx]->ang.ph = ph;
}
void Skeleton::drawSkeleton() // draw the complete skeleton
{

}
void Skeleton::drawBone(int idx) // draw bone at origin
{

}
void Skeleton::drawLeg(int idx, float i, char ch, angles ang_l[5]) // draw leg: i - +/- axis rotation specifier, ch - starting char signifier
{

}
void Skeleton::drawArm(int idx, float i, char ch, angles ang_a[5]) // draw arm: i - +/- axis rotation specifier, ch - starting char signifier
{

}
void Skeleton::drawLabel(char ch) // draw char signifier labels at each bone
{

}