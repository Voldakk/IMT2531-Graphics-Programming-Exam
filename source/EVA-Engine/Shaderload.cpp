/*
	Copied with some modifications from
	http://prod3.imt.hig.no/imt2531/imt2531_lectures/blob/master/lab04/include/Shaderload.h
*/

#include "Shaderload.hpp"

#include <vector>
#include <fstream>
#include <iostream>
#include <string>

#include <GL/glew.h>
#include "GLFW/glfw3.h"

const std::string SHADER_PATH = "./assets/shaders/";

void ShaderLoad::ReadShaderSource(const char *fname, std::vector<char> &buffer) 
{
	std::ifstream in;
	in.open(fname, std::ios::binary);

	if (!in.fail()) 
	{
		// Get the number of bytes stored in this file
		in.seekg(0, std::ios::end);
		const auto length = (size_t)in.tellg();

		// Go to start of the file
		in.seekg(0, std::ios::beg);

		// Read the content of the file in a buffer
		buffer.resize(length + 1);
		in.read(&buffer[0], length);
		in.close();
		// Add a valid C - string end
		buffer[length] = '\0';
	}
	else 
	{
		std::cerr << "Unable to open " << fname << " I'm out!" << std::endl;
		//std::cerr << "Unable to open " << fname << " I'm out!" << std::endl;
		exit(-1);
	}
}
GLuint ShaderLoad::LoadAndCompileShader(const char *fname, const GLenum shaderType) 
{
	std::cout << "ShaderLoad::LoadAndCompileShader - " << fname << "\n";

	// Load a shader from an external file
	std::vector<char> buffer;
	ReadShaderSource(fname, buffer);
	const char *src = &buffer[0];

	// Create shaders
	const auto shader = glCreateShader(shaderType);
	//attach the shader source code to the shader objec
	glShaderSource(shader, 1, &src, nullptr);

	// Compile the shader
	glCompileShader(shader);
	// Comile the shader, translates into internal representation and checks for errors.
	GLint compileOK;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileOK);
	if (!compileOK) 
	{
		char infolog[1024];;
		glGetShaderInfoLog(shader, 1024, nullptr, infolog);
		std::cout << "The program failed to compile with the error:" << std::endl << infolog << std::endl;
		glfwTerminate();
		getchar();
		exit(-1);
	}
	return shader;
}
GLuint ShaderLoad::CreateProgram(const char *pathVertShader, const char *pathFragShader) 
{
	// Load and compile the vertex and fragment shaders
	const auto vertexShader = LoadAndCompileShader((SHADER_PATH + pathVertShader).c_str(), GL_VERTEX_SHADER);
	const auto fragmentShader = LoadAndCompileShader((SHADER_PATH + pathFragShader).c_str(), GL_FRAGMENT_SHADER);

	// Create a program object and attach the two shaders we have compiled, the program object contains
	// both vertex and fragment shaders as well as information about uniforms and attributes common to both.
	const auto shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// Now that the fragment and vertex shader has been attached, we no longer need these two separate objects and should delete them.
	// The attachment to the shader program will keep them alive, as long as we keep the shaderProgram.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Link the different shaders that are bound to this program, this creates a final shader that 
	// we can use to render geometry with.
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	return shaderProgram;
}