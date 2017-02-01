#define _USE_MATH_DEFINES 
#include "AnalogClock.h"
#include <cmath>
#include <iostream>
AnalogClock::AnalogClock(float Width, float Height)
{
	width = Width;
	height = Height;

	clockWindow.create(sf::VideoMode(width, height), "Zacks's Clock"/*, sf::Style::None*/);
	//clockWindow.setMouseCursorVisible(false);
	doHandsTick = true;
	clockWindow.setFramerateLimit(60);
	previousPosition = clockWindow.getPosition();
	setup();
	run();
}

void AnalogClock::tickHands(bool tickHands)
{
	doHandsTick = tickHands;
}

void AnalogClock::setup()
{
	view = clockWindow.getDefaultView();

	smallerWH = width <= height ? width : height;
	biggerWH = width <= height ? height : width;

	float halfWidth = width / 2.f;
	float halfHeight = height / 2.f;

	sf::Vector2f temp;

	//Set the size for the hour tick
	temp = sf::Vector2f(smallerWH / 160, smallerWH / 10);
	hourShape.setSize(temp);
	//Set the size for the minute tick
	temp = sf::Vector2f(smallerWH / 160, smallerWH / 20);
	sixtiethShape.setSize(temp);

	hourShape.setPosition(halfWidth, 0);
	sixtiethShape.setPosition(halfWidth, 0);

	hourShape.setFillColor(sf::Color::Color(255,255,255));
	sixtiethShape.setFillColor(sf::Color::Color(155, 155, 155));

	hourShape.setOrigin(hourShape.getSize().x / 2, 0);
	sixtiethShape.setOrigin(sixtiethShape.getSize().x / 2, 0);

	//Set the size for the hour hand
	temp = sf::Vector2f(smallerWH / 160, (smallerWH / 2));
	hourHand.setSize(temp);
	//Set the size for the minute hand
	temp = sf::Vector2f(smallerWH / 160, (smallerWH / 2));
	minuteHand.setSize(temp);
	//Set the size for the second hand
	temp = sf::Vector2f(smallerWH / 160, minuteHand.getSize().y);
	secondHand.setSize(temp);

	hourHand.setOrigin(hourHand.getSize().x / 2, hourShape.getSize().y);
	minuteHand.setOrigin(minuteHand.getSize().x / 2, sixtiethShape.getSize().y);
	secondHand.setOrigin(secondHand.getSize().x / 2, sixtiethShape.getSize().y);

	hourHand.setPosition(halfWidth, halfHeight);
	minuteHand.setPosition(halfWidth, halfHeight);
	secondHand.setPosition(halfWidth, halfHeight);

	hourHand.setRotation(180);
	minuteHand.setRotation(180);
	secondHand.setRotation(180);

	hourHand.setFillColor(sf::Color::Color(255,0,0));
	minuteHand.setFillColor(sf::Color::Color(205, 0, 0));
	secondHand.setFillColor(sf::Color::Color(155, 0, 0));


	topper.setRadius(hourHand.getSize().x * 2);
	topper.setOrigin(topper.getRadius(), topper.getRadius());
	topper.setPosition(halfWidth, halfHeight);
	topper.setFillColor(sf::Color::Red);

	//Set up the font of the 'digital' clock
	theFont.loadFromFile("arial.ttf");
	timeStringShape.setFont(theFont);
	//Setup the text of the text shape
	timeStringShape.setOutlineColor(sf::Color::White);
	timeStringShape.setOutlineThickness(0.f);
	timeStringShape.setFillColor(sf::Color::White);
	timeStringShape.setStyle(sf::Text::Regular);
	timeStringShape.setCharacterSize(smallerWH / 20.f);
	setNumberLocation(!doHandsTick);
}

void AnalogClock::update()
{
	clockTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

	//removes the days from the clock in miliseconds. Meaning, we now have hours
	fullTime = clockTime.count();
	baseTime = fullTime % 86400000;

	hours = (baseTime / 3600000.f);
	hours -= 6;
	//Daylight savings time correction
	hours -= 1;
	if (hours >= 12)
		hours -= 12;
	if (hours <= 1)
		hours += 12;
	int hoursInt = floor(hours);
	//Incremental seconds and minute hand
	if (doHandsTick)
	{
		minutes = floor(((baseTime % 36000000) / 60 / 1000) % 60);
		seconds = floor(((baseTime % 3600000) / 1000) % 60);
	}
	else
	{
		minutes = ((hours - hoursInt) * 60);

		seconds = ((hours - hoursInt) * 60 * 60);
		secondsInt = floor(seconds);
		seconds -= secondsInt;
		secondsInt %= 60;
		seconds = secondsInt + seconds;
		miliseconds = (seconds - (int)floor(seconds)) * 100;
	}

	//Adjust for UTC to my time zone;
	//Debug stuff
	//std::cout << fullTime << " | " << baseTime << " | " << hours << ":" << minutes << ":" << seconds << std::endl;
	

	hourHand.setRotation((hours / 12.f) * 360.f + 180.f);
	minuteHand.setRotation((minutes / 60.f) * 360.f + 180.f);
	secondHand.setRotation((seconds / 60.f) * 360.f + 180.f);

	hoursString = "0" + std::to_string(hoursInt);
	minutesString = std::to_string((int)floor(minutes));
	secondsString = std::to_string((int)floor(seconds));
	milisecondsString = std::to_string((int)miliseconds);

	if (hours >= 10)
	{
		hoursString = std::to_string(hoursInt);
	}
	if (minutes < 10)
	{
		minutesString = "0" + minutesString;
	}
	if (seconds < 10)
	{
			secondsString = "0" + secondsString;
	}
	if (miliseconds < 10)
	{
		milisecondsString = "0" + milisecondsString;
	}
	timeStr = hoursString + ":" + minutesString + ":" + secondsString;
	if (!doHandsTick)
		timeStr += ":" + milisecondsString;
	timeStringShape.setString(timeStr);
}

