varying vec2 f_texcoord;

uniform sampler2D mytexture;
uniform vec2 tc_offset[9];

void main(void) {
    vec4 sample[9];
    vec4 maxValue = vec4(0.0);
    
    for (int i = 0; i < 9; i++)
    {
    sample[i] = texture2D(mytexture, gl_TexCoord[0].st + tc_offset[i]);
    maxValue = max(sample[i], maxValue);
}
    gl_FragColor = maxValue;
}
