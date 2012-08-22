//#include <iostream>					//included for debugging (cout) purposes.
#include "Physics.h"

bool gravityIsTurnedOn = true;

Physics::Physics(void)
{
	current._size = 1;
	current._mass = 1;
	current._inverseMass = 1.0f / current._mass;
	current._position = Vector(0,0,0);		
	current._momentum = Vector(0,0,0);
	current._orientation.identity();
	current._angularMomentum = Vector(0, 0, 0);
	current._inertiaTensor = current._mass * current._size * current._size * 1.0f / 6.0f;
	current._inverseInertiaTensor = 1.0f / current._inertiaTensor;
	current.recalculate();
	previous = current;
}


Physics::~Physics(void)
{
}

void Physics::update(float t, float dt)
{
	previous = current;
	integrate(current, t, dt);  
}

void Physics::render(float alpha)
{		
	glPushMatrix();
		
	//Takes the remainder of the timestep and interpolates in order to make the 
	//	animation smoother.
	State state = interpolate(previous, current, alpha);	
	
	//	Draws the object  //
		
	glTranslatef(state._position.x, state._position.y, state._position.z);	//Translation after physics
		
	float angle;													//This bunch here
	Mathematics::Vector axis;										//	contains the
	state._orientation.angleAxis(angle, axis);						//	rotation of the
	glRotatef(angle/Mathematics::pi*180, axis.x, axis.y, axis.z);	//	object after physics
		
	glPopMatrix();
}

Physics::State Physics::interpolate(const State &a, const State &b, float alpha)
{
	State state = b;
	state._position = a._position*(1-alpha) + b._position*alpha;
	state._momentum = a._momentum*(1-alpha) + b._momentum*alpha;
	state._orientation = slerp(a._orientation, b._orientation, alpha);
	state._angularMomentum = a._angularMomentum*(1-alpha) + b._angularMomentum*alpha;
	state.recalculate();
	return state;
}
		
Physics::Derivative Physics::evaluate(const State &state, float t)
{
	Derivative output;
	output._velocity = state._velocity;
	output._spin = state._spin;
	forces(state, t, output._force, output._torque);
	return output;
}
	
Physics::Derivative Physics::evaluate(State &state, float t, float dt, const Derivative &derivative)
{
	state._position += derivative._velocity * dt;
	state._momentum += derivative._force * dt;
	state._orientation += derivative._spin * dt;
	state._angularMomentum += derivative._torque * dt;
	state.recalculate();
		
	Derivative output;
	output._velocity = state._velocity;
	output._spin = state._spin;
	forces(state, dt, output._force, output._torque);
	return output;		
}

void Physics::integrate(State &state, float t, float dt)
{
	Derivative a = evaluate(state, dt);
	Derivative b = evaluate(state, t, dt*0.5f, a);
	Derivative c = evaluate(state, t, dt*0.5f, b);
	Derivative d = evaluate(state, t, dt, c);
	
	state._position += 1.0f/6.0f * dt * (a._velocity + 2.0f*(b._velocity + c._velocity) + d._velocity);
	state._momentum += 1.0f/6.0f * dt * (a._force + 2.0f*(b._force + c._force) + d._force);
	state._orientation += 1.0f/6.0f * dt * (a._spin + 2.0f*(b._spin + c._spin) + d._spin);
	state._angularMomentum += 1.0f/6.0f * dt * (a._torque + 2.0f*(b._torque + c._torque) + d._torque);
	state.recalculate();
}

Physics::State Physics::getState()
{
	return current;
}