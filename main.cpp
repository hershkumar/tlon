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


class Circle {
    public:
        // radius of the circle
        float radius = 10.f;
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
        sf::Vector2f force = {0.0, 1.0f};
        // number of circles
        int num_circles = 5;
        // the time step
        float dt = 0.01f;

        // bounding circle
        float bounding_radius = 350.f;
        // bounding circle bg_color
        sf::Color bounding_color = sf::Color::Black;
        sf::CircleShape bounding_circle = sf::CircleShape(bounding_radius);
        
        sf::Vector2f center = {static_cast<float>(WINDOW_WIDTH/2.0), static_cast<float>( WINDOW_HEIGHT/2.0)};

    Simulator() {
        // create the circles
        for (int i = 0; i < num_circles; i++){
            Circle circle;
            circle.pos = {static_cast<float>(WINDOW_WIDTH/2.0) + 40.0f, static_cast<float>( WINDOW_HEIGHT/2.0)};
            circle.old_pos = {static_cast<float>(WINDOW_WIDTH/2.0), static_cast<float>( WINDOW_HEIGHT/2.0)};
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

    
    void update(float dt) {
        // for each circle, apply the force and then update the position
        for (int i = 0; i < num_circles; i++){
            applyForce(circles[i]);
            applyConstraint(circles[i]);
            circles[i].updatePosition(dt);
        }    
    }

    void applyForce(Circle &circle){
        circle.accelerate(force);
    }

    void applyConstraint(Circle &circle){
        // compute the distance from the center of the circle to the center of the window
        sf::Vector2f vec_to_center = circle.pos - center;
        float dist = sqrt(vec_to_center.x * vec_to_center.x + vec_to_center.y * vec_to_center.y);
        if (dist > bounding_radius - circle.radius){
            sf::Vector2f normal = vec_to_center / dist;
            circle.pos  = center + normal * (bounding_radius - circle.radius);
        }
        
    }

    //TODO: collision detection
    

    void draw(sf::RenderWindow &window){
        for (int i = 0; i < num_circles; i++){
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

        // clear the window with black color
        window.clear(bg_color);
        // draw the bounding circle
        window.draw(sim.bounding_circle);
        // step the simulation forwards
        sim.update(sim.dt);
        // draw the circles to the window
        sim.draw(window);

        // display frame number in top left
        window.draw(fps(frame_num, font));

        // end the current frame
        window.display();
    }

    return 0;
}