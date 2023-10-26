#include <string>
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include "glm/vec3.hpp"

#ifndef BasicShader_H
#define BasicShader_H


class BasicShader
{
protected:
    virtual std::string getVShader() = 0;
    virtual std::string getFShader() = 0;
    GLuint shaderProgram;
    GLint modelLoc;
    GLint viewLoc;
    GLint projectionLoc;

public:
    virtual void init() = 0;

    void use()
    {
        glUseProgram(shaderProgram);
    }

    void setVec3(std::string name, glm::vec3 value);

private:
    GLuint initProgram(std::string vSource, std::string fSource);
};

#endif