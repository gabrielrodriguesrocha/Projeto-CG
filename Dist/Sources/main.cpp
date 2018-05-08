// Local Headers
#include "glitter.hpp"
#include "shader.hpp"
#include "mesh.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <math.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main(int argc, char * argv[]) {

    // Load GLFW and Create a Window
    glfwInit();

    /*
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	*/
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));
	glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);

	// Build and Compile Shaders
	// -------------
	Mirage::Shader shader;
	shader.attach("main.vert").attach("main.frag");
	shader.link();

	Mirage::Mesh mesh("stormtrooper.obj");
	printf("%0.2f %0.2f %0.2f\n", (mesh.getCenter()).x, (mesh.getCenter()).y, (mesh.getCenter()).z);
	Mirage::Mesh mesh2("monkey.obj");
	printf("%0.2f %0.2f %0.2f\n", (mesh2.getCenter()).x, (mesh2.getCenter()).y, (mesh2.getCenter()).z);


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	int param = 0;
    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
		processInput(mWindow);

        // Background Fill Color
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		//newvalue = sin(param*M_PI/180)*10)
		shader.activate();
		glm::mat4 model, model2;
		model = glm::translate(model, glm::vec3(-mesh.getCenter().x +(0.0f),-mesh.getCenter().y +(0.0f), -mesh.getCenter().z +(-5.0f)));
		model2 = glm::translate(model2, glm::vec3(-mesh2.getCenter().x +(sin(param*M_PI/180)*4),-mesh2.getCenter().y +(0.0f), -mesh2.getCenter().z +((cos(param*M_PI/180)*4)-5.0f)));;
		
		glm::mat4 view;
		view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), // eye
						   glm::vec3(0.0f, 0.0f, -3.0f), // center
						   glm::vec3(0.0f, 1.0f, 0.0f));// up

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(75.0f), (float) mWidth / mHeight, 0.1f, 100.0f);

		glm::mat3 normal, normal2;
		normal = glm::inverse(glm::mat3(view * model)); 
		normal2 = glm::inverse(glm::mat3(view * model2));

		int modelLoc = glGetUniformLocation(shader.get(), "modelMatrix");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		int viewLoc = glGetUniformLocation(shader.get(), "viewMatrix");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		int projectionLoc = glGetUniformLocation(shader.get(), "projectionMatrix");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		int normalLoc = glGetUniformLocation(shader.get(), "normalMatrix");
		glUniformMatrix3fv(normalLoc, 1, GL_TRUE, glm::value_ptr(normal));

		int ambientLightColorU = glGetUniformLocation(shader.get(), "ambientLightColor");
		int directionalLightU = glGetUniformLocation(shader.get(), "directionalLight");
		int materialSpecularU = glGetUniformLocation(shader.get(), "materialSpecular");
		int materialAmbientU = glGetUniformLocation(shader.get(), "materialAmbient");
		int materialDiffuseU = glGetUniformLocation(shader.get(), "materialDiffuse");
		int shininessU = glGetUniformLocation(shader.get(), "shininess");

		glm::vec3 ambientLightColor = glm::vec3(0.2f, 0.2f, 0.2f);

		glm::vec3 directionalLight = glm::vec3(-0.5f, 0.5f, 0.5f);

		glm::vec3 materialSpecular = glm::vec3(0.5f, 0.5f, 0.5f);

		float ambient = 0.2f;
		float diffuse = 0.5f;
		float shininess = 100.0f;

		glUniform3fv(ambientLightColorU, 1, glm::value_ptr(ambientLightColor));
		glUniform3fv(directionalLightU, 1, glm::value_ptr(directionalLight));
		glUniform3fv(materialSpecularU, 1, glm::value_ptr(materialSpecular));
		
		glUniform1f(materialAmbientU, ambient);
		glUniform1f(materialDiffuseU, diffuse);
		glUniform1f(shininessU, shininess);

		mesh.draw(shader.get());

		modelLoc = glGetUniformLocation(shader.get(), "modelMatrix");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));

		normalLoc = glGetUniformLocation(shader.get(), "normalMatrix");
		glUniformMatrix3fv(normalLoc, 1, GL_TRUE, glm::value_ptr(normal2));

		mesh2.draw(shader.get());

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
		param+=1;
    }   glfwTerminate();
    return EXIT_SUCCESS;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
