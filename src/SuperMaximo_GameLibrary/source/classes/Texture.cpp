//============================================================================
// Name        : Texture.cpp
// Author      : Max Foster
// Created on  : 6 Jun 2011
// Version     : 1.0
// Copyright   : http://creativecommons.org/licenses/by/3.0/
// Description : SuperMaximo GameLibrary Texture class
//============================================================================

#include <iostream>
#include <vector>
using namespace std;

#include <GL/glew.h>
#include <SDL/SDL_image.h>

#include <SuperMaximo_GameLibrary/Display.h>
#include <SuperMaximo_GameLibrary/classes/Texture.h>
using namespace SuperMaximo;

namespace SuperMaximo {

Texture::operator GLuint() const {
	return texture;
}

Texture::Texture(const string & name, textureTypeEnum textureType, const string & fileName, ...) : name_(name) {
	vector<string> fileNames;
	fileNames.push_back(fileName);
	va_list files;
	va_start(files, fileName);
	for (short i = 0; i < 4; i++) {
		char * file = va_arg(files, char *);
		fileNames.push_back(file);
	}
	reload(textureType, fileNames);
}

Texture::Texture(const string & name, textureTypeEnum textureType, unsigned numLayers, ...) : name_(name) {
	vector<string> fileNames;
	va_list files;
	va_start(files, numLayers);
	for (unsigned i = 0; i < numLayers; i++) {
		char * file = va_arg(files, char *);
		fileNames.push_back(file);
	}
	reload(textureType, fileNames);
}

Texture::Texture(const string & name, textureTypeEnum textureType, const vector<string> & fileNames) : name_(name) {
	reload(textureType, fileNames);
}

Texture::Texture(const string & name, textureTypeEnum textureType, unsigned numLayers, string * fileNames) : name_(name) {
	reload(textureType, numLayers, fileNames);
}

Texture::~Texture() {
	glDeleteTextures(1, &texture);
}

void Texture::reload(textureTypeEnum textureType, const string & fileName, ...) {
	type_ = textureType;
	if (textureType == TEXTURE_3D) cout << "Cannot create a 3D texture with the arguments given" << endl; else {
		SDL_Surface * image = IMG_Load(fileName.c_str());
		if (image == NULL) cout << "Could not load image " << fileName << endl; else {
			width_ = image->w;
			height_ = image->h;
			GLenum textureFormat;
			if (image->format->BytesPerPixel == 4) {
				if (image->format->Rmask == 0x000000ff) textureFormat = GL_RGBA; else textureFormat = GL_BGRA;
			} else {
				if (image->format->Rmask == 0x000000ff) textureFormat = GL_RGB; else textureFormat = GL_BGR;
			}
			glGenTextures(1, &texture);
			glBindTexture(textureType, texture);
			glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			if (textureType != TEXTURE_CUBE) {
				glTexImage2D(textureType, 0, image->format->BytesPerPixel, image->w, image->h, 0, textureFormat,
						GL_UNSIGNED_BYTE, image->pixels);
				SDL_FreeSurface(image);
			} else {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, image->format->BytesPerPixel, image->w, image->h, 0,
						textureFormat, GL_UNSIGNED_BYTE, image->pixels);
				SDL_FreeSurface(image);

				GLenum sides[5] = {GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
						GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z};
				va_list files;
				va_start(files, fileName);
				for (short i = 0; i < 4; i++) {
					char * file = va_arg(files, char *);
					image = IMG_Load(file);
					if (image == NULL) cout << "Could not load image " << file << endl; else {
						glTexImage2D(sides[i], 0, image->format->BytesPerPixel, image->w, image->h, 0, textureFormat,
								GL_UNSIGNED_BYTE, image->pixels);
						SDL_FreeSurface(image);
					}
				}
				va_end(files);
			}
			glBindTexture(textureType, 0);
		}
	}
}

