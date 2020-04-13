#include "scene.hpp"

Scene::Scene() {}

Scene::Scene(std::string filename) {
    std::fstream fs;
    fs.open(filename, std::fstream::in);
    std::string s;
    int width;
    int height;
    fs >> s >> width >> s >> height;
    double screen_distance;
    fs >> s >> screen_distance;
    double resolution_coef;
    fs >> s >> resolution_coef;

    fs >> s >> depth;

    Vector cam_pos;
    fs >> s >> cam_pos;
    Vector cam_dir;
    fs >> s >> cam_dir;
    camera = Camera(cam_pos, cam_dir, screen_distance, width, height, resolution_coef);

    std::string type;
    while (fs >> type) {
        if (type == "Primitive") {
            Primitive* prim;
            fs >> prim;
            objs.push_back(prim);
        } else if (type == "Light") {
            Light light;
            fs >> light;
            lights.push_back(light);
        } else if (type == "Model") {
            Model model;
            fs >> model;
            for (Triangle *triag : model.get_triangles()) {
                objs.push_back(triag);
            }
        }
    }
}

std::vector<std::vector<Vector>> render_image(Camera camera, std::vector<Primitive*> objects, std::vector<Light> lights, int depth, int verbose);

std::vector<std::vector<Vector>> Scene::render() {
    return render_image(camera, objs, lights, depth, 0);
}
