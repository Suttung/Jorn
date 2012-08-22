#pragma once

#include "SDL.h"
#include "SDL_opengl.h"

class TextureLoader
{
	GLuint texture;			// This is a handle to our texture object
	SDL_Surface *surface;	// This surface will tell us the details of the image
	GLenum texture_format;
	GLint  nOfColors;

public:
	TextureLoader(const char * path);
	~TextureLoader(void);

	void drawTexturedObject();
};