void Texture::reload(textureTypeEnum textureType, unsigned numLayers, ...) {
	type_ = textureType;
	if (textureType == TEXTURE_3D) {
		glGenTextures(1, &texture);
		if (texture2dArrayDisabled()) {
			textureType = TEXTURE_2D;
			type_ = TEXTURE_2D;
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		} else {
			glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		bool initialised = false;
		va_list files;
		va_start(files, numLayers);
		for (unsigned i = 0; i < numLayers; i++) {
			char * file = va_arg(files, char *);
			SDL_Surface * image = IMG_Load(file);
			if (image == NULL) cout << "Could not load image " << file << endl; else {
				width_ = image->w;
				height_ = image->h;
				GLenum textureFormat;
				if (image->format->BytesPerPixel == 4) {
					if (image->format->Rmask == 0x000000ff) textureFormat = GL_RGBA; else textureFormat = GL_BGRA;
				} else {
					if (image->format->Rmask == 0x000000ff) textureFormat = GL_RGB; else textureFormat = GL_BGR;
				}
				if (!initialised) {
					if (texture2dArrayDisabled()) glTexImage2D(GL_TEXTURE_2D, 0, image->format->BytesPerPixel,
							image->w*numLayers, image->h, 0, textureFormat, GL_UNSIGNED_BYTE, NULL);
					else glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, image->format->BytesPerPixel, image->w, image->h,
							numLayers, 0, textureFormat, GL_UNSIGNED_BYTE, NULL);
					initialised = true;
				}
				if (texture2dArrayDisabled()) glTexSubImage2D(GL_TEXTURE_2D, 0, image->w*i, 0, image->w, image->h,
						textureFormat, GL_UNSIGNED_BYTE, image->pixels);
				else glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, image->w, image->h, 1, textureFormat,
						GL_UNSIGNED_BYTE, image->pixels);
				SDL_FreeSurface(image);
			}
		}
		va_end(files);
		glBindTexture(textureType, 0);
	} else cout << "Wrong type of texture specified" << endl;
}

void Texture::reload(textureTypeEnum textureType, const vector<string> & fileNames) {
	type_ = textureType;
	if (textureType == TEXTURE_3D) {
		glGenTextures(1, &texture);
		if (texture2dArrayDisabled()) {
			textureType = TEXTURE_2D;
			type_ = TEXTURE_2D;
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		} else {
			glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		bool initialised = false;
		for (unsigned i = 0; i < fileNames.size(); i++) {
			SDL_Surface * image = IMG_Load(fileNames[i].c_str());
			if (image == NULL) cout << "Could not load image " << fileNames[i] << endl; else {
				width_ = image->w;
				height_ = image->h;
				GLenum textureFormat;
				if (image->format->BytesPerPixel == 4) {
					if (image->format->Rmask == 0x000000ff) textureFormat = GL_RGBA; else textureFormat = GL_BGRA;
				} else {
					if (image->format->Rmask == 0x000000ff) textureFormat = GL_RGB; else textureFormat = GL_BGR;
				}
				if (!initialised) {
					if (texture2dArrayDisabled()) glTexImage2D(GL_TEXTURE_2D, 0, image->format->BytesPerPixel,
							image->w*fileNames.size(), image->h, 0, textureFormat, GL_UNSIGNED_BYTE, NULL);
					else glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, image->format->BytesPerPixel, image->w, image->h,
							fileNames.size(), 0, textureFormat, GL_UNSIGNED_BYTE, NULL);
					initialised = true;
				}
				if (texture2dArrayDisabled()) glTexSubImage2D(GL_TEXTURE_2D, 0, image->w*i, 0, image->w, image->h,
						textureFormat, GL_UNSIGNED_BYTE, image->pixels);
				else glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, image->w, image->h, 1, textureFormat,
						GL_UNSIGNED_BYTE, image->pixels);
				SDL_FreeSurface(image);
			}
		}
		glBindTexture(textureType, 0);
	} else {
		SDL_Surface * image = IMG_Load(fileNames[0].c_str());
		if (image == NULL) cout << "Could not load image " << fileNames[0] << endl; else {
			width_ = image->w;
			height_ = image->h;
			GLenum textureFormat;
			if (image->format->BytesPerPixel == 4) {
				if (image->format->Rmask == 0x000000ff) textureFormat = GL_RGBA; else textureFormat = GL_BGRA;
			} else {
				if (image->format->Rmask == 0x000000ff) textureFormat = GL_RGB; else textureFormat = GL_BGR;
			}
			glGenTextures(1, &texture);
			glBindTexture(textureType, texture);
			glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			if (textureType != TEXTURE_CUBE) {
				glTexImage2D(textureType, 0, image->format->BytesPerPixel, image->w, image->h, 0, textureFormat,
						GL_UNSIGNED_BYTE, image->pixels);
				SDL_FreeSurface(image);
			} else {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, image->format->BytesPerPixel, image->w, image->h, 0,
						textureFormat, GL_UNSIGNED_BYTE, image->pixels);
				SDL_FreeSurface(image);

				GLenum sides[5] = {GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
						GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z};
				for (short i = 0; i < 4; i++) {
					image = IMG_Load(fileNames[i+1].c_str());
					if (image == NULL) cout << "Could not load image " << fileNames[i+1] << endl; else {
						glTexImage2D(sides[i], 0, image->format->BytesPerPixel, image->w, image->h, 0, textureFormat,
								GL_UNSIGNED_BYTE, image->pixels);
						SDL_FreeSurface(image);
					}
				}
			}
			glBindTexture(textureType, 0);
		}
	}
}

