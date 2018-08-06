// Local Headers
#include "glitter.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include "scene.hpp"
#include "camera.hpp"
#include "util.hpp"

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

Mirage::Camera camera(glm::vec3(0.0f, 6.0f, 20.0f));
float lastX = mWidth / 2.0f;
float lastY = mHeight / 2.0f;
bool firstMouse = true;
bool topView = false;
bool keyPress = false;

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


	/* 
	 * Build and Compile Shaders
	 * -------------------------
	 */

	Mirage::Shader mainShader;
	mainShader.attach("main.vert").attach("main.frag");
	mainShader.link();

	Mirage::Shader trippyShader;
	trippyShader.attach("main.vert").attach("trippy.frag");
	trippyShader.link();
  
	// Load the mesh (collection of vertices) of each object
	Mirage::Mesh charmander("Charmander/Charmander.obj",// filename
							  & mainShader, // shader
							  glm::vec3(0.5f, 0.5f, 0.5f),// material specular
							  Mirage::ADS {0.2f, 0.5f, 100.0f }); // ambient, diffuse and shininess
	//charmander.setSubMeshShader("Texture_1", & trippyShader);
	Mirage::Mesh eevee("Eevee/Eevee.obj",// filename
							  & mainShader, // shader
							  glm::vec3(0.5f, 0.5f, 0.5f),// material specular
							  Mirage::ADS {0.2f, 0.5f, 100.0f }); // ambient, diffuse and shininess
	Mirage::Mesh mew("Mew/Mew.obj",// filename
							  & mainShader, // shader
							  glm::vec3(0.5f, 0.5f, 0.5f),// material specular
							  Mirage::ADS {0.2f, 0.5f, 100.0f }); // ambient, diffuse and shininess
	Mirage::Mesh pikachu("Pikachu/PikachuM.obj",// filename
							  & mainShader, // shader
							  glm::vec3(0.5f, 0.5f, 0.5f),// material specular
							  Mirage::ADS {0.2f, 0.5f, 100.0f }); // ambient, diffuse and shininess
	Mirage::Mesh superTrainingStage("Super Training Stage/Super Training Stage.obj",// filename
							  & mainShader, // shader
							  glm::vec3(0.5f, 0.5f, 0.5f),// material specular
							  Mirage::ADS {0.2f, 0.5f, 100.0f }); // ambient, diffuse and shininess
	Mirage::Mesh charizard("Charizard/CharizardMegaX.obj",// filename
							  & mainShader, // shader
							  glm::vec3(0.5f, 0.5f, 0.5f),// material specular
							  Mirage::ADS {0.2f, 0.5f, 100.0f }); // ambient, diffuse and shininess
	Mirage::Mesh clefaball("Clefaball/clefaball.obj",// filename
							  & mainShader, // shader
							  glm::vec3(0.5f, 0.5f, 0.5f),// material specular
							  Mirage::ADS {0.2f, 0.5f, 100.0f }); // ambient, diffuse and shininess
	Mirage::Mesh pokepyramid("Pokepyramid/pokepyramid.obj",// filename
							  & mainShader, // shader
							  glm::vec3(0.5f, 0.5f, 0.5f),// material specular
							  Mirage::ADS {0.2f, 0.2f, 3.0f }); // ambient, diffuse and shininess
	Mirage::Scene scene({& charmander, & eevee, & mew, & pikachu, & charizard, & superTrainingStage, & clefaball, & pokepyramid}, // meshes
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

	
	glfwSetCursorPos(mWindow, mWidth/2.0, mHeight/2.0);
	
    /* 
     * Rendering Loop
	 * --------------
	 *
	 * 	glfwWindowShouldClose(mWindow) return the value of "close" flag from the window
     */

	float tPikachu = 0.0f, tCharmander = 0.0f, tMew = 0.0f, rMew = 0.0f;
	bool pikachuDone = true, pikachuStart = true, charmanderDone = true, charmanderStart = false;

	glm::vec3 pikachu1P = glm::vec3(6.0f, 0.0f, 0.0f);
	glm::vec3 pikachu2P = glm::vec3(0.0f, 2.0f, -2.0f);
	glm::vec3 pikachu3P = glm::vec3(0.0f, 6.0f, 2.0f);
	glm::vec3 pikachu4P = glm::vec3(-4.0f, 2.0f, 0.0f);

	glm::vec3 charmander1P = glm::vec3(-10.0f, 0.0f, 0.0f);
	glm::vec3 charmander2P = glm::vec3(0.0f, 2.0f, -2.0f);
	glm::vec3 charmander3P = glm::vec3(0.0f, 6.0f, 2.0f);
	glm::vec3 charmander4P = glm::vec3(1.0f, 0.0f, 0.0f);

	glm::vec3 mew1P = glm::vec3(2.0f, 1.0f, -5.0f);
	glm::vec3 mew2P = glm::vec3(0.0f, 10.0f, -4.0f);
	glm::vec3 mew3P = glm::vec3(-2.0f, 3.0f, -3.0f);
	glm::vec3 mew4P = glm::vec3(-10.0f, 10.0f, 0.0f);

	glm::vec3 pikachuPos, charmanderPos, mewPos;

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

		if (pikachuStart) {
			if (!pikachuDone) {
				tPikachu += 0.01;
				if (tPikachu > 1) {
					tPikachu = 1;
					pikachuDone = true;
				}
			}
			else {
				tPikachu -= 0.01;
				if (tPikachu < 0) {
					tPikachu = 0;
					pikachuDone = false;
					charmanderStart = true;
					pikachuStart = false;
				}
			}
		}

		if (charmanderStart) {
			if (!charmanderDone) {
				tCharmander += 0.01;
				if (tCharmander > 1) {
					tCharmander = 1;
					charmanderDone = true;
				}
			}
			else {
				tCharmander -= 0.01;
				if (tCharmander < 0) {
					tCharmander = 0;
					charmanderDone = false;
					charmanderStart = false;
					pikachuStart = true;
				}
			}
		}

		if (tMew < 1 && pikachuStart) {
			tMew += 0.01;
		}

		bezier(pikachuPos, pikachu1P, pikachu2P, pikachu3P, pikachu4P, tPikachu);
		bezier(charmanderPos, charmander1P, charmander2P, charmander3P, charmander4P, tCharmander);
		bezier(mewPos, mew1P, mew2P, mew3P, mew4P, tMew);

		glm::mat4 charmanderModelMatrix = glm::translate(glm::mat4(1.0f), charmanderPos + glm::vec3(-charmander.getCenter().x, -charmander.getCenter().y, -charmander.getCenter().z + (-5.0f)));
		charmanderModelMatrix = glm::rotate(charmanderModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		charmanderModelMatrix = glm::rotate(charmanderModelMatrix, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		charmanderModelMatrix = glm::scale(charmanderModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
		//charmander.setSubMeshModelMatrix("Texture_1", glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));

		glm::mat4 eeveeModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-eevee.getCenter().x, -eevee.getCenter().y + abs(5.0f * sin(4.0f * tPikachu)), -eevee.getCenter().z + (-15.0f)));
		eeveeModelMatrix = glm::rotate(eeveeModelMatrix, glm::radians(90.0f), glm::vec3(-0.5f, 0.0f, 0.0f));
		eeveeModelMatrix = glm::scale(eeveeModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));

		glm::mat4 mewModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-mew.getCenter().x, -mew.getCenter().y, -mew.getCenter().z));
		if (tMew >= 1) {
			mewModelMatrix = glm::translate(mewModelMatrix, glm::vec3(charmanderPos.x + sin(rMew*M_PI/180)*4, mewPos.y, charmanderPos.z + (cos(rMew*M_PI/180)*4) - 5.0f));
			rMew -= 1.0f;
		}
		else {
			mewModelMatrix = glm::translate(mewModelMatrix, mewPos + glm::vec3(-mew.getCenter().x, -mew.getCenter().y, -mew.getCenter().z - 1.0f));
		}
		mewModelMatrix = glm::rotate(mewModelMatrix, glm::radians(90.0f), glm::vec3(-0.5f, 0.0f, 0.0f));
		mewModelMatrix = glm::scale(mewModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));

		glm::mat4 pikachuModelMatrix = glm::translate(glm::mat4(1.0f), pikachuPos + glm::vec3(-pikachu.getCenter().x, -pikachu.getCenter().y, -pikachu.getCenter().z + (-5.0f)));
		pikachuModelMatrix = glm::rotate(pikachuModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		pikachuModelMatrix = glm::rotate(pikachuModelMatrix, glm::radians(45.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		pikachuModelMatrix = glm::scale(pikachuModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));

		glm::mat4 superTrainingStageModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-superTrainingStage.getCenter().x, -superTrainingStage.getCenter().y, -superTrainingStage.getCenter().z + (-5.0f)));
		superTrainingStageModelMatrix = glm::scale(superTrainingStageModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
		superTrainingStageModelMatrix = glm::rotate(superTrainingStageModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 charizardModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(charizard.getCenter().x + (12.0f), charizard.getCenter().y + (2.0f), -charizard.getCenter().z + (-5.0f)));
		charizardModelMatrix = glm::rotate(charizardModelMatrix, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		charizardModelMatrix = glm::rotate(charizardModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		charizardModelMatrix = glm::scale(charizardModelMatrix, glm::vec3(0.07f, 0.07f, 0.07f));

		glm::mat4 clefaballModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(pikachuPos.x, 8.0f + sin(rMew*M_PI/25), -5.0f) + glm::vec3(-clefaball.getCenter().x, -clefaball.getCenter().y, -clefaball.getCenter().z));

		glm::mat4 pokepyramidModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-pokepyramid.getCenter().x, -pokepyramid.getCenter().y, -pokepyramid.getCenter().z));
		pokepyramidModelMatrix = glm::scale(pokepyramidModelMatrix, glm::vec3(5.0f));
		pokepyramidModelMatrix = glm::translate(pokepyramidModelMatrix, glm::vec3(0.0f, 1.0f, -6.0f));

		charmander.setModelMatrix(charmanderModelMatrix);
		eevee.setModelMatrix(eeveeModelMatrix);
		mew.setModelMatrix(mewModelMatrix);
		pikachu.setModelMatrix(pikachuModelMatrix);
		superTrainingStage.setModelMatrix(superTrainingStageModelMatrix);
		charizard.setModelMatrix(charizardModelMatrix);
		clefaball.setModelMatrix(clefaballModelMatrix);
		pokepyramid.setModelMatrix(pokepyramidModelMatrix);
		scene.draw();


        /*
         * Flip Buffers and Draw
         * ---------------------
         *
         * glfwSwapBuffers	-> swap the front and back buffers of the window
         * glfwPollEvents	-> process events that have already been received
         * glfwTerminate	-> destroy all remaining windows and cursors
         */ 
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
		//lastTime = glfwGetTime();
    }   glfwTerminate();
    return EXIT_SUCCESS;
}


