#ifndef _EVENTLISTENER_H_
#define _EVENTLISTENER_H_

#include "Event.h"
#include <mutex>
#include <vector>

class EventListener
{
private:
	
	std::vector<std::shared_ptr<Event> > m_queue[2];
	int m_currentQueue = 0;
	std::mutex m_mutex;

public:

	// is called when queued event is processed
	virtual void VProcessEvent( Event& n_event ) = 0;

	// is called when EventManager processes events and 
	// this EventListener is registered for this particular event type
	void QueueEvent(std::shared_ptr<Event> n_event);

	// processes all queued events
	void ProcessQueuedEvents();
};

class EventListener_DEBUG_OUTPUT : public EventListener
{
public:

	void VProcessEvent( Event& n_event );
};

// this is a default implementation of a human view listener
class GameView_Human;
class HumanViewListener : public EventListener
{
protected:
	GameView_Human* m_view;
public:
	HumanViewListener(GameView_Human* n_view);
	virtual void VProcessEvent( Event& n_event );
};

// TODO default logic listener
#endif