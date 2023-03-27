#include <stdio.h>
#include <stdlib.h>
#include <chrono>


//SFML includes
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;


// function that returns a text object for the frame_count
//TODO: change this to compute the fps
sf::Text fps(double frame_num, sf::Font font) {
    sf::Text frame_text;
    frame_text.setFont(font);
    frame_text.setString(std::to_string(frame_num));
    frame_text.setCharacterSize(15);
    frame_text.setFillColor(sf::Color::Red);
    frame_text.setStyle(sf::Text::Bold);
    return frame_text;
}


sf::Text printDrawn(int num, sf::Font font){
    sf::Text drawn_text;
    drawn_text.setFont(font);
    drawn_text.setString(std::to_string(num));
    drawn_text.setCharacterSize(15);
    drawn_text.setFillColor(sf::Color::Red);
    drawn_text.setStyle(sf::Text::Bold);
    drawn_text.setPosition({0, 20});
    return drawn_text;
}

class Circle {
    public:
        // radius of the circle
        float radius = 15.f;
        // current position as a vector
        sf::Vector2f pos;
        // old position as a vector
        sf::Vector2f old_pos;
        // current acceleration
        sf::Vector2f accel = {0,0};
        // the SFML object to be drawn
        sf::CircleShape shape;
        // the color
        sf::Color color = sf::Color::White;
        // whether the circle has been drawn onto the window
        bool drawn;
        Circle(){
            shape.setRadius(radius);
            shape.setFillColor(color);
            shape.setPosition(pos);
            shape.setPointCount(100);
        }

        Circle (float radius, sf::Vector2f pos){
            shape.setRadius(radius);
            shape.setFillColor(color);
            shape.setPosition(pos);
        }

        // update the shape position
        void updateShape(){
            shape.setPosition(pos);
            shape.setFillColor(color);
        }
        // verlet integration for position update
        void updatePosition(float dt){
            sf::Vector2f vel = pos - old_pos;
            old_pos = pos;
            pos = pos + vel + accel * dt * dt;
            accel = {};

            // update the shape position to match the new position
            updateShape();
        }

        void accelerate(sf::Vector2f force){
            accel += force;
        }

};



class Simulator{
    public:
        // the circles
        std::vector<Circle> circles;
        // the force to be applied
        sf::Vector2f force = {0, 1.0f};
        // number of circles
        int num_circles = 2000;
        // the time step
        float dt = 0.01f;

        // bounding circle
        float bounding_radius = 375.f;
        // bounding circle bg_color
        sf::Color bounding_color = sf::Color::Black;
        // shape for the bounding circle
        sf::CircleShape bounding_circle = sf::CircleShape(bounding_radius);
        // center of the screen
        sf::Vector2f center = {static_cast<float>(WINDOW_WIDTH/2.0), static_cast<float>( WINDOW_HEIGHT/2.0)};
        // number of currently drawn circles
        int num_drawn = 0;
        // number of substeps
        int substeps = 1;

    Simulator() {
        // create the circles
        for (int i = 0; i < num_circles; i++){
            Circle circle;
            circle.shape.setOrigin({circle.radius, circle.radius});
            circle.pos = {static_cast<float>(WINDOW_WIDTH/2.0), static_cast<float>(WINDOW_HEIGHT/2.0) - 200.0};
            circle.old_pos = circle.pos;
            circle.drawn = false;
            circles.push_back(circle);
        }

        bounding_circle.setFillColor(bounding_color);
        bounding_circle.setPointCount(100);
        // move the bounding circle to the center of the window
        bounding_circle.setOrigin({bounding_circle.getRadius(), bounding_circle.getRadius()});
        bounding_circle.setPosition({static_cast<float>(WINDOW_WIDTH/2.0), static_cast<float>( WINDOW_HEIGHT/2.0)});
    }

    Simulator(int num_circles, sf::Vector2f force, float dt) {
        this->num_circles = num_circles;
        this->force = force;
        this->dt = dt;

        Simulator();
    }

    //TODO: substep this like in the video
    void update(float dt) {
        float sdt = dt / static_cast<float>(substeps);
        for (int i = 0; i < substeps; i++){
            applyForce();
            applyConstraint();
            checkCollisions();
            updateAllPositions(sdt);
        }
    }

    void updateAllPositions(float dt){
        for (int i = 0; i < num_drawn; i++){
            circles[i].updatePosition(dt);
        }
    }

    void applyForce(){
        for (int i = 0; i < num_drawn; i++){
            circles[i].accelerate(force);
        }
    }

    void applyForceToSingle(Circle &circle, sf::Vector2f force){
        circle.accelerate(force);
    }

    void applyConstraint(){
        for (int i = 0; i < num_drawn; i++){
            // compute the distance from the center of the circle to the center of the window
            sf::Vector2f vec_to_center = circles[i].pos - center;
            float dist = sqrt(vec_to_center.x * vec_to_center.x + vec_to_center.y * vec_to_center.y);
            if (dist > bounding_radius - circles[i].radius){
                sf::Vector2f normal = vec_to_center / dist;
                circles[i].pos  = center + normal * (bounding_radius - circles[i].radius);
            }
        }
    }
    // naive n^2 algorithm
    void checkCollisions(){
        for (int i = 0; i < num_drawn; i++){
            for (int j = 0; j < num_drawn; j++){
                if (i != j){
                    sf::Vector2f vec_to_other = circles[i].pos - circles[j].pos;
                    float dist = sqrt(vec_to_other.x * vec_to_other.x + vec_to_other.y * vec_to_other.y);
                    // if the distance between the two is less than the sum of their radii, they are colliding
                    if (dist < circles[i].radius + circles[j].radius){
                        // compute the normal vector along the collision axis
                        sf::Vector2f normal = vec_to_other / dist;
                        // move the two objects till they don't collide anymore
                        float delta = circles[i].radius + circles[j].radius - dist;
                        circles[i].pos += .5f * delta * normal;
                        circles[j].pos -= .5f * delta * normal;
                    }
                }
            }
        }
    }

    void draw(sf::RenderWindow &window){
        for (int i = 0; i < num_drawn; i++){
            circles[i].updateShape();
            window.draw(circles[i].shape);
        }
    }

};

int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "tlon");

    // background color (gray)
    sf::Color bg_color = sf::Color(55,55,55);

    // load a font, throw an error if its not there
    sf::Font font;
    if (!font.loadFromFile("assets/arial.ttf"))
    {
        throw std::runtime_error("Could not find font file.");
    }

    // make a simulator instance
    Simulator sim;

    int frame_num = 1;
    while (window.isOpen())
    {
        frame_num++;

        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // based on the frame number, set the correct number of circles to be drawn
        if (frame_num % 250 == 0){
            if (sim.num_drawn < sim.num_circles){
                sim.circles[sim.num_drawn].drawn = true;
                sf::Vector2f spawnForce = {static_cast<float>(sin(sim.num_drawn/(10*2*3.14))),static_cast<float>(cos(sim.num_drawn/(10*2*3.14)))};
                spawnForce *= 2000.0f;
                sim.applyForceToSingle(sim.circles[sim.num_drawn], spawnForce);
                sim.num_drawn++;
            }
        }
        // clear the window with black color
        window.clear(bg_color);
        // draw the bounding circle
        window.draw(sim.bounding_circle);
        // step the simulation forwards
        sim.update(sim.dt);
        // draw the correct circles to the window
        sim.draw(window);

        // display frame number in top left
        window.draw(fps(frame_num, font));
        window.draw(printDrawn(sim.num_drawn, font));
        // end the current frame
        window.display();
    }

    return 0;
}