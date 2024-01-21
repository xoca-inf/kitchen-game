uniform sampler2D texture0;
uniform vec4 u_color;
uniform float time;
varying vec2 vUV;

void w4_main()
{
    float t = (sin(time * 1.5) + 1.0) / 2.0 * 0.5 + 0.5;
    vec4 sprite = texture2D(texture0, vUV);
    gl_FragColor = (sprite * t  + (1.0 - t) * vec4(1.0, 0.3, 0.0, sprite.w)) * u_color;
}
