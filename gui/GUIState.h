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
	virtual GUIStatePtr nextState() = 0;
};