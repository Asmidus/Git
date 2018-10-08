#pragma once

#include <string>
#include <GL/glew.h>

namespace Bengine {
	class GLSLProgram {
	public:
		GLSLProgram();
		~GLSLProgram();

		void compileShaders(std::string vertexShaderFilePath, std::string fragmentShaderFilePath);
		void linkShaders();

		void use();
		void unuse();
		GLint getUniformLocation(const std::string& uniformName);
	private:
		void compileShader(const std::string filePath, GLuint id);
		GLuint _programID;
		GLuint _vertexShaderID;
		GLuint _fragmentShaderID;
	};
}

