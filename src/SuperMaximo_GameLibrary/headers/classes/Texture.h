//============================================================================
// Name        : Texture.h
// Author      : Max Foster
// Created on  : 6 Jun 2011
// Version     : 1.0
// Copyright   : http://creativecommons.org/licenses/by/3.0/
// Description : SuperMaximo GameLibrary Texture class
//============================================================================

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <iostream>
#include <vector>
#include <GL/glew.h>

namespace SuperMaximo {

enum textureTypeEnum {
	TEXTURE_1D = GL_TEXTURE_1D,
	TEXTURE_2D = GL_TEXTURE_2D,
	TEXTURE_3D = GL_TEXTURE_3D,
	TEXTURE_RECTANGLE = GL_TEXTURE_RECTANGLE,
	TEXTURE_CUBE = GL_TEXTURE_CUBE_MAP
};

class Texture {
	std::string name_;
	GLuint texture;
	textureTypeEnum type_;
	int width_, height_;

public:
	operator GLuint();

	Texture(const std::string & newName, textureTypeEnum textureType, const std::string & fileName, ...);
	Texture(const std::string & newName, textureTypeEnum textureType, unsigned numLayers, ...);
	Texture(const std::string & newName, textureTypeEnum textureType, const std::vector<std::string> & fileNames);
	Texture(const std::string & newName, textureTypeEnum textureType, unsigned numLayers, std::string * fileNames);
	~Texture();

	void reload(textureTypeEnum textureType, const std::string & fileName, ...);
	void reload(textureTypeEnum textureType, unsigned numLayers, ...);
	void reload(textureTypeEnum textureType, const std::vector<std::string> & fileNames);
	void reload(textureTypeEnum textureType, unsigned numLayers, std::string * fileNames);

	std::string name();
	textureTypeEnum type();
	int width(), height();
};

}

#endif /* TEXTURE_H_ */
