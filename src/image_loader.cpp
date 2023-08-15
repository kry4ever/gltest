#include "stb_image.h"
#include <iostream>
#include <GL/glew.h>

GLuint load_image(const char* path, GLint location, GLint index){
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
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, texture);
	const char* suffix = "png";
	bool endsWith = (strcmp(path + strlen(path) - strlen(suffix), suffix) == 0);
	if (endsWith)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	}else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	}
	
	
	glUniform1i(location, index);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
    return texture;
}