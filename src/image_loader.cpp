#include "stb_image.h"
#include <iostream>
#include <GL/glew.h>

GLuint load_image(const char* path){
    int width, height, channels;
	unsigned char* image = stbi_load(path, &width, &height, &channels, 0);
	if (image == nullptr)
	{
		std::cout << "load image failed" << std::endl;
		return 0;
	}else
	{
		std::cout << "load image success " << *image << std::endl;
	}

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 0); 
    return texture;
}