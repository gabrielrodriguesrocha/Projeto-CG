// Local Headers
#include "scene.hpp"

#include <iostream>
#include <glm/ext.hpp>

// Define Namespace
namespace Mirage
{
    Scene::Scene(std::initializer_list<Mesh *> list) : Scene() {
        /*for (auto i : list) {
            mObjects.push_back(i);
        }*/
        mObjects.insert(mObjects.end(), list.begin(), list.end());
    }
    
    Scene::Scene(std::initializer_list<Mesh *> list,
          glm::mat4 viewMatrix,
          glm::mat4 projectionMatrix,
          glm::vec3 directionalLight,
          glm::vec3 ambientLightColor) : Scene() {

        /*for (auto i : list) {
            mObjects.push_back(i);
        }*/
        mObjects.insert(mObjects.end(), list.begin(), list.end());
        mViewMatrix = viewMatrix;
        mProjectionMatrix = projectionMatrix;
        mDirectionalLight = directionalLight;
        mAmbientLightColor = ambientLightColor;
    }

    void Scene::draw() {

        for (auto i : mObjects) {

            glm::mat4 mModelMatrix = i->getModelMatrix();
            glm::mat3 mNormalMatrix = glm::inverse(glm::mat3(mViewMatrix * mModelMatrix));
            ADS ads = i->getADS();
            i->activateShader();

            int modelLoc = glGetUniformLocation(i->getShader(), "modelMatrix");
		    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mModelMatrix));

            int viewLoc = glGetUniformLocation(i->getShader(), "viewMatrix");
		    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mViewMatrix));
    
		    int projectionLoc = glGetUniformLocation(i->getShader(), "projectionMatrix");
		    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));

            int normalLoc = glGetUniformLocation(i->getShader(), "normalMatrix");
		    glUniformMatrix3fv(normalLoc, 1, GL_TRUE, glm::value_ptr(mNormalMatrix));

            int ambientLightColorU = glGetUniformLocation(i->getShader(), "ambientLightColor");
            glUniform3fv(ambientLightColorU, 1, glm::value_ptr(mAmbientLightColor));

		    int directionalLightU = glGetUniformLocation(i->getShader(), "directionalLight");
            glUniform3fv(directionalLightU, 1, glm::value_ptr(mDirectionalLight));

            int materialSpecularU = glGetUniformLocation(i->getShader(), "materialSpecular");
            glUniform3fv(materialSpecularU, 1, glm::value_ptr(i->getMaterialSpecular()));

		    int materialAmbientU = glGetUniformLocation(i->getShader(), "materialAmbient");
            glUniform1f(materialAmbientU, ads.ambient);

		    int materialDiffuseU = glGetUniformLocation(i->getShader(), "materialDiffuse");
            glUniform1f(materialDiffuseU, ads.diffuse);

		    int shininessU = glGetUniformLocation(i->getShader(), "shininess");
            glUniform1f(shininessU, ads.shininess);
            
            i->draw();
        }
    }

    void Scene::setViewMatrix(glm::mat4 m) {
        mViewMatrix = m;
    }

    void Scene::setProjectionMatrix(glm::mat4 m) {
        mProjectionMatrix = m;
    }

    void Scene::setDirectionalLight(glm::vec3 m) {
        mDirectionalLight = m;
    }

    void Scene::setAmbientLightColor(glm::vec3 m) {
        mAmbientLightColor = m;
    }

    glm::mat4 Scene::getViewMatrix() {
        return mViewMatrix;
    }

    glm::mat4 Scene::getProjectionMatrix() {
        return mProjectionMatrix;
    }

    glm::vec3 Scene::getDirectionalLight() {
        return mDirectionalLight;
    }

    glm::vec3 Scene::getAmbientLightColor() {
        return mAmbientLightColor;
    }
}
