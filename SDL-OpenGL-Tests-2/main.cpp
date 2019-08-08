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

#define GLEW_STATIC
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <SOIL2/SOIL2.h>

#define  GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace glm;

#include "file.hpp"
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
#include "items/items.h"
#include "sphere.hpp"
#include "physicsObjects/collisionInfo.h"
#include "physicsObjects/physicsObject.hpp"
#include "physicsObjects/physicsSphere.hpp"
#include "perlinMap.hpp"

int windowWidth = 1080, windowHeight = 760;
std::string windowTitle = "SDL-OpenGL-Tests-2";

bool antiAliasing = false;
bool running = true;
bool checkMouse = false;
bool invOpen = false;
bool opticsOn = false;
bool wireframe = false;

int main(int argc, const char * argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Failed to initialize SDL2");
        return EXIT_FAILURE;
    }
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    if(antiAliasing) {
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 64);
    }
    
    SDL_Window *window = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    
    SDL_GL_SetSwapInterval(1);
    
    SDL_GLContext context = SDL_GL_CreateContext(window);
    SDL_Event windowEvent;

    if (window == NULL) {
        printf("Failed to open Window");
        return EXIT_FAILURE;
    }
    
    glewExperimental = GL_TRUE;
    
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW");
        return EXIT_FAILURE;
    }
    
    glViewport(0, 0, windowWidth, windowHeight);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_MULTISAMPLE);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LEQUAL);
    
    mat4 projection = perspective(radians(45.0f), float(windowWidth) / float(windowHeight), 0.1f, 100.0f);
    mat4 uiProjection = ortho(-1.0f * float(windowWidth) / 1000.0f, 1.0f * float(windowWidth) / 1000.0f, -1.0f * float(windowHeight) / 1000.0f, 1.0f * float(windowHeight) / 1000.0f);
    
    int frame = 0;
    long nextMeasure = SDL_GetTicks() + 1e3;
    int fps = 0;
    
    float lastFrame = 0.0f;
    float currentFrame = 0.0f;
    float deltaTime = 0.0f;
    
    float zoom = 1.0f;
    
    std::list<std::pair<float, Object*>> objects;
    std::vector<UIObject*> uiObjects;
    std::vector<PhysicsSphere*> physicsObjects;
    
    File noTexShaderVertexFile("resources/shaders/noTex.vs"), noTexShaderFragmentFile("resources/shaders/noTex.fs");
    File basicShaderVertexFile("resources/shaders/basic.vs"), basicShaderFragmentFile("resources/shaders/basic.fs");
    File blurShaderVertexFile("resources/shaders/blur.vs"), blurShaderFragmentFile("resources/shaders/blur.fs");
    File uiShaderVertexFile("resources/shaders/ui.vs"), uiShaderFragmentFile("resources/shaders/ui.fs");
    File skyboxShaderVertexFile("resources/shaders/skybox.vs"), skyboxShaderFragmentFile("resources/shaders/skybox.fs");
    
    string spaceSkyboxLocation[6] = {
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
    
    Camera cam(vec3(0.0f, 2.0f, 0.0f), &deltaTime, &windowEvent, &checkMouse);
    
    RenderData renderData;
    RenderData skyboxData;
    RenderData uiData;
    
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
    
    
    Cubemap spaceSkybox(spaceSkyboxLocation);
    
    Texture debugTexture("resources/textures/debug2.png");
    Texture atlasTexture("resources/textures/atlas.gif");
    Texture gradientTexture("resources/textures/grad.png");
    Texture grassTexture("resources/textures/nebel.jpg");
    Texture compassHeadbarTex("resources/textures/compassBar2.png");
    Texture compassSelectorTex("resources/textures/compassSelector.png");
    Texture declinationMeterTex("resources/textures/declinationMeter2.png");
    Texture declinationMeterSelectorTex("resources/textures/declinationMeterSelector.png");
    Texture jupiterTexture("resources/textures/jupiter.jpg");
    Texture jupiterTextureDecomp("resources/textures/jupiter.png");
    Texture jupiterTextureDecomp2("resources/textures/jupiter2.png");
    Texture stoneTexture("resources/textures/stone.png");
    
    PerlinMap map(420, 100, 0.5f, &basicShader, &renderData);
    map.setTexture(stoneTexture);
    map.setPosition(vec3(0.0f, -2.0f, 0.0f));
    objects.push_back(std::make_pair(0.0f, &map));
    
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
    
    UIRectangle debug(&uiShader, &uiData);
    debug.setTexture(debugTexture);
    
    UIRectangle compassBar(&uiShader, &uiData, compassUVs);
    compassBar.setTexture(compassHeadbarTex);
    compassBar.setXTexOffset(cam.getYawPointer());
    compassBar.setXTexMultiplier(0.00093f * 2.0f);
    compassBar.setPixelSize(vec2(682.0f, 128.0f) / vec2(4.0f));
    uiObjects.push_back(&compassBar);
    
    UIRectangle declinationMeterBar(&uiShader, &uiData, pitchUVs);
    declinationMeterBar.setTexture(declinationMeterTex);
    declinationMeterBar.setYTexOffset(cam.getPitchPointer());
    declinationMeterBar.setYTexMultiplier(-0.004448784722222f);
    declinationMeterBar.setPixelSize(vec2(128.0f, 410.0f) / vec2(4.0f));
    uiObjects.push_back(&declinationMeterBar);
    
    UIRectangle compassSelector(&uiShader, &uiData);
    compassSelector.setTexture(compassSelectorTex);
    compassSelector.setPixelSize(vec2(256.0f / 4.0f));
    uiObjects.push_back(&compassSelector);
    
    UIRectangle declinationMeterSelector(&uiShader, &uiData);
    declinationMeterSelector.setTexture(declinationMeterSelectorTex);
    declinationMeterSelector.setPixelSize(vec2(256.0f / 4.0f));
    uiObjects.push_back(&declinationMeterSelector);
    
    
    UIRectangle inv(&uiShader, &uiData);
    inv.setTexture(Texture("resources/textures/inv.png"));
    inv.setPixelSize(vec2(512.0f, 256.0f));
    
    
    
    Cube skybox(&skyboxShader, &skyboxData);
    skybox.setCubemap(spaceSkybox);
    
    
    
    Cube sharpCube(&basicShader, &renderData);
    sharpCube.setTexture(grassTexture);
    sharpCube.setPosition(vec3(2.0f, 1.0f, -4.0f));
    sharpCube.setRotation(angleAxis(radians(45.0f), vec3(1.0f, 0.0f, 0.0f)));
    sharpCube.addRotation(angleAxis(radians(45.0f), vec3(0.0f, 1.0f, 0.0f)));
    
    objects.push_back(std::make_pair(0.0f, &sharpCube));
    
    Cube bluredCube(&blurShader, &renderData);
    bluredCube.setTexture(grassTexture);
    bluredCube.setPosition(vec3(2.0f, 1.0f, -3.0f));
    bluredCube.setRotation(angleAxis(radians(45.0f), vec3(1.0f, 0.0f, 0.0f)));
    bluredCube.addRotation(angleAxis(radians(45.0f), vec3(0.0f, 1.0f, 0.0f)));
    
    objects.push_back(std::make_pair(0.0f, &bluredCube));
    
    Cube farCube(&basicShader, &renderData);
    farCube.setTexture(grassTexture);
    farCube.setPosition(vec3(0.0f, 0.0f, -110.0f));
    
    objects.push_back(std::make_pair(0.0f, &farCube));
    
    Sphere sphere(&basicShader, &renderData);
    sphere.setPosition(vec3(0.0f));
    sphere.setTexture(gradientTexture);
    objects.push_back(std::make_pair(0.0f, &sphere));
    
    UIText text("Pi = 3.141592f\nTest", &uiShader, &uiData);
    text.setSize(vec2(0.5f));
//    uiObjects.push_back(&text);
    
    UIText fpsText("", &uiShader, &uiData);
    fpsText.setSize(vec2(0.25f));
    uiObjects.push_back(&fpsText);
    
    /*
    Sphere earth(&basicShader, &renderData);
    earth.setTexture(atlasTexture);
    //6371000.0f
    float earthRadius = 63710.0f;
    earth.setRadius(earthRadius);
//    earth.setPosition(vec3(earthRadius, earthRadius, 0.0f));
    earth.setPosition(vec3(earthRadius * 1.0f / sqrt(2.0f), earthRadius * 1.0f / sqrt(2.0f), 0.0f));
    earth.setRotation(angleAxis(3.14f, vec3(1.0f, 0.0f, 0.0f)));
    objects.push_back(std::make_pair(0.0f, &earth));
    */
    
    Sphere jupiter(&basicShader, &renderData);
    jupiter.setTexture(jupiterTexture);
    jupiter.setPosition(vec3(10.0f));
    objects.push_back(std::make_pair(0.0f, &jupiter));
    
    Sphere jupiterDecomp(&basicShader, &renderData);
    jupiterDecomp.setTexture(jupiterTextureDecomp);
    jupiterDecomp.setPosition(vec3(12.0f, 10.0f, 10.0f));
    objects.push_back(std::make_pair(0.0f, &jupiterDecomp));
    
    Sphere jupiterDecomp2(&basicShader, &renderData);
    jupiterDecomp2.setTexture(jupiterTextureDecomp2);
    jupiterDecomp2.setPosition(vec3(14.0f, 10.0f, 10.0f));
    objects.push_back(std::make_pair(0.0f, &jupiterDecomp2));
    
    
    for(list<pair<float, Object*>>::iterator it = objects.begin(); it != objects.end(); it++) {
        it->first = length2(cam.getPosition() - it->second->getPosition());
    }
    
    objects.sort();
    
    vec3 oldCamPos = cam.getPosition();
    
    PhysicsSphere s1(1.0f, vec3(0.0f)), s2(1.0f, vec3(1.0f, 1.0f, 0.0f));
    CollisionInfo in = sphereSphereCollision(&s1, &s2);
    cout << in.collision << "\t" << in.collisionDepth << endl;
    printVec3(in.collisionPosition);
    
    in = spherePointCollision(&s1, vec3(1.0f, 0.5f, 0.0f));
    cout << in.collision << endl;
    
    physicsObjects.push_back(&s1);
    cam.setCollisonObjectsPointer(&physicsObjects);
    
    while(running) {
        if(SDL_GetTicks() > nextMeasure) {
            fps = frame;
            frame = 0;
            nextMeasure += 1e3;
            
            fpsText.setText(std::to_string(fps) + " FPS" + "\nFrametime: " + std::to_string(1.0f / fps) + " ms");
        }
        
        currentFrame = SDL_GetTicks() / 1000.0f;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        SDL_SetWindowTitle(window, (windowTitle + "     FPS: " + std::to_string(fps) +  "  Frametime: " + std::to_string(1.0f / fps) + "    Camera Pos X: " + std::to_string(cam.getPosition().x) + " Y: " + std::to_string(cam.getPosition().y) + " Z: " + std::to_string(cam.getPosition().z)).c_str());
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
        
        while(SDL_PollEvent(&windowEvent) != 0) {
            if(windowEvent.type == SDL_QUIT) {
                running = false;
            }
            
            if(windowEvent.type == SDL_MOUSEBUTTONDOWN) {
                if(windowEvent.button.button == SDL_BUTTON_LEFT) {
                    checkMouse = true;
                }
            }
            
            if(windowEvent.type == SDL_MOUSEWHEEL) {
                zoom += float(windowEvent.wheel.y) * 0.025f;
                
                if(zoom < 1.0f) {
                    zoom = 1.0f;
                }
                if(zoom > 25.0f) {
                    zoom = 25.0f;
                }
            }
            
            if(windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_i) {
                swapBool(&invOpen);
            }
            
            if(windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_o) {
                swapBool(&opticsOn);
            }
            
            if(windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_f) {
                swapBool(&wireframe);
            }
            
            
            if(!invOpen) {
                cam.processMouseInput();
            }
        }
        
        if(!invOpen) {
            cam.processInput();
        }
        
        
        if(checkMouse) {
            SDL_SetRelativeMouseMode(SDL_TRUE);
        }
        else {
            SDL_SetRelativeMouseMode(SDL_FALSE);
        }
        
        if(wireframe == true) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        
        
        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glViewport(0, 0, windowWidth, windowHeight);
        if(!opticsOn) {
            projection = infinitePerspective(radians(45.0f), float(windowWidth) / float(windowHeight), 0.005f);
            cam.setMouseSensitivity(0.25f);
        }
        else {
            projection = perspective(radians(zoom), float(windowWidth) / float(windowHeight), 0.5f, 1000.0f);
            cam.setMouseSensitivity(0.0025f);
        }
        
        if(cam.getPosition() != oldCamPos) {
            for(list<pair<float, Object*>>::iterator it = objects.begin(); it != objects.end(); it++) {
                if(it->second == &map) {
                    it->first = INFINITY;
                }
                else {
                    it->first = length2(cam.getPosition() - it->second->getPosition());
                }
            }
            
            objects.sort();
        }
        
        uiProjection = ortho(-1.0f * float(windowWidth) / 1000.0f, 1.0f * float(windowWidth) / 1000.0f, -1.0f * float(windowHeight) / 1000.0f, 1.0f * float(windowHeight) / 1000.0f);
        renderData.viewMat = cam.getViewMatrix();
        skyboxData.viewMat = mat4(mat3(cam.getViewMatrix()));
        
        glDepthMask(GL_FALSE);
        skyboxShader.use();
        skybox.render();
        glDepthMask(GL_TRUE);
        
//        earth.addRotation(angleAxis(radians(0.001f), vec3(0.0f, 1.0f, 0.0f)));
        
        for(list<pair<float, Object*>>::reverse_iterator it = objects.rbegin(); it != objects.rend(); it++) {
            
            it->second->getShaderPointer()->use();
            if(it->second == &bluredCube) {
                blurShader.sendFloat(1.0 / 300.0f * zoom, "offset");
            }
            it->second->render();
        }
        
        
        glClear(GL_DEPTH_BUFFER_BIT);
        
        uiShader.use();
        
       
        compassSelector.setPixelPosition(vec2(0.0f, (float(windowHeight) / 2.0f) - 32.0f));
        declinationMeterSelector.setPixelPosition(vec2((-float(windowWidth) / 2.0f) + 32.0f, 0.0f));
        compassBar.setPixelPosition(vec2(0.0f, (float(windowHeight) / 2.0f) - 32.0f));
        declinationMeterBar.setPixelPosition(vec2((-float(windowWidth) / 2.0f) + 32.0f, 0.0f));
        fpsText.setPixelPosition(vec2(-float(windowWidth) / 2.0f + (charWidth / 2.0f) * 0.25f, float(windowHeight) / 2.0f - (charHeight / 2.0f) * 0.25f));
        
        
        for(int i = 0; i < uiObjects.size(); i++) {
            uiObjects[i]->render();
        }
        
        if(invOpen) {
            inv.render();
        }
        
        SDL_GL_SwapWindow(window);
        glFlush();
        
        frame ++;
        oldCamPos = cam.getPosition();
        
        if(fps > 70) {
            SDL_Delay(16);
        }
    }
    
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
