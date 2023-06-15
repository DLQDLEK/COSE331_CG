#include "scene.h"
#include "binary/animation.h"
#include "binary/skeleton.h"
#include "binary/player.h"

Shader* Scene::vertexShader = nullptr;
Shader* Scene::fragmentShader = nullptr;
Program* Scene::program = nullptr;
Camera* Scene::camera = nullptr;
Object* Scene::player = nullptr;
Texture* Scene::diffuse = nullptr;
Material* Scene::material = nullptr;
Object* Scene::lineDraw = nullptr;
Texture* Scene::lineColor = nullptr;
Material* Scene::lineMaterial = nullptr;

bool Scene::upperFlag = true;
bool Scene::lowerFlag = true;

void Scene::setup(AAssetManager* aAssetManager) {
    Asset::setManager(aAssetManager);

    Scene::vertexShader = new Shader(GL_VERTEX_SHADER, "vertex.glsl");
    Scene::fragmentShader = new Shader(GL_FRAGMENT_SHADER, "fragment.glsl");

    Scene::program = new Program(Scene::vertexShader, Scene::fragmentShader);

    Scene::camera = new Camera(Scene::program);
    Scene::camera->eye = vec3(0.0f, 0.0f, 80.0f);

    Scene::diffuse = new Texture(Scene::program, 0, "textureDiff", playerTexels, playerSize);
    Scene::material = new Material(Scene::program, diffuse);
    Scene::player = new Object(program, material, playerVertices, playerIndices);
    player->worldMat = scale(vec3(1.0f / 3.0f));

    Scene::lineColor = new Texture(Scene::program, 0, "textureDiff", {{0xFF, 0x00, 0x00}}, 1);
    Scene::lineMaterial = new Material(Scene::program, lineColor);
    Scene::lineDraw = new Object(program, lineMaterial, {{}}, {{}}, GL_LINES);
}

void Scene::screen(int width, int height) {
    Scene::camera->aspect = (float) width/height;
}

