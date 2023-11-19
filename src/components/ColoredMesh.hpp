#ifndef COLORED_MESH_HPP
#define COLORED_MESH_HPP

#include "../resources/Color.hpp"
#include "Mesh.hpp"

namespace GEngine {
class ColoredMesh : public Mesh {
   private:
    Color color;
    bool withLighting;

   public:
    ColoredMesh(string meshPath, Color color, bool withLighting = false) : Mesh(meshPath) {
        this->color = color;
        this->withLighting = withLighting;
    }

    void Load() {
        string line;
        ifstream reader(this->meshPath);

        vector<double> tmpVertecies;
        vector<double> tmpNormals;

        vector<double> finalVertecies;
        vector<double> finalNormal;

        if (reader.is_open()) {
            while (!reader.eof()) {
                reader >> line;
                switch (line[0]) {
                    case 'v':
                        if (line.length() > 1) {
                            if (line[1] != 't') {
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

        for (int i = 0; i < vertexCount * VERTEX_LENGHT; i++) {
            verticies[i] = finalVertecies[i];
        }
        for (int i = 0; i < vertexCount * NORMAL_LENGHT; i++) {
            normals[i] = finalNormal[i];
        }
    }

    void Render(RenderConfig handlerConfig) {
        RenderConfig config = this->configIsOverriden ? this->config : handlerConfig;
        glDisable(GL_TEXTURE_2D);
        // Activer transparence
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4d(color.GetR(), color.GetG(), color.GetB(), color.GetA());
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_DOUBLE, 0, verticies);
        if (config.drawLines) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        glDisableClientState(GL_VERTEX_ARRAY);
        glEnable(GL_TEXTURE_2D);
        if (config.drawLines) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        glColor4d(1.0, 1.0, 1.0, 1.0);
    }
};
}  // namespace GEngine

#endif
