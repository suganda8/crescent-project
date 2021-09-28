#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Shader.h"
#include"Texture.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"


using namespace std;

const unsigned int width = 800;
const unsigned int height = 800;

GLfloat vertices[] =
{
	// COORDINATES - COLORS - TEXTURE COORDINATES
	-0.5f,	0.0f,  0.5f,	0.83f, 0.70f, 0.44f,	0.0f, 0.0f,		// Lower left corner
	-0.5f,	0.0f, -0.5f,	0.83f, 0.70f, 0.44f,	5.0f, 0.0f,		// Upper left corner
	 0.5f,	0.0f, -0.5f,	0.83f, 0.70f, 0.44f,	0.0f, 0.0f,		// Upper right corner
	 0.5f,	0.0f,  0.5f,	0.83f, 0.70f, 0.44f,	5.0f, 0.0f,		// Lower right corner
	 0.0f,	0.8f,  0.0f,	0.92f, 0.86f, 0.76f,	2.5f, 5.0f,		// Lower right corner
};

GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};

int main()
{
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 4.6
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	// GLFW using Core (Modern OpenGL - not compatibility one)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Crescent", NULL, NULL);

	if (window == NULL)
	{
		cout << "Failed to create GLFW window." << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();

	// Specify the viewport of OpenGL in the window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);

	// Generates Shader object using shaders default.vert and default vert
	Shader shaderProgram("default.vert", "default.frag");

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generate Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generate Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	/*
	* I'm doing this relative path thing in order to centralize all the resources into one folder and not
	* duplicate them between tutorial folders. You can just copy paste the resources from the 'Resources'
	* folder and then give a relative path from this folder to whatever resource you want to get to.
	* Also note that this requires C++17, so go to Project Properties, C/C++, Language, and select C++17
	*/
	// Github
	// std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
	// std::string texPath = "/Resources/YoutubeOpenGL 6 - Textures/";

	// Texture
	// Texture popCat((parentDir + texPath + "pop_cat.png").c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	// popCat.texUnit(shaderProgram, "tex0", 0);


	// Original code from the tutorial
	// Texture
	Texture popCat("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	popCat.texUnit(shaderProgram, "tex0", 0);

	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	// Main Loop Event
	while (!glfwWindowShouldClose(window))
	{
		// Specify color of background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clear the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Swap the back buffer with the front buffer
		shaderProgram.Activate();

		camera.Inputs(window);
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

		// Binds texture so that is appears in rendering
		popCat.Bind();
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw the triangle using the GL_TRIANGLES primitive
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);

		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	popCat.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}