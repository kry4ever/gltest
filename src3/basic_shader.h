#include <string>
#include <iostream>
#include <GL/glew.h>

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

private:
    GLuint initProgram(std::string vSource, std::string fSource);
};

#endif