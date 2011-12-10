//============================================================================
// Name        : Sprite.h
// Author      : Max Foster
// Created on  : 30 May 2011
// Version     : 1.0
// Copyright   : http://creativecommons.org/licenses/by/3.0/
// Description : SuperMaximo GameLibrary Sprite class
//============================================================================

#ifndef SPRITE_H_
#define SPRITE_H_

#include <iostream>
#include <vector>
#include <SDL/SDL_image.h>
#include <GL/glew.h>

namespace SuperMaximo {

class Object;
class Shader;

typedef void (*customDrawFunctionType)(void*, Shader*, void*);

struct spriteDrawParams {
	int x, y;
	unsigned frame;
	float depth, rotation, xScale, yScale, alpha;
};

class Sprite {
	SDL_Surface * image;
	std::vector<GLuint> texture_;
	unsigned frames, vertices_, framerate_;
	int originX_, originY_;
	std::string name_;
	SDL_Rect rect;
	GLuint vao, vbo;
	Shader * boundShader_;
	customDrawFunctionType customDrawFunction;

	void initBuffer();

public:
	friend class Object;

	Sprite(const std::string & newName, const std::string & fileName, int imageX = 0, int imageY = 0, int imageWidth = 0,
			int imageHeight = 0, int aniFrames = 1, unsigned framerate = 1, int newOriginX = 0, int newOriginY = 0,
			void (*customBufferFunction)(GLuint*, Sprite*, void*) = NULL, void * customData = NULL);

	Sprite(const std::string & newName, SDL_Surface * surface, int imageX = 0, int imageY = 0, int imageWidth = 0,
			int imageHeight = 0, int aniFrames = 1, unsigned framerate = 1, int newOriginX = 0, int newOriginY = 0,
			void (*customBufferFunction)(GLuint*, Sprite*, void*) = NULL, void * customData = NULL);
	~Sprite();

	std::string name();

	unsigned frameCount();
	void setFramerate(unsigned newFramerate);
	unsigned framerate();

	void draw(int x, int y, float depth, float rotation = 0.0f, float xScale = 1.0f, float yScale = 1.0f,
			float alpha = 1.0f, unsigned frame = 1, Shader * shaderOverride = NULL,
			customDrawFunctionType customDrawFunctionOverride = NULL);

	void draw(Object * object);
	void defaultDraw(Shader * shaderToUse, spriteDrawParams * params);

	SDL_Surface * drawToSurface(float rotation = 0.0f, float xScale = 1.0f, float yScale = 1.0f,
			float alpha = 1.0f, unsigned frame = 1);

	int width();
	int height();
	int originX();
	int originY();

	SDL_Surface * surface();
	GLuint texture(unsigned frame);

	unsigned vertices();

	void bindShader(Shader * shader);
	Shader * boundShader();

	void bindCustomDrawFunction(customDrawFunctionType newCustomDrawFunction);
	customDrawFunctionType boundCustomDrawFunction();
};

}

#endif /* SPRITE_H_ */
