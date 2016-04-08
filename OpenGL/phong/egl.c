#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GLES3/gl3.h>

static GLint s_program = 0;

static GLint attribute_coord3d = 0;

void
draw(void)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(s_program);

    glEnableVertexAttribArray(attribute_coord3d);

    GLfloat vertices[] = {
        0.0, 0.0, -0.2, 1.0,
        -0.5, -0.5, -0.4, 1.0,
        0.5, -0.5, -0.2, 1.0,
        -0.5, 0.5, -0.1, 1.0,
        0.5,  0.5, -0.99, 1.0,
    };

    glVertexAttribPointer( attribute_coord3d, 4, GL_FLOAT, GL_FALSE, 0, vertices );

    glDrawArrays(GL_POINTS, 0, 5);
    glDisableVertexAttribArray(attribute_coord3d);
}


static GLuint
load_shader(const char *file, GLenum type)
{
    FILE *fd = fopen(file, "r");
    if (! fd) {
        fprintf(stderr, "Error opening `%s': %s\n", file, strerror(errno));
        abort();
    }

    if (fseek(fd, 0, SEEK_END) < 0) {
        fprintf(stderr, "Error getting size of `%s': %s\n", strerror(errno));
        fclose(fd);
        abort();
    }
    long size = ftell(fd);
    if (size < 0) {
        fprintf(stderr, "Error getting size of `%s': %s\n", strerror(errno));
        fclose(fd);
        abort();
    }
    rewind(fd);

    char *data = malloc(size);
    if (! data) {
        fprintf(stderr, "Error allocating memory: %s\n", strerror(errno));
        fclose(fd);
        abort();
    }

    size = fread(data, 1, size, fd);
    if (size < 0) {
        fprintf(stderr, "Error reading `%s': %s\n", file, strerror(errno));
        free(data);
        fclose(fd);
        abort();
    }

    fclose(fd);

    {
        GLuint shader = glCreateShader(type);
        const GLchar *tmp[] = { data };
        glShaderSource(shader, 1, tmp, NULL);
        free(data);
        glCompileShader(shader);
        GLint compile_ok = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_ok);
        if (! compile_ok) {
            const char *stype;
            switch (type) {
                case GL_VERTEX_SHADER:
                    stype = "vertex";
                    break;
                case GL_FRAGMENT_SHADER:
                    stype = "fragment";
                    break;
                default:
                    stype = "unknown";
                    break;
            }
            fprintf(stderr, "Error in %s shader `%s'\n", stype, file);
            {
                char buf[16<<10];
                GLsizei size = 0;
                glGetShaderInfoLog(shader, sizeof(buf), &size, buf);
                fwrite(buf, 1, size, stderr);
            }
            abort();
        }
        return shader;
    }
}

void
init_shaders(void)
{
    s_program = glCreateProgram();
    glAttachShader(s_program, load_shader("test.vs", GL_VERTEX_SHADER));
    glAttachShader(s_program, load_shader("test.fs", GL_FRAGMENT_SHADER));

    glLinkProgram(s_program);
    GLint link_ok = GL_FALSE;
    glGetProgramiv(s_program, GL_LINK_STATUS, &link_ok);
    if (! link_ok) {
        fprintf(stderr, "Error linking shaders\n");
        abort();
    }

    attribute_coord3d = glGetAttribLocation(s_program, "coord3d");
    if (attribute_coord3d < 0) {
        fprintf(stderr, "Error in coord2d\n");
    }
}
