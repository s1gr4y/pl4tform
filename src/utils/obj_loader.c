#include "obj_loader.h"

void readObjFile(char path[], int path_length, float** data, unsigned int* dataSize, int** indices, unsigned int* indexSize) {
    printf("here?");

    if (path == NULL) {
        printf("path is null...\n");
        return;
    }

    if (path_length < 0 || strlen(path) != path_length) {   //can remove 2nd conditional
        printf("err in length.\n");
        return;
    }

    if (strcmp((path + (path_length) - 3), "obj") != 0) {
        printf("did not provide obj file, returning.\n");
        return;
    }

    FileReader fReader;
    fReader.f_ptr = fopen(path, "r");
    memset(fReader.buff, 0, sizeof(fReader.buff));
    int response = 0;
    bool lineChange = false;
    //fgets(&character, sizeof(char), fReader.f_ptr);

    unsigned int vertex_count = 0;
    unsigned int vertex_normal_count = 0;
    unsigned int vertex_texture_count = 0;

    unsigned int face_count = 0;

    // first pass to simply check how many vertices, faces, textures, etc for mem alloc.
    while (fgets(fReader.buff, sizeof(fReader.buff)-1, fReader.f_ptr) != NULL) {
        //printf("Read in %255s", fReader.buff);
        ///*
        if (fReader.buff[strlen(fReader.buff) - 1] != '\n') {   // then buffer not big enough for the line
            printf("should not be longer than 254, returning.\n");
            break;
        }

        if (fReader.buff == NULL) { //just in case
            break;
        }
        ///*
        if (strlen(fReader.buff) > 2) {
            if (fReader.buff[0] == 'v') {
                if (fReader.buff[1] == 'n') {   //normal
                    vertex_normal_count++;
                } else if (fReader.buff[1] == 't') {   //texture
                    vertex_texture_count++;
                } else { //vertex
                    vertex_count++;
                }
            } else if (fReader.buff[0] == 'f') {
                int number_of_verts = 0;
                for (int k = 0; k < strlen(fReader.buff); k++) {
                    //printf("here at fReader.buff[%d] = %d\n", k, (int)fReader.buff[k]);
                    if (fReader.buff[k] == ' ') {
                        number_of_verts++;
                    }
                }
                if (number_of_verts > 4) {
                    printf("ERROR, can't support faces with more than 4 vertices!");
                    exit(1);
                    break;
                } else if (number_of_verts == 4) {
                    face_count += 2;    // add an extra face to triangulate
                    //printf("ever get here?\n");
                } else {    // 3 verts, (normal)
                    face_count++;
                }
            }   //otherwise ignore
        }
        //*/
    }

    //printf("here2!!!!!!!!!?");
    //exit(1);
    //fclose(fReader.f_ptr);

    bool hasNormals = false;
    bool hasTextures = false;
    // to parse with this format use below
    //sscanf(string, "%s %d %*[^0-9] %d %d %*[^0-9] %d %d %*[^0-9] %d %d %*[^0-9] %d", tmp, &(arr[i+0]), &(arr[i+1]), &(arr[i+2]), &(arr[i+3]), &(arr[i+4]), &(arr[i+5]), &(arr[i+6]), &(arr[i+7]));
    if (vertex_normal_count > 0) {  // we need to at least parse faces with "#//# #//# #//#" notation
        hasNormals = true;
    }
    if (vertex_texture_count > 0) { // we need to at least parse faces with "#/#/# #/#/# #/#/#" notation
        hasTextures = true;
        // worry about this later
    }

    int increment = 6; // say 6 anyway for fun
    increment = 8; // to include texture units

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

    memset(*indices, 0, sizeof(int) * face_count * 3);
    memset(*data, 0, sizeof(float) * vertex_count * increment);

    memset(vertex_textures, 0, sizeof(float) * vertex_texture_count * 2);   // clearing just in case
    //printf("we have %d space for data\n", vertex_count * increment);
    //printf("we have %d space for faces\n", face_count * 3);
    //exit(1);

    // 2nd pass to actually set the data merge, free, pass data to mesh type, then free again.
    char tmp[10];
    fReader.f_ptr = fopen(path, "r");
    while (fgets(fReader.buff, sizeof(fReader.buff)-1, fReader.f_ptr) != NULL) {
        //printf("Read in %254s\n", fReader.buff);
        if (fReader.buff[strlen(fReader.buff) - 1] != '\n') {   // then buffer not big enough for the line
            printf("should not be longer than 255, returning.\n");
            break;
        }

        if (fReader.buff[0] == 'v') {
            if (fReader.buff[1] == 'n') {   //normal
                sscanf((fReader.buff), "%2s %f %f %f", tmp, &(vertex_normals[vertex_normal_index+0]), &(vertex_normals[vertex_normal_index+1]), &(vertex_normals[vertex_normal_index+2]));
                vertex_normal_index += 3;
            } else if (fReader.buff[1] == 't') {   //texture
                sscanf((fReader.buff), "%2s %f %f", tmp, &(vertex_textures[vertex_texture_index+0]), &(vertex_textures[vertex_texture_index+1]));
                vertex_texture_index += 2;
            } else { //vertex
                sscanf((fReader.buff), "%1s %f %f %f", tmp, &(vertices[vertex_index+0]), &(vertices[vertex_index+1]), &(vertices[vertex_index+2]));
                //printf("\n%s", fReader.buff);
                //printf("vertices[%d+0] = %f\n", vertex_index, vertices[vertex_index+0]);
                vertex_index += 3;
            }
        } else if (fReader.buff[0] == 'f') {
            int counter = 0;
            int number_of_verts = 0;
            for (int k = 0; k < strlen(fReader.buff); k++) {
                //printf("here at fReader.buff[%d] = %d\n", k, (int)fReader.buff[k]);
                if (fReader.buff[k] == ' ') {
                    counter += 1;
                }
                if (counter >= 1) {
                    number_of_verts++;
                    counter = 0;
                }
            }
            if (number_of_verts > 4) {
                printf("ERROR, can't support faces with more than 4 vertices!");
                break;
            } else if (number_of_verts == 4) {
                if (!hasNormals) {
                    sscanf((fReader.buff), "%1s %d %d %d %*s", tmp, &(faces[face_index+0]), &(faces[face_index+1]), &(faces[face_index+2]));
                    face_index += 3;
                    sscanf((fReader.buff), "%1s %d %*s %d %d", tmp, &(faces[face_index+0]), &(faces[face_index+1]), &(faces[face_index+2]));
                } else {
                    sscanf(fReader.buff, "%1s %d %*[^0-9] %d %d %*[^0-9] %d %d %*[^0-9] %d %*s", tmp, &(faces[face_index+0]), &(face_normals[face_normal_index+0]), 
                    &(faces[face_index+1]), &(face_normals[face_normal_index+1]), &(faces[face_index+2]), &(face_normals[face_normal_index+2]));
                    face_index += 3;
                    face_normal_index += 3;
                    sscanf(fReader.buff, "%1s %d %*[^0-9] %d %*s %d %*[^0-9] %d %d %*[^0-9] %d", tmp, &(faces[face_index+0]), &(face_normals[face_normal_index+0]), 
                    &(faces[face_index+1]), &(face_normals[face_normal_index+1]), &(faces[face_index+2]), &(face_normals[face_normal_index+2]));
                }
            } else {    // 3 verts, (normal)
                if (!hasNormals) {
                    sscanf((fReader.buff), "%1s %d %d %d", tmp, &(faces[face_index+0]), &(faces[face_index+1]), &(faces[face_index+2]));
                } else {
                    sscanf(fReader.buff, "%1s %d %*[^0-9] %d %d %*[^0-9] %d %d %*[^0-9] %d", tmp, &(faces[face_index+0]), &(face_normals[face_normal_index+0]), 
                    &(faces[face_index+1]), &(face_normals[face_normal_index+1]), &(faces[face_index+2]), &(face_normals[face_normal_index+2]));
                }
            }
            //printf("\n%s", fReader.buff);
            //printf("faces[%d+0] = %d\n", face_index, faces[face_index+0]);

            //printf("last indexed values %d %d\n", face_index, face_normal_index);

            face_normal_index += 3;
            face_index += 3;
        }   //otherwise ignore
    }

    //printf("here3?\n");

    ///*
    int indexer = 0;    // need another 'i' since we only have 3 vals per vertex normal
    int indexer2 = 0;    // need another 'i' since we only have 2 vals per vertex texture
    for (unsigned int i = 0; i < vertex_count*increment; i += increment) {
        (*data)[i + 0] = vertices[indexer + 0];
        (*data)[i + 1] = vertices[indexer + 1];
        (*data)[i + 2] = vertices[indexer + 2];
        //printf("vertex %d, %f %f %f\n", indexer, (*data)[indexer+0], (*data)[indexer+1], (*data)[indexer+2]);
        if (hasNormals) {
            (*data)[i + 3] = vertex_normals[indexer + 0];   // technically we should do vertex_normals[face_normals[i + 0]]
            (*data)[i + 4] = vertex_normals[indexer + 1];   // but since not interating over faces we assume index is the same
            (*data)[i + 5] = vertex_normals[indexer + 2];
        }
        if (hasTextures) {
            (*data)[i + 6] = vertex_textures[indexer2 + 0];   // technically we should do vertex_normals[face_normals[i + 0]]
            (*data)[i + 7] = vertex_textures[indexer2 + 1];   // but since not interating over faces we assume index is the same
        }
        indexer += 3;
        indexer2+= 2;
    }
    *dataSize = (unsigned)vertex_count*increment;

    //printf("here4?\n");

    // calculate normals
    for (unsigned int i = 0; i < face_count*3; i += 3) {    //bc obj starts at 1
        //printf("face at at %d", faces[i + 2] - 1); 
        //exit(1);
        (*indices)[i + 0] = faces[i + 0] - 1;
        (*indices)[i + 1] = faces[i + 1] - 1;
        (*indices)[i + 2] = faces[i + 2] - 1;
        // we will assume that vertex normals have the same index as vertices
        //printf("lower indices[%d] = %d\n", i+0, (*indices)[i + 0]);
        if (!hasNormals) {
            //proposed method to calculate vertex normals.
            //facted_normal = normalize( cross( 2-3, 1-3))
            //printf("grabbing vertex at %d", (faces[i + 2] - 1)*3 + 0); 
            //printf(" with %f \n", vertices[((faces[i + 2] - 1)*3) + 0]);
            vec3 v3;
            v3[0] = vertices[((faces[i + 2] - 1)*3) + 0];
            v3[1] = vertices[((faces[i + 2] - 1)*3) + 1];
            v3[2] = vertices[((faces[i + 2] - 1)*3) + 2];

            vec3 v2;
            v2[0] = vertices[((faces[i + 1] - 1)*3) + 0];
            v2[1] = vertices[((faces[i + 1] - 1)*3) + 1];
            v2[2] = vertices[((faces[i + 1] - 1)*3) + 2];

            vec3 v1;
            v1[0] = vertices[((faces[i + 0] - 1)*3) + 0];
            v1[1] = vertices[((faces[i + 0] - 1)*3) + 1];
            v1[2] = vertices[((faces[i + 0] - 1)*3) + 2];

            vec3 res1;
            glm_vec3_sub(v2, v3, res1);
            vec3 res2;
            glm_vec3_sub(v1, v3, res2);
            vec3 res3;
            glm_vec3_crossn(res1, res2, res3); // res3 is our vertex normal
            //printf("moding data at %d\n", (faces[i + 0] - 1)*3 + 3);
            ///*
            //logic: "faces[i+#]-1" gets the vertex in the array
            //we then multiply by 3*2 since we have 6 floats (3 for verts, 3 for normals)
            //3*2==increment or (3*2 + 2) if we include textures
            //then we update the 3 verts shared by triangle. 
            (*data)[(faces[i + 0] - 1)*(increment) + 3] += res3[0];
            (*data)[(faces[i + 0] - 1)*(increment) + 4] += res3[1];
            (*data)[(faces[i + 0] - 1)*(increment) + 5] += res3[2];

            (*data)[(faces[i + 1] - 1)*(increment) + 3] += res3[0];
            (*data)[(faces[i + 1] - 1)*(increment) + 4] += res3[1];
            (*data)[(faces[i + 1] - 1)*(increment) + 5] += res3[2];

            (*data)[(faces[i + 2] - 1)*(increment) + 3] += res3[0];
            (*data)[(faces[i + 2] - 1)*(increment) + 4] += res3[1];
            (*data)[(faces[i + 2] - 1)*(increment) + 5] += res3[2];
            //*/
        } else {
            (*data)[(faces[i]-1) + 3] = vertex_normals[i + 0];
            (*data)[(faces[i]-1) + 4] = vertex_normals[i + 1];
            (*data)[(faces[i]-1) + 5] = vertex_normals[i + 2];
        }
        //printf("vertex at %d == %f %f %f\n", i, (*data)[i+0], (*data)[i+1], (*data)[i+2]);
    }
    *indexSize = (unsigned)face_count*3;

    for (unsigned int i = 0; i < vertex_count*increment; i += increment) {  //normalized all summed vectors
        vec3 normalized;
        normalized[0] = (*data)[i + 3];
        normalized[1] = (*data)[i + 4];
        normalized[2] = (*data)[i + 5];
        glm_vec3_normalize(normalized);
        (*data)[i + 3] = normalized[0];
        (*data)[i + 4] = normalized[1];
        (*data)[i + 5] = normalized[2];
        //printf("vertex %d, %f %f %f\n", i, (*data)[i+0], (*data)[i+1], (*data)[i+2]);
        //printf("vertex normal %d, %f %f %f\n", i, (*data)[i+3], (*data)[i+4], (*data)[i+5]);
    }

    //*/

    free(vertices);
    free(vertex_textures);
    free(vertex_normals);
    free(faces);

    fclose(fReader.f_ptr);
    //printf("end\n");
    //return vertices;
}