#include <string>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

static GLuint main_program;
static GLuint buffer_pos;
static GLuint vao;
static GLuint time_location;

static std::string vertex_shader(
        "#version 330\n"
        "layout(location = 0) in vec4 position;\n"
        "uniform float time;\n"
        "\n"
        "void main()\n"
        "{\n"
        "       float scale = 3.14159f * 2.0f / 5.0f;\n"
        "       float curr = mod(time, 5.0f);\n"
        "       vec4 vec_offset = vec4(\n"
        "               cos(time * scale) * 0.5f,\n"
        "               sin(time * scale) * 0.5f,\n"
        "               0.0f,\n"
        "               0.0f);\n"
        "       gl_Position = position + vec_offset;\n"
        "}\n"
);

static std::string frag_shader(
        "#version 330\n"
        "out vec4 output_color;\n"
        "\n"
        "uniform float time;\n"
        "\n"
        "const vec4 first_color  = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
        "const vec4 second_color = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n"
        "void main()\n"
        "{\n"
        "       float curr = mod(time, 5.0f);\n"
        "       float lerp = curr / 5.0f;\n"
        "       output_color = mix(first_color, second_color, lerp);\n"
        "}\n"
);

static const float vertex_pos[] = {
        0.0f, 0.5f, 0.0f, 1.0f,
        0.5f, -0.366f, 0.0f, 1.0f,
        -0.5f, -0.366f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
};

static GLuint create_shader(GLenum shader_type, const std::string &shader_str)
{
        GLuint shader = glCreateShader(shader_type);
        const char *str_data = shader_str.c_str();
        glShaderSource(shader, 1, &str_data, NULL);

        glCompileShader(shader);

        GLint status; glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

        if (status == GL_FALSE) {
                fprintf(stderr, "Couldn't compile shader\n");
                fprintf(stderr, "Shader:\n%s\n================", shader_str.c_str());
                exit(1);
        }

        return shader;
}

static GLuint create_program(const std::vector<GLuint> &shaders)
{
        GLuint program = glCreateProgram();
        
        for (size_t i = 0; i < shaders.size(); i++)
                glAttachShader(program, shaders[i]);

        glLinkProgram(program);
        
        GLint status; glGetProgramiv(program, GL_LINK_STATUS, &status);
        if (status == GL_FALSE) {
                fprintf(stderr, "Linking failed\n");
                exit(1);
        }

        return program;
}

static void init_program(void)
{
        /* crate vector of shaders */
        std::vector<GLuint> shaders;
        shaders.push_back(create_shader(GL_VERTEX_SHADER, vertex_shader));
        shaders.push_back(create_shader(GL_FRAGMENT_SHADER, frag_shader));
       
        /* add shaders to program, then delete shaders */
        main_program = create_program(shaders);
        std::for_each(shaders.begin(), shaders.end(), glDeleteShader);

        /* get offset location from main program */
        time_location = glGetUniformLocation(main_program, "time");
}

static void init_vertex_buffer(void)
{
        glGenBuffers(1, &buffer_pos);

        glBindBuffer(GL_ARRAY_BUFFER, buffer_pos);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_pos), vertex_pos, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
}

static void init(void)
{
        init_program();
        init_vertex_buffer();

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
}

static void draw(void)
{
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(main_program);

        glUniform1f(time_location, glfwGetTime());
        
        glBindBuffer(GL_ARRAY_BUFFER, buffer_pos);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)48);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glUseProgram(0);
}

void window_resized(GLFWwindow *window, int width, int height)
{
        int new_height = (int)((9*width) / 16.0 + 0.5);
        glViewport(0, new_height/2, (GLsizei)width, (GLsizei)new_height);
}

int main(void)
{
        GLFWwindow *window;

        /* init GLFW */
        if (!glfwInit())
                exit(EXIT_FAILURE);

        /* Create windowed mode window and OpenGL context */
        window = glfwCreateWindow(640, 480, "Hello, Triangle!", NULL, NULL);
        if (!window) {
                glfwTerminate();
                exit(EXIT_FAILURE);
        }

        /* set current context for window */
        glfwMakeContextCurrent(window);
        glfwSetWindowSizeCallback(window, window_resized);

        /* init glew */
        if (glewInit() != GLEW_OK) {
                fprintf(stderr, "Couldn't init glew\n");
                exit(EXIT_FAILURE);
        }

        /* init triangle program */
        init();

        /* loop until the window is closed */
        while (!glfwWindowShouldClose(window)) {
                /* call render */
                draw();

                /* show rendered buffer */
                glfwSwapBuffers(window);

                /* poll user input events */
                glfwPollEvents();
        }

        glfwTerminate();
        return 0;

}
