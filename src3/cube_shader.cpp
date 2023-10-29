#include "cube_shader.h"
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <GL/glew.h>
#include "camera.h"

std::string cube_shader::getVShader()
{
    return R"(
            #version 330 core
            layout (location = 0) in vec3 position;
            layout (location = 1) in vec3 normal;
            layout (location = 2) in vec2 texCoord;

            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection;

            out vec2 TexCoord;

            
            out vec3 normalDirection;
            out vec3 FragPos;

            void main()
            {
                gl_Position = projection * view * model * vec4(position, 1.0);
                TexCoord = texCoord;
                FragPos = vec3(model * vec4(position, 1.0f));
                normalDirection = mat3(transpose(inverse(model))) * normal;  
            }
        )";
}

std::string cube_shader::getFShader()
{
    return R"(
            #version 330 core

            struct Material {
                sampler2D diffuse;
                sampler2D specular;
                float shininess;
            }; 

            uniform Material material;

            struct Light {
                vec3 position;

                vec3 ambient;
                vec3 diffuse;
                vec3 specular;
            };

            uniform Light light;

            out vec4 color;
            in vec2 TexCoord;
            in vec3 normalDirection;
            in vec3 FragPos;

            uniform vec3 lightColor;
            uniform vec3 viewPos;


            void main()
            {
                vec3 norm = normalize(normalDirection);
                vec3 lightDir = normalize(light.position - FragPos);

                vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

                float diff = max(dot(norm, lightDir), 0.0);
                vec3 diffuse = diff * vec3(texture(material.diffuse, TexCoord)) * light.diffuse;

                vec3 viewDir = normalize(viewPos - FragPos);
                vec3 reflectDir = reflect(-lightDir, norm);

                float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
                vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
                //vec3 specular = material.specular * spec * light.specular;  

                vec3 result = ambient + diffuse + specular;
                color = vec4(result, 1.0f);
            }
        )";
}

glm::mat4 cube_shader::createModel()
{
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.3f, 0.5f));
    return model;
}

void cube_shader::init()
{
    BasicShader::init();
    use();
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = stbi_load("res/container2.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // texture 2
    // ---------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("res/container2_specular.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    // ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:

    glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "material.specular"), 1);

    GLint objectColorLoc = glGetUniformLocation(shaderProgram, "objectColor");
    GLint lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
    glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f); // 我们所熟悉的珊瑚红
    glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);   // 依旧把光源设置为白色

    // GLint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPosition");
    // glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);

    // extern glm::vec3 cameraPos;
    extern Camera camera;
    GLint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
    glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);

    // GLint ambientLoc = glGetUniformLocation(shaderProgram, "material.ambient");
    // GLint diffuseLoc = glGetUniformLocation(shaderProgram, "material.diffuse");
    // GLint specularLoc = glGetUniformLocation(shaderProgram, "material.specular");
    GLint shinessLoc = glGetUniformLocation(shaderProgram, "material.shininess");
    // glUniform3f(ambientLoc, 1.0f, 0.5f, 0.31f);
    // glUniform3f(diffuseLoc, 1.0f, 0.5f, 0.31f);
    // glUniform3f(specularLoc, 0.5f, 0.5f, 0.5f);
    glUniform1f(shinessLoc, 32.0f);

    GLint light_ambientLoc = glGetUniformLocation(shaderProgram, "light.ambient");
    GLint light_diffuseLoc = glGetUniformLocation(shaderProgram, "light.diffuse");
    GLint light_specularLoc = glGetUniformLocation(shaderProgram, "light.specular");
    GLint light_posLoc = glGetUniformLocation(shaderProgram, "light.position");
    glUniform3f(light_ambientLoc, 0.2f, 0.2f, 0.2f);
    glUniform3f(light_diffuseLoc, 0.5f, 0.5f, 0.5f);
    glUniform3f(light_specularLoc, 1.0f, 1.0f, 1.0f);
    extern glm::vec3 lightPos;
    glUniform3f(light_posLoc, lightPos.x, lightPos.y, lightPos.z);

    mVAO = initVAO();
}

void cube_shader::draw(glm::mat4 view, glm::mat4 projection)
{
    glBindVertexArray(mVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(createModel()));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

GLuint cube_shader::initVAO()
{
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    return VAO;
}