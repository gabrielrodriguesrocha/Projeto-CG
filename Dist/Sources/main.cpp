// Local Headers
#include "glitter.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include "scene.hpp"
#include "camera.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/ext.hpp>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <math.h>
#include <iostream>


void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

/*// position
glm::vec3 position = glm::vec3( 0, 0, 0 );
// horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// vertical angle : 0, look at the horizon
float verticalAngle = 0.0f;
float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;
double xpos, ypos;*/
Mirage::Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = mWidth / 2.0f;
float lastY = mHeight / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

int main(int argc, char * argv[]) {

    // Initiate the GLFW library
    glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    // tell GLFW to capture our mouse
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Load OpenGL
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));
    
    // Set the framebuffer size callback of the current window
	glfwSetFramebufferSizeCallback(mWindow, framebufferSizeCallback);

	glfwSetCursorPosCallback(mWindow, mouseCallback);
    glfwSetScrollCallback(mWindow, scrollCallback);


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
							  & stormtrooperShader, // shader
							  glm::vec3(0.5f, 0.5f, 0.5f), // material specular
							  Mirage::ADS {0.2f, 0.5f, 100.0f }); // ambient, diffuse and shininess
	Mirage::Mesh monkey("monkey.obj",// filename
							  & monkeyShader, // shader
							  glm::vec3(0.5f, 0.5f, 0.5f),// material specular
							  Mirage::ADS {0.2f, 0.5f, 100.0f }); // ambient, diffuse and shininess
	Mirage::Scene scene({& monkey, & stormtrooper}, // meshes
						glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f)), // view matrix
						glm::perspective(glm::radians(75.0f), (float) mWidth / mHeight, 0.1f, 100.0f), // projection matrix
						glm::vec3(-0.5f, 5.0f, 4.5f), // directional light
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
	glfwSetCursorPos(mWindow, mWidth/2.0, mHeight/2.0);
	
    /* Rendering Loop
	 * --------------
	 *
	 * glfwWindowShouldClose(mWindow) return the value of "close" flag from the window
     */
    while (glfwWindowShouldClose(mWindow) == false) {
		float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

    	// Process keyboard and mouse input
		processInput(mWindow);

		// Create viewMatrix from processed input
		scene.setViewMatrix(camera.getViewMatrix());

        /* 
         * glClearColor(1.0f, 1.0f, 1.0f, 1.0f);-> Specify clear values for the color buffers
         * glClear(GL_COLOR_BUFFER_BIT);		-> Clear the buffers enabled for color writing to preset value
         * glClear(GL_DEPTH_BUFFER_BIT);		-> Clear the depth buffer to preset value
         */
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		glm::mat4 stormtrooperModelMatrix = glm::translate( glm::mat4(), 
															glm::vec3(-stormtrooper.getCenter().x +(0.0f),
																	  -stormtrooper.getCenter().y +(0.0f), 
																	  -stormtrooper.getCenter().z +(-5.0f)));
		stormtrooperModelMatrix = glm::scale(stormtrooperModelMatrix, 
											 glm::vec3(glm::abs(sin(param*M_PI/180))+1, 
											 		   glm::abs(sin(param*M_PI/180))+1, 
													   glm::abs(sin(param*M_PI/180))+1));

		glm::mat4 monkeyModelMatrix = glm::translate(glm::mat4(), 
												     glm::vec3(-monkey.getCenter().x +(sin(param*M_PI/180)*4),
													 		   -monkey.getCenter().y + 0.0f, 
															   -monkey.getCenter().z +((cos(param*M_PI/180)*4)-5.0f)));
		
		monkeyModelMatrix = glm::rotate(monkeyModelMatrix,
										7.0f, 
										glm::vec3(0.0f, 1.0f, 0.0f));

		stormtrooper.setModelMatrix(stormtrooperModelMatrix);
		monkey.setModelMatrix(monkeyModelMatrix);
		scene.draw();

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
		param+=1;
		//lastTime = glfwGetTime();
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

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(RIGHT, deltaTime);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

	std::cout << xpos << " " << ypos << std::endl;

    camera.processMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.processMouseScroll(yoffset);
}