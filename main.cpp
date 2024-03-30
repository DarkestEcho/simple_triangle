#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSources[] =
{
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0",

    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(6.0f, 0.2f, 0.7f, 1.0f);\n"
    "}\0",
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main(int, char**)
{
    // we first initialize GLFW 
    glfwInit();

    /*
        we can configure GLFW using glfwWindowHint.
        param hint The first argument of glfwWindowHint tells us what option we want to configure.
        param value The second argument is an integer that sets the value of our option.
    */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // creating a window object. This window object holds all the windowing data and is required by most of GLFW's other functions.
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // here we can check if GLAD loaded correctly
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // setting viewport parameters
    // x
    // y
    // width
    // heigh
    glViewport(0, 0, 800, 600);

    // telling GLFW we want to call this function on every window resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // build and compile our shader program
 // ------------------------------------
 // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success[2];
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success[0]);
    if (!success[0])
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShaders[] =
    {
        glCreateShader(GL_FRAGMENT_SHADER),
        glCreateShader(GL_FRAGMENT_SHADER),
    };
    glShaderSource(fragmentShaders[0], 1, &fragmentShaderSources[0], NULL);
    glShaderSource(fragmentShaders[1], 1, &fragmentShaderSources[1], NULL);

    glCompileShader(fragmentShaders[0]);
    glCompileShader(fragmentShaders[1]);

    // check for shader compile errors
    glGetShaderiv(fragmentShaders[0], GL_COMPILE_STATUS, &success[0]);
    glGetShaderiv(fragmentShaders[1], GL_COMPILE_STATUS, &success[1]);

    if (!success[0])
    {
        glGetShaderInfoLog(fragmentShaders[0], 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    if (!success[1])
    {
        glGetShaderInfoLog(fragmentShaders[0], 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderPrograms[] =
    {
        glCreateProgram(),
        glCreateProgram()
    };

    glAttachShader(shaderPrograms[0], vertexShader);
    glAttachShader(shaderPrograms[1], vertexShader);
    glAttachShader(shaderPrograms[0], fragmentShaders[0]);
    glAttachShader(shaderPrograms[1], fragmentShaders[1]);

    glLinkProgram(shaderPrograms[0]);
    glLinkProgram(shaderPrograms[1]);
    // check for linking errors
    glGetProgramiv(shaderPrograms[0], GL_LINK_STATUS, &success[0]);
    glGetProgramiv(shaderPrograms[1], GL_LINK_STATUS, &success[1]);

    if (!success[0])
    {
        glGetProgramInfoLog(shaderPrograms[0], 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    if (!success[1])
    {
        glGetProgramInfoLog(shaderPrograms[1], 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShaders[0]);
    glDeleteShader(fragmentShaders[1]);

    float vertices[2][9] =
    {
        {
            // first triangle
            -0.9f, -0.5f, 0.0f,  // left 
            -0.2f, -0.6f, 0.0f,  // right
            -0.4f, 0.6f, 0.0f  // top 
        },
        {   // second triangle
            0.0f, -0.5f, 0.0f,  // left
            0.9f, -0.8f, 0.0f,  // right
            0.45f, 0.1f, 0.0f   // top 
        }
    };

    unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]), vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // glBindVertexArray(0); // no need to unbind at all as we directly bind a different VAO the next few lines

        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAOs[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[1]), vertices[1], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // glBindVertexArray(0); // no need to unbind at all as we directly bind a different VAO the next few lines


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // here we define main render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);
        // rendering commands here
        glClearColor(0.2f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderPrograms[0]);

        glBindVertexArray(VAOs[0]); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderPrograms[1]);
        glBindVertexArray(VAOs[1]); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0); // no need to unbind it every time 

        // check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(shaderPrograms[0]);
    glDeleteProgram(shaderPrograms[1]);

    glfwTerminate();

    return 0;
}

// we have to create this function to manage cases when the user changes the window size
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
