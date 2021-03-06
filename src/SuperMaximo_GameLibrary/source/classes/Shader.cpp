//============================================================================
// Name        : Shader.cpp
// Author      : Max Foster
// Created on  : 31 May 2011
// Version     : 1.0
// Copyright   : http://creativecommons.org/licenses/by/3.0/
// Description : SuperMaximo GameLibrary OpenGL GLSL Shader class
//============================================================================

#include <iostream>
#include <vector>
#include <fstream>
#include <cstdarg>
using namespace std;

#include <GL/glew.h>

#include <SuperMaximo_GameLibrary/Display.h>
#include <SuperMaximo_GameLibrary/classes/Shader.h>

namespace SuperMaximo {

Shader::operator GLuint() const {
	return program_;
}

Shader::Shader(const string & name, const string & vertexShaderFile, const string & fragmentShaderFile, ...) :
		name_(name) {
	program_ = 0;
	for (int i = 0; i <= EXTRA9_LOCATION; i++) {
		uniformLocation_[i] = -1;
	}
	string text = "";
	ifstream file;
	file.open(vertexShaderFile.c_str());
	if (file.is_open()) {
		while (!file.eof()) {
			string tempStr;
			getline(file, tempStr);
			text += tempStr+"\n";
		}
		text += '\0';
		file.close();
	} else {
		cout << "Could not open file " << vertexShaderFile << endl;
		return;
	}

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLchar * arr[1];
	arr[0] = (GLchar *)text.c_str();
	glShaderSource(vertexShader, 1, (const GLchar **)arr, NULL);
	glCompileShader(vertexShader);
	int success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		cout << "Error with compiling vertex shader (" << vertexShaderFile << ")" << endl;
		char log[1024];
		glGetShaderInfoLog(vertexShader, 1024, NULL, log);
		cout << log << endl;
		glDeleteShader(vertexShader);
		return;
	}

	text = "";
	file.open(fragmentShaderFile.c_str());
	if (file.is_open()) {
		while (!file.eof()) {
			string tempStr;
			getline(file, tempStr);
			text += tempStr+"\n";
		}
		text += '\0';
		file.close();
	} else {
		glDeleteShader(vertexShader);
		cout << "Could not open file " << fragmentShaderFile << endl;
		return;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	arr[0] = (GLchar *)text.c_str();
	glShaderSource(fragmentShader, 1, (const GLchar **)arr, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		cout << "Error with compiling fragment shader (" << fragmentShaderFile << ")" << endl;
		char log[1024];
		glGetShaderInfoLog(fragmentShader, 1024, NULL, log);
		cout << log << endl;
		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);
		return;
	}

	program_ = glCreateProgram();
	glAttachShader(program_, vertexShader);
	glAttachShader(program_, fragmentShader);

	va_list attributes;
	va_start(attributes, fragmentShaderFile);
	int num = va_arg(attributes, int);
	for (int i = 0; i < num; i++) {
		int index = va_arg(attributes, int);
		char * attr = va_arg(attributes, char *);
		glBindAttribLocation(program_, index, attr);
	}
	va_end(attributes);

	glLinkProgram(program_);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glGetProgramiv(program_, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		cout << "Error with linking shader program" << endl;
		char log[1024];
		glGetProgramInfoLog(program_, 1024, NULL, log);
		cout << log << endl;
		glDeleteProgram(program_);
		return;
	}
}

