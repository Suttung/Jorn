#include "UFO.h"

UFO::UFO(void)
{
	_enginePower =  10.0f;
	_tiltSpeed = 0.1f;
	current._size = 1;
	current._mass = 1.0;
	current._inverseMass = 1.0f / current._mass;
	current._position = Vector(150,200,0);		
	current._momentum = Vector(0,0,0);
	current._orientation.identity();
	current._angularMomentum = Vector(0, 0, 0);
	current._inertiaTensor = current._mass * current._size * current._size * 1.0f / 6.0f;
	current._inverseInertiaTensor = 1.0f / current._inertiaTensor;
	current.recalculate();
	previous = current;
}

UFO::~UFO(void)
{
}

void UFO::body()
{
	glBegin( GL_QUADS );
            glColor3f( 0.f, 1.f, 1.f );
            glVertex2f( current._position.x - 100.f, current._position.y -50.f );
            glVertex2f( current._position.x + 100.f, current._position.y -50.f );
            glVertex2f( current._position.x + 50.f, current._position.y + 50.f );
            glVertex2f( current._position.x - 50.f, current._position.y + 50.f );
        glEnd();
}

void UFO::reset()
{
	current._position = Vector(-50, 0, 0);
	current._momentum = Vector(0, 0, 0);
	current._velocity = Vector(0, 0, 0);
	current._orientation.identity();
	current._angularMomentum = Vector(0, 0, 0);
	current._angularVelocity = Vector(0, 0, 0);
}


void UFO::render(float alpha)
{		
	glPushMatrix();
																
	State state = interpolate(previous, current, alpha);	
	
	glTranslatef(state._position.x, state._position.y, state._position.z);
	float angle;
	Mathematics::Vector axis;		
	state._orientation.angleAxis(angle, axis);
	glRotatef(angle/pi*180, axis.x, axis.y, axis.z);
	_tiltInRadians = angle;
		
	if ((axis.x + axis.y + axis.z) < 0)					// Checks to se if the axis is flipped
	{
		_axisIsFlipped = true;
	}
	else
	{
		_axisIsFlipped = false;
	}
	
	//Renders the body of the ufo.
	body();
				
		
	glPopMatrix();
}

void UFO::controls()
{
	if (keyboardStates[SDLK_w] == true)
	{
		_engineIsThrusting = true;
	}
	if (!keyboardStates[SDLK_w])
	{
		_engineIsThrusting = false;
	}
		
	if (keyboardStates[SDLK_s]) 
	{
		_engineIsReversed = true;
	}
	if (!keyboardStates[SDLK_s]) 
	{
		_engineIsReversed = false;
	}
	
	if (keyboardStates[SDLK_a])
	{
		_isTiltingLeft = true;
	}
	if (!keyboardStates[SDLK_a]) 
	{
		_isTiltingLeft = false;
	}
	
	if (keyboardStates[SDLK_d])
	{
		_isTiltingRight = true;
	}
	if (!keyboardStates[SDLK_d]) 
	{
		_isTiltingRight = false;
	}
}

void UFO::forces(const State &state, float t, Vector &force, Vector &torque)
{	
		
	force.zero();
	torque.zero();

	float actualEnginePower = 0.000004 * _enginePower;

		
	if (_engineIsThrusting) 
	{
		if (_axisIsFlipped)
		{
			force.x += actualEnginePower * sinf(_tiltInRadians);
		}
		else
		{
			force.x += actualEnginePower * sinf(_tiltInRadians+pi);
		}
		force.y -= actualEnginePower * sinf(_tiltInRadians + pi/2);
	}
	else if (_engineIsReversed)
	{
		if (_axisIsFlipped) 
		{
			force.x -= (actualEnginePower * 0.2) * sinf(_tiltInRadians);
		}
		else
		{
			force.x -= (actualEnginePower * 0.2) * sinf(_tiltInRadians+pi);	
		}
			force.y += (actualEnginePower * 0.2) * sinf(_tiltInRadians + pi/2);
	}
	else
		
	if (gravityIsTurnedOn) 
	{
		force.y += GRAVITY * 0.000001;
	}
		

	//THESE ARE NOT WORKING FOR TILT AND HAS BEEN CHANGED TO SIDE-ENGINES ATM!!!

	if (_isTiltingLeft)
	{
		force.x -= _enginePower * 0.000001;
	}
	if (_isTiltingRight) 
	{
		force.x += _enginePower * 0.000001;
	}
		
	//gives a "dampening effect to the torque.
	torque -= 0.1f * state._angularVelocity;		
}