#pragma once

// Local Headers
#include "mesh.hpp"

// System Headers
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

// Standard Headers
#include <map>
#include <memory>
#include <vector>

namespace Mirage
{
    class Scene 
    {
        public:
            // Default Constructors
            Scene() { }
            ~Scene() { }

            // Custom Constructors
            Scene(std::initializer_list<Mesh *> list);
            Scene(std::initializer_list<Mesh *> list,
                  glm::mat4 ViewMatrix,
                  glm::mat4 ProjectionMatrix,
                  glm::vec3 DirectionalLight,
                  glm::vec3 AmbientLightColor);

            // Public Methods
            void draw();

            // Getters and Setters
            void setViewMatrix(glm::mat4 m);
            void setProjectionMatrix(glm::mat4 m);
            void setDirectionalLight(glm::vec3 m);
            void setAmbientLightColor(glm::vec3 m);
            glm::mat4 getViewMatrix();
            glm::mat4 getProjectionMatrix();
            glm::vec3 getDirectionalLight();
            glm::vec3 getAmbientLightColor();

        private:
            std::vector<Mesh *> mObjects;
            glm::mat4 mViewMatrix;
            glm::mat4 mProjectionMatrix;
            glm::vec3 mDirectionalLight;
            glm::vec3 mAmbientLightColor;
    };
};