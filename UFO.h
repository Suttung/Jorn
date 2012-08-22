#pragma once

#include "Physics.h"
#include "SDL_keysym.h"					//Needed for the keyboard symbols to work.
#include "SDL_opengl.h"

extern uint8_t * keyboardStates;

class UFO : public Physics
{
public:
	UFO(void);
	~UFO(void);

	void body();
	void render(float alpha);
	void reset();
	void controls();

private:
	float _enginePower;
	float _tiltSpeed;
	bool _engineIsThrusting;
	bool _engineIsReversed;
	bool _isTiltingLeft;
	bool _isTiltingRight;
	
	
	void forces(const State &state, float t, Vector &force, Vector &torque);
};

