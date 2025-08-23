#ifndef TEXTURED_MESH_HPP
#define TEXTURED_MESH_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_ttf.h>

#include <fstream>
#include <string>
#include <vector>

#include "../math/Matrix44D.hpp"
#include "../math/Vector3D.hpp"
#include "../utils/StringUtils.hpp"
#include "Mesh.hpp"

using namespace std;
namespace GEngine {
class TexturedMesh : public Mesh {
   private:
    string texturePath;
    double *textureCoords;
    unsigned int textureId;

    const static unsigned char TEXTURE_LENGHT = 2;

   public:
    TexturedMesh(string meshPath, string texturePath) : Mesh(meshPath) {
        vertexCount = textureId = 0;
        this->texturePath = texturePath;
    }

    ~TexturedMesh() {
        delete[] textureCoords;
    }

    void Load() {
        string line;
        ifstream reader(this->meshPath);

        vector<double> tmpVertecies;
        vector<double> tmpNormals;
        vector<double> tmpTextCoords;

        vector<double> finalVertecies;
        vector<double> finalNormal;
        vector<double> finalText;

        if (reader.is_open()) {
            while (!reader.eof()) {
                reader >> line;
                switch (line[0]) {
                    case 'v':
                        if (line.length() > 1) {
                            if (line[1] == 't') {
                                for (int i = 0; i < TEXTURE_LENGHT; i++) {
                                    reader >> line;
                                    tmpTextCoords.push_back(stod(line));
                                }
                            } else {
                                for (int i = 0; i < NORMAL_LENGHT; i++) {
                                    reader >> line;
                                    tmpNormals.push_back(stod(line));
                                }
                            }
                        } else {
                            for (int i = 0; i < VERTEX_LENGHT; i++) {
                                reader >> line;
                                tmpVertecies.push_back(stod(line));
                            }
                        }
                        getline(reader, line);
                        break;
                    case 'f':
                        for (int i = 0; i < 3; i++) {
                            reader >> line;
                            string *t = StringUtils::Split(line, '/');

                            int index = stoi(t[0]) - 1;  // dans le .obj, le vertex 0 sera noté comme 1
                            for (int i = index * VERTEX_LENGHT; i < (index * VERTEX_LENGHT) + VERTEX_LENGHT; i++) {
                                finalVertecies.push_back(tmpVertecies[i]);
                            }

                            index = stod(t[1]) - 1;
                            for (int i = index * TEXTURE_LENGHT; i < (index * TEXTURE_LENGHT) + TEXTURE_LENGHT; i++) {
                                finalText.push_back(tmpTextCoords[i]);
                            }

                            index = stod(t[2]) - 1;
                            for (int i = index * NORMAL_LENGHT; i < (index * NORMAL_LENGHT) + NORMAL_LENGHT; i++) {
                                finalNormal.push_back(tmpNormals[i]);
                            }
                        }
                        break;
                    default:
                        getline(reader, line);
                        break;
                }
            }
            reader.close();
        }

        this->vertexCount = finalVertecies.size() / VERTEX_LENGHT;
        this->faceCount = this->vertexCount / VERTEX_LENGHT;
        this->verticies = new double[vertexCount * VERTEX_LENGHT];
        this->normals = new double[vertexCount * NORMAL_LENGHT];
        this->textureCoords = new double[vertexCount * TEXTURE_LENGHT];

        for (int i = 0; i < vertexCount * VERTEX_LENGHT; i++) {
            verticies[i] = finalVertecies[i];
        }
        for (int i = 0; i < vertexCount * NORMAL_LENGHT; i++) {
            normals[i] = finalNormal[i];
        }
        for (int i = 0; i < vertexCount * TEXTURE_LENGHT; i++) {
            textureCoords[i] = finalText[i];
        }
    }

    void Render(RenderConfig config) {
        if (textureId == 0) {
            glGenTextures(1, &textureId);
            glBindTexture(GL_TEXTURE_2D, textureId);
            SDL_Surface *sdlSurface = IMG_Load(texturePath.c_str());
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sdlSurface->w, sdlSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, sdlSurface->pixels);
            SDL_FreeSurface(sdlSurface);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glBindTexture(GL_TEXTURE_2D, textureId);
        glVertexPointer(3, GL_DOUBLE, 0, verticies);
        glNormalPointer(GL_DOUBLE, 0, normals);
        glTexCoordPointer(2, GL_DOUBLE, 0, textureCoords);

        if (config.drawLines) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        glDrawArrays(GL_TRIANGLES, 0, vertexCount);

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);

        if (config.drawLines) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
};
}  // namespace GEngine

#endif