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

    auto image = scene.render();
    save_image(image, "image.ppm");

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

    double pi = 3.1415926535;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);

        cout << scene.camera.o << " | " << scene.camera.d << endl;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            scene.camera.o += scene.camera.d;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            scene.camera.o += scene.camera.d * -1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            scene.camera.o += scene.camera.ort1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            scene.camera.o += scene.camera.ort1 * -1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            scene.camera.o += Vector(0, 0, 1);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            scene.camera.o += Vector(0, 0, -1);
        }

        sf::Vector2i mxy = sf::Mouse::getPosition(window);
        int cur_mx = mxy.x;
        int cur_my = mxy.y;
        double dx = cur_mx - mx;
        double dy = cur_my - my;
        sf::Mouse::setPosition(sf::Vector2i(mx, my), window);
        //mx = cur_mx;
        //my = cur_my;

        dx *= sense;
        dy *= sense;

        double cx = scene.camera.d.x;
        double cy = scene.camera.d.y;
        double cz = scene.camera.d.z;
        double cyy = cy / (cy * cy + cx * cx);
        double cxx = cx / (cy * cy + cx * cx);

        scene.camera.d = rotz(scene.camera.d, -pi * dx / 130);
        scene.camera.d = roty(scene.camera.d, -pi * dy * cxx / 130);
        scene.camera.d = rotx(scene.camera.d, +pi * dy * cyy / 130);


        scene.camera.update();

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
