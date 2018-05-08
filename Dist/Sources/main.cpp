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

	Mirage::Mesh stormtrooper("stormtrooper.obj");
	printf("%0.2f %0.2f %0.2f\n", (stormtrooper.getCenter()).x, (stormtrooper.getCenter()).y, (stormtrooper.getCenter()).z);
	Mirage::Mesh monkey("monkey.obj");
	printf("%0.2f %0.2f %0.2f\n", (monkey.getCenter()).x, (monkey.getCenter()).y, (monkey.getCenter()).z);


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

		// Model Matrices
		glm::mat4 modelStormtrooper, modelMonkey;
		modelStormtrooper = glm::scale(glm::translate(modelStormtrooper, glm::vec3(-stormtrooper.getCenter().x +(0.0f),-stormtrooper.getCenter().y +(0.0f), -stormtrooper.getCenter().z +(-5.0f))), glm::vec3(glm::abs(sin(param*M_PI/180))+1, glm::abs(sin(param*M_PI/180))+1, glm::abs(sin(param*M_PI/180))+1));
		modelMonkey = glm::rotate(glm::translate(modelMonkey, glm::vec3(-monkey.getCenter().x +(sin(param*M_PI/180)*4),-monkey.getCenter().y + 0.0f, -monkey.getCenter().z +((cos(param*M_PI/180)*4)-5.0f))), 7.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		
		// View Matrices
		glm::mat4 view;
		view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), // eye
						   glm::vec3(0.0f, 0.0f, -3.0f), // center
						   glm::vec3(0.0f, 1.0f, 0.0f));// up

		// Projection Matrices
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(75.0f), (float) mWidth / mHeight, 0.1f, 100.0f);

		// Normal Matrices
		glm::mat3 normalStormtrooper, normalMonkey;
		normalStormtrooper = glm::inverse(glm::mat3(view * modelStormtrooper)); 
		normalMonkey = glm::inverse(glm::mat3(view * modelMonkey));

		int modelLoc = glGetUniformLocation(shader.get(), "modelMatrix");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelStormtrooper));

		int viewLoc = glGetUniformLocation(shader.get(), "viewMatrix");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		int projectionLoc = glGetUniformLocation(shader.get(), "projectionMatrix");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		int normalLoc = glGetUniformLocation(shader.get(), "normalMatrix");
		glUniformMatrix3fv(normalLoc, 1, GL_TRUE, glm::value_ptr(normalStormtrooper));

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

		stormtrooper.draw(shader.get());

		modelLoc = glGetUniformLocation(shader.get(), "modelMatrix");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMonkey));

		normalLoc = glGetUniformLocation(shader.get(), "normalMatrix");
		glUniformMatrix3fv(normalLoc, 1, GL_TRUE, glm::value_ptr(normalMonkey));

		monkey.draw(shader.get());

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

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
