#include <math.h>
#include <GLES3/gl3.h>

struct mat4 {
  GLfloat x[16];
};

float
radians(float x)
{
  return x * M_PI / 180;
}

void
matrix_projection(struct mat4 *matrix, float fovy, float aspect, float znear, float zfar)
{
    const float f = 1.0f / tanf(radians(fovy) / 2);
    const float a = (zfar + znear) / (znear - zfar);
    const float b = (2 * zfar * znear) / (znear - zfar);

    *matrix = (struct mat4) {{
        f / aspect, 0.0f,       0.0f,       0.0f,
        0.0f,       f,          0.0f,       0.0f,
        0.0f,       0.0f,       a,          b,
        0.0f,       0.0f,       -1.0f,      0.0f,
    }};
}
