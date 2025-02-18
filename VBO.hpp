#ifndef VBO_HPP
#define VBO_HPP

struct buf_t
{
   void* off;
   int type;
   int n;
};
struct vbo_t
{
   int type,n,stride;
   unsigned int buf,ele;
   buf_t vertex,color,normal,texture;
   float dim;
};

class VBO
{
    public:
        VBO();
        ~VBO();

        void print();
        void draw();

    private:
        void loadModel();
        void loadPLY();

        int type,n,stride;
        unsigned int buf,ele;
        buf_t vertex,color,normal,texture;
        float dim;
};

#endif