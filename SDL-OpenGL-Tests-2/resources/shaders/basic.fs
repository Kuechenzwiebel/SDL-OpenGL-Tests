#version 330 core

out vec4 color;

in vec3 SpacePos;
in vec2 Uvs;
in vec3 Normals;

uniform sampler2D tex;
uniform vec3 viewPos;

uniform int wireframe;

void main() {
    /*
    vec3 normals = normalize(Normals);
    
    vec3 lightDir = normalize(vec3(0.0f, 2.45f, 0.0f) - SpacePos);
    float diff = max(dot(normals, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0f);
    
    color = vec4(diffuse + vec3(0.2f), 1.0f) * texture(tex, Uvs);*/
     
    if(wireframe == 0) {
        color = texture(tex, Uvs);
    }
    else {
        color = vec4(1.0f);
    }
}
