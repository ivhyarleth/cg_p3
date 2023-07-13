#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;
out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;

    float azul = (aPos.z + 60) / 100.0;
    float verde = 1.0 - abs(aPos.z) / 39.0;
    float rojo = 1.0 - abs(aPos.z - 20) / 20.0;

    if (azul >= verde && azul >= rojo) {
        color = vec3(0.0f, 0.0f, azul);
    } else if (verde >= azul && verde >= rojo) {
        color = vec3(0.0f, verde, 0.0f);
    } else {
        color = vec3(rojo, 0.0f, 0.0f);
    }

    gl_Position = projection * view * vec4(FragPos, 1.0);
}