Shader::Shader(const string & name, const string & vertexShaderFile, const string & fragmentShaderFile,
		const vector<int> & enums, const vector<char *> & attributeNames) : name_(name) {
	program_ = 0;
	for (short i = 0; i <= EXTRA9_LOCATION; i++) uniformLocation_[i] = -1;
	string text = "";
	ifstream file;
	file.open(vertexShaderFile.c_str());
	if (file.is_open()) {
		while (!file.eof()) {
			string tempStr;
			getline(file, tempStr);
			text += tempStr+"\n";
		}
		text += '\0';
		file.close();
	} else {
		cout << "Could not open file " << vertexShaderFile << endl;
		return;
	}

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLchar * arr[1];
	arr[0] = (GLchar *)text.c_str();
	glShaderSource(vertexShader, 1, (const GLchar **)arr, NULL);
	glCompileShader(vertexShader);
	int success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		cout << "Error with compiling vertex shader" << endl;
		char log[1024];
		glGetShaderInfoLog(vertexShader, 1024, NULL, log);
		cout << log << endl;
		glDeleteShader(vertexShader);
		return;
	}

	text = "";
	file.open(fragmentShaderFile.c_str());
	if (file.is_open()) {
		while (!file.eof()) {
			string tempStr;
			getline(file, tempStr);
			text += tempStr+"\n";
		}
		text += '\0';
		file.close();
	} else {
		glDeleteShader(vertexShader);
		cout << "Could not open file " << fragmentShaderFile << endl;
		return;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	arr[0] = (GLchar *)text.c_str();
	glShaderSource(fragmentShader, 1, (const GLchar **)arr, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		cout << "Error with compiling fragment shader" << endl;
		char log[1024];
		glGetShaderInfoLog(fragmentShader, 1024, NULL, log);
		cout << log << endl;
		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);
		return;
	}

	program_ = glCreateProgram();
	glAttachShader(program_, vertexShader);
	glAttachShader(program_, fragmentShader);

	for (unsigned i = 0; i < enums.size(); i++) glBindAttribLocation(program_, enums[i], attributeNames[i]);

	glLinkProgram(program_);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glGetProgramiv(program_, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		cout << "Error with linking shader program" << endl;
		char log[1024];
		glGetProgramInfoLog(program_, 1024, NULL, log);
		cout << log << endl;
		glDeleteProgram(program_);
		return;
	}
}

Shader::Shader(const string & name, const string & vertexShaderFile, const string & fragmentShaderFile,
		unsigned count, int * enums, const char ** attributeNames) : name_(name) {
	program_ = 0;
	for (short i = 0; i <= EXTRA9_LOCATION; i++) uniformLocation_[i] = -1;
	string text = "";
	ifstream file;
	file.open(vertexShaderFile.c_str());
	if (file.is_open()) {
		while (!file.eof()) {
			string tempStr;
			getline(file, tempStr);
			text += tempStr+"\n";
		}
		text += '\0';
		file.close();
	} else {
		cout << "Could not open file " << vertexShaderFile << endl;
		return;
	}

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLchar * arr[1];
	arr[0] = (GLchar *)text.c_str();
	glShaderSource(vertexShader, 1, (const GLchar **)arr, NULL);
	glCompileShader(vertexShader);
	int success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		cout << "Error with compiling vertex shader" << endl;
		char log[1024];
		glGetShaderInfoLog(vertexShader, 1024, NULL, log);
		cout << log << endl;
		glDeleteShader(vertexShader);
		return;
	}

	text = "";
	file.open(fragmentShaderFile.c_str());
	if (file.is_open()) {
		while (!file.eof()) {
			string tempStr;
			getline(file, tempStr);
			text += tempStr+"\n";
		}
		text += '\0';
		file.close();
	} else {
		glDeleteShader(vertexShader);
		cout << "Could not open file " << fragmentShaderFile << endl;
		return;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	arr[0] = (GLchar *)text.c_str();
	glShaderSource(fragmentShader, 1, (const GLchar **)arr, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		cout << "Error with compiling fragment shader" << endl;
		char log[1024];
		glGetShaderInfoLog(fragmentShader, 1024, NULL, log);
		cout << log << endl;
		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);
		return;
	}

	program_ = glCreateProgram();
	glAttachShader(program_, vertexShader);
	glAttachShader(program_, fragmentShader);

	for (unsigned i = 0; i < count; i++) glBindAttribLocation(program_, enums[i], attributeNames[i]);

	glLinkProgram(program_);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glGetProgramiv(program_, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		cout << "Error with linking shader program" << endl;
		char log[1024];
		glGetProgramInfoLog(program_, 1024, NULL, log);
		cout << log << endl;
		glDeleteProgram(program_);
		return;
	}
}

