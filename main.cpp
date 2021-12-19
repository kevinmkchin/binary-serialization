#include <cstdint>

#include "bytebuffer.h"

struct vec3
{
    float x,y,z;
};

struct GameObject
{
    vec3 pos;
    vec3 rot;
    float size;
};

int main(int argc, char* argv[])
{
    if(argc == 3)
    {
        char* filePath = argv[2];

        if(*argv[1] == 'r')
        {
            ByteBuffer b = {0};
            if(ByteBufferReadFromFile(&b, filePath))
            {
                GameObject read;
                ByteBufferRead(&b, vec3, &read.pos);
                ByteBufferRead(&b, vec3, &read.rot);
                ByteBufferRead(&b, float, &read.size);

                printf("pos %f, %f, %f \n", read.pos.x, read.pos.y, read.pos.z);
                printf("rot %f, %f, %f \n", read.rot.x, read.rot.y, read.rot.z);
                printf("size %f \n", read.size);

                ByteBufferFree(&b);
            }
        }
        else if(*argv[1] == 'w')
        {
            GameObject obj1;
            obj1.pos = { 1.f, 4.f, 3.f };
            obj1.rot = { 23.f, -40.f, 90.f };
            obj1.size = 3.f;

            ByteBuffer b = ByteBufferNew();
            ByteBufferWrite(&b, vec3, obj1.pos);
            ByteBufferWrite(&b, vec3, obj1.rot);
            ByteBufferWrite(&b, float, obj1.size);

            ByteBufferWriteToFile(&b, filePath);

            ByteBufferFree(&b);
        }
    }

    return 0;
}