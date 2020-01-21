#include "Observer.h"



Observer::Observer()
{
}


Observer::~Observer()
{
}

void Subject::Attach(Observer *o) {
	m_observers.push_back(o);
}

void Subject::Detach(Observer *o) {
	m_observers.remove(o);
}

void Subject::Notify() {
	for (const auto& o : m_observers) {
		o->Update(this);
	}
}
