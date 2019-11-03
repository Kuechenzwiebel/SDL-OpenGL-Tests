//
//  main.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 06.03.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <cmath>
#include <list>
#include <utility>
#include <chrono>
#include <thread>
#include <mutex>
#include <iomanip>

#define GLEW_STATIC
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <SOIL2/SOIL2.h>

#define GLM_FORCE_SWIZZLE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/gtx/norm.hpp>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <HG_List/HG_List.h>

using namespace glm;

#include "texture.hpp"
#include "shader.hpp"
#include "object.hpp"
#include "triangle.hpp"
#include "utils.hpp"
#include "camera.hpp"
#include "rectangle.hpp"
#include "ui/uiObject.hpp"
#include "ui/uiRectangle.hpp"
#include "ui/uiText.hpp"
#include "cube.hpp"
#include "cubemap.hpp"
#include "items/items.hpp"
#include "sphere.hpp"
#include "physicsObjects/collisionInfo.h"
#include "physicsObjects/physicsObject.hpp"
#include "physicsObjects/physicsSphere.hpp"
#include "perlinMap/mapChunk.hpp"
#include "physicsObjects/aabb.hpp"
#include "physicsObjects/obb.hpp"
#include "physicsObjects/ray.hpp"
#include "perlinMap/perlinNoise.hpp"
#include "physicsObjects/physicsWorld.hpp"
#include "objModel.hpp"
#include "perlinMap/map.hpp"

int windowWidth = 1080, windowHeight = 760;
//int windowWidth = 760, windowHeight = 760;
std::string windowTitle = "SDL-OpenGL-Tests-2";

bool running = true;
bool checkMouse = false;
bool inVehicle = false;
bool invOpen = false;
bool opticsOn = false;
bool wireframe = false;
bool render = true;
bool speedSelect = false;

vec3 oldCamFootPos;
vec2 oldCamMapPos;

bool sortDone = false;
bool sort = false;
std::mutex sortMutex;

int ipow(int x, unsigned int y) {
    unsigned int result = 1;
    for(int i = 0; i < y; i++) {
        result *= x;
    }
    return result;
}

float round(float value, unsigned int digits) {
    if(digits == 0.0f) {
        return round(value);
    }
    
    value *= ipow(10, digits);
    value = round(value);
    value /= ipow(10, digits);
    return value;
}

