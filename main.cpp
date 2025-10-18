#include <iostream>
#include <cmath>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include <GL/glut.h> // for bitmap text rendering



// Window dimensions
const GLuint WIDTH = 1200, HEIGHT = 600;

// Light position
glm::vec3 lightPos(1.2f, 1.0f, 5.0f);

// Helper function to render text
void RenderText(float x, float y, const char* text)
{
    glRasterPos2f(x, y); // 2D position in screen coordinates or transformed coordinates
    while (*text)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text++);
    }
}

int main()
{
    // Initialize GLUT (required for glutBitmapCharacter)
    int argc = 1;
    char* argv[1] = {(char*)"Something"};
    glutInit(&argc, argv);
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Phong Specular Highlights", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();
    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_DEPTH_TEST);

    Shader lightingShader("shaders/lighting.vs", "shaders/lighting.frag");

    GLfloat vertices[] = {
        // Positions          // Normals
        -0.5f,-0.5f,-0.5f,  0.0f, 0.0f,-1.0f,
         0.5f,-0.5f,-0.5f,  0.0f, 0.0f,-1.0f,
         0.5f, 0.5f,-0.5f,  0.0f, 0.0f,-1.0f,
         0.5f, 0.5f,-0.5f,  0.0f, 0.0f,-1.0f,
        -0.5f, 0.5f,-0.5f,  0.0f, 0.0f,-1.0f,
        -0.5f,-0.5f,-0.5f,  0.0f, 0.0f,-1.0f,

        -0.5f,-0.5f, 0.5f,  0.0f, 0.0f, 1.0f,
         0.5f,-0.5f, 0.5f,  0.0f, 0.0f, 1.0f,
         0.5f, 0.5f, 0.5f,  0.0f, 0.0f, 1.0f,
         0.5f, 0.5f, 0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f,-0.5f, 0.5f,  0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f,-0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f,-0.5f,-0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f,-0.5f,-0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f,-0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

         0.5f, 0.5f, 0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, 0.5f,-0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,-0.5f,-0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,-0.5f,-0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,-0.5f, 0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, 0.5f, 0.5f,  1.0f, 0.0f, 0.0f,

        -0.5f,-0.5f,-0.5f,  0.0f,-1.0f, 0.0f,
         0.5f,-0.5f,-0.5f,  0.0f,-1.0f, 0.0f,
         0.5f,-0.5f, 0.5f,  0.0f,-1.0f, 0.0f,
         0.5f,-0.5f, 0.5f,  0.0f,-1.0f, 0.0f,
        -0.5f,-0.5f, 0.5f,  0.0f,-1.0f, 0.0f,
        -0.5f,-0.5f,-0.5f,  0.0f,-1.0f, 0.0f,

        -0.5f, 0.5f,-0.5f,  0.0f, 1.0f, 0.0f,
         0.5f, 0.5f,-0.5f,  0.0f, 1.0f, 0.0f,
         0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f,
         0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f,-0.5f,  0.0f, 1.0f, 0.0f
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    GLfloat shininessValues[] = { 2, 4, 8, 16, 32, 64, 128, 256 };

    // Cube grid layout
    int rows = 2;
    int cols = 4;
    float spacing = 2.0f; // distance between cubes

    // Calculate maximum distance from center to corner of the grid
    float maxX = (cols - 1) / 2.0f * spacing + 0.5f; // half cube included
    float maxY = (rows - 1) / 2.0f * spacing + 0.5f;
    float maxDistance = sqrt(maxX*maxX + maxY*maxY + 0.5f*0.5f); // include cube half depth
    float fov = 30.0f;
    float aspect = (float)WIDTH / HEIGHT;

    // Position camera far enough to see all cubes
    float cameraZ = maxDistance / tan(glm::radians(fov / 2.0f));
    glm::vec3 cameraPos(0.0f, 0.0f, cameraZ);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader.use();

        glUniform3f(glGetUniformLocation(lightingShader.Program, "objectColor"), 1.0f, 0.5f, 0.31f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "lightColor"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "viewPos"), cameraPos.x, cameraPos.y, cameraPos.z);

        glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::perspective(glm::radians(fov), aspect, 0.1f, 100.0f);

        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);

        int index = 0;
        for (int row = 0; row < rows; ++row)
        {
            for (int col = 0; col < cols; ++col)
            {
                glm::mat4 model = glm::mat4(1.0f);

                float x = (col - (cols - 1)/2.0f) * spacing;
                float y = (row - (rows - 1)/2.0f) * -spacing;
                float z = 0.0f;

                model = glm::translate(model, glm::vec3(x, y, z));
                model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f));

                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                glUniform1f(glGetUniformLocation(lightingShader.Program, "shininess"), shininessValues[index]);

                glDrawArrays(GL_TRIANGLES, 0, 36);

                // Render the number under the cube
                char numStr[16];
                sprintf(numStr, "%d", (int)shininessValues[index]);

                // Transform cube position to screen space
                glm::vec4 pos = projection * view * glm::vec4(x, y - 0.4f, z, 1.0f);
                pos /= pos.w; // perspective divide

                // Convert to normalized device coordinates (NDC -> screen)
                float screenX = (pos.x * 0.5f + 0.5f) * WIDTH;
                float screenY = (pos.y * 0.5f + 0.5f) * HEIGHT;

                // --- Begin Text Rendering ---
                glDisable(GL_DEPTH_TEST); // disable depth so text is on top

                glMatrixMode(GL_PROJECTION);
                glPushMatrix();
                glLoadIdentity();
                gluOrtho2D(0, WIDTH, 0, HEIGHT);

                glMatrixMode(GL_MODELVIEW);
                glPushMatrix();
                glLoadIdentity();

                // Flip Y axis because OpenGL 0,0 is bottom-left

                glColor3f(1.0f, 1.0f, 1.0f);

                RenderText(screenX, HEIGHT - screenY, numStr);

                glPopMatrix();
                glMatrixMode(GL_PROJECTION);
                glPopMatrix();
                glMatrixMode(GL_MODELVIEW);

                glEnable(GL_DEPTH_TEST); // restore depth testing
                // --- End Text Rendering ---
                index++;
            }
        }


        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}