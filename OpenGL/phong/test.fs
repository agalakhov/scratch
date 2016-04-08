#version 300 es

in highp mat4x4 D;
in highp mat4x4 VPi;
in highp mat4x4 MTi;

out lowp vec4 color;

void main(void) {

    const lowp vec3 surface = vec3(1.0, 0.1, 0.1); // vec3( 0.58, 0.42, 0.20 );

    const lowp vec3 ambient = 0.25 * vec3(1.0, 0.0, 0.0);
    const lowp vec3 diffuse = 0.5 * vec3(0.0, 1.0, 0.0);
    const lowp vec3 specular = 2.8 * vec3(0.0, 0.0, 1.0);
    const lowp float alpha = 5.0;
    const highp vec3 source = vec3(0.0, 0.0, 5.0);

    highp vec4 x1w = vec4( gl_FragCoord.xy, 0.0, 1.0 );

    highp mat4x4 VPMTi = MTi * VPi;
    highp vec4 x1p =  VPMTi * x1w;
    x1p[2] = 0.0;

    highp vec4 c3 = VPMTi[2];

    highp float a = dot(c3, D * c3);
    highp float b = dot(x1p, D * c3);
    highp float c = dot(x1p, D * x1p);
    highp float d = (b * b) - (4.0 * a * c);

    if (d < 0.0)
        discard;

    highp float zw = (- b + sqrt(d)) / (2.0 * a);
    highp vec4 xw = vec4( x1w.xy, zw, x1w.w );

    mediump vec3 ne = normalize(vec3(transpose(MTi) * D * VPMTi * xw));
    highp vec4 pe4 = VPi * xw;
    highp vec3 pe = pe4.xyz * pe4.w; // FIXME

    // Phong reflection model
    highp vec3 le = normalize(source - pe);
    highp vec3 e = normalize(vec3(0.0, 0.0, 5.0) - pe);
    highp vec3 re = normalize(reflect(-le, ne));

    lowp float Kdiffuse = max(0, dot(le, ne));
    lowp float Kspecular = pow(max(0, dot(re, e)), alpha);

 //   color = vec4(ne, 1.0);
    color = vec4( ambient + Kdiffuse * diffuse + Kspecular * specular, 1.0 );
}

