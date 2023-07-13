#include "include/glad.h"
#include <GLFW/glfw3.h>
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"

#include "shader_m.h"
#include "camera.h"
#include "Objeto.h"
#include "glut_ply.h"

#include <iostream>
#include <fstream>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;

// camera
Camera camera(glm::vec3(45.0f, -12.0f, -46.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float tiempoInicial = 0.0f, tiempoTranscurrido = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 30.0f, 2.0f);
bool boton_presionado = false;

glm::vec3 crossProduct(glm::vec3 diff_ba, glm::vec3 diff_ca) {
    glm::vec3 dir = glm::vec3(0, 0, 0);
    dir.x = diff_ba.y * diff_ca.z - diff_ba.z * diff_ca.y;
    dir.y = -(diff_ba.x * diff_ca.z - diff_ba.z * diff_ca.x);
    dir.z = diff_ba.x * diff_ca.y - diff_ba.y * diff_ca.x;
    return dir;
}

void normalizeVec3(glm::vec3 &v) {
    float answer = sqrt((v.x * v.x + v.y * v.y + v.z * v.z));
    v.x = v.x / answer;
    v.y = v.y / answer;
    v.z = v.z / answer;
}

glm::vec3 calcularNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    glm::vec3 diff_ba = b - a;
    glm::vec3 diff_ca = c - a;
    glm::vec3 dir = crossProduct(diff_ba, diff_ca);
    normalizeVec3(dir);
    return dir;
}

int countRows(const std::string& filename) {
    int rows = 0;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line))
        rows++;
    file.close();
    return rows - 1;
}

void lecturaMultiplesColumnas(float* array, const std::string& archivo) {
    // Leer el archivo .csv
    std::ifstream fin;
    fin.open(archivo);
    if (!fin.is_open()) throw std::runtime_error("Could not open file");
    std::vector<std::string> encabezado;
    std::vector<std::vector<std::string>> rows;
    std::string palabra, linea;
    getline(fin, linea);
    int i = 0;
    while (!fin.eof()) {
        getline(fin, linea);
        std::stringstream s(linea);
        while (getline(s, palabra, ',')) {
            array[i] = stof(palabra);
            i++;
        }
    }
    fin.close();
}

int main() {
    srand(time(NULL));
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader program
    Shader lightingShader("../2.2.basic_lighting.vs", "../2.2.basic_lighting.fs");
    Shader lightCubeShader("../2.2.light_cube.vs", "../2.2.light_cube.fs");
    int n_vertices = 1000000;
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    glm::vec3 lightDir(-0.2f, -1.0f, -0.3f);
    lightingShader.use();
    lightingShader.setVec3("lightColor", lightColor);
    lightingShader.setVec3("lightDir", lightDir);

    std::string csvPath = "../triangulos6.csv"; // 3 y 6
    int rows = countRows(csvPath) * 6;
    float* vertices = new float[rows];
    lecturaMultiplesColumnas(vertices, csvPath);


    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    unsigned int VBO, cubeVAO; // VAO: Vertex Array Object - VBO: Vertex Buffer Object
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * rows, vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    int N_TRIANGLES = 10000000;
    // render loop
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        // render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("lightColor", 1.0f, 0.0f, 1.0f);
        lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setVec3("viewPos", camera.Position);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, N_TRIANGLES);

        // Imprimir coordenadas de la cámara
        glm::vec3 cameraPosition = camera.Position;
        std::cout << "Camera Position: (" << cameraPosition.x << ", " << cameraPosition.y << ", " << cameraPosition.z << ")" << std::endl;
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete[] vertices;
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        boton_presionado = true;
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE) {
        if (boton_presionado) {
            boton_presionado = false;
            tiempoInicial = static_cast<float>(glfwGetTime());
        }
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
