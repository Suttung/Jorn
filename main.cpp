/*This source code copyrighted by Lazy Foo' Productions (2004-2012)
and may not be redistributed without written permission.*/

//The headers
#include <iostream>
#include "SDL_opengl.h"
#include "SDL_image.h"
#include "UFO.h"


//Screen attributes
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int SCREEN_BPP = 32;


//The frame rate
const int FRAMES_PER_SECOND = 60;

//Event handler
SDL_Event event;

//Physics Timer
double frameTime = 0;
double previousTime = 0;
double newTime = 0;

float t = 0.0f;								//The Time the physics Simulation has been running in secs
float dt = 10;							//Physics updates per second. (in millisec 
											//	NEVER!!!! Put in the value as a fraction!!!! EVER!!!!
float accumulatorOfFrameTime = 0.0f;		//Allows the engine to update physics at the correct rate.

int frame = 0;      
int currentTime;
int baseTime = 0;
char title[100];

//PLAYER OBJECT
UFO Player;

//state Of Keyboard
uint8_t * keyboardStates = SDL_GetKeyState(NULL);


//Rendering flag
bool renderQuad = true;

//The timer
class FPSTimer
{
    private:
    //The clock time when the timer started
    int startTicks;

    //The ticks stored when the timer was paused
    int pausedTicks;

    //The timer status
    bool paused;
    bool started;

    public:
    //Initializes variables
    FPSTimer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    int get_ticks();

    //Checks the status of the timer
    bool is_started();
    bool is_paused();
};

bool initGL()
{
    //Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

	glOrtho(0.0f, 1280, 720, 0.0f, -1.0f, 1.0f);

    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //Initialize clear color
    glClearColor( 0.f, 0.f, 0.f, 1.f );

    //Check for error
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
        return false;
    }

    return true;
}

FPSTimer  timePassedCounter;

bool init()
{
    //Initialize SDL
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    {
        return false;
    }

    //Create Window
    if( SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_OPENGL ) == NULL )
    {
        return false;
    }

    //Enable unicode
    SDL_EnableUNICODE( SDL_TRUE );

    //Initialize OpenGL
    if( initGL() == false )
    {
        return false;
    }

    //Set caption
    SDL_WM_SetCaption( "OpenGL Test", NULL );

	

	timePassedCounter.start();

    return true;
}



void update()
{
	
	
	
	newTime = timePassedCounter.get_ticks();
	frameTime = newTime - previousTime;
	if (frameTime > 250) {
		frameTime = 250;
	}
	previousTime = newTime;
	accumulatorOfFrameTime += frameTime;
	
	while (accumulatorOfFrameTime>=dt)
	{
		Player.controls();
		Player.update(t, dt);
		t += dt;
		accumulatorOfFrameTime -= dt;
	}

	std::cout << t;
}

void render()
{
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );

	Player.render();

    //Update screen
    SDL_GL_SwapBuffers();
}

void clean_up()
{
    //Quit SDL
    SDL_Quit();
}

FPSTimer::FPSTimer()
{
    //Initialize the variables
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void FPSTimer::start()
{
    //Start the timer
    started = true;

    //Unpause the timer
    paused = false;

    //Get the current clock time
    startTicks = SDL_GetTicks();
}

void FPSTimer::stop()
{
    //Stop the timer
    started = false;

    //Unpause the timer
    paused = false;
}

void FPSTimer::pause()
{
    //If the timer is running and isn't already paused
    if( ( started == true ) && ( paused == false ) )
    {
        //Pause the timer
        paused = true;

        //Calculate the paused ticks
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void FPSTimer::unpause()
{
    //If the timer is paused
    if( paused == true )
    {
        //Unpause the timer
        paused = false;

        //Reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;

        //Reset the paused ticks
        pausedTicks = 0;
    }
}

int FPSTimer::get_ticks()
{
    //If the timer is running
    if( started == true )
    {
        //If the timer is paused
        if( paused == true )
        {
            //Return the number of ticks when the timer was paused
            return pausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            return SDL_GetTicks() - startTicks;
        }
    }

    //If the timer isn't running
    return 0;
}

bool FPSTimer::is_started()
{
    return started;
}

bool FPSTimer::is_paused()
{
    return paused;
}

int main( int argc, char *argv[] )
{
    //Quit flag
    bool quit = false;

    //Initialize
    if( init() == false )
    {
        return 1;
    }

    //The frame rate regulator
    FPSTimer fps;

	//Wait for user exit
	while( quit == false )
	{
        //Start the frame timer
        fps.start();

        //While there are events to handle
		while( SDL_PollEvent( &event ) )
		{
			if( event.type == SDL_QUIT )
			{
                quit = true;
            }      
		}

        //Run frame update
        update();

        //Render frame
        render();

        //Cap the frame rate
        if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
        {
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }
	}

	//Clean up
	clean_up();

	return 0;
}
