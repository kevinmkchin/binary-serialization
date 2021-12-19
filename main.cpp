#include <stdio.h>
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
    if(argc > 1)
    {
        char* filePath = argv[1];
        FILE* fp;
        fp = fopen(filePath, "r");
        if(fp == NULL)
        {
            printf("File doesn't exist.\n");
            return -1;
        }

        fseek(fp, 0, SEEK_END);
        size_t sz = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        ByteBuffer b = ByteBufferNew();
        ByteBufferResize(&b, sz);
        fread(b.data, 1, sz, fp);

        fclose(fp);

        GameObject read;
        ByteBufferRead(&b, vec3, &read.pos);
        ByteBufferRead(&b, vec3, &read.rot);
        ByteBufferRead(&b, float, &read.size);

        printf("pos %f, %f, %f \n", read.pos.x, read.pos.y, read.pos.z);
        printf("rot %f, %f, %f \n", read.rot.x, read.rot.y, read.rot.z);
        printf("size %f \n", read.size);

        ByteBufferFree(&b);
    }

    GameObject obj1;
    obj1.pos = { 1.f, 4.f, 3.f };
    obj1.rot = { 23.f, -40.f, 90.f };
    obj1.size = 3.f;

    ByteBuffer b = ByteBufferNew();
    ByteBufferWrite(&b, vec3, obj1.pos);
    ByteBufferWrite(&b, vec3, obj1.rot);
    ByteBufferWrite(&b, float, obj1.size);

    FILE* fp;
    fp = fopen("object.bb", "w");
    fwrite(b.data, 1, b.size, fp);
    fclose(fp);

    ByteBufferFree(&b);

    return 0;
}