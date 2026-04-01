#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <vector>

#include "Engine/Graphics/Vertex.h"

using DirectX::SimpleMath::Vector4;
using DirectX::SimpleMath::Vector2;

class ModelImporter
{
public:
    static bool LoadModel(const std::string& filename,
        std::vector<Vertex>& outVertices,
        std::vector<uint32_t>& outIndices)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(filename,
            aiProcess_Triangulate |             // все полигоны в треугольники
            aiProcess_JoinIdenticalVertices |   // объединяет одинаковые вершины
            aiProcess_GenNormals |              // генерируем нормали, если их нет
            aiProcess_FlipUVs);                 // переворачиваем UV по Y (если нужно)

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "Assimp failed to load model: " << importer.GetErrorString() << std::endl;
            return false;
        }

        outVertices.clear();
        outIndices.clear();

        for (unsigned int m = 0; m < scene->mNumMeshes; ++m)
        {
            aiMesh* mesh = scene->mMeshes[m];

            for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
            {
                Vertex v;
                v.position = Vector4(
                    mesh->mVertices[i].x,
                    mesh->mVertices[i].y,
                    mesh->mVertices[i].z,
                    1.0f);

                // Vertex color
                if (mesh->HasVertexColors(0))
                {
                    v.color = Vector4(
                        mesh->mColors[0][i].r,
                        mesh->mColors[0][i].g,
                        mesh->mColors[0][i].b,
                        mesh->mColors[0][i].a);
                }
                else
                {
                    v.color = Vector4(1, 1, 1, 1);
                }

                // UV координаты
                if (mesh->HasTextureCoords(0))
                {
                    v.uv = Vector2(
                        mesh->mTextureCoords[0][i].x,
                        mesh->mTextureCoords[0][i].y);
                }
                else
                {
                    v.uv = Vector2(0, 0);
                }

                if (mesh->HasNormals())
                {
                    v.normal = Vector4(
                        mesh->mNormals[i].x,
                        mesh->mNormals[i].y,
                        mesh->mNormals[i].z,
                        0.0f);
                }
                else
                {
                    v.normal = Vector4(0, 1, 0, 0);
                }

                outVertices.push_back(v);
            }

            // Индексы
            for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
            {
                aiFace& face = mesh->mFaces[i];
                if (face.mNumIndices != 3) continue; // только треугольники
                outIndices.push_back(face.mIndices[0]);
                outIndices.push_back(face.mIndices[1]);
                outIndices.push_back(face.mIndices[2]);
            }
        }
        return true;
    }
};
