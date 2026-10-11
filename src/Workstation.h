#ifndef SENECA_WORKSTATION_H
#define SENECA_WORKSTATION_H

#include <deque>
#include <iostream>
#include <string>
#include "CustomerOrder.h"
#include "Station.h"

namespace seneca
{
	/* Global deques for CustomerOrder */
	extern std::deque<CustomerOrder> g_pending;
	extern std::deque<CustomerOrder> g_completed;
	extern std::deque<CustomerOrder> g_incomplete;

	class Workstation : public Station
	{
		std::deque<CustomerOrder> m_orders;
		Workstation* m_pNextStation{};
	public:
		Workstation(const std::string& str) : Station(str) {}
		void setNextStation(Workstation* station = nullptr) { m_pNextStation = station; }
		Workstation* getNextStation() const { return m_pNextStation; }

		void fill(std::ostream& os);
		bool attemptToMoveOrder();
		void display(std::ostream& os) const;
		Workstation& operator+=(CustomerOrder&& newOrder);

		/* Copies and Moves are Prohibited */
		Workstation(Workstation&) = delete;
		Workstation(Workstation&&) = delete;
		Workstation& operator=(Workstation&) = delete;
		Workstation& operator=(Workstation&&) = delete;
	};
}

#endif // !SENECA_WORKSTATION_H
