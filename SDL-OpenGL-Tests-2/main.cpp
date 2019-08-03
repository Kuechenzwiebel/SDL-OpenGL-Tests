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

#include <bullet/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <bullet/BulletDynamics/btBulletDynamicsCommon.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace glm;

#include "file.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "triangle.hpp"
#include "utils.hpp"
#include "camera.hpp"
#include "rectangle.hpp"
#include "ui/uiRectangle.hpp"
#include "ui/uiText.hpp"
#include "cube.hpp"
#include "cubemap.hpp"
#include "items/items.h"
#include "sphere.hpp"
#include "physicsObjects/physicsSphere.hpp"
#include "ui/uiObject.hpp"

int windowWidth = 1080, windowHeight = 760;
std::string windowTitle = "SDL-OpenGL-Tests-2";

bool antiAliasing = false;
bool running = true;
bool checkMouse = false;
bool invOpen = false;
bool opticsOn = false;
bool stepping = false;
bool wireframe = false;


btDynamicsWorld* world;
btDispatcher* dispatcher;
btCollisionConfiguration* collisionConfig;
btBroadphaseInterface* broadphase;
btConstraintSolver* solver;


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
    
    collisionConfig = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfig);
    broadphase = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver(); 
    world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
    world->setGravity(btVector3(0.0f, -9.82f, 0.0f));
    
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
    
    Camera cam(vec3(0.0f), &deltaTime, &windowEvent, &checkMouse);
    
    RenderData renderData;
    RenderData uiData;
    RenderData skyboxData;
    
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
    
//    Sphere sphere(&basicShader, &renderData);
//    sphere.setPosition(vec3(4.0f, 2.0f, 3.0f));
//    objects.push_back(std::make_pair(0.0f, &sphere));
    
    Rectangle plane(&basicShader, &renderData);
    plane.setTexture(debugTexture);
    plane.setRotation(angleAxis(radians(180.0f), vec3(1.0f, 0.0f, 0.0f)));
    plane.setSize(vec3(10.0f, 10.0f, 10.0f));
    plane.setPosition(vec3(-10.0f, -10.0f, 15.0f));
    
    btTransform t;
    t.setIdentity();
    t.setOrigin(btVector3(4.0f, -10.0f, 3.0f));
    
    btStaticPlaneShape* planeShape = new btStaticPlaneShape(btVector3(0.0f, 1.0f, 0.2f), btScalar(0.0f));
    btMotionState* planeMotion = new btDefaultMotionState(t);
    btRigidBody* planeBody = new btRigidBody(0.0f, planeMotion, planeShape);
    world->addRigidBody(planeBody);
    

    btTransform ts;
    ts.setIdentity();
    ts.setOrigin(btVector3(0.5f, 0.7f, 0.0f));
    btSphereShape* sphereShape = new btSphereShape(1.0f);
    btVector3 sphereInertia;
    sphereShape->calculateLocalInertia(1.0f, sphereInertia);
    
    btMotionState* sphereMotion = new btDefaultMotionState(ts);
    btRigidBody* sphereBody = new btRigidBody(100.0f, sphereMotion, sphereShape);
    world->addRigidBody(sphereBody);
    
    btTransform bt;
    bt.setIdentity();
 
    
    UIText text("Pi = 3.141592f\nTest", &uiShader, &uiData);
    text.setSize(vec2(0.5f));
    uiObjects.push_back(&text);
    
    UIText fpsText("", &uiShader, &uiData);
    fpsText.setSize(vec2(0.25f));
    uiObjects.push_back(&fpsText);
    
    PhysicsSphere nSphere(&basicShader, &renderData, world, 3.0f);
    objects.push_back(std::make_pair(0.0f, &nSphere));
    nSphere.setPosition(vec3(0.5f, 0.0f, 0.0f));
    nSphere.setRadius(4.0f);
    nSphere.setTexture(atlasTexture);
    
    std::vector<PhysicsSphere*> spheres;
    
    Sphere sphere(&basicShader, &renderData);
    PhysicsSphere s2(&basicShader, &renderData, world, 3.0f);
    sphere.setTexture(atlasTexture);
    sphere.setPosition(vec3(-0.7f, 0.2f, 0.0f));
    s2.setTexture(gradientTexture);
    s2.setPosition(vec3(1.0f));
    objects.push_back(std::make_pair(0.0f, &sphere));
    objects.push_back(std::make_pair(0.0f, &s2));
    
    
    
    Sphere earth(&basicShader, &renderData);
    earth.setTexture(atlasTexture);
    //6371000.0f
    float earthRadius = 63710.0f;
    earth.setRadius(earthRadius);
