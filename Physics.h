#pragma once

#include "SDL_opengl.h"
// Included to make some awesome math availible.
#include "Mathematics.h"
#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"

using namespace Mathematics;

const float GRAVITY = 9.81f;				// Note that this is the gravity for earth, should be exchanged if we ever wanted to calculate the gravity by the mass of the planet.

extern bool gravityIsTurnedOn;				// This defines wether or not the global gravity (for all objects) is turned on.

class Physics
{
public:
	struct State
	{
		Vector _position;
		Vector _momentum;			//Primary physical Atributes
		Quaternion _orientation;
		Vector _angularMomentum;

		Vector _velocity;
		Quaternion _spin;			//Secondary ------||--------
		Vector _angularVelocity;

		float _size;
		float _mass;
		float _inverseMass;			//Object Atributes.
		float _inertiaTensor;
		float _inverseInertiaTensor;

		//Recalculates the states between their "forms"
		void recalculate()
		{
			_velocity = _momentum * _inverseMass;
			_angularVelocity = _angularMomentum * _inverseInertiaTensor;
			_orientation.normalize();
			_spin = 0.5 * Quaternion(0,_angularVelocity.x,_angularVelocity.y,_angularVelocity.z) * _orientation;
		}
	};

	Physics(void);
	virtual ~Physics(void);

	//At this point I'm not sure why it takes the total time as one of the parameters.																		//NEEDS PROFESSIONALS!
	void update(float t, float dt);
	void render(float alpha = 1.0f);
	State getState();

	protected:
	float _tiltInRadians;
	bool _axisIsFlipped;
	
	
	State previous;			//These save the states of the objects. 
    State current;			// Where the object was and where it is/will be.
	
	State interpolate(const State &a, const State &b, float alpha);
	
	struct Derivative
	{
		Vector _velocity;
		Vector _force;			//The derivatives of the primary and secondary states.
		Quaternion _spin;
		Vector _torque;
	};

	
	Derivative evaluate(const State &state, float t);
	
	Derivative evaluate(State &state, float t, float dt, const Derivative &derivative);							//These function needs to be checked for N00B mistakes.
																												//	what is the correct parameter? (total-time or deltaTime?)
	void integrate(State &state, float t, float dt);
		
	virtual void forces(const State &state, float t, Vector &force, Vector &torque) = 0;						//	ALL objects with physics are affected by forces, thus they all need
																												//	to implement the forces function.
};																												