void AnalogClock::draw()
{
	float angle = 0.f;
	//Draw each tick mark onto the clock
	//Tick marks for every second
	float angleInterval = 360.f / 60.f;
	for (int i = 0;i < 60;i++)
	{
		angle = i * angleInterval;
		sixtiethShape.setRotation(angle); //1000 - 500 * cos(270 - 360) - 500 | 500 * sin(270 - 210) + 500
		sixtiethShape.setPosition(getX(angle), getY(angle));
		clockWindow.draw(sixtiethShape);
	}
	//Tick marks for every hour
	angleInterval = 360.f / 12.f;
	for (int i = 0;i < 12;i++)
	{
		angle = i * angleInterval;
		hourShape.setRotation(angle); //1000 - 500 * cos(270 - 360) - 500 | 500 * sin(270 - 210) + 500
		hourShape.setPosition(getX(angle), getY(angle));
		clockWindow.draw(hourShape);
	}

	//Draw the actual time onto the clock
	clockWindow.draw(timeStringShape);
	//Draw the hands and the topper
	clockWindow.draw(hourHand);
	clockWindow.draw(minuteHand);
	clockWindow.draw(secondHand);
	clockWindow.draw(topper);
}

void AnalogClock::run()
{
	while (clockWindow.isOpen())
	{
		while (clockWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				clockWindow.close();
			}
			if (event.type == sf::Event::Resized)
			{
				width = clockWindow.getSize().x;
				height = clockWindow.getSize().y;
				clockWindow.setView(view = sf::View(sf::FloatRect(0.f, 0.f, width, height)));
				setup();
			}
		}

		if (clockWindow.hasFocus())
		{
			//Input goes in here
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !spacePressed)
			{
				spacePressed = true;
				doHandsTick = !doHandsTick;
				setNumberLocation(!doHandsTick);
			}
			else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				spacePressed = false;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::F11) && !f11Pressed)
			{
				f11Pressed = true;
				isFullscreen = !isFullscreen;
				if (!isFullscreen)
				{
					width = origionalSize.x;
					height = origionalSize.y;
					clockWindow.create(sf::VideoMode(width, height), "Zacks Clock", sf::Style::Default);
					clockWindow.setPosition(previousPosition);
				}
				else
				{
					origionalSize.x = width;
					origionalSize.y = height;
					previousPosition = clockWindow.getPosition();
					width = sf::VideoMode::getDesktopMode().width;
					height = sf::VideoMode::getDesktopMode().height;
					clockWindow.create(sf::VideoMode(width, height), "Zacks Clock", sf::Style::None);
					clockWindow.setPosition(sf::Vector2i(0.f, 0.f));
				}
				setup();
			}
			else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::F11) && f11Pressed)
				f11Pressed = false;
		}

		update();
		clockWindow.clear(sf::Color::Black);
		draw();
		clockWindow.display();
	}
}

float AnalogClock::getX(float angle)
{
	return (width / 2.f) - ((smallerWH / 2.f) * cos((270 - angle) * (M_PI / 180)));
}
float AnalogClock::getY(float angle)
{
	return (height / 2.f) + ((smallerWH / 2.f) * sin((270 - angle) * (M_PI / 180)));
}

void AnalogClock::setNumberLocation(bool ms)
{
	if(ms)
		timeStringShape.setString("00:00:00:00");
	else
		timeStringShape.setString("00:00:00");
	//Set the origin to be the center of the text shape
	timeStringShape.setOrigin(timeStringShape.getLocalBounds().left + timeStringShape.getLocalBounds().width / 2.0f,
	timeStringShape.getLocalBounds().top + timeStringShape.getLocalBounds().height / 2.0f);
	//Then set the position to be the center along x, and 75% down the clock
	timeStringShape.setPosition(sf::Vector2f(width / 2.0f, (height / 2.f) + (hourHand.getSize().y / 2.f)));
}