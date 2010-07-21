#pragma once
#include <memory>
#include <SDL/SDL.h>

class GUIState;
typedef std::auto_ptr<GUIState> GUIStatePtr;

class GUIState
{
public:
	GUIState() {};
	virtual ~GUIState() {};

	virtual void render() = 0;
	virtual void processEvent(SDL_Event& e) = 0;

        /**
         * Returns true if this state needs to transition to a different state.
         */
        virtual bool needsTransition() const = 0;
        /**
         * Returns the next state, or GUIStatePtr() if the Application should
         * quit.
         */
	virtual GUIStatePtr nextState() = 0;
};
