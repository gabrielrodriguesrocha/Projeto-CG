// Local Headers
#include "glitter.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include "scene.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <math.h>
#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main(int argc, char * argv[]) {

    // Initiate the GLFW library
    glfwInit();

	/* Create window with:
	 * Width (int) = mWidth
	 * Height (int) = mHeight
	 * Title (const char*) = "OpenGL"
	 * Monitor (GLFWmonitor *) = nullptr
	 * Share (GLFWmonitor *) = nullptr
	 */
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

    // Create Context for the current window
    glfwMakeContextCurrent(mWindow);
    
    // Load OpenGL
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));
    
    // Set the framebuffer size callback of the current window
	glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);


	/* Build and Compile Shaders
	 * -------------------------
	 */
	Mirage::Shader monkeyShader;
	monkeyShader.attach("main.vert").attach("main.frag");
	monkeyShader.link();
	Mirage::Shader stormtrooperShader;
	stormtrooperShader.attach("main.vert").attach("trippy.frag");
	stormtrooperShader.link();

	        
    

	// Load the mesh (collection of vertices) of each object
	Mirage::Mesh stormtrooper("stormtrooper.obj", // filename
							  & monkeyShader, // shader
							  glm::vec3(0.5f, 0.5f, 0.5f), // material specular
							  Mirage::ADS {0.2f, 0.5f, 100.0f }); // ambient, diffuse and shininess

	Mirage::Mesh monkey("monkey.obj",// filename
							  & monkeyShader, // shader
							  glm::vec3(0.5f, 0.5f, 0.5f),// material specular
							  Mirage::ADS {0.2f, 0.5f, 100.0f }); // ambient, diffuse and shininess

	Mirage::Scene scene({& stormtrooper, & monkey}, // meshes
						glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f)), // view matrix
						glm::perspective(glm::radians(75.0f), (float) mWidth / mHeight, 0.1f, 100.0f), // projection matrix
						glm::vec3(0.5f, 3.0f, -3.0f), // directional light
						glm::vec3(0.2f, 0.2f, 0.2f)); // ambient light colour

	
	/* 
	 * glEnable(GL_DEPTH_TEST); -> Do depth comparisons and update the depth buffer
	 * glEnable(GL_CULL_FACE); 	-> Cull polygons based on their winding in window coordinates
	 * glCullFace(GL_BACK);		-> Back-facing polygons are candidates for culling
	 */
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	
	int param = 0;
	
    /* Rendering Loop
	 * --------------
	 *
	 * glfwWindowShouldClose(mWindow) return the value of "close" flag from the window
     */
    while (glfwWindowShouldClose(mWindow) == false) {
    	// Process keyboard and mouse input
		processInput(mWindow);

        /* 
         * glClearColor(1.0f, 1.0f, 1.0f, 1.0f);-> Specify clear values for the color buffers
         * glClear(GL_COLOR_BUFFER_BIT);		-> Clear the buffers enabled for color writing to preset value
         * glClear(GL_DEPTH_BUFFER_BIT);		-> Clear the depth buffer to preset value
         */
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		stormtrooper.setModelMatrix(glm::scale(glm::translate(glm::mat4(), glm::vec3(-stormtrooper.getCenter().x +(0.0f),-stormtrooper.getCenter().y +(0.0f), -stormtrooper.getCenter().z +(-5.0f))), glm::vec3(glm::abs(sin(param*M_PI/180))+1, glm::abs(sin(param*M_PI/180))+1, glm::abs(sin(param*M_PI/180))+1)));	
		monkey.setModelMatrix(glm::rotate(glm::translate(glm::mat4(), glm::vec3(-monkey.getCenter().x +(sin(param*M_PI/180)*4),-monkey.getCenter().y + 0.0f, -monkey.getCenter().z +((cos(param*M_PI/180)*4)-5.0f))), 7.0f, glm::vec3(0.0f, 1.0f, 0.0f)));

		scene.draw();

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
		param+=1;
    }   glfwTerminate();
    return EXIT_SUCCESS;
}


/* processInput(GLFWwindow *window)
 * If the last state reported for the ESC key on this window is 
 * "GLFW_PRESS", set the "close" flag to true (terminate the loop)
 */
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
