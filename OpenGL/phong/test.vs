#version 300 es

in vec4 coord3d;

out mat4x4 D;
out mat4x4 VPi;
out mat4x4 MTi;

void main(void) {

  const mat4x4 E = mat4x4( 1.0, 0.0, 0.0, 0.0,
                           0.0, 1.0, 0.0, 0.0,
                           0.0, 0.0, 1.0, 0.0,
                           0.0, 0.0, 0.0, 1.0 );

  mat4x4 T = mat4x4( 0.2, 0.0, 0.0, 0.0,
                     0.0, 0.2, 0.0, 0.0,
                     0.0, 0.0, 0.2, 0.0,
                     coord3d.xyz, 1.0 );

  const mat4x4 M = E;
  const mat4x4 P = E;
  const mat4x4 V = mat4x4(  320.0,   0.0, 0.0, 0.0,
                             0.0,  240.0, 0.0, 0.0,
                             0.0,    0.0, 1.0, 0.0,
                            320.0, 240.0, 0.0, 1.0 );

  D = mat4x4( 1.0, 0.0, 0.0, 0.0,
              0.0, 1.0, 0.0, 0.0,
              0.0, 0.0, 1.0, 0.0,
              0.0, 0.0, 0.0, -1.0 );

  VPi = inverse( V * P );
  MTi = inverse( M * T );

  highp vec4 pos = P * M * T * vec4( 0.0, 0.0, 0.0, 1.0 );
  pos[2] = 0.0;

  gl_Position = pos;
  gl_PointSize = 150.0;
}

