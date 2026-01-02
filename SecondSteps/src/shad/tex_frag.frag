#version 330 core
out vec4 FragColor;

in vec3 v_color;
in vec2 v_texCoord;

uniform sampler2D u_texture1;
uniform sampler2D u_texture2;

void main()
{
    vec4 tex_col1 = texture(u_texture1, v_texCoord);
    vec4 tex_col2 = texture(u_texture2, vec2(1-v_texCoord.x, v_texCoord.y));

    vec4 tex_col = mix(tex_col1, tex_col2, 0.4f);

    FragColor = vec4(tex_col.rgb, tex_col.a);
} 