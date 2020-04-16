#include <string>
#include <algorithm>
#include "ray_tracer.hpp"
#include <SFML/Graphics.hpp>

std::vector<std::vector<sf::RectangleShape>> image_to_rects(std::vector<std::vector<Vector>>& image, int rect_size) {
    std::vector<std::vector<sf::RectangleShape>> ret(image.size(), std::vector<sf::RectangleShape>(image[0].size(), sf::RectangleShape(sf::Vector2f(rect_size, rect_size))));
    for (int i = 0; i < image.size(); ++i) {
        for (int j = 0; j < image[0].size(); ++j) {
            ret[i][j].setPosition(i * rect_size, j * rect_size);
            int r = (255 * std::max(0.0, std::min(image[i][j].x, 1.0)));
            int g = (255 * std::max(0.0, std::min(image[i][j].y, 1.0)));
            int b = (255 * std::max(0.0, std::min(image[i][j].z, 1.0)));
            //cout << r << ' ' << g << ' ' << 
            ret[i][j].setFillColor(sf::Color(r, g, b));
        }
    }

    return ret;
}

int main() {
    std::string filename = "scene.txt";
    Scene scene(filename);
    if (scene.to_display_player) {
        scene.objs.push_back(&scene.player);
    }

    auto image = scene.render();

    int rect_size = scene.pixel_size;
    auto rects = image_to_rects(image, rect_size);

    int w = rects.size() * rect_size;
    int h = rects[0].size() * rect_size;

    sf::RenderWindow window(sf::VideoMode(w, h), "My window");

    sf::RectangleShape rectangle(sf::Vector2f(120.f, 50.f));
    rectangle.setFillColor(sf::Color(200, 150, 100));
    rectangle.setPosition(10, 50);

    int mx = w / 2;
    int my = h / 2;
    double sense = 0.1;
    sf::Mouse::setPosition(sf::Vector2i(mx, my), window);
    Vector offset(0, 0, 0);

    double pi = 3.1415926535;
    double speed = 1;
    Vector gravity = {0, 0, -speed * 0.35};

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);

        int xxx, yyy, zzz, ddd;
        xxx = scene.player.c.x;
        yyy = scene.player.c.y;
        zzz = scene.player.c.z;
        ddd = scene.camera.dist;

        cout << xxx << " " << yyy << " " << zzz << " | " << ddd << endl;

        Vector shift;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            shift += scene.camera.d;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            shift += scene.camera.d * -1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            shift += scene.camera.ort1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            shift += scene.camera.ort1 * -1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
            scene.camera.dist -= 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
            scene.camera.dist += 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
            scene.camera.dist = 100;
        }
        shift.z = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            shift += Vector(0, 0, 1.5);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            shift += Vector(0, 0, -1.5);
        }
        shift = shift.normal() * speed;
        Vector init = scene.player.c;
        shift += gravity;
        scene.player.c += shift;
        Vector shifted = shift;

        std::sort(scene.objs.begin(), scene.objs.end(),
            [scene](Primitive *a, Primitive *b) {
                return scene.player.dist(a) < scene.player.dist(b);
            });

        for (auto obj : scene.objs) {
            Vector inter_norm = scene.player.intersects(obj);
            if (inter_norm.len()) {
                //cout << "bam " << scene.player.c << '\n' ;
                double t = shift.dot(inter_norm) / inter_norm.len();
                //shift -= inter_norm.normal() * t;
                //scene.player.c -= shifted;
                //scene.player.c += shift;
                //shifted = shift;
                scene.player.c += inter_norm;
            }
        }

        scene.camera.o = scene.player.c + scene.camera_offset;

        // ROTATING

        sf::Vector2i mxy = sf::Mouse::getPosition(window);
        int cur_mx = mxy.x;
        int cur_my = mxy.y;
        int cur_dx = cur_mx - mx;
        int cur_dy = cur_my - my;
        sf::Mouse::setPosition(sf::Vector2i(mx, my), window);

        double dx = cur_dx * sense;
        double dy = cur_dy * sense;

        double cx = scene.camera.d.x;
        double cy = scene.camera.d.y;
        double cz = scene.camera.d.z;
        double cyy = cy / (cy * cy + cx * cx);
        double cxx = cx / (cy * cy + cx * cx);

        scene.camera.d = rotz(scene.camera.d, +pi * dx / 130);
        scene.camera.d = roty(scene.camera.d, +pi * dy * cxx / 130);
        scene.camera.d = rotx(scene.camera.d, -pi * dy * cyy / 130);

        scene.camera.update();

        // DRAWING

        image = scene.render();
        rects = image_to_rects(image, rect_size);


        for (int x = 0; x < rects.size(); ++x) {
            for (int y = 0; y < rects[0].size(); ++y) {
                window.draw(rects[x][y]);
            }
        }

        window.display();
    }

    return 0;
}
