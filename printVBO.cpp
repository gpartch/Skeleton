#include "Skeleton.hpp"

void Skeleton::printVBO(const vbo_t vbo, int v) {
    printf("vbo_t:\n");
    printf("  type: %d\n", vbo.type);
    printf("  n: %d\n", vbo.n);
    printf("  stride: %d\n", vbo.stride);
    printf("  buf: %u\n", vbo.buf);
    printf("  ele: %u\n", vbo.ele);

    if(v == 1)
    {
        printf("  vertex:\n");
        printf("    off: %p\n", vbo.vertex.off);
        printf("    type: %d\n", vbo.vertex.type);
        printf("    n: %d\n", vbo.vertex.n);

        printf("  color:\n");
        printf("    off: %p\n", vbo.color.off);
        printf("    type: %d\n", vbo.color.type);
        printf("    n: %d\n", vbo.color.n);

        printf("  normal:\n");
        printf("    off: %p\n", vbo.normal.off);
        printf("    type: %d\n", vbo.normal.type);
        printf("    n: %d\n", vbo.normal.n);

        printf("  texture:\n");
        printf("    off: %p\n", vbo.texture.off);
        printf("    type: %d\n", vbo.texture.type);
        printf("    n: %d\n", vbo.texture.n);

        printf("  dim: %f\n", vbo.dim);
    }
}

// void displayVBO(const vbo_t& vbo) {
//     std::cout << "vbo_t:" << std::endl;
//     std::cout << "  type: " << vbo.type << std::endl;
//     std::cout << "  n: " << vbo.n << std::endl;
//     std::cout << "  stride: " << vbo.stride << std::endl;
//     std::cout << "  buf: " << vbo.buf << std::endl;
//     std::cout << "  ele: " << vbo.ele << std::endl;

//     std::cout << "  vertex:" << std::endl;
//     std::cout << "    off: " << vbo.vertex.off << std::endl;
//     std::cout << "    type: " << vbo.vertex.type << std::endl;
//     std::cout << "    n: " << vbo.vertex.n << std::endl;

//     std::cout << "  color:" << std::endl;
//     std::cout << "    off: " << vbo.color.off << std::endl;
//     std::cout << "    type: " << vbo.color.type << std::endl;
//     std::cout << "    n: " << vbo.color.n << std::endl;

//     std::cout << "  normal:" << std::endl;
//     std::cout << "    off: " << vbo.normal.off << std::endl;
//     std::cout << "    type: " << vbo.normal.type << std::endl;
//     std::cout << "    n: " << vbo.normal.n << std::endl;

//     std::cout << "  texture:" << std::endl;
//     std::cout << "    off: " << vbo.texture.off << std::endl;
//     std::cout << "    type: " << vbo.texture.type << std::endl;
//     std::cout << "    n: " << vbo.texture.n << std::endl;

//     std::cout << "  dim: " << vbo.dim << std::endl;
// }