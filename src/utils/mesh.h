#ifndef MESH_H
#define MESH_H

struct Mesh {
	//  render data
	unsigned int VAO, VBO, EBO;
	// mesh data
	struct Vertex vertices[];
	unsigned int indices[];
	struct Texture textures[];
};

struct Vertex {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    char* type;
};

//don't have classes here (:
//Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
//void Draw(Shader &shader);
void setupMesh();

#endif
