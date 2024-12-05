#include "graphics/ShapeData.h"
#include <iostream>
#include <fstream>
#include <strstream>
#include <linalg/linalg.h>

ShapeData::ShapeData(const char* shapeFile) {
	this->loadFromOBJFile(shapeFile, true);
	this->sendToGPU();
}

void ShapeData::setColor(float r, float g, float b) {
    // Update the color of all vertices
    for (auto& v : this->vertices) {
        v.color.r = r;
        v.color.g = g;
        v.color.b = b;
    }

    // Bind the VBO again and update only the color data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertex) * 0, numVertices * sizeof(Vertex), vertices.data());
}

void ShapeData::cleanup() {
	vertices.clear();
	indicies.clear();
	numIndicies = numVertices = 0;

	if (VBO) glDeleteBuffers(1, &VBO);
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (IBO) glDeleteBuffers(1, &IBO);

    VBO = VAO = IBO = 0;
}

void ShapeData::sendToGPU() {
	// Generate and bind Vertex Array Object (VAO)
	glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generate and bind Vertex Buffer Object (VBO)
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

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

    // Load indicies
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndicies * sizeof(GLushort), indicies.data(), GL_STATIC_DRAW);

    glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBindVertexArray(0);
}

void ShapeData::draw(GLuint programID, Mat4x4 matFullTransform) {
	GLint uniformLocation = glGetUniformLocation(programID, "matFullTransform");
	if (uniformLocation == -1) {
		std::cerr << "Uniform 'matFullTransform' not found in shader program\n";
	} else {
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &matFullTransform[0][0]);
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			std::cerr << "Error while setting uniform 'matFullTransform': " << error << "\n";
		}
	}

    glBindVertexArray(VAO);

    if (numIndicies) {
		glDrawElements(GL_TRIANGLES, numIndicies, GL_UNSIGNED_SHORT, 0);
	} else { 
        glDrawArrays(GL_TRIANGLES, 0, numVertices);
	}
}

void ShapeData::loadFromOBJFile(std::string fileName, bool hasTexture) {
    std::ifstream f(fileName);
	if (!f.is_open()) {
		std::cout << stderr << "Error opening object file.\n";
	}

	cleanup();

	// Local cache of verts
	std::vector<Vec3> verts;
	std::vector<Vec2> texs;

	while (!f.eof()) {
		char line[128];
		f.getline(line, 128);

		std::strstream s;
		s << line;

		char junk;

		if (line[0] == 'v') {
			if (line[1] == 't') {
				Vec2 vt;
				s >> junk >> junk >> vt.u >> vt.v;
				texs.push_back(vt);
			} else {
				Vec3 v;
				s >> junk >> v.x >> v.y >> v.z;
				verts.push_back(v);
			}
		}
		if (!hasTexture) {
			if (line[0] == 'f') {
				int f[3];
				s >> junk >> f[0] >> f[1] >> f[2];
				vertices.push_back(Vertex(verts[f[0] - 1]));  
                vertices.push_back(Vertex(verts[f[1] - 1]));
                vertices.push_back(Vertex(verts[f[2] - 1]));
			}
		} else {
			if (line[0] == 'f') {
				s >> junk;

				std::string tokens[6];
				int nTokenCount = -1;

				while (!s.eof()) {
					char c = s.get();
					if (c == ' ' || c == '/')
						nTokenCount++;
					else
						tokens[nTokenCount].append(1, c);
				}
                    
				tokens[nTokenCount].pop_back();
                
				vertices.push_back(Vertex(verts[stoi(tokens[0]) - 1], texs[stoi(tokens[1]) - 1]));
                vertices.push_back(Vertex(verts[stoi(tokens[2]) - 1], texs[stoi(tokens[3]) - 1]));
                vertices.push_back(Vertex(verts[stoi(tokens[4]) - 1], texs[stoi(tokens[5]) - 1]));
			}
		}
	}

    numVertices = vertices.size();
}