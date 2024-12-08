#include "renderer/Shape.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <linalg/linalg.h>
#include <tuple>
#include <map>

Shape::Shape(const char* shapeFile, bool loadByIndices) {
	this->loadFromOBJFile(shapeFile, loadByIndices);
}

void Shape::cleanup() {
	vertices.clear();
	indices.clear();
    
    // Unbind and delete buffers
	if (VBO) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &VBO);
    } 
    if (VAO) {
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &VAO);
    } 
    if (IBO) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &IBO);
    } 

    VBO = VAO = IBO = 0;
}

void Shape::loadToGPU() {    
    // Generate and bind Vertex Array Object (VAO)
	glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generate and bind Vertex Buffer Object (VBO)
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "Error during buffer data upload: " << error << "\n";
	}

    // Define vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

	error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "Error during VAO setup: " << error << "\n";
	}

    // Generate and bind Index Buffer Object (IBO)
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Shape::drawInstances(GLuint numInstances) {
    // Draw shape
    if (!indices.empty()) {
        glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0, numInstances);
    } else {
        glDrawArraysInstanced(GL_TRIANGLES, 0, vertices.size(), numInstances);
    }
}

// Loads a an object file to the shader. Loads by indices by default
void Shape::loadFromOBJFile(std::string filename, bool loadByIndices) {
	cleanup();

	if (loadByIndices)
		loadByIndexArray(filename);
	else
		loadByVertexArray(filename);
    
    loadToGPU();
}

void Shape::loadByIndexArray(std::string filename) {
	std::ifstream f(filename);
    if (!f.is_open()) {
        std::cerr << "Error opening object file: " << filename << "\n";
        return;
    }
    // Local caches for raw vertex data
    std::vector<Vec3> verts;  // Position vertices
    std::vector<Vec2> texs;   // Texture coordinates
    std::vector<Vec3> norms;  // Normals

    // Map to store unique vertices and their indices
    std::map<std::tuple<int, int, int>, GLushort> uniqueVertexMap; // {v, vt, vn} -> index
    GLuint currentIndex = 0;

    std::string line;
    while (std::getline(f, line)) {
        std::stringstream s(line);

        // Skip empty lines or comments
        if (line.empty() || line[0] == '#') continue;

        char junk;

        // Reading vertex data
        if (line[0] == 'v') {
            if (line[1] == 't') { // Texture coordinates (vt)
                Vec2 vt;
                s >> junk >> junk >> vt.u >> vt.v;
                texs.push_back(vt);
            } else if (line[1] == 'n') { // Normals (vn)
                Vec3 vn;
                s >> junk >> junk >> vn.x >> vn.y >> vn.z;
                norms.push_back(vn);
            } else { // Positions (v)
                Vec3 v;
                s >> junk >> v.x >> v.y >> v.z;
                verts.push_back(v);
            }
        }

        // Reading face data
        else if (line[0] == 'f') {
            s >> junk; // Skip 'f'
            std::string token;
            while (std::getline(s, token, ' ')) {
                if (token.empty()) continue;

                std::stringstream faceStream(token);
                std::string vIndexStr, vtIndexStr, vnIndexStr;

                // Parse the face component (v/vt/vn)
                std::getline(faceStream, vIndexStr, '/');
                std::getline(faceStream, vtIndexStr, '/');
                std::getline(faceStream, vnIndexStr, '/');

                // Extract indices (subtract 1 for zero-based indexing)
                int vIndex = vIndexStr.empty() ? -1 : std::stoi(vIndexStr) - 1;
                int vtIndex = vtIndexStr.empty() ? -1 : std::stoi(vtIndexStr) - 1;
                int vnIndex = vnIndexStr.empty() ? -1 : std::stoi(vnIndexStr) - 1;

                // Create a unique key for this vertex
                std::tuple<int, int, int> key = {vIndex, vtIndex, vnIndex};

                // Check if the vertex is already in the map
                if (uniqueVertexMap.find(key) == uniqueVertexMap.end()) {
                    // Add the new unique vertex
                    uniqueVertexMap[key] = currentIndex++;
                    Vertex vertex;

                    // Add the position, texture, and normal to the vertex
                    if (vIndex != -1) vertex.position = verts[vIndex];
                    if (vtIndex != -1) vertex.texCoord = texs[vtIndex];
                    if (vnIndex != -1) vertex.normal = norms[vnIndex];

                    // Store the vertex
                    vertices.push_back(vertex);
                }
	
                // Add the index to the indices array
                this->indices.push_back(uniqueVertexMap[key]);
            }
        }
    }

    f.close();
}

void Shape::loadByVertexArray(std::string filename) {
	std::ifstream f(filename);
    if (!f.is_open()) {
        std::cerr << "Error opening object file: " << filename << "\n";
        return;
    }
    // Local caches for verts, texs, and normals
    std::vector<Vec3> verts;
    std::vector<Vec2> texs;
    std::vector<Vec3> norms;

    std::string line;
    while (std::getline(f, line)) {
        std::stringstream s(line);

        // Skip empty lines or comments
        if (line.empty() || line[0] == '#') continue;

        char junk;
        
        // Reading vertices
        if (line[0] == 'v') {
            if (line[1] == 't') { // v/t (texture)
                Vec2 vt;
                s >> junk >> junk >> vt.u >> vt.v;
                texs.push_back(vt);
            } else if (line[1] == 'n') { // v//n (normal)
                Vec3 vn;
                s >> junk >> junk >> vn.x >> vn.y >> vn.z;
                norms.push_back(vn);
            } else { // v (position only)
                Vec3 v;
                s >> junk >> v.x >> v.y >> v.z;
                verts.push_back(v);
            }
        }

        // Processing face definitions
        else if (line[0] == 'f') {
			s >> junk;
            std::string token;
            while (getline(s, token, ' ')) {
				if (token.empty()) continue;

                std::stringstream faceStream(token);
                std::string vIndexStr, vtIndexStr, vnIndexStr;
				
                std::getline(faceStream, vIndexStr, '/');
                std::getline(faceStream, vtIndexStr, '/');
                std::getline(faceStream, vnIndexStr, ' ');

                // Extract indices (subtract 1 for zero-based indexing)
                int vIndex = vIndexStr.empty() ? -1 : std::stoi(vIndexStr) - 1;
                int vtIndex = vtIndexStr.empty() ? -1 : std::stoi(vtIndexStr) - 1;
                int vnIndex = vnIndexStr.empty() ? -1 : std::stoi(vnIndexStr) - 1;

                // Create Vertex based on the number of components available
                if (vtIndex != -1 && vnIndex != -1) {
                    vertices.push_back(Vertex(verts[vIndex], texs[vtIndex], norms[vnIndex]));
                } else if (vtIndex != -1) {
                    vertices.push_back(Vertex(verts[vIndex], texs[vtIndex]));
                } else if (vnIndex != -1) {
                    vertices.push_back(Vertex(verts[vIndex], Vec2(0,0), norms[vnIndex]));
                } else {
                    vertices.push_back(Vertex(verts[vIndex]));
                }
            }
        }
    }

    f.close();
}