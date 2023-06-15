#include "scene.h"
#include "binary/teapot.h"
#include "binary/rgb.h"
#include "binary/cloud.h"
#include "binary/tex_flower.h"
#include "checker.h"

Shader* Scene::vertexShader = nullptr;
Shader* Scene::fragmentShader = nullptr;
Program* Scene::program = nullptr;
Camera* Scene::camera = nullptr;
Object* Scene::teapot = nullptr;
Texture* Scene::diffuse = nullptr;
Texture* Scene::dissolve = nullptr;
Material* Scene::material = nullptr;
Light* Scene::light = nullptr;

int Scene::width = 0;
int Scene::height = 0;

// Arcball variables
float lastMouseX = 0, lastMouseY = 0;
float currentMouseX = 0, currentMouseY = 0;

void Scene::setup(AAssetManager* aAssetManager) {
    Asset::setManager(aAssetManager);

    Scene::vertexShader = new Shader(GL_VERTEX_SHADER, "vertex.glsl");
    Scene::fragmentShader = new Shader(GL_FRAGMENT_SHADER, "fragment.glsl");

    Scene::program = new Program(Scene::vertexShader, Scene::fragmentShader);

    Scene::camera = new Camera(Scene::program);
    Scene::camera->eye = vec3(20.0f, 30.0f, 20.0f);

    Scene::light = new Light(program);
    Scene::light->position = vec3(15.0f, 15.0f, 0.0f);

    //////////////////////////////
    /* TODO: Problem 2 : Change the texture of the teapot
     *  Modify and fill in the lines below.
     */
    vector<Texel> new_rgbTexels(rgbTexels.size());

    for(int i = 0; i < rgbTexels.size(); i++) {
        if(rgbTexels[i].blue == 0xFF) {
            new_rgbTexels[i].blue = 0x00;
            new_rgbTexels[i].green = 0xFF;
        }
        if(rgbTexels[i].red == 0xFF) {
            new_rgbTexels[i].red = 0x00;
            new_rgbTexels[i].blue = 0xFF;
        }
        if(rgbTexels[i].green == 0xFF) {
            new_rgbTexels[i].green = 0x00;
            new_rgbTexels[i].red = 0xFF;
        }
    }

    Scene::diffuse  = new Texture(Scene::program, 0, "textureDiff", new_rgbTexels, rgbSize);
    //////////////////////////////

    Scene::material = new Material(Scene::program, diffuse, dissolve);
    Scene::teapot = new Object(program, material, teapotVertices, teapotIndices);
}

void Scene::screen(int width, int height) {
    Scene::camera->aspect = (float) width/height;
    Scene::width = width;
    Scene::height = height;
}

void Scene::update(float deltaTime) {
    static float time = 0.0f;

    Scene::program->use();

    Scene::camera->update();
    Scene::light->update();

    Scene::teapot->draw();
    time += deltaTime;
}

void Scene::mouseDownEvents(float x, float y) {
    lastMouseX = currentMouseX = x;
    lastMouseY = currentMouseY = y;
}

void Scene::mouseMoveEvents(float x, float y) {
    //////////////////////////////
    /* TODO: Problem 3 : Implement Phong lighting
     *  Fill in the lines below.
     */
    float x1 = 2 * x / Scene::width - 1;
    float y1 = -1 * (2 * y / Scene::height - 1);

    vec3 touchPos(x1, y1, 0.0f);
    float touchPosLen = length(touchPos);

    if(touchPosLen > 1.0f)
        touchPos = normalize(touchPos);
    else
        touchPos.z = sqrt(1 - touchPosLen*touchPosLen);

    float x2 = 2 * lastMouseX / Scene::width - 1;
    float y2 = -1 * (2 * lastMouseY / Scene::height - 1);
    vec3 lastTouchPos(x2, y2, 0.0f);
    float lastTouchLen = length(lastTouchPos);

    if(lastTouchLen > 1.0f)
        lastTouchPos = normalize(lastTouchPos);
    else
        lastTouchPos.z = sqrt(1-lastTouchLen*lastTouchLen);

    vec3 axis = cross(lastTouchPos, touchPos);
    float theta = degrees(acos(dot(lastTouchPos, touchPos)));
    mat4 tmpmat = glm::inverse(lookAt(camera->eye, camera->at, camera->up));

    mat4 rotateMat = rotate(mat4(1.0f), radians(theta), vec3 (tmpmat*vec4(axis,0.0f)));
    light->position = vec3(rotateMat * vec4(light->position, 1.0f));
    lastMouseX = x;
    lastMouseY = y;
    //////////////////////////////
}