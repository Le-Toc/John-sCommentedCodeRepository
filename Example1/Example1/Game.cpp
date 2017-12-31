#include "Game.hpp"
#include "StringHelpers.hpp"

const float Game::PlayerSpeed = 100.f;																//This is used to move the player
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);										//Used to tell the program when to update

																									//Game Constructor
Game::Game() : mWindow(sf::VideoMode(640, 480), "SFML Application", sf::Style::Close),				//This creates the game window
mTexture(),
mPlayer(),																						//mPlayer is a sprite
mFont(),
mStatisticsText(),
mStatisticsUpdateTime(),																		//mStatisticsUpdateTime is a time object
mStatisticsNumFrames(0),																		//mStatisticsNumFrames is a size_t
mIsMovingUp(false), mIsMovingDown(false), mIsMovingRight(false), mIsMovingLeft(false)
{
	//Test if the file can be found
	if (!mTexture.loadFromFile("Media/Textures/Eagle.png"))											//Load the texture Eagle.png into mTexture.  If this cannot be done do the following...
	{
	}

	//Create the sprite image
	mPlayer.setTexture(mTexture);																	//Set the texture of the player
	mPlayer.setPosition(100.f, 100.f);																//Set the position of the player

																									//Create the text
	mFont.loadFromFile("Media/Sansation.ttf");														//Load the font file into the variable mFont
	mStatisticsText.setFont(mFont);																	//Set mStatisticsText to mFont
	mStatisticsText.setPosition(5.f, 5.f);															//Set the position of the text
	mStatisticsText.setCharacterSize(10);															//Set the size of the character
}

//Runs the whole game
void Game::run()
{
	//Only called once
	sf::Clock clock;																				//Declare a clock called clock
	sf::Time timeSinceLastUpdate = sf::Time::Zero;													//Declare a time object called timeSinceLastUpdate and set it to 0

																									//Start of the game loop
	while (mWindow.isOpen())																		//While the window is still open...
	{
		/*Controlling The Frames/ Updates:
		The code to do with time makes sure that all of the updates are called.  It does this
		by first taking the current time and giving it to a variable.  It then resets the time.
		It asked if the variable is greater than a time, in this case 0.016secs,  If it is the
		update methods are called.  Before the update methods are called however the variable
		is reduced by one 0.016.  This accounts for one update.  This means that if the program
		is running late the update fuction will be called twice in a row or as many times as is
		needed to get it back to a good speed.  Using this method no update is lost.
		*/

		//Check if the game needs to be updated
		//Restart the clock everytime the program loops
		sf::Time elapsedTime = clock.restart();														//clock.restart() puts the time counter back to zero and also returns the current time.  Declare a time object called elapsedTime and give it the value of the time sence the clock started

																									//Add the time to the timeSinceLastUpdate
		timeSinceLastUpdate += elapsedTime;

		while (timeSinceLastUpdate > TimePerFrame)													//Time per frame is equal to 0.016 secs.  So if an update hasn't occured in 0.016 seconds update
		{
			//Account for the update
			timeSinceLastUpdate -= TimePerFrame;													//Reduce the time since the last update by 1.6 seconds

																									//Call the updates for the game
			processEvents();
			update(TimePerFrame);
		}

		updateStatistics(elapsedTime);
		render();
	}
}

//This program decides what to do with player input
void Game::processEvents()
{
	sf::Event event;																				//Create an event called event

	while (mWindow.pollEvent(event))																//While there is an event to process...  pollEvent(Event) returns true if there is an event that needs to be processed by checking if there is an event at the top of the event queue.
	{
		switch (event.type)																			//Activate a switch statement based on the type of the event...
		{
		case sf::Event::KeyPressed:																	//If the event is a key pressed...
			handlePlayerInput(event.key.code, true);												//Call the handlePlayerInput function.  Get the code of the key that was pressed in the event and send that in
			break;

		case sf::Event::KeyReleased:																//If the event is a key released...
			handlePlayerInput(event.key.code, false);
			break;

		case sf::Event::Closed:																		//If the the user clicked the x button...
			mWindow.close();																		//Close the window
			break;
		}
	}
}

//Sets variables based on player input
void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)//									//Take in a code and assign it to a keyboard key value called key
{
	if (key == sf::Keyboard::W)																		//If key is equal to w...
		mIsMovingUp = isPressed;																	//Set to mIsMovingUp to either true or false

	else if (key == sf::Keyboard::S)
		mIsMovingDown = isPressed;

	else if (key == sf::Keyboard::A)
		mIsMovingLeft = isPressed;

	else if (key == sf::Keyboard::D)
		mIsMovingRight = isPressed;
}

//Updates game based on those variables
void Game::update(sf::Time elapsedTime)//This method adjusts the player position based on the results of the player input from handlePlayerInput and the time per frame which stays the same
{
	sf::Vector2f movement(0.f, 0.f);																//Declare a vector2 with the values (0, 0) and call it movement

	if (mIsMovingUp)																				//If the player is moving up..
		movement.y -= PlayerSpeed;																	//Decrease the y by the speed of the player.  Decreasing the y value will move the player up the screen

	if (mIsMovingDown)
		movement.y += PlayerSpeed;

	if (mIsMovingLeft)
		movement.x -= PlayerSpeed;

	if (mIsMovingRight)
		movement.x += PlayerSpeed;

	mPlayer.move(movement * elapsedTime.asSeconds());												//.move() is a transformable.  Multiply the movement by the amount of seconds that have gone by and move the player by that much.  elapsedTime.asSeconds() will return 0.016.
}

//Updates the game stats text
void Game::updateStatistics(sf::Time elapsedTime)													//Takes in the current time
{
	//Log the time and the frames
	mStatisticsUpdateTime += elapsedTime;															//Set mStatisticsUpdateTime to the current time
	mStatisticsNumFrames += 1;																		//Increase the number of frames by 1

																									//Update the stats every second
	if (mStatisticsUpdateTime >= sf::seconds(1.0f))													//if mStatisticsUpdateTime is greater than or equal to 1...
	{
		mStatisticsText.setString("Frames / Second = " + toString(mStatisticsNumFrames) + "\n" +
			"Time / Update = "
			+ toString(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us");		//Set the text of mStatisticsText.  Output the frames per second and the time between frames per second

		mStatisticsUpdateTime -= sf::seconds(1.0f);													//Set the update time to 1 second
		mStatisticsNumFrames = 0;																	//Set the number of frames to 0
	}
}

//Displays everthing in the game onto the screen
void Game::render()
{
	mWindow.clear();																				//Clear the window
	mWindow.draw(mPlayer);																			//Draw the player onto the screen
	mWindow.draw(mStatisticsText);																	//Draw the text onto the screen
	mWindow.display();																				//Display the contents of the window.  Nothing is drawn onto the screen until this point is reached.
}