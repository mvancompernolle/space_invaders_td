#pragma once

#ifndef ON_KEY_OBSERVER_H
#define ON_KEY_OBSERVER_H

class OnKeyObserver {
public:
	OnKeyObserver();
	~OnKeyObserver();
	virtual void onKeyPressed( unsigned key ) = 0;
	virtual void onKeyReleased( unsigned key ) = 0;
};

#endif // ON_KEY_OBSERVER_H

