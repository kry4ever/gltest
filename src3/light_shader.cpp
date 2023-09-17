#include "light_shader.h"
#include "glm/gtc/matrix_transform.hpp"

extern glm::vec3 lightPos;

glm::mat4 LightShader::createModel()
{
    auto model = glm::mat4();
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f));
    return model;
}

std::string LightShader::getFShader()
{
   return R"(
            #version 330 core
            out vec4 color;

            void main()
            {
                color = vec4(1.0, 1.0, 1.0, 1.0);
            }
        )";
}
