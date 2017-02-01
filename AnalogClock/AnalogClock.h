#pragma once
#include <SFML\Graphics.hpp>
#include <chrono>

class AnalogClock
{
private:
	//Displays the clock
	sf::RenderWindow clockWindow;
	//Saves the view of the window for use when resizing the widow.
	sf::View view;
	//Saves the previous size of the window before fullscreening
	sf::Vector2f origionalSize;
	//The origional center
	sf::Vector2i previousPosition;
	float smallerWH, biggerWH;
	unsigned int width, height;
	//Event handler for the window
	sf::Event event;
	//The background clock that does the counting
	std::chrono::milliseconds clockTime;

	//Used to draw the hour hand
	sf::RectangleShape hourHand;
	//Used to draw the minute hand
	sf::RectangleShape minuteHand;
	//Used to draw the second hand
	sf::RectangleShape secondHand;

	//The little peice on top of all of the hands.
	sf::CircleShape topper;
	//Used to draw the tick for a hour
	sf::RectangleShape hourShape;
	//Used to draw the tick for a 60th of the circle
	sf::RectangleShape sixtiethShape;

	//long longs to store the miliseconds since epoch (Jan 1, 1970), and time today
	long long fullTime, baseTime;
	//doubles for each piece of time to store 'exact' time.
	float hours, minutes, seconds, miliseconds;
	//Int for hours and seconds only.
		//Hours simply for printing
		//Seconds used to calculate the exact number of seconds this minute
	int hoursInt, secondsInt;

	//Used to draw the time as a string onto the clock
	sf::Text timeStringShape;
	sf::Font theFont;
	std::string hoursString = "00";
	std::string minutesString = "00";
	std::string secondsString = "00";
	std::string milisecondsString = "00";
	std::string timeStr;

	bool doHandsTick;
	bool isFullscreen = false;
	bool spacePressed;
	bool f11Pressed;

	//Sets up each shape
	void setup();
	//Updates the angles for every hand, which also means it updates the time.
	void update();
	//Runs the window for the clock
	void run();
	//Draws all shapes to the window
	void draw();

	float getX(float angle);
	float getY(float angle);

	void setNumberLocation(bool ms);
public:
	//Constructor for the clock
	//Takes the width and height of the window
	AnalogClock(float width, float height);

	//Method determining if the hands should tick.
	void tickHands(bool tickHands);
};