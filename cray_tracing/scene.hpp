#ifndef SCENE
#define SCENE

#include <vector>
#include <string>
#include <fstream>
#include "vector.hpp"
#include "primitive.hpp"
#include "model.hpp"
#include "light.hpp"
#include "camera.hpp"

struct Scene {
    std::vector<Primitive*> objs;
    std::vector<Light> lights;
    Camera camera;
    int depth;

    Scene();
    Scene(std::string filename);

    std::vector<std::vector<Vector>> render();
};

#endif // SCENE
