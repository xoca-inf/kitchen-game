uniform sampler2D texture0;
uniform vec4 u_color;
varying vec2 vUV;

void w4_main()
{
    gl_FragColor = texture2D(texture0, vUV) * u_color;
}