void objectSort(Camera *cam, std::list<std::pair<float, Object*>> *objects) {
    std::cout << "Sort Thread ID: " << std::this_thread::get_id() << std::endl;
    
    while(running) {
        if(sort) {
            sortMutex.lock();
            sortDone = false;
            sortMutex.unlock();
            
            if(cam->getFootPosition() != oldCamFootPos) {
                for(std::list<std::pair<float, Object*>>::iterator it = objects->begin(); it != objects->end(); it++) {
                    it->first = length(cam->getFootPosition() - it->second->getRealPosition());
                }
                
                objects->sort();
            }
            
            sortMutex.lock();
            sortDone = true;
            sort = false;
            sortMutex.unlock();
        }
        else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}

int main(int argc, const char * argv[]) {
    std::cout << "Main Thread ID: " << std::this_thread::get_id() << std::endl;
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Failed to initialize SDL2");
        return EXIT_FAILURE;
    }
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    
    SDL_Window *window = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    SDL_Event windowEvent;

    SDL_GL_SetSwapInterval(1);
    
    if (window == NULL) {
        printf(PRINTF_RED);
        printf("Failed to open Window!\n");
        printf(PRINTF_DEFAULT);
        return EXIT_FAILURE;
    }
    
    glewExperimental = GL_TRUE;
    
    if (glewInit() != GLEW_OK) {
        printf(PRINTF_RED);
        printf("Failed to initialize GLEW!\n");
        printf(PRINTF_DEFAULT);
        return EXIT_FAILURE;
    }
    
    glViewport(0, 0, windowWidth, windowHeight);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_MULTISAMPLE);
    glDisable(GL_CULL_FACE);  
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LEQUAL);
    
    mat4 projection = perspective(radians(45.0f), float(windowWidth) / float(windowHeight), 0.1f, 100.0f);
    mat4 uiProjection = ortho(-1.0f * float(windowWidth) / 1000.0f, 1.0f * float(windowWidth) / 1000.0f, -1.0f * float(windowHeight) / 1000.0f, 1.0f * float(windowHeight) / 1000.0f);
    
    int frame = 0;
    long nextMeasure = SDL_GetTicks() + 1e3;
    int fps = 0;
    unsigned long long totalFrames = 0;
    
    float lastFrame = 0.0f;
    float currentFrame = 0.0f;
    float deltaTime = 0.0f;
    
    float zoom = 1.0f;
    
    float runTime = 0.0f;
    
    std::list<std::pair<float, Object*>> objects;
    std::vector<UIObject*> uiObjects;
    PhysicsWorld physicsWorld;
    
    hg::File noTexShaderVertexFile("resources/shaders/noTex.vs"), noTexShaderFragmentFile("resources/shaders/noTex.fs");
    hg::File basicShaderVertexFile("resources/shaders/basic.vs"), basicShaderFragmentFile("resources/shaders/basic.fs");
    hg::File blurShaderVertexFile("resources/shaders/blur.vs"), blurShaderFragmentFile("resources/shaders/blur.fs");
    hg::File uiShaderVertexFile("resources/shaders/ui.vs"), uiShaderFragmentFile("resources/shaders/ui.fs");
    hg::File skyboxShaderVertexFile("resources/shaders/skybox.vs"), skyboxShaderFragmentFile("resources/shaders/skybox.fs");
    hg::File textShaderVertexFile("resources/shaders/text.vs"), textShaderFragmentFile("resources/shaders/text.fs");
    
    
    std::string skyboxLocation[6] = {
        "resources/textures/skyboxes/skybox2RT.png",
        "resources/textures/skyboxes/skybox2LF.png",
        "resources/textures/skyboxes/skybox2UP.png",
        "resources/textures/skyboxes/skybox2DN.png",
        "resources/textures/skyboxes/skybox2FT.png",
        "resources/textures/skyboxes/skybox2BK.png"
    };
    
    Shader noTexShader(noTexShaderVertexFile.readFile(), noTexShaderFragmentFile.readFile());
    Shader basicShader(basicShaderVertexFile.readFile(), basicShaderFragmentFile.readFile());
    Shader blurShader(blurShaderVertexFile.readFile(), blurShaderFragmentFile.readFile());
    Shader uiShader(uiShaderVertexFile.readFile(), uiShaderFragmentFile.readFile());
    Shader skyboxShader(skyboxShaderVertexFile.readFile(), skyboxShaderFragmentFile.readFile());
    Shader textShader(textShaderVertexFile.readFile(), textShaderFragmentFile.readFile());
    
    Camera cam(&deltaTime, &windowEvent, &checkMouse);
    cam.objects = &physicsWorld;
    
    RenderData renderData;
    RenderData skyboxData;
    RenderData uiData;
    RenderData minimapData;
    
    renderData.projection = &projection;
    renderData.viewMat = cam.getViewMatrix();
    renderData.windowWidth = &windowWidth;
    renderData.windowHeight = &windowHeight;
    
    skyboxData.projection = &projection;
    skyboxData.viewMat = mat4(mat3(cam.getViewMatrix()));
    skyboxData.windowWidth = &windowWidth;
    skyboxData.windowHeight = &windowHeight;
    
    uiData.projection = &uiProjection;
    uiData.viewMat = lookAt(vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    uiData.windowWidth = &windowWidth;
    uiData.windowHeight = &windowHeight;
    
    minimapData.projection = &projection;
    minimapData.viewMat = lookAt(vec3(0.0f, 100.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    minimapData.windowWidth = &windowWidth;
    minimapData.windowHeight = &windowHeight;
    
    
    Cubemap spaceSkybox(skyboxLocation);
    
    Texture gradientTexture("resources/textures/grad.png", TEXTURE_NO_MIP_MAP);
    Texture gradient2Texture("resources/models/grad.png", TEXTURE_NO_MIP_MAP);
    Texture nebulaTexture("resources/textures/nebel.jpg");
    Texture compassHeadbarTex("resources/textures/compassBar2.png");
    Texture compassSelectorTex("resources/textures/compassSelector.png", TEXTURE_NO_MIP_MAP);
    Texture declinationMeterTex("resources/textures/declinationMeter2.png");
    Texture declinationMeterSelectorTex("resources/textures/declinationMeterSelector.png", TEXTURE_NO_MIP_MAP);
    Texture jupiterTexture("resources/textures/jupiter.jpg");
    Texture stoneTexture("resources/textures/stones.png");
    Texture invTexture("resources/textures/inv.png", TEXTURE_NO_MIP_MAP);
    Texture crosshairTexture("resources/textures/crosshair.png", TEXTURE_NO_MIP_MAP);
    Texture speedSelectionBackroundTexture("resources/textures/speedSelectionBackround.png", TEXTURE_NO_MIP_MAP);
    
    
    PerlinNoise noise(420);
    noise.frequency = 200.0f;
    noise.multiplier = 40.0f;
    noise.octaves = 4;
    noise.offset = -2.0f;
    
    cam.setMapNoise(&noise);
    
    Map map(&noise, &basicShader, &renderData);
    
    Cube aabbTest(&basicShader, &renderData);
    aabbTest.setTexture(&gradient2Texture);
    aabbTest.setPosition(vec3(3.0f, -1.0f, 0.0f));
    objects.push_back(std::make_pair(0.0f, &aabbTest));
    
    AABB aabb1(vec3(4.5f, -1.5f, -0.5f), vec3(5.5f, -0.5f, 0.5f));
    physicsWorld.objects.push_back(&aabb1);
    
    Triangle tri(&noTexShader, &renderData);
    tri.setPosition(vec3(0.0f, 0.0f, -1.5f));
    tri.calculateModelMat();
    
    Triangle tri2(&noTexShader, &renderData);
    tri2.setPosition(vec3(-1.0f, 0.0f, 2.0f));
    tri2.calculateModelMat();
    
    objects.push_back(std::make_pair(0.0f, &tri));
    objects.push_back(std::make_pair(0.0f, &tri2));

    vec2 compassUVs[] = {
        vec2(0.0832f * 1.5f, 0.0f),
        vec2(0.0832f * 2.5f, 0.0f),
        vec2(0.0832f * 2.5f, 1.0f),
        vec2(0.0832f * 1.5f, 0.0f),
        vec2(0.0832f * 1.5f, 1.0f),
        vec2(0.0832f * 2.5f, 1.0f)
    };
    
    vec2 pitchUVs[] = {
        vec2(0.0f, 0.5f - 0.088975694444444f),
        vec2(1.0f, 0.5f - 0.088975694444444f),
        vec2(1.0f, 0.5f + 0.088975694444444f),
        vec2(0.0f, 0.5f - 0.088975694444444f),
        vec2(0.0f, 0.5f + 0.088975694444444f),
        vec2(1.0f, 0.5f + 0.088975694444444f)
    };
    
    UIRectangle compassBar(&uiShader, &uiData, compassUVs);
    compassBar.setTexture(&compassHeadbarTex);
    compassBar.setXTexOffset(cam.getYawPointer());
    compassBar.setXTexMultiplier(0.00093f * 2.0f);
    compassBar.setPixelSize(vec2(682.0f, 128.0f) / vec2(4.0f));
    uiObjects.push_back(&compassBar);
    
    UIRectangle declinationMeterBar(&uiShader, &uiData, pitchUVs);
    declinationMeterBar.setTexture(&declinationMeterTex);
    declinationMeterBar.setYTexOffset(cam.getPitchPointer());
    declinationMeterBar.setYTexMultiplier(-0.004448784722222f);
    declinationMeterBar.setPixelSize(vec2(128.0f, 410.0f) / vec2(4.0f));
    uiObjects.push_back(&declinationMeterBar);
    
    UIRectangle compassSelector(&uiShader, &uiData);
    compassSelector.setTexture(&compassSelectorTex);
    compassSelector.setPixelSize(vec2(256.0f / 4.0f));
    uiObjects.push_back(&compassSelector);
    
    UIRectangle declinationMeterSelector(&uiShader, &uiData);
    declinationMeterSelector.setTexture(&declinationMeterSelectorTex);
    declinationMeterSelector.setPixelSize(vec2(256.0f / 4.0f));
    uiObjects.push_back(&declinationMeterSelector);
    
    
    UIRectangle inv(&uiShader, &uiData);
    inv.setTexture(&invTexture);
    inv.setPixelSize(vec2(512.0f, 256.0f));
    
    UIRectangle speedSelectionBackround(&uiShader, &uiData);
    speedSelectionBackround.setTexture(&speedSelectionBackroundTexture);
    speedSelectionBackround.setPixelSize(vec2(256.0f, 128.0f));
    
    
    UIRectangle crosshair(&uiShader, &uiData);
    crosshair.setTexture(&crosshairTexture);
    crosshair.setPixelSize(vec2(25.0f));
    uiObjects.push_back(&crosshair);
    
    
    Cube skybox(&skyboxShader, &skyboxData);
    skybox.setCubemap(&spaceSkybox);
    
    Cube sharpCube(&basicShader, &renderData);
    sharpCube.setTexture(&nebulaTexture);
    sharpCube.setPosition(vec3(5.0f));
    sharpCube.setSize(vec3(2.0f));
    sharpCube.setRotation(vec4(0.0f, 1.0f, 1.0f, radians(45.0f)));
    objects.push_back(std::make_pair(0.0f, &sharpCube));
    
    
    
    Cube bluredCube(&blurShader, &renderData);
    bluredCube.setTexture(&nebulaTexture);
    bluredCube.setPosition(vec3(2.0f, 1.0f, -3.0f));
    bluredCube.setRotation(vec4(1.0f, 1.0f, 1.0f, radians(45.0f)));
    objects.push_back(std::make_pair(0.0f, &bluredCube));
    
    OBB obb1(sharpCube.getPosition(), sharpCube.getRotation(), sharpCube.getSize());
    physicsWorld.objects.push_back(&obb1);
    
    OBB obb2(bluredCube.getPosition(), bluredCube.getRotation(), bluredCube.getSize());
    physicsWorld.objects.push_back(&obb2);
    
    
    Cube farCube(&basicShader, &renderData);
    farCube.setTexture(&nebulaTexture);
    farCube.setPosition(vec3(0.0f, 0.0f, -110.0f));
    objects.push_back(std::make_pair(0.0f, &farCube));
    
    
    
    Sphere sphere(&basicShader, &renderData, &wireframe);
    sphere.setPosition(vec3(0.0f));
    sphere.setRadius(0.2f);
    sphere.setTexture(&gradientTexture);
    objects.push_back(std::make_pair(0.0f, &sphere));
    
    Sphere lightSource(&basicShader, &renderData);
    lightSource.setRadius(0.3f);
    lightSource.setTexture(&gradient2Texture);
    lightSource.setPosition(vec3(0.0f, 2.45f, 0.0f));
    objects.push_back(std::make_pair(0.0f, &lightSource));
    
    
    
    UIText fpsText("", &textShader, &uiData);
    fpsText.setSize(vec2(0.25f));
    uiObjects.push_back(&fpsText);
    
    UIText positionText("", &textShader, &uiData);
    positionText.setSize(vec2(0.25f));
    uiObjects.push_back(&positionText);
    
    UIText speedTextms("", &textShader, &uiData);
    speedTextms.setSize(vec2(0.25f));
    
    UIText speedTextkmh("", &textShader, &uiData);
    speedTextkmh.setSize(vec2(0.25f));
    
    
    UIText speedSelectionInspectionSpeedText("Inspection speed 0.6m/s", &textShader, &uiData);
    speedSelectionInspectionSpeedText.setSize(vec2(0.25f));
    
    UIText speedSelectionRegularSpeedText("Regular speed 1.6m/s", &textShader, &uiData);
    speedSelectionRegularSpeedText.setSize(vec2(0.25f));
    
    UIText speedSelectionSprintingSpeedText("Sprinting speed 4.6m/s", &textShader, &uiData);
    speedSelectionSprintingSpeedText.setSize(vec2(0.25f));
    
    
    UIText rayText("Ray hit:", &textShader, &uiData);
    rayText.setSize(vec2(0.25f));
    rayText.setPixelPosition(vec2(100.0f));
    uiObjects.push_back(&rayText);
    
    
    Sphere jupiter(&basicShader, &renderData);
    jupiter.setTexture(&jupiterTexture);
    jupiter.setPosition(vec3(10.0f));
    objects.push_back(std::make_pair(0.0f, &jupiter));
    
    
    
    oldCamFootPos = cam.getFootPosition();
    
    PhysicsSphere s1(0.2f, vec3(0.0f)), s2(1.0f, vec3(1.0f, 1.0f, 0.0f));
    CollisionInfo in = spherePointCollision(&s1, vec3(0.5f, 0.5f, 0.0f));
    in = spherePointCollision(&s1, vec3(1.0f, 0.5f, 0.0f));
    
    physicsWorld.objects.push_back(&s1);
    
    compassSelector.setPixelPosition(vec2(0.0f, (float(windowHeight) / 2.0f) - 32.0f));
    declinationMeterSelector.setPixelPosition(vec2((-float(windowWidth) / 2.0f) + 32.0f, 0.0f));
    compassBar.setPixelPosition(vec2(0.0f, (float(windowHeight) / 2.0f) - 32.0f));
    declinationMeterBar.setPixelPosition(vec2((-float(windowWidth) / 2.0f) + 32.0f, 0.0f));
    fpsText.setPixelPosition(vec2(-float(windowWidth) / 2.0f + (charWidth / 2.0f) * 0.25f, float(windowHeight) / 2.0f - (charHeight / 2.0f) * 0.25f));
    positionText.setPixelPosition(vec2(-float(windowWidth) / 2.0f + (charWidth / 2.0f) * 0.25f, -float(windowHeight) / 2.0f + (charHeight / 2.0f) * 0.25f));
    
    speedSelectionInspectionSpeedText.setPixelPosition(vec2(-128.0f + 15.0f, 64.0f - 15.0f));
    speedSelectionRegularSpeedText.setPixelPosition(vec2(-128.0f + 15.0f, 0.0f));
    speedSelectionSprintingSpeedText.setPixelPosition(vec2(-128.0f + 15.0f, -64.0f + 15.0f));
    
    Ray crosshairRay(vec3(0.0f), vec3(0.0f), 0.1f);
    bool crosshairRayCollision = false;
    
    
    ObjModel axisFront("resources/models/axis.obj", &basicShader, &renderData, &wireframe);
    axisFront.setPosition(vec3(1.65f, 2.0f, 0.0f));
    objects.push_back(std::make_pair(0.0f, &axisFront));
    
    ObjModel axisBack("resources/models/axis.obj", &basicShader, &renderData, &wireframe);
    axisBack.setPosition(vec3(-1.65f, 2.0f, 0.0f));
    objects.push_back(std::make_pair(0.0f, &axisBack));
    
    
    ObjModel base("resources/models/vehicle new/Base.obj", &basicShader, &renderData, &wireframe);
    base.setRealPosition(vec3(1000000.0f));
    objects.push_back(std::make_pair(0.0f, &base));
    
    
    ObjModel backWindow1("resources/models/vehicle new/Back_Window_1.obj", &basicShader, &renderData, &wireframe);
    ObjModel backWindow2("resources/models/vehicle new/Back_Window_2.obj", &basicShader, &renderData, &wireframe);
    
//    objects.push_back(std::make_pair(0.0f, &backWindow1));
//    objects.push_back(std::make_pair(0.0f, &backWindow2));
    
    
    ObjModel backSideWindow1("resources/models/vehicle new/Back_Side_Window_1.obj", &basicShader, &renderData, &wireframe);
    ObjModel backSideWindow2("resources/models/vehicle new/Back_Side_Window_2.obj", &basicShader, &renderData, &wireframe);
    ObjModel backSideWindow3("resources/models/vehicle new/Back_Side_Window_3.obj", &basicShader, &renderData, &wireframe);
    ObjModel backSideWindow4("resources/models/vehicle new/Back_Side_Window_4.obj", &basicShader, &renderData, &wireframe);
    /*
    objects.push_back(std::make_pair(0.0f, &backSideWindow1));
    objects.push_back(std::make_pair(0.0f, &backSideWindow2));
    objects.push_back(std::make_pair(0.0f, &backSideWindow3));
    objects.push_back(std::make_pair(0.0f, &backSideWindow4));
    */
    
    ObjModel middleWindow1("resources/models/vehicle new/Middle_Window_1.obj", &basicShader, &renderData, &wireframe);
    ObjModel middleWindow2("resources/models/vehicle new/Middle_Window_2.obj", &basicShader, &renderData, &wireframe);
    ObjModel middleWindow3("resources/models/vehicle new/Middle_Window_3.obj", &basicShader, &renderData, &wireframe);
    ObjModel middleWindow4("resources/models/vehicle new/Middle_Window_4.obj", &basicShader, &renderData, &wireframe);
    /*
    objects.push_back(std::make_pair(0.0f, &middleWindow1));
    objects.push_back(std::make_pair(0.0f, &middleWindow2));
    objects.push_back(std::make_pair(0.0f, &middleWindow3));
    objects.push_back(std::make_pair(0.0f, &middleWindow4));
    */
    
    ObjModel frontSideWindow1("resources/models/vehicle new/Front_Side_Window_1.obj", &basicShader, &renderData, &wireframe);
    ObjModel frontSideWindow2("resources/models/vehicle new/Front_Side_Window_2.obj", &basicShader, &renderData, &wireframe);
    ObjModel frontSideWindow3("resources/models/vehicle new/Front_Side_Window_3.obj", &basicShader, &renderData, &wireframe);
    ObjModel frontSideWindow4("resources/models/vehicle new/Front_Side_Window_4.obj", &basicShader, &renderData, &wireframe);
    /*
    objects.push_back(std::make_pair(0.0f, &frontSideWindow1));
    objects.push_back(std::make_pair(0.0f, &frontSideWindow2));
    objects.push_back(std::make_pair(0.0f, &frontSideWindow3));
    objects.push_back(std::make_pair(0.0f, &frontSideWindow4));
    */
    
    ObjModel frontWindow1("resources/models/vehicle new/Front_Window_1.obj", &basicShader, &renderData, &wireframe);
    ObjModel frontWindow2("resources/models/vehicle new/Front_Window_2.obj", &basicShader, &renderData, &wireframe);
    
//    objects.push_back(std::make_pair(0.0f, &frontWindow1));
//    objects.push_back(std::make_pair(0.0f, &frontWindow2));
    

    vec3 axisFrontMiddlePosition, axisBackMiddlePosition;
    vec3 axisFrontRight, axisBackRight;
    vec3 axisFrontLeft, axisBackLeft;
    
    float wheelDistance = 1.73f;
    float wheelDiameter = 0.76f;
    float alphaFrontRight = 0.0f, alphaBackRight = 0.0f;
    float alphaFrontLeft = 0.0f, alphaBackLeft = 0.0f;
    
    float valpha = 0.0f;
    float va = 3.18f, vb = 0.0f;
    
    float totalRotation = 0.0f;
    float totalRotationChange = 0.0f;
    vec3 vehicleMidPosition(0.0f, 4.0f, 0.0f);
    
    mat4 totalRotationMat(1);
    
    mat4 vehicleModelMat(1);
    mat4 vehicleWindowRotation(1);
    vec3 vehicleWindowPosition;
    float vehicleSpeed = 0.0f;
    
    std::thread sortThread(objectSort, &cam, &objects);
    
    srand(3982765348);
    
    while(running) {
        sortMutex.lock();
        sort = true;
        sortMutex.unlock();
        
        if(SDL_GetTicks() > nextMeasure) {
            fps = frame;
            frame = 0;
            nextMeasure += 1000;
            
            fpsText.setText(std::to_string(fps) + " FPS" + "\nFrametime: " + std::to_string(1000.0f / fps) + " ms");
        }
        
        currentFrame = SDL_GetTicks() / 1000.0f;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        SDL_SetWindowTitle(window, (windowTitle + "     FPS: " + std::to_string(fps) +  "  Frametime: " + std::to_string(1000.0f / fps) + " ms    Camera Pos X: " + std::to_string(cam.getFootPosition().x) + " Y: " + std::to_string(cam.getFootPosition().y) + " Z: " + std::to_string(cam.getFootPosition().z)).c_str());
     
        while(SDL_PollEvent(&windowEvent) != 0) {
            if(windowEvent.type == SDL_QUIT)
                running = false;
            
            if(windowEvent.type == SDL_MOUSEBUTTONDOWN) {
                if(windowEvent.button.button == SDL_BUTTON_LEFT) {
                    checkMouse = true;
                    render = true;
                }
            }
            
            if(windowEvent.type == SDL_WINDOWEVENT) {
                if(windowEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
                    
                    compassSelector.setPixelPosition(vec2(0.0f, (float(windowHeight) / 2.0f) - 32.0f));
                    declinationMeterSelector.setPixelPosition(vec2((-float(windowWidth) / 2.0f) + 32.0f, 0.0f));
                    compassBar.setPixelPosition(vec2(0.0f, (float(windowHeight) / 2.0f) - 32.0f));
                    declinationMeterBar.setPixelPosition(vec2((-float(windowWidth) / 2.0f) + 32.0f, 0.0f));
                    fpsText.setPixelPosition(vec2(-float(windowWidth) / 2.0f + (charWidth / 2.0f) * 0.25f, float(windowHeight) / 2.0f - (charHeight / 2.0f) * 0.25f));
                    positionText.setPixelPosition(vec2(-float(windowWidth) / 2.0f + (charWidth / 2.0f) * 0.25f, -float(windowHeight) / 2.0f + (charHeight / 2.0f) * 0.25f));
                    
                    speedSelectionInspectionSpeedText.setPixelPosition(vec2(-128.0f + 15.0f, 64.0f - 15.0f));
                    speedSelectionRegularSpeedText.setPixelPosition(vec2(-128.0f + 15.0f, 0.0f));
                    speedSelectionSprintingSpeedText.setPixelPosition(vec2(-128.0f + 15.0f, -64.0f + 15.0f));
                }
                
                if(windowEvent.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
                    render = true;
                
                if(windowEvent.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
                    render = false;
            }
            
            if(windowEvent.type == SDL_MOUSEWHEEL) {
                totalRotation += float(windowEvent.wheel.x) * 0.025f;
                
//                vehicleSpeed += float(windowEvent.wheel.y) * 0.15f;
            }
            
            if(windowEvent.type == SDL_KEYDOWN) {
                if(windowEvent.key.keysym.sym == SDLK_i && !speedSelect)
                    swapBool(&invOpen);
                
                if(windowEvent.key.keysym.sym == SDLK_o)
                    swapBool(&opticsOn);
                
                if(windowEvent.key.keysym.sym == SDLK_f)
                    swapBool(&wireframe);
                
                if(windowEvent.key.keysym.sym == SDLK_v)
                    swapBool(&inVehicle);
                
                if(windowEvent.key.keysym.sym == SDLK_c && !invOpen) {
                    swapBool(&speedSelect);
                    swapBool(&checkMouse);
                }
                
                if(inVehicle) {
                    if(windowEvent.key.keysym.sym == SDLK_a)
                        totalRotationChange += 5.0f * deltaTime;
                    
                    if(windowEvent.key.keysym.sym == SDLK_d)
                        totalRotationChange -= 5.0f * deltaTime;
                }
                
                if(windowEvent.key.keysym.sym == SDLK_ESCAPE) {
                    render = false;
                    checkMouse = false;
                }
            }
            
            if(!invOpen && !speedSelect)
                cam.processMouseInput();
            
            if(speedSelect) {
                if(windowEvent.motion.x - windowWidth / 2.0f  >= -119 && windowEvent.motion.x - windowWidth / 2.0f  <= 104 &&
                   windowEvent.motion.y - windowHeight / 2.0f >= -58  && windowEvent.motion.y - windowHeight / 2.0f <= -41) {
                    speedSelectionInspectionSpeedText.setColorMultiplier(vec4(1.0f));
                    if(windowEvent.type == SDL_MOUSEBUTTONDOWN) {
                        if(windowEvent.button.button == SDL_BUTTON_LEFT) {
                            swapBool(&speedSelect);
                            cam.setMovementSpeed(INSPECTION);
                        }
                    }
                }
                else
                    speedSelectionInspectionSpeedText.setColorMultiplier(vec4(0.8f, 0.8f, 0.8f, 1.0f));
                
                if(windowEvent.motion.x - windowWidth / 2.0f  >= -119 && windowEvent.motion.x - windowWidth / 2.0f  <= 92 &&
                   windowEvent.motion.y - windowHeight / 2.0f >= -10  && windowEvent.motion.y - windowHeight / 2.0f <= 10) {
                    speedSelectionRegularSpeedText.setColorMultiplier(vec4(1.0f));
                    if(windowEvent.type == SDL_MOUSEBUTTONDOWN) {
                        if(windowEvent.button.button == SDL_BUTTON_LEFT) {
                            swapBool(&speedSelect);
                            cam.setMovementSpeed(REGULAR);
                        }
                    }
                }
                else
                    speedSelectionRegularSpeedText.setColorMultiplier(vec4(0.8f, 0.8f, 0.8f, 1.0f));
                
                if(windowEvent.motion.x - windowWidth / 2.0f  >= -119 && windowEvent.motion.x - windowWidth / 2.0f  <= 113 &&
                   windowEvent.motion.y - windowHeight / 2.0f >= 39   && windowEvent.motion.y - windowHeight / 2.0f <= 58) {
                    speedSelectionSprintingSpeedText.setColorMultiplier(vec4(1.0f));
                    if(windowEvent.type == SDL_MOUSEBUTTONDOWN) {
                        if(windowEvent.button.button == SDL_BUTTON_LEFT) {
                            swapBool(&speedSelect);
                            cam.setMovementSpeed(SPRINTING);
                        }
                    }
                }
                else
                    speedSelectionSprintingSpeedText.setColorMultiplier(vec4(0.8f, 0.8f, 0.8f, 1.0f));
            }
        }
        
        if(checkMouse && !speedSelect)
            SDL_SetRelativeMouseMode(SDL_TRUE);
        else
            SDL_SetRelativeMouseMode(SDL_FALSE);
        
        if(render) {
            totalRotation += totalRotationChange;
            fadeToZero(&totalRotationChange, deltaTime, 30.0f);
            
            vehicleMidPosition.x += vehicleSpeed * deltaTime * cos(totalRotation);
            vehicleMidPosition.z += vehicleSpeed * deltaTime * -sin(totalRotation);
            
            totalRotationMat = rotate(mat4(1), totalRotation, vec3(0.0f, 1.0f, 0.0f));
            cam.inVehicle = inVehicle;
            
            
            axisFrontMiddlePosition = (translate(mat4(1), vehicleMidPosition) *
                                       totalRotationMat *
                                       vec4(1.5215f, 0.0f, 0.0f, 1.0f)).xyz();
            
            axisBackMiddlePosition = (translate(mat4(1), vehicleMidPosition) *
                                      totalRotationMat *
                                      vec4(-1.5215f, 0.0f, 0.0f, 1.0f)).xyz();
            
            axisFrontMiddlePosition.y = noise.octaveNoise(axisFrontMiddlePosition.x, axisFrontMiddlePosition.z);
            axisBackMiddlePosition.y = noise.octaveNoise(axisBackMiddlePosition.x, axisBackMiddlePosition.z);

            
            axisFrontRight = (translate(mat4(1), vehicleMidPosition) *
                              totalRotationMat *
                              vec4(1.5215f, 0.0f, wheelDistance / 2.0f, 1.0f)).xyz();
            
            axisBackRight = (translate(mat4(1), vehicleMidPosition) *
                             totalRotationMat *
                             vec4(-1.5215f, 0.0f, wheelDistance / 2.0f, 1.0f)).xyz();
            
            axisFrontLeft = (translate(mat4(1), vehicleMidPosition) *
                             totalRotationMat *
                             vec4(1.5215f, 0.0f, -wheelDistance / 2.0f, 1.0f)).xyz();
            
            axisBackLeft = (translate(mat4(1), vehicleMidPosition) *
                            totalRotationMat *
                            vec4(-1.5215f, 0.0f, -wheelDistance / 2.0f, 1.0f)).xyz();
            
            
            axisFrontRight.y = noise.octaveNoise(axisFrontRight.x, axisFrontRight.z);
            axisFrontLeft.y  = noise.octaveNoise(axisFrontLeft.x, axisFrontLeft.z);
            axisBackRight.y  = noise.octaveNoise(axisBackRight.x, axisBackRight.z);
            axisBackLeft.y   = noise.octaveNoise(axisBackLeft.x, axisBackLeft.z);
    
            
            alphaFrontRight = atan((axisFrontMiddlePosition.y - axisFrontRight.y) / (wheelDistance * 0.5f));
            alphaBackRight =  atan((axisBackMiddlePosition.y - axisBackRight.y) / (wheelDistance * 0.5f));
            
            alphaFrontLeft = -atan((axisFrontMiddlePosition.y - axisFrontLeft.y) / (wheelDistance * 0.5f));
            alphaBackLeft =  -atan((axisBackMiddlePosition.y - axisBackLeft.y) / (wheelDistance * 0.5f));
            
            
            vb = (axisFrontMiddlePosition.y + axisBackMiddlePosition.y) / 2.0f;
            valpha = atan((axisFrontMiddlePosition.y - (axisBackMiddlePosition.y / 2.0f)) / -1.5215f);
            
            
            
            
            base.setModelMat(translate(mat4(1), vec3(vehicleMidPosition.x, (axisFrontMiddlePosition.y + axisBackMiddlePosition.y) / 2.0f + wheelDiameter * 5.0f / 8.0f, vehicleMidPosition.z)) *
                             totalRotationMat *
                             rotate(mat4(1), valpha, vec3(0.0f, 0.0f, 1.0f)));
            
            axisFront.setModelMat(translate(mat4(1), vec3(vehicleMidPosition.x, axisFrontMiddlePosition.y + wheelDiameter * 5.0f / 8.0f, vehicleMidPosition.z)) *
                                  totalRotationMat *
                                  translate(mat4(1), vec3(1.5215f, 0.0f, 0.0f)) *
                                  rotate(mat4(1), fmin(alphaFrontRight, alphaFrontRight), vec3(1.0f, 0.0f, 0.0f)));
            
            axisBack.setModelMat(translate(mat4(1), vec3(vehicleMidPosition.x, axisBackMiddlePosition.y + wheelDiameter * 5.0f / 8.0f, vehicleMidPosition.z)) *
                                 totalRotationMat *
                                 translate(mat4(1), vec3(-1.5215f, 0.0f, 0.0f)) *
                                 rotate(mat4(1), fmin(alphaBackRight, alphaBackLeft), vec3(1.0f, 0.0f, 0.0f)));
            
            
            if(inVehicle) {
                cam.setFootPosition(vec3(vehicleMidPosition.x, (axisFrontMiddlePosition.y + axisBackMiddlePosition.y) / 2.0f, vehicleMidPosition.z) +
                                (rotate(mat4(1), totalRotation, vec3(0.0f, 1.0f, 0.0f)) *
                                 rotate(mat4(1), valpha, vec3(0.0f, 0.0f, 1.0f)) *
                                 rotate(mat4(1), fmin(fmin(alphaFrontRight, alphaFrontLeft), fmin(alphaBackRight, alphaBackLeft)), vec3(1.0f, 0.0f, 0.0f)) *
                                 vec4(0.35f, 0.45f + 0.97f, -0.45f, 1.0f)).xyz());
            }
            
            /*
            backWindow1.setModelMat(vehicleModelMat);
            backWindow2.setModelMat(vehicleModelMat);
            
            backSideWindow1.setModelMat(vehicleModelMat);
            backSideWindow2.setModelMat(vehicleModelMat);
            backSideWindow3.setModelMat(vehicleModelMat);
            backSideWindow4.setModelMat(vehicleModelMat);
            
            middleWindow1.setModelMat(vehicleModelMat);
            middleWindow2.setModelMat(vehicleModelMat);
            middleWindow3.setModelMat(vehicleModelMat);
            middleWindow4.setModelMat(vehicleModelMat);
            
            frontSideWindow1.setModelMat(vehicleModelMat);
            frontSideWindow2.setModelMat(vehicleModelMat);
            frontSideWindow3.setModelMat(vehicleModelMat);
            frontSideWindow4.setModelMat(vehicleModelMat);
            
            frontWindow1.setModelMat(vehicleModelMat);
            frontWindow2.setModelMat(vehicleModelMat);
            
            
            backWindow1.setRealPosition(vehicleWindowPosition + (vehicleWindowRotation * vec4(vec3(-0.95f, 0.33f, 0.0f) - vec3(0.98f, 0.0f, 0.0f), 1.0f)).xyz());
            backWindow2.setRealPosition(vehicleWindowPosition + (vehicleWindowRotation * vec4(vec3(-0.9f, 0.33f, 0.0f) - vec3(0.98f, 0.0f, 0.0f), 1.0f)).xyz());
            
            backSideWindow1.setRealPosition(vehicleWindowPosition + (vehicleWindowRotation * vec4(vec3(-0.2f, 0.25f, 0.84f) - vec3(0.98f, 0.0f, 0.0f), 1.0f)).xyz());
            backSideWindow2.setRealPosition(vehicleWindowPosition + (vehicleWindowRotation * vec4(vec3(-0.2f, 0.24f, 0.78f) - vec3(0.98f, 0.0f, 0.0f), 1.0f)).xyz());
            backSideWindow3.setRealPosition(vehicleWindowPosition + (vehicleWindowRotation * vec4(vec3(-0.2f, 0.25f, -0.84f) - vec3(0.98f, 0.0f, 0.0f), 1.0f)).xyz());
            backSideWindow4.setRealPosition(vehicleWindowPosition + (vehicleWindowRotation * vec4(vec3(-0.2f, 0.25f, -0.79f) - vec3(0.98f, 0.0f, 0.0f), 1.0f)).xyz());
            
            middleWindow1.setRealPosition(vehicleWindowPosition + (vehicleWindowRotation * vec4(vec3(0.45f, 0.34f, -0.84f) - vec3(0.98f, 0.0f, 0.0f), 1.0f)).xyz());
            middleWindow2.setRealPosition(vehicleWindowPosition + (vehicleWindowRotation * vec4(vec3(0.45f, 0.34f, -0.78f) - vec3(0.98f, 0.0f, 0.0f), 1.0f)).xyz());
            middleWindow3.setRealPosition(vehicleWindowPosition + (vehicleWindowRotation * vec4(vec3(0.45f, 0.34f, 0.77f) - vec3(0.98f, 0.0f, 0.0f), 1.0f)).xyz());
            middleWindow4.setRealPosition(vehicleWindowPosition + (vehicleWindowRotation * vec4(vec3(0.45f, 0.34f, 0.83f) - vec3(0.98f, 0.0f, 0.0f), 1.0f)).xyz());
            
            frontSideWindow1.setRealPosition(vehicleWindowPosition + (vehicleWindowRotation * vec4(vec3(1.32f, 0.34f, -0.84f) - vec3(0.98f, 0.0f, 0.0f), 1.0f)).xyz());
            frontSideWindow2.setRealPosition(vehicleWindowPosition + (vehicleWindowRotation * vec4(vec3(1.32f, 0.33f, -0.78f) - vec3(0.98f, 0.0f, 0.0f), 1.0f)).xyz());
            frontSideWindow3.setRealPosition(vehicleWindowPosition + (vehicleWindowRotation * vec4(vec3(1.32f, 0.33f, 0.77f) - vec3(0.98f, 0.0f, 0.0f), 1.0f)).xyz());
            frontSideWindow4.setRealPosition(vehicleWindowPosition + (vehicleWindowRotation * vec4(vec3(1.32f, 0.33f, 0.83f) - vec3(0.98f, 0.0f, 0.0f), 1.0f)).xyz());
            
            frontWindow1.setRealPosition(vehicleWindowPosition + (vehicleWindowRotation * vec4(vec3(1.79f, 0.38f, 0.0f) - vec3(0.98f, 0.0f, 0.0f), 1.0f)).xyz());
            frontWindow2.setRealPosition(vehicleWindowPosition + (vehicleWindowRotation * vec4(vec3(1.75f, 0.36f, 0.0f) - vec3(0.98f, 0.0f, 0.0f), 1.0f)).xyz());
            */
            
            
            
            
            
            
            
            if(!invOpen)
                cam.processInput();
            
            if(wireframe == true)
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            else
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
            
            
            
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            glViewport(0, 0, windowWidth, windowHeight);
            if(!opticsOn) {
                projection = infinitePerspective(radians(46.9f), float(windowWidth) / float(windowHeight), 0.005f);
                cam.setMouseSensitivity(0.25f);
            }
            else {
                projection = infinitePerspective(radians(zoom), float(windowWidth) / float(windowHeight), 0.5f);
                cam.setMouseSensitivity(0.0025f);
            }
            
            while(!sortDone) {
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
            
            uiProjection = ortho(-1.0f * float(windowWidth) / 1000.0f, 1.0f * float(windowWidth) / 1000.0f, -1.0f * float(windowHeight) / 1000.0f, 1.0f * float(windowHeight) / 1000.0f);
            renderData.viewMat = cam.getViewMatrix();
            skyboxData.viewMat = mat4(mat3(cam.getViewMatrix()));
            
            
            glDepthMask(GL_FALSE);
            skyboxShader.use();
            skybox.render();
            glDepthMask(GL_TRUE);
            
            
            basicShader.use();
            if(oldCamMapPos != glm::vec2(float((int(round(cam.getFootPosition().x)) / CHUNK_SIZE) * CHUNK_SIZE), float((int(round(cam.getFootPosition().z)) / CHUNK_SIZE) * CHUNK_SIZE))) {
                map.update(cam.getFootPosition());
            }
            map.render();
            
            for(std::list<std::pair<float, Object*>>::reverse_iterator it = objects.rbegin(); it != objects.rend(); it++) {
                it->second->getShaderPointer()->use();
                if(it->second->getShaderPointer() == &basicShader)
                    it->second->getShaderPointer()->sendVec3(cam.getEyePosition(), "viewPos");
                it->second->render();
            }
            

            
            glClear(GL_DEPTH_BUFFER_BIT);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            
            crosshairRay.setRayStartPosition(cam.getEyePosition());
            crosshairRay.setRayDirection(cam.getEyePosition() + cam.getFront());
            crosshairRay.reset();
            
            crosshairRayCollision = false;
            
            for(int i = 0; i < 50; i++) {
                crosshairRayCollision = worldPointCollision(&physicsWorld, crosshairRay.getRayPosition());
                if(crosshairRayCollision)
                    break;
                crosshairRay.step();
            }
            
            if(crosshairRayCollision)
                rayText.setText("Ray hit!");
            else
                rayText.setText("No ray hit!");
            
            
            if(oldCamFootPos != cam.getFootPosition()) {
                std::stringstream positionStream;
                positionStream << std::fixed << std::setprecision(2) << "Position:   X: " << cam.getFootPosition().x << "  Y: " << cam.getFootPosition().y << "  Z: " << cam.getFootPosition().z;
                positionText.setText(positionStream.str());
            }
            
            if(inVehicle) {
                std::stringstream speedStreamms;
                speedStreamms << std::fixed << std::setprecision(2) << vehicleSpeed << "m/s";
                speedTextms.setPixelPosition(vec2(float(windowWidth) / 2.0f - (charWidth * 0.25f * speedStreamms.str().length()) + (charWidth / 2.0f * 0.25f), float(windowHeight) / 2.0f - (charHeight / 2.0f) * 0.25f));
                speedTextms.setText(speedStreamms.str());
                
                std::stringstream speedStreamkmh;
                speedStreamkmh << std::fixed << std::setprecision(1) << vehicleSpeed * 3.6f << "km/h";
                speedTextkmh.setPixelPosition(vec2(float(windowWidth) / 2.0f - (charWidth * 0.25f * speedStreamkmh.str().length()) + (charWidth / 2.0f * 0.25f), float(windowHeight) / 2.0f - (charHeight * 1.5f) * 0.25f));
                speedTextkmh.setText(speedStreamkmh.str());
            }
            
            
            for(int i = 0; i < uiObjects.size(); i++) {
                uiObjects[i]->getShaderPointer()->use();
                uiObjects[i]->render();
            }
            
            if(inVehicle) {
                uiShader.use();
                
                speedTextms.render();
                speedTextkmh.render();
            }
            
            if(invOpen)
                inv.render();
            
            if(speedSelect) {
                speedSelectionBackround.render();
                speedSelectionInspectionSpeedText.render();
                speedSelectionRegularSpeedText.render();
                speedSelectionSprintingSpeedText.render();
            }

            
            frame ++;
            totalFrames++;
            runTime += deltaTime;
            oldCamFootPos = cam.getFootPosition();
            oldCamMapPos = glm::vec2(float((int(round(cam.getFootPosition().x)) / CHUNK_SIZE) * CHUNK_SIZE), float((int(round(cam.getFootPosition().z)) / CHUNK_SIZE) * CHUNK_SIZE));
            
            SDL_GL_SwapWindow(window);
            glFlush();
        }
        else
            SDL_Delay(33);
    }
    
    sortThread.detach();
    
    SDL_SetRelativeMouseMode(SDL_FALSE);
    
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