/* 
 * processInput(GLFWwindow *window):
 * If the last state reported for the ESC key on this window is 
 * "GLFW_PRESS", set the "close" flag to true (terminate the loop)
 */
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { glfwSetWindowShouldClose(window, true); }
	
	if(!topView) {	

    	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { camera.processKeyboard(FORWARD, deltaTime); }
    	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { camera.processKeyboard(BACKWARD, deltaTime); }
    	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { camera.processKeyboard(LEFT, deltaTime); }
    	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { camera.processKeyboard(RIGHT, deltaTime); }
	}

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) { keyPress = true; }

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE && keyPress) {
	{
		if (!topView)
		{
			camera.setPosition(glm::vec3(0.0f, 40.0f, -5.0f));
			camera.setUp(glm::vec3(0.0f, 0.0f, -1.0f));
			camera.setFront(glm::vec3(0.0f, -1.0f, 0.0f));
			topView = true;
		}
		else
		{
			camera.setPosition(glm::vec3(0.0f, 6.0f, 20.0f));
			camera.setUp(glm::vec3(0.0f, 1.0f, 0.0f));
			camera.setFront(glm::vec3(0.0f, 0.0f, -1.0f));
			topView = false;
		}
		keyPress = false;
	}
	}
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
	if(!topView)
    	camera.processMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	std::cout << yoffset << std::endl;
    camera.processMouseScroll(yoffset);
}
