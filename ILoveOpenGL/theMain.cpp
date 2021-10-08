//#include <glad/glad.h>
//
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>

#include "GLCommon.h"

//#include "linmath.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr


#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>       // "smart array"
#include <sstream>  
#include <fstream>      // C++ file I-O library (look like any other stream)

#include "cVAOManager.h"
#include "cMesh.h"

bool LoadPlyFile(std::string fileName);

glm::vec3 cameraEye = glm::vec3(0.0, 0.0, -4.0f);

//vector to store filelist
std::vector<std::string> _fileName;


cVAOManager gVAOManager;
std::vector<cMesh> g_vecMeshes;


static const char* vertex_shader_text =
"#version 110\n"
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec3 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"#version 110\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    float cameraSpeed = 0.1f;

    // Basic camera controls
    if (key == GLFW_KEY_A)
    {
        cameraEye.x -= cameraSpeed;     // Go left
    }
    if (key == GLFW_KEY_D)
    {
        cameraEye.x += cameraSpeed;     // Go right
    }

    if (key == GLFW_KEY_W)
    {
        cameraEye.z += cameraSpeed;     // Go forward
    }
    if (key == GLFW_KEY_S)
    {
        cameraEye.z -= cameraSpeed;     // Go backwards
    }

    if (key == GLFW_KEY_Q)
    {
        cameraEye.y -= cameraSpeed;     // Go "Down"
    }
    if (key == GLFW_KEY_E)
    {
        cameraEye.y += cameraSpeed;     // Go "Up"
    }


    std::cout << "Camera: "
        << cameraEye.x << ", "
        << cameraEye.y << ", "
        << cameraEye.z << std::endl;

}


void getModelFromFile(std::string fileName);
void loadIntoVAO(std::vector<std::string> fileName, GLuint program);



int main(void)
{
    GLFWwindow* window;


    GLuint vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vcol_location;

    glfwSetErrorCallback(error_callback);

    if ( ! glfwInit() )
    {
        return -1;
        //exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(1200, 640, "OpenGL is great!", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
// Tiny change from the original documentation code
    gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress);
//    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);


    mvp_location = glGetUniformLocation(program, "MVP");



    //
    getModelFromFile("scene_file.txt");
    loadIntoVAO(_fileName, program);


    // Add the models I'd like to draw in the scene

    
    //cMesh LoadingModels[30];
    //std::stringstream modelList[30];
    //for (int i = 0; i < _fileName.size(); i++) {
    //    modelList[i] << _fileName.at(i);
    //    strtok
    //    td::string a = "0.0f, 1.0f, 2.0f";
    //    LoadingModels[i].meshName = modelList[i].str();
    //    LoadingModels[i].orientationXYZ = glm::vec3(0.0f, 1.0f, 2.0f);
    //    LoadingModels[i].positionXYZ = glm::vec3(2.0f, 8.0f, 6.0f);
    //    g_vecMeshes.push_back(LoadingModels[i]);
    //}
    ////



    
    //cMesh bunny2;
    //bunny2.meshName = "bun_zipper_res2 (justXYZ).ply";

   // cMesh TheISS;
   // TheISS.meshName = "Assembled_ISS.ply";

    //g_vecMeshes.push_back(bunny2);
   // g_vecMeshes.push_back(TheISS);


    while ( ! glfwWindowShouldClose(window) )
    {
        float ratio;
        int width, height;
        glm::mat4 m, p, v, mvp;
//        mat4x4 m, p, mvp;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        // *******************************************************
        // Screen is cleared and we are ready to draw the scene...
        // *******************************************************



        for (unsigned int index = 0; index != g_vecMeshes.size(); index++)
        {

            m = glm::mat4(1.0f);
            //mat4x4_identity(m);

            glm::mat4 rotateZ = glm::rotate( glm::mat4(1.0f),
                                             0.0f,   //(float)glfwGetTime(),
                                             glm::vec3(0.0f, 0.0f, 1.0f));

    //        std::cout << (float)glfwGetTime() << std::endl;

            m = m * rotateZ;

            //        mat4x4_rotate_Z(m, m, (float)glfwGetTime());

            p = glm::perspective(0.6f,
                ratio,
                0.1f,
                1000.0f);

            v = glm::mat4(1.0f);

            //glm::vec3 cameraEye = glm::vec3(0.0, 0.0, -4.0f);
            glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
            glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

            v = glm::lookAt( cameraEye,
                             cameraTarget,
                             upVector);
    //        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        
            mvp = p * v * m;
    //        mat4x4_mul(mvp, p, m);

            glUseProgram(program);

            glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));
    //        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)mvp);

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                // GL_POINT, GL_LINE, and GL_FILL)

            // glPointSize(20.0f); sometimes this works... Hey, it's not my fault!


            sModelDrawInfo modelInfo;
    //        if (gVAOManager.FindDrawInfoByModelName("bun_zipper_res2 (justXYZ).ply", modelInfo))
    //        if (gVAOManager.FindDrawInfoByModelName("Assembled_ISS.ply", modelInfo))

            if ( gVAOManager.FindDrawInfoByModelName( g_vecMeshes[index].meshName, modelInfo) )
            {
                glBindVertexArray(modelInfo.VAO_ID);
                glDrawElements(GL_TRIANGLES,
                               modelInfo.numberOfIndices, 
                               GL_UNSIGNED_INT, 
                               (void*)0);
                glBindVertexArray(0);
            }


        }//for (unsigned int index
        // Scene is drawn



        // "Present" what we've drawn.
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}



void getModelFromFile(std::string fileName) {

    
    float f = 0.1f;
    int numofLines = 0;
    int i = 0;
    std::ifstream theFile(fileName);
    while (theFile.good())
    {
        std::string line;
        std::getline(theFile, line);
        ++numofLines;
    }

    std::ifstream theFileGet(fileName);
    if (!theFileGet.is_open())
    {
        std::cout << "ERROR: Didn't open the file" << std::endl;
    }
    else {
        for (int i = 0 ; i < numofLines ; i++) {

            cMesh LoadingModels;
            float posAndOri[7];
            theFileGet >> LoadingModels.meshName;
            theFileGet >> posAndOri[0];
            theFileGet >> posAndOri[1];
            theFileGet >> posAndOri[2];
            theFileGet >> posAndOri[3];
            theFileGet >> posAndOri[4];
            theFileGet >> posAndOri[5];
            theFileGet >> posAndOri[6];
            LoadingModels.positionXYZ = glm::vec3(posAndOri[0], posAndOri[1], posAndOri[2]);
            LoadingModels.orientationXYZ = glm::vec3(posAndOri[3], posAndOri[4], posAndOri[5]);
            LoadingModels.scale = posAndOri[6];
            _fileName.push_back(LoadingModels.meshName);
            g_vecMeshes.push_back(LoadingModels);

        }
    }
}


void loadIntoVAO(std::vector<std::string> fileName, GLuint program) {
    sModelDrawInfo modelBunny;

    for (int i = 0; i < _fileName.size(); i++) {
        if (gVAOManager.LoadModelIntoVAO(_fileName[i], modelBunny, program))
        {
            std::cout << "Loaded the model OK" << std::endl;
        }
        else
        {
            std::cout << "Error: Didn't load the model OK" << std::endl;
        }
    }
    
}

