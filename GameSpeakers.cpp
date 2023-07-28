#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

const int windowWidth = 600;
const int windowHeight = 600;
const float pixelsPerCentimeter = 100.0f; // Adjust this based on your desired scale

// Function to calculate the interference pattern based on speaker positions and frequencies
void calculateInterferencePattern(float speaker1X, float speaker1Y, float speaker2X, float speaker2Y, float frequency1, float frequency2, float speedOfSound, float distanceCentimeters, sf::VertexArray& interferencePattern) {
    // Convert the distance between the speakers from centimeters to pixels
    float distancePixels = distanceCentimeters * pixelsPerCentimeter;
    for (int x = 0; x < windowWidth; x++) {
        for (int y = 0; y < windowHeight; y++) {
            // Calculate the distances from each speaker to the current pixel (x, y)
            float distanceToSpeaker1 = std::sqrt(std::pow(x - speaker1X, 2) + std::pow(y - speaker1Y, 2));
            float distanceToSpeaker2 = std::sqrt(std::pow(x - speaker2X, 2) + std::pow(y - speaker2Y, 2));

            // Calculate the wavelengths of each sound wave
            float wavelength1 = speedOfSound / frequency1;
            float wavelength2 = speedOfSound / frequency2;

            // Calculate the phase difference based on the distances and wavelengths
            float phaseDifference = (distanceToSpeaker2 - distanceToSpeaker1) * (2 * M_PI) / (wavelength1 - wavelength2);

            // Calculate the intensity (brightness) at the current pixel based on the phase difference
            float maxIntensity = 255.0f; // You can adjust this value to control the intensity range
            float intensity = std::abs(std::cos(phaseDifference) * maxIntensity);

            // Set the color and position of the point in the interferencePattern vertex array
            sf::Color color(intensity, intensity, intensity); // Use a grayscale color based on intensity
            interferencePattern.append(sf::Vertex(sf::Vector2f(x, y), color));
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Sound Interferometry");

    // Set up the vertex array to hold the interference pattern points
    sf::VertexArray interferencePattern(sf::Points);

    // Speaker positions
    float speaker1X = 48 * windowWidth / 100.0f;
    float speaker1Y = windowHeight / 4.0f;
    float speaker2X = 52 * windowWidth / 100.0f;
    float speaker2Y = windowHeight / 4.0f;

    // User input for frequency, speed of sound, and distance between speakers
    float frequency1, frequency2, speedOfSound, distanceCentimeters;
    std::cout << "Enter the frequency of speaker 1 (in Hz): ";
    std::cin >> frequency1;
    std::cout << "Enter the frequency of speaker 2 (in Hz): ";
    std::cin >> frequency2;
    std::cout << "Enter the speed of sound (in m/s): ";
    std::cin >> speedOfSound;
    std::cout << "Enter the distance between the two speakers (in centimeters): ";
    std::cin >> distanceCentimeters;

    // Convert distanceCentimeters to distancePixels
    float distancePixels = distanceCentimeters * pixelsPerCentimeter;

    // Flags to track speaker dragging
    bool draggingSpeaker1 = false;
    bool draggingSpeaker2 = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed) {
                // Check if the mouse is pressed on a speaker and initiate dragging
                if (event.mouseButton.button == sf::Mouse::Left) {
                    float mouseX = static_cast<float>(event.mouseButton.x);
                    float mouseY = static_cast<float>(event.mouseButton.y);

                    if (std::sqrt(std::pow(mouseX - speaker1X, 2) + std::pow(mouseY - speaker1Y, 2)) <= 10) {
                        draggingSpeaker1 = true;
                    }
                    else if (std::sqrt(std::pow(mouseX - speaker2X, 2) + std::pow(mouseY - speaker2Y, 2)) <= 10) {
                        draggingSpeaker2 = true;
                    }
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased) {
                // End dragging when the mouse button is released
                draggingSpeaker1 = false;
                draggingSpeaker2 = false;
            }
            else if (event.type == sf::Event::MouseMoved) {
                // Update speaker positions while dragging
                if (draggingSpeaker1) {
                    speaker1X = static_cast<float>(event.mouseMove.x);
                    speaker1Y = static_cast<float>(event.mouseMove.y);
                }
                else if (draggingSpeaker2) {
                    speaker2X = static_cast<float>(event.mouseMove.x);
                    speaker2Y = static_cast<float>(event.mouseMove.y);
                }
            }
        }

        // Update the interference pattern when the speakers are moved
        calculateInterferencePattern(speaker1X, speaker1Y, speaker2X, speaker2Y, frequency1, frequency2, speedOfSound, distancePixels, interferencePattern);


        // Update the interference pattern when the speakers are moved
        calculateInterferencePattern(speaker1X, speaker1Y, speaker2X, speaker2Y, frequency1, frequency2, speedOfSound, distancePixels, interferencePattern);

        window.clear(sf::Color::Black);

        // Draw the interference pattern
        window.draw(interferencePattern);

        // Draw the speakers as circles
        sf::CircleShape speaker1Circle(5);
        speaker1Circle.setFillColor(sf::Color::Red);
        speaker1Circle.setPosition(speaker1X - 10, speaker1Y - 10);

        sf::CircleShape speaker2Circle(5);
        speaker2Circle.setFillColor(sf::Color::Blue);
        speaker2Circle.setPosition(speaker2X - 10, speaker2Y - 10);

        window.draw(speaker1Circle);
        window.draw(speaker2Circle);

        window.display();
    }

    return 0;
}
