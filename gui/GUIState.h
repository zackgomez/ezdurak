#pragma once
#include <memory>
#include <SDL/SDL.h>

class GUIState;
typedef std::auto_ptr<GUIState> GUIStatePtr;

/** 
 * The GUIApp class uses this as the states in its implementation of a state
 * machine.  Has methods for transitioning to another state as well as 
 * responsibilities.  A GUIState must implement the render and processEvent 
 * methods.
 */
class GUIState
{
public:
	virtual ~GUIState() {};

        /** 
         * Called once per frame.
         */
	virtual void render() = 0;
        /** 
         * A user event, can be ignored.
         * 
         * @param e The event
         */
	virtual void processEvent(SDL_Event& e) = 0;

        /**
         * Returns true if this state needs to transition to a different state.
         */
        virtual bool needsTransition() const = 0;
        /**
         * Returns the next state, or GUIStatePtr() if the Application should
         * quit.  In this implementation, only QuitState returns a null ptr,
         * given a uniform exit point for the application to save state.
         */
	virtual GUIStatePtr nextState() = 0;

protected:
	GUIState() {};
};
