#include "obj_loader.h"

void readObjFile(char path[], int path_length, float** data, unsigned int* dataSize, int** indices, unsigned int* indexSize) {
    printf("here?");

    if (path_length < 0 || strlen(path) != path_length-1) {
        printf("err in length.\n");
        return;
    }

    if (strcmp((path + (path_length - 1) - 3), "obj") != 0) {
        printf("did not provide obj file, returning.\n");
        return;
    }

    FileReader fReader;

    fReader.f_ptr = fopen(path, "r");
    int response = 0;
    bool lineChange = false;
    //fgets(&character, sizeof(char), fReader.f_ptr);

    unsigned int vertex_count = 0;
    unsigned int vertex_normal_count = 0;
    unsigned int vertex_texture_count = 0;

    unsigned int face_count = 0;

    // first pass to simply check how many vertices, faces, textures, etc for mem alloc.
    while (fgets(fReader.buff, sizeof(fReader.buff), fReader.f_ptr)) {
        if (fReader.buff[strlen(fReader.buff) - 1] != '\n') {   // then buffer not big enough for the line
            printf("should not be longer than 255, returning.\n");
            break;
        }

        if (fReader.buff[0] == 'v') {
            if (fReader.buff[1] == 'n') {   //normal
                vertex_normal_count++;
            } else if (fReader.buff[1] == 't') {   //texture
                vertex_texture_count++;
            } else { //vertex
                vertex_count++;
            }
        } else if (fReader.buff[0] == 'f') {
            face_count++;
        }   //otherwise ignore
    }

    printf("here2?");

    
    //scanf((buff+1), "%f %f %f", str1, str2, str3);
    //scanf((buff+1), "%[^/]d %[^/]d %[^/]d, str1, str2, str3);

    fclose(fReader.f_ptr);

    unsigned int vertex_index = 0;
    unsigned int vertex_normal_index = 0;
    unsigned int vertex_texture_index = 0;
    unsigned int face_index = 0;
    unsigned int data_index = 0;


    float* vertices = (float*)malloc(sizeof(float) * vertex_count * 3);
    float* vertex_normals = (float*)malloc(sizeof(float) * vertex_normal_count * 3);
    float* vertex_textures = (float*)malloc(sizeof(float) * vertex_texture_count * 2);

    //float (*data)[8] = (float*)malloc(sizeof(float[8]) * vertex_texture_count);
    int* faces = (int*)malloc(sizeof(int) * face_count * 3);
    *indices = (int*)malloc(sizeof(int) * face_count * 3);
    *data = (float*)malloc(sizeof(float) * vertex_count * 3);

    // 2nd pass to actually set the data merge, free, pass data to mesh type, then free again.
    char tmp[10];
    fReader.f_ptr = fopen(path, "r");
    while (fgets(fReader.buff, sizeof(fReader.buff), fReader.f_ptr)) {
        if (fReader.buff[strlen(fReader.buff) - 1] != '\n') {   // then buffer not big enough for the line
            printf("should not be longer than 255, returning.\n");
            break;
        }

        if (fReader.buff[0] == 'v') {
            if (fReader.buff[1] == 'n') {   //normal
                sscanf((fReader.buff), "%s %f %f %f", tmp, &(vertex_normals[vertex_normal_index+0]), &(vertex_normals[vertex_normal_index+1]), &(vertex_normals[vertex_normal_index+2]));
                vertex_normal_index += 3;
            } else if (fReader.buff[1] == 't') {   //texture
                sscanf((fReader.buff), "%s %f %f", tmp, &(vertex_textures[vertex_texture_index+0]), &(vertex_textures[vertex_texture_index+1]));
                vertex_texture_index += 2;
            } else { //vertex
                sscanf((fReader.buff), "%s %f %f %f", tmp, &(vertices[vertex_index+0]), &(vertices[vertex_index+1]), &(vertices[vertex_index+2]));
                //printf("\n%s", fReader.buff);
                //printf("vertices[%d+0] = %f\n", vertex_index, vertices[vertex_index+0]);
                vertex_index += 3;
            }
        } else if (fReader.buff[0] == 'f') {
            sscanf((fReader.buff), "%s %d %d %d", tmp, &(faces[face_index+0]), &(faces[face_index+1]), &(faces[face_index+2]));
            //printf("\n%s", fReader.buff);
            //printf("faces[%d+0] = %d\n", face_index, faces[face_index+0]);
            face_index += 3;
        }   //otherwise ignore
    }

    printf("here3?\n");

    ///*
    for (unsigned int i = 0; i < vertex_count*3; i += 3) {
        (*data)[i + 0] = vertices[i + 0];
        (*data)[i + 1] = vertices[i + 1];
        (*data)[i + 2] = vertices[i + 2];
        int k = i-1;
        if ( fabs(vertices[k+0]) <= 1 && fabs(vertices[k+1]) <= 1 && fabs(vertices[k+2]) <= 1) {
            printf("index %d has a bad val %f\n", k+0, vertices[k+0]);
            printf("index %d has a bad val %f\n", k+1, vertices[k+1]);
            printf("index %d has a bad val %f\n", k+2, vertices[k+2]);
        }
        //printf("data[%d] = %f\n", i+2, (*data)[i + 0]);
    }
    *dataSize = (unsigned)vertex_count*3;

    for (unsigned int i = 0; i < face_count*3; i += 3) {    //bc obj starts at 1
        (*indices)[i + 0] = faces[i + 0] - 1;
        (*indices)[i + 1] = faces[i + 1] - 1;
        (*indices)[i + 2] = faces[i + 2] - 1;
        //printf("lower indices[%d] = %d\n", i+0, (*indices)[i + 0]);
    }
    *indexSize = (unsigned)face_count*3;
    //*/

    //free(vertices);
    //free(vertex_textures);
    //free(vertex_normals);
    //free(faces);

    fclose(fReader.f_ptr);
    printf("edn\n");
    //return vertices;
}