Shader::~Shader() {
	glDeleteProgram(program_);
}

const string & Shader::name() const {
	return name_;
}

void Shader::bind() {
	bindShader(this);
}

void Shader::use() const {
	glUseProgram(program_);
}

GLint Shader::setUniformLocation(shaderLocationEnum dstLocation, const string & locationName) {
	uniformLocation_[dstLocation] = glGetUniformLocation(program_, locationName.c_str());
	return uniformLocation_[dstLocation];
}

GLint Shader::uniformLocation(shaderLocationEnum location) const {
	return uniformLocation_[location];
}

void Shader::setUniform1(shaderLocationEnum location, GLfloat * data, unsigned count) const {
	glUniform1fv(uniformLocation_[location], count, data);
}

void Shader::setUniform2(shaderLocationEnum location, GLfloat * data, unsigned count) const {
	glUniform2fv(uniformLocation_[location], count, data);
}

void Shader::setUniform3(shaderLocationEnum location, GLfloat * data, unsigned count) const {
	glUniform3fv(uniformLocation_[location], count, data);
}

void Shader::setUniform4(shaderLocationEnum location, GLfloat * data, unsigned count) const {
	glUniform4fv(uniformLocation_[location], count, data);
}

void Shader::setUniform9(shaderLocationEnum location, GLfloat * data, unsigned count) const {
	glUniformMatrix3fv(uniformLocation_[location], count, GL_FALSE, data);
}

void Shader::setUniform16(shaderLocationEnum location, GLfloat * data, unsigned count) const {
	glUniformMatrix4fv(uniformLocation_[location], count, GL_FALSE, data);
}

void Shader::setUniform1(shaderLocationEnum location, int * data, unsigned count) const {
	glUniform1iv(uniformLocation_[location], count, data);
}

void Shader::setUniform2(shaderLocationEnum location, int * data, unsigned count) const {
	glUniform2iv(uniformLocation_[location], count, data);
}

void Shader::setUniform3(shaderLocationEnum location, int * data, unsigned count) const {
	glUniform3iv(uniformLocation_[location], count, data);
}

void Shader::setUniform4(shaderLocationEnum location, int * data, unsigned count) const {
	glUniform4iv(uniformLocation_[location], count, data);
}

void Shader::setUniform1(shaderLocationEnum location, GLfloat data) const {
	glUniform1f(uniformLocation_[location], data);
}

void Shader::setUniform2(shaderLocationEnum location, GLfloat data1, GLfloat data2) const {
	glUniform2f(uniformLocation_[location], data1, data2);
}

void Shader::setUniform3(shaderLocationEnum location, GLfloat data1, GLfloat data2, GLfloat data3) const {
	glUniform3f(uniformLocation_[location], data1, data2, data3);
}

void Shader::setUniform4(shaderLocationEnum location, GLfloat data1, GLfloat data2, GLfloat data3, GLfloat data4) const {
	glUniform4f(uniformLocation_[location], data1, data2, data3, data4);
}

void Shader::setUniform1(shaderLocationEnum location, int data) const {
	glUniform1i(uniformLocation_[location], data);
}

void Shader::setUniform2(shaderLocationEnum location, int data1, int data2) const {
	glUniform2i(uniformLocation_[location], data1, data2);
}

void Shader::setUniform3(shaderLocationEnum location, int data1, int data2, int data3) const {
	glUniform3i(uniformLocation_[location], data1, data2, data3);
}

void Shader::setUniform4(shaderLocationEnum location, int data1, int data2, int data3, int data4) const {
	glUniform4i(uniformLocation_[location], data1, data2, data3, data4);
}

void Shader::setUniform2(shaderLocationEnum location, const vec2 & data) const {
	glUniform2f(uniformLocation_[location], data.x, data.y);
}

void Shader::setUniform3(shaderLocationEnum location, const vec3 & data) const {
	glUniform3f(uniformLocation_[location], data.x, data.y, data.z);
}

void Shader::setUniform4(shaderLocationEnum location, const vec4 & data) const {
	glUniform4f(uniformLocation_[location], data.x, data.y, data.z, data.w);
}

}
