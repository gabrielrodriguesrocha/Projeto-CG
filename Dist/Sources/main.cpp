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

// Object Vertices

float vertices[] = {
	// positions         // colors
	0.5f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
	0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
};

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
	//shader.activate();

	Mirage::Mesh mesh("bunny.obj");


    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
		processInput(mWindow);

        // Background Fill Color
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
		
		shader.activate();

		glm::mat4 model;
		model = glm::scale(model, glm::vec3(0.1f));

		glm::mat4 view (1.0f);
		view = glm::scale(view, glm::vec3(0.05f));

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), (float) mWidth / mHeight, 0.1f, 100.0f);

		int modelLoc = glGetUniformLocation(shader.get(), "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		int viewLoc = glGetUniformLocation(shader.get(), "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		int projectionLoc = glGetUniformLocation(shader.get(), "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		mesh.draw(shader.get());

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
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