#pragma once

// System Headers
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <shader.hpp>

// Standard Headers
#include <map>
#include <memory>
#include <vector>




// Define Namespace
namespace Mirage
{
    // Vertex Format
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
    };

    struct Point{
	    float x, y, z;
    };

    struct ADS {
        float ambient, diffuse, shininess;
    };

    class Mesh
    {
    public:

        // Implement Default Constructor and Destructor
         Mesh() { glGenVertexArrays(1, & mVertexArray); }
        ~Mesh() { glDeleteVertexArrays(1, & mVertexArray); }

        // Implement Custom Constructors
        Mesh(std::string const & filename);
        Mesh(std::string const & filename,
             Shader * shader,
             glm::vec3 materialSpecular,
             ADS ads);
        Mesh(std::vector<Vertex> const & vertices,
             std::vector<GLuint> const & indices,
             std::map<GLuint, std::string> const & textures);

        // Public Member Functions
        void draw();
        void draw(GLuint shader);

	    
        void setModelMatrix(glm::mat4 m);
        void setMaterialSpecular(glm::vec3 m);
        void setADS(ADS m);
        void setShader (Shader * shader);

        void activateShader ();

        glm::mat4 getModelMatrix();
        glm::vec3 getMaterialSpecular();
        ADS getADS();
        GLuint getShader ();
        Point getCenter();

    private:

        // Disable Copying and Assignment
        Mesh(Mesh const &) = delete;
        Mesh & operator=(Mesh const &) = delete;

        // Private Member Functions
        void parse(std::string const & path, aiNode const * node, aiScene const * scene);
        void parse(std::string const & path, aiMesh const * mesh, aiScene const * scene);
        std::map<GLuint, std::string> process(std::string const & path,
                                              aiMaterial * material,
                                              aiTextureType type);

        // Private Member Containers
        std::vector<std::unique_ptr<Mesh>> mSubMeshes;
        std::vector<GLuint> mIndices;
        std::vector<Vertex> mVertices;
        std::map<GLuint, std::string> mTextures;

        // Private Member Variables
        GLuint mVertexArray;
        GLuint mVertexBuffer;
        GLuint mElementBuffer;

        glm::mat4 mModelMatrix;
        glm::vec3 mMaterialSpecular;
        ADS mADS;

        Point mCenter;

        Shader * mShader;

    };
};
