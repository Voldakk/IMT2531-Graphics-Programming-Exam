#pragma once

#include <vector>

#include <GL/glew.h>

// A helper class for reading and compiling shaders
class ShaderLoad
{
	public:
		/// <summary>Creates a shader program from the provided shader files</summary>
		/// <param name="path_vert_shader">The path to the vertex shader</param>  
		/// <param name="path_frag_shader">The path to the fragment shader</param>  
		/// <returns>A reference to the shader program</returns>  
		static GLuint CreateProgram(const char *path_vert_shader, const char *path_frag_shader);
	
	private:
		/// <summary>Reads the file and stores the content in the provided buffer</summary>
		/// <param name="fname">The path to the file</param>  
		/// <param name="buffer">The buffer the contents of the file should be read to</param>  
		static void ReadShaderSource(const char *fname, std::vector<char> &buffer);

		/// <summary></summary>
		/// <param name="fname">The path to the file</param>  
		/// <param name="shaderType">The type of shader to be compiled</param>  
		/// <returns>A refference to the complied shader</returns>  
		static GLuint LoadAndCompileShader(const char *fname, GLenum shaderType);
};