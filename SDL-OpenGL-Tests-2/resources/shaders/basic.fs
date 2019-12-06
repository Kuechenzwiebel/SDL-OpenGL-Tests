#version 330 core

out vec4 color;

in vec3 SpacePos;
in vec2 Uvs;
in vec3 Normals;

uniform sampler2D tex;
uniform vec3 viewPos;

uniform int wireframe;

void main() {
    if(wireframe == 1) {
        color = vec4(1.0f);
        return; 
    }
    
    vec3 normals = normalize(Normals);
    
    vec3 lightDir = normalize(vec3(1.0f, 2.45f, 0.0f));
    float diff = max(dot(normals, lightDir), 0.0);
    vec3 diffuse = diff * vec3(0.8f);
    
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - SpacePos);
    vec3 reflectDir = reflect(-lightDir, normals);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * vec3(1.0f);
    
    color = vec4(diffuse, 1.0f) * texture(tex, Uvs);
}
