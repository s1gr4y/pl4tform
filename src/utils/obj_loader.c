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
            int counter = 0;
            int number_of_verts = 0;
            for (int k = 0; k < strlen(string); k++) {
                if (string[k] == '/') {
                    counter += 1;
                }
                if (counter == 2) {
                    number_of_verts++;
                    counter = 0;
                }
            }
            if (number_of_verts > 4) {
                printf("ERROR, can't support faces with more than 4 vertices!");
                break;
            } else if (number_of_verts == 4) {
                face_count += 2;    // add an extra face to triangulate
            } else {    // 3 verts, (normal)
                face_count++;
            }
        }   //otherwise ignore
    }

    printf("here2?");
    fclose(fReader.f_ptr);

    bool hasNormals = false;
    // to parse with this format use below
    //sscanf(string, "%s %d %*[^0-9] %d %d %*[^0-9] %d %d %*[^0-9] %d %d %*[^0-9] %d", tmp, &(arr[i+0]), &(arr[i+1]), &(arr[i+2]), &(arr[i+3]), &(arr[i+4]), &(arr[i+5]), &(arr[i+6]), &(arr[i+7]));
    if (vertex_normal_count > 0) {  // we need to at least parse faces with "#//# #//# #//#" notation
        hasNormals = true;
    }
    if (vertex_texture_count > 0) { // we need to at least parse faces with "#/#/# #/#/# #/#/#" notation
        // worry about this later
    }

    int increment = 3;
    if (hasNormals) {
        increment = 6;
    }


    unsigned int vertex_index = 0;
    unsigned int vertex_normal_index = 0;
    unsigned int vertex_texture_index = 0;
    unsigned int face_index = 0;
    unsigned int face_normal_index = 0;
    unsigned int face_texture_index = 0;
    unsigned int data_index = 0;


    float* vertices = (float*)malloc(sizeof(float) * vertex_count * 3);
    float* vertex_normals = (float*)malloc(sizeof(float) * vertex_normal_count * 3);
    float* vertex_textures = (float*)malloc(sizeof(float) * vertex_texture_count * 2);

    //float (*data)[8] = (float*)malloc(sizeof(float[8]) * vertex_texture_count);
    int* faces = (int*)malloc(sizeof(int) * face_count * 3);
    int* face_normals = (int*)malloc(sizeof(int) * face_count * 3); // indices for vn and vts
    int* face_textures = (int*)malloc(sizeof(int) * face_count * 2);
    *indices = (int*)malloc(sizeof(int) * face_count * 3);
    *data = (float*)malloc(sizeof(float) * vertex_count * increment);

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
            int counter = 0;
            int number_of_verts = 0;
            for (int k = 0; k < strlen(string); k++) {
                if (string[k] == '/') {
                    counter += 1;
                }
                if (counter == 2) {
                    number_of_verts++;
                    counter = 0;
                }
            }
            if (number_of_verts > 4) {
                printf("ERROR, can't support faces with more than 4 vertices!");
                break;
            } else if (number_of_verts == 4) {
                if (!hasNormals) {
                    sscanf((fReader.buff), "%s %d %d %d %*s", tmp, &(faces[face_index+0]), &(faces[face_index+1]), &(faces[face_index+2]));
                    face_index += 3;
                    sscanf((fReader.buff), "%s %d %*s %d %d", tmp, &(faces[face_index+0]), &(faces[face_index+1]), &(faces[face_index+2]));
                } else {
                    sscanf(string, "%s %d %*[^0-9] %d %d %*[^0-9] %d %d %*[^0-9] %d %*s", tmp, &(faces[face_index+0]), &(face_normals[face_normals_index+0]), 
                    &(faces[face_indexi+1]), &(face_normals[face_normals_index+1]), &(faces[face_index+2]), &(face_normals[face_normals_index+2]));
                    face_index += 3;
                    face_normals_index += 3;
                    sscanf(string, "%s %d %*[^0-9] %d %*s %d %*[^0-9] %d %d %*[^0-9] %d", tmp, &(faces[face_index+0]), &(face_normals[face_normals_index+0]), 
                    &(faces[face_indexi+1]), &(face_normals[face_normals_index+1]), &(faces[face_index+2]), &(face_normals[face_normals_index+2]));
                }
            } else {    // 3 verts, (normal)
                if (!hasNormals) {
                    sscanf((fReader.buff), "%s %d %d %d", tmp, &(faces[face_index+0]), &(faces[face_index+1]), &(faces[face_index+2]));
                } else {
                    sscanf(string, "%s %d %*[^0-9] %d %d %*[^0-9] %d %d %*[^0-9] %d", tmp, &(faces[face_index+0]), &(face_normals[face_normals_index+0]), 
                    &(faces[face_indexi+1]), &(face_normals[face_normals_index+1]), &(faces[face_index+2]), &(face_normals[face_normals_index+2]));
                }
            }
            //printf("\n%s", fReader.buff);
            //printf("faces[%d+0] = %d\n", face_index, faces[face_index+0]);
            face_normals_index += 3;
            face_index += 3;
        }   //otherwise ignore
    }

    printf("here3?\n");

    ///*
    for (unsigned int i = 0; i < vertex_count*3; i += increment) {
        (*data)[i + 0] = vertices[i + 0];
        (*data)[i + 1] = vertices[i + 1];
        (*data)[i + 2] = vertices[i + 2];
        int k = i-1;
        if (hasNormals) {
            (*data)[i + 3] = vertex_normals[i + 0];   // technically we should do vertex_normals[face_normals[i + 0]]
            (*data)[i + 4] = vertex_normals[i + 1];   // but since not interating over faces we assume index is the same
            (*data)[i + 5] = vertex_normals[i + 2];
        }
        if ( fabs(vertices[k+0]) <= 1 && fabs(vertices[k+1]) <= 1 && fabs(vertices[k+2]) <= 1) {
            //printf("index %d has a bad val %f\n", k+0, vertices[k+0]);
            //printf("index %d has a bad val %f\n", k+1, vertices[k+1]);
            //printf("index %d has a bad val %f\n", k+2, vertices[k+2]);
        }
        //printf("data[%d] = %f\n", i+2, (*data)[i + 0]);
    }
    *dataSize = (unsigned)vertex_count*3;

    for (unsigned int i = 0; i < face_count*3; i += 3) {    //bc obj starts at 1
        (*indices)[i + 0] = faces[i + 0] - 1;
        (*indices)[i + 1] = faces[i + 1] - 1;
        (*indices)[i + 2] = faces[i + 2] - 1;
        // we will assume that vertex normals have the same index as vertices
        //printf("lower indices[%d] = %d\n", i+0, (*indices)[i + 0]);
        if (!hasNormals) {
            //proposed method to calculate vertex normals.
            //facted_normal = normalize( cross( 2-3, 1-3))
            vec3 v3;
            v3[0] = vertices[((face_normals[i + 2] - 1)*3) + 0];
            v3[1] = vertices[((face_normals[i + 2] - 1)*3) + 1];
            v3[2] = vertices[((face_normals[i + 2] - 1)*3) + 2];

            vec3 v2;
            v2[0] = vertices[((face_normals[i + 1] - 1)*3) + 0];
            v2[1] = vertices[((face_normals[i + 1] - 1)*3) + 1];
            v2[2] = vertices[((face_normals[i + 1] - 1)*3) + 2];

            vec3 v1;
            v1[0] = vertices[((face_normals[i + 0] - 1)*3) + 0];
            v1[1] = vertices[((face_normals[i + 0] - 1)*3) + 1];
            v1[2] = vertices[((face_normals[i + 0] - 1)*3) + 2];

            vec3 res1;
            glm_vec3_sub(v2, v3, res1);
            vec3 res2;
            glm_vec3_sub(v1, v3, res2);
            vec3 res3;
            glm_vec3_crossn(res1, res2, res3); // res3 is our vertex normal
            (*data)[i + 3] = res3[0];
            (*data)[i + 4] = res3[1];
            (*data)[i + 5] = res3[2];
        }
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