void Texture::reload(textureTypeEnum textureType, unsigned numLayers, string * fileNames) {
	type_ = textureType;
	if (textureType == TEXTURE_3D) {
		glGenTextures(1, &texture);
		if (texture2dArrayDisabled()) {
			textureType = TEXTURE_2D;
			type_ = TEXTURE_2D;
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		} else {
			glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		bool initialised = false;
		for (unsigned i = 0; i < numLayers; i++) {
			SDL_Surface * image = IMG_Load(fileNames[i].c_str());
			if (image == NULL) cout << "Could not load image " << fileNames[i] << endl; else {
				width_ = image->w;
				height_ = image->h;
				GLenum textureFormat;
				if (image->format->BytesPerPixel == 4) {
					if (image->format->Rmask == 0x000000ff) textureFormat = GL_RGBA; else textureFormat = GL_BGRA;
				} else {
					if (image->format->Rmask == 0x000000ff) textureFormat = GL_RGB; else textureFormat = GL_BGR;
				}
				if (!initialised) {
					if (texture2dArrayDisabled()) glTexImage2D(GL_TEXTURE_2D, 0, image->format->BytesPerPixel,
							image->w*numLayers, image->h, 0, textureFormat, GL_UNSIGNED_BYTE, NULL);
					else glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, image->format->BytesPerPixel, image->w, image->h,
							numLayers, 0, textureFormat, GL_UNSIGNED_BYTE, NULL);
					initialised = true;
				}
				if (texture2dArrayDisabled()) glTexSubImage2D(GL_TEXTURE_2D, 0, image->w*i, 0, image->w, image->h,
						textureFormat, GL_UNSIGNED_BYTE, image->pixels);
				else glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, image->w, image->h, 1, textureFormat,
						GL_UNSIGNED_BYTE, image->pixels);
				SDL_FreeSurface(image);
			}
		}
		glBindTexture(textureType, 0);
	} else {
		SDL_Surface * image = IMG_Load(fileNames[0].c_str());
		if (image == NULL) cout << "Could not load image " << fileNames[0] << endl; else {
			width_ = image->w;
			height_ = image->h;
			GLenum textureFormat;
			if (image->format->BytesPerPixel == 4) {
				if (image->format->Rmask == 0x000000ff) textureFormat = GL_RGBA; else textureFormat = GL_BGRA;
			} else {
				if (image->format->Rmask == 0x000000ff) textureFormat = GL_RGB; else textureFormat = GL_BGR;
			}
			glGenTextures(1, &texture);
			glBindTexture(textureType, texture);
			glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			if (textureType != TEXTURE_CUBE) {
				glTexImage2D(textureType, 0, image->format->BytesPerPixel, image->w, image->h, 0, textureFormat,
						GL_UNSIGNED_BYTE, image->pixels);
				SDL_FreeSurface(image);
			} else {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, image->format->BytesPerPixel, image->w, image->h, 0,
						textureFormat, GL_UNSIGNED_BYTE, image->pixels);
				SDL_FreeSurface(image);

				GLenum sides[5] = {GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
						GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z};
				for (short i = 0; i < 4; i++) {
					image = IMG_Load(fileNames[i+1].c_str());
					if (image == NULL) cout << "Could not load image " << fileNames[i+1] << endl; else {
						glTexImage2D(sides[i], 0, image->format->BytesPerPixel, image->w, image->h, 0, textureFormat,
								GL_UNSIGNED_BYTE, image->pixels);
						SDL_FreeSurface(image);
					}
				}
			}
			glBindTexture(textureType, 0);
		}
	}
}

const string & Texture::name() const {
	return name_;
}

textureTypeEnum Texture::type() const {
	return type_;
}

int Texture::width() const {
	return width_;
}

int Texture::height() const {
	return height_;
}

}
