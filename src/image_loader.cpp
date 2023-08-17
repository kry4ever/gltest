#include "stb_image.h"
#include <iostream>
#include <GL/glew.h>

static GLuint load_image(const char* path, GLint location, GLint index){
    int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* image = stbi_load(path, &width, &height, &channels, 0);
	if (image == nullptr)
	{
		std::cout << "load image failed" << std::endl;
		return 0;
	}else
	{
		std::cout << "load image success " << width << "," << height << "," << channels << std::endl;
	}

	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	const char* suffix = "png";
	bool endsWith = (strcmp(path + strlen(path) - strlen(suffix), suffix) == 0);
	if (endsWith)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	}else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	}
	
	glGenerateMipmap(GL_TEXTURE_2D);
	glUniform1i(location, index);
	
	stbi_image_free(image);
    return texture;
}