void Scene::update(float deltaTime) {
    Scene::program->use();
    Scene::camera->update();

    /*
     *
     * Write your code.
     *
     */

    // Line Drawer
    glLineWidth(30);

    static float accTime1 = 0.0f;
    static float accTime2 = 0.0f;

    vector<mat4> skeP(27, mat4(1.0f));
    for (int i = 1; i < skeP.size(); i++)
    {
        skeP[i] = translate(mat4(1.0f), jOffsets[i]);
    }

    vector<mat4> skeD(27, mat4(1.0f));
    for (int i = 1; i < skeD.size(); i++)
    {
        skeD[i] = skeD[jParents[i]] * skeP[i];
    }

    vector<mat4> forkiR(27, mat4(1.0f));
    for (int i = 0; i < forkiR.size(); i++)
    {
        forkiR[i] =
                rotate(mat4(1.0f), radians(motions[0][3*i+5]), vec3(0.0f, 0.0f, 1.0f))*
                rotate(mat4(1.0f), radians(motions[0][3*i+3]), vec3(1.0f, 0.0f, 0.0f))*
                rotate(mat4(1.0f), radians(motions[0][3*i+4]), vec3(0.0f, 1.0f, 0.0f));
    }

    vector<mat4> forkiR_1(27, mat4(1.0f));
    for (int i = 0; i < forkiR_1.size(); i++)
    {
        forkiR_1[i] =
                rotate(mat4(1.0f), radians(motions[1][3*i+5]), vec3(0.0f, 0.0f, 1.0f))*
                rotate(mat4(1.0f), radians(motions[1][3*i+3]), vec3(1.0f, 0.0f, 0.0f))*
                rotate(mat4(1.0f), radians(motions[1][3*i+4]), vec3(0.0f, 1.0f, 0.0f));
    }

    vector<mat4> forkiR_2(27, mat4(1.0f));
    for (int i = 0; i < forkiR_2.size(); i++)
    {
        forkiR_2[i] =
                rotate(mat4(1.0f), radians(motions[2][3*i+5]), vec3(0.0f, 0.0f, 1.0f))*
                rotate(mat4(1.0f), radians(motions[2][3*i+3]), vec3(1.0f, 0.0f, 0.0f))*
                rotate(mat4(1.0f), radians(motions[2][3*i+4]), vec3(0.0f, 1.0f, 0.0f));
    }

    vector<mat4> forkiR_3(27, mat4(1.0f));
    for (int i = 0; i < forkiR_3.size(); i++)
    {
        forkiR_3[i] =
                rotate(mat4(1.0f), radians(motions[3][3*i+5]), vec3(0.0f, 0.0f, 1.0f))*
                rotate(mat4(1.0f), radians(motions[3][3*i+3]), vec3(1.0f, 0.0f, 0.0f))*
                rotate(mat4(1.0f), radians(motions[3][3*i+4]), vec3(0.0f, 1.0f, 0.0f));
    }

    vector<mat4> forkiR_t(27, mat4(1.0f));
    vector<quat> A(27);
    vector<quat> B(27);
    vector<quat> C(27);

    int tmp1 = static_cast<int>(accTime1) % 4;
    float cycleTime1 = accTime1 - static_cast<int>(accTime1);

    int tmp2 = static_cast<int>(accTime2) % 4;
    float cycleTime2 = accTime2 - static_cast<int>(accTime2);


    if (tmp1 == 0)
    {
        for (int i = 12; i < forkiR_t.size(); i++)
        {
            A[i] = quat_cast(forkiR[i]);
            B[i] = quat_cast(forkiR_1[i]);
            C[i] = slerp(A[i], B[i], cycleTime1);
            forkiR_t[i] = mat4_cast(C[i]);
        }
        if(Scene::upperFlag == true)
            accTime1 += deltaTime;
    }
    else if (tmp1 == 1)
    {
        for (int i = 12; i < forkiR_t.size(); i++)
        {
            A[i] = quat_cast(forkiR_1[i]);
            B[i] = quat_cast(forkiR_2[i]);
            C[i] = slerp(A[i], B[i], cycleTime1);
            forkiR_t[i] = mat4_cast(C[i]);
        }
        if(Scene::upperFlag == true)
            accTime1 += deltaTime;
    }
    else if (tmp1 == 2)
    {
        for (int i = 12; i < forkiR_t.size(); i++)
        {
            A[i] = quat_cast(forkiR_2[i]);
            B[i] = quat_cast(forkiR_3[i]);
            C[i] = slerp(A[i], B[i], cycleTime1);
            forkiR_t[i] = mat4_cast(C[i]);
        }
        if(Scene::upperFlag == true)
            accTime1 += deltaTime;
    }
    else
    {
        for (int i = 12; i < forkiR_t.size(); i++)
        {
            A[i] = quat_cast(forkiR_3[i]);
            B[i] = quat_cast(forkiR[i]);
            C[i] = slerp(A[i], B[i], cycleTime1);
            forkiR_t[i] = mat4_cast(C[i]);
        }
        if(Scene::upperFlag == true)
            accTime1 += deltaTime;
    }

    if (tmp2 == 0)
    {
        for (int i = 0; i < 11; i++)
        {
            A[i] = quat_cast(forkiR[i]);
            B[i] = quat_cast(forkiR_1[i]);
            C[i] = slerp(A[i], B[i], cycleTime2);
            forkiR_t[i] = mat4_cast(C[i]);
        }
        if(Scene::lowerFlag == true)
            accTime2 += deltaTime;
    }
    else if (tmp2 == 1)
    {
        for (int i = 0; i < 11; i++)
        {
            A[i] = quat_cast(forkiR_1[i]);
            B[i] = quat_cast(forkiR_2[i]);
            C[i] = slerp(A[i], B[i], cycleTime2);
            forkiR_t[i] = mat4_cast(C[i]);
        }
        if(Scene::lowerFlag == true)
            accTime2 += deltaTime;
    }
    else if (tmp2 == 2)
    {
        for (int i = 0; i < 11; i++)
        {
            A[i] = quat_cast(forkiR_2[i]);
            B[i] = quat_cast(forkiR_3[i]);
            C[i] = slerp(A[i], B[i], cycleTime2);
            forkiR_t[i] = mat4_cast(C[i]);
        }
        if(Scene::lowerFlag == true)
            accTime2 += deltaTime;
    }
    else
    {
        for (int i = 0; i < 11; i++)
        {
            A[i] = quat_cast(forkiR_3[i]);
            B[i] = quat_cast(forkiR[i]);
            C[i] = slerp(A[i], B[i], cycleTime2);
            forkiR_t[i] = mat4_cast(C[i]);
        }
        if(Scene::lowerFlag == true)
            accTime2 += deltaTime;
    }

    vector<mat4> forkiA_t(27, mat4(1.0f));
    for (int i = 1; i < forkiA_t.size(); i++)
    {
        forkiA_t[i] = forkiA_t[jParents[i]] * skeP[i] * forkiR_t[i];
    }

    vector<mat4> forkiI_t(27, mat4(1.0f));
    for (int i = 0; i < forkiI_t.size(); i++)
    {
        forkiI_t[i] = forkiA_t[i] * inverse(skeD[i]);
    }

    vector<Vertex> pplayerVertices(playerVertices);

    for(int i = 0; i < pplayerVertices.size(); i++)
    {
        vec3 tmp = {0.0f, 0.0f, 0.0f};
        for(int j = 0; j < 4; j++)
        {
            tmp += pplayerVertices[i].weight[j] * vec3(forkiI_t[pplayerVertices[i].bone[j]] * vec4(pplayerVertices[i].pos, 1.0f));
        }
        pplayerVertices[i].pos = tmp;
    }

    Scene::player->load(pplayerVertices, playerIndices);
    Scene::player->draw();
}

void Scene::setUpperFlag(bool flag)
{
    Scene::upperFlag = flag;
}

void Scene::setLowerFlag(bool flag)
{
    Scene::lowerFlag = flag;
}