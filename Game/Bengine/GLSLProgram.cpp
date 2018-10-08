#include "stdafx.h"
#include "GLSLProgram.h"
#include "Errors.h"
#include <fstream>
#include <vector>

namespace Bengine {
	GLSLProgram::GLSLProgram() : _programID(0), _vertexShaderID(0), _fragmentShaderID(0) {}


	GLSLProgram::~GLSLProgram() {}

	//Initialize the shaders
	void GLSLProgram::compileShaders(std::string vertexShaderFilePath, std::string fragmentShaderFilePath) {
		_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (_vertexShaderID == 0) {
			fatalError("Vertex shader failed to initialize");
		}

		_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (_fragmentShaderID == 0) {
			fatalError("Fragment shader failed to initialize");
		}
		compileShader(vertexShaderFilePath, _vertexShaderID);
		compileShader(fragmentShaderFilePath, _fragmentShaderID);
	}

	//Connect the shaders together
	void GLSLProgram::linkShaders() {
		_programID = glCreateProgram();
		glAttachShader(_programID, _vertexShaderID);
		glAttachShader(_programID, _fragmentShaderID);

		glLinkProgram(_programID);
		GLuint isLinked = 0;
		glGetProgramiv(_programID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<char> infoLog(maxLength);
			glGetProgramInfoLog(_programID, maxLength, &maxLength, &infoLog[0]);
			glDeleteProgram(_programID);
			glDeleteShader(_vertexShaderID);
			glDeleteShader(_fragmentShaderID);
			std::printf("%s\n", &(infoLog[0]));
			fatalError("Shaders failed to link");
			return;
		}
		glDetachShader(_programID, _vertexShaderID);
		glDetachShader(_programID, _fragmentShaderID);
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);
	}

	void GLSLProgram::use() {
		glUseProgram(_programID);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
	}

	void GLSLProgram::unuse() {
		glUseProgram(0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}

	GLint GLSLProgram::getUniformLocation(const std::string & uniformName) {
		GLint location = glGetUniformLocation(_programID, uniformName.c_str());
		if (location == GL_INVALID_INDEX) {
			fatalError("Uniform " + uniformName + " not found in shader");
		}
		return location;
	}

	//Take the shader file and read it into a string to be compiled
	void GLSLProgram::compileShader(const std::string filePath, GLuint id) {
		std::ifstream vertexFile(filePath);
		if (vertexFile.fail()) {
			fatalError("Failed to open " + filePath);
		}
		std::string fileContents = "";
		std::string line;
		while (std::getline(vertexFile, line)) {
			fileContents += line + "\n";
		}
		vertexFile.close();
		const char* contentsPtr = fileContents.c_str();
		glShaderSource(id, 1, &contentsPtr, nullptr);
		glCompileShader(id);
		GLint success = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<char> errorLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);
			glDeleteShader(id);
			std::printf("%s\n", &(errorLog[0]));
			fatalError("Shader " + filePath + " failed to compile");
			return;
		}
	}
}