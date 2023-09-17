#include "cube_shader.h"

#ifndef light_shader_H
#define light_shader_H

class LightShader : public cube_shader{

protected:
    glm::mat4 createModel() override;
    std::string getFShader() override;
};

#endif