//    earth.setPosition(vec3(earthRadius, earthRadius, 0.0f));
    earth.setPosition(vec3(earthRadius * 1.0f / sqrt(2.0f), earthRadius * 1.0f / sqrt(2.0f), 0.0f));
    earth.setRotation(angleAxis(3.14f, vec3(1.0f, 0.0f, 0.0f)));
    objects.push_back(std::make_pair(0.0f, &earth));
    
    
    
    for(list<pair<float, Object*>>::iterator it = objects.begin(); it != objects.end(); it++) {
        it->first = length2(cam.getPosition() - it->second->getPosition());
    }
    
    objects.sort();
    
    vec3 oldCamPos = cam.getPosition();
    
//    (&basicShader, &renderData, world, 3.0f);
//    testSphere.setTexture(atlasTexture);
    
    PhysicsSphere otherSphere(&basicShader, &renderData, world, 3.0f);
    otherSphere.setTexture(gradientTexture);
    otherSphere.setPosition(vec3(-1.0f, 10.0f, 2.0f));
    
    PhysicsSphere testSphere(otherSphere);
    
    Sphere sp1(&basicShader, &renderData), sp2(&basicShader, &renderData);
    sp1.setTexture(gradientTexture);
    sp2.setTexture(grassTexture);
    
    Item i(3);
    
    Slot s;
    s += i;
    
    while(running) {
        if(SDL_GetTicks() > nextMeasure) {
            fps = frame;
            frame = 0;
            nextMeasure += 1e3;
            
            fpsText.setText("FPS = " + std::to_string(fps) + "\nFrametime: " + std::to_string(1.0f / fps) + " ms");
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
            
            if(windowEvent.type == SDL_WINDOWEVENT_SIZE_CHANGED) {
                return 4;
            }
            
            if(windowEvent.type == SDL_WINDOWEVENT_RESIZED) {
                return 5;
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
            
            if(windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_r) {
                swapBool(&stepping);
            }
            
            if(windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_f) {
                swapBool(&wireframe);
            }
       
            
            if(windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_n) {
                spheres.push_back(new PhysicsSphere(&basicShader, &renderData, world, 2.0f, GLMVec3ToBt(cam.getFront() + cam.getPosition())));
                spheres[spheres.size() - 1]->setPosition(cam.getPosition() - vec3(0.0f, 1.4f, 0.0f));
                spheres[spheres.size() - 1]->setTexture(gradientTexture);
                objects.push_back(std::make_pair(0.0f, spheres[spheres.size() - 1]));
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
            projection = infinitePerspective(radians(45.0f), float(windowWidth) / float(windowHeight), 0.1f);
            cam.setMouseSensitivity(0.25f);
        }
        else {
            projection = perspective(radians(zoom), float(windowWidth) / float(windowHeight), 0.5f, 1000.0f);
            cam.setMouseSensitivity(0.0025f);
        }
        
        if(cam.getPosition() != oldCamPos) {
            for(list<pair<float, Object*>>::iterator it = objects.begin(); it != objects.end(); it++) {
                it->first = length2(cam.getPosition() - it->second->getPosition());
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
        
        basicShader.use();
        planeBody->getMotionState()->getWorldTransform(t);
        plane.setPosition(vec3(t.getOrigin().x(), t.getOrigin().y(), t.getOrigin().z()));
        plane.render();
        testSphere.render();
        
        sphereBody->getMotionState()->getWorldTransform(bt);
        sphere.setPosition(vec3(bt.getOrigin().x(), bt.getOrigin().y(), bt.getOrigin().z()));
        
        earth.addRotation(angleAxis(0.001f, vec3(0.0f, 1.0f, 0.0f)));
        
        for(list<pair<float, Object*>>::reverse_iterator it = objects.rbegin(); it != objects.rend(); it++) {
            it->second->getShaderPointer()->use();
            it->second->render();
        }
        
        
        basicShader.use();
        sp1.render();
        
        
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
        
        if(stepping) {
            world->stepSimulation(fps);
        }
        
        SDL_GL_SwapWindow(window);
        glFlush();
        
        frame ++;
        oldCamPos = cam.getPosition();
        
        if(fps > 70) {
            SDL_Delay(16);
        }
    }
    
    delete planeShape;
    delete planeMotion;
    delete planeBody;
    delete sphereShape;
    delete sphereMotion;
    delete sphereBody;
    delete world;
    delete dispatcher;
    delete collisionConfig;
    delete solver;
    delete broadphase;
    
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
