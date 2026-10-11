#include "Workstation.h"

/* Define global deques for CustomerOrder */
std::deque<seneca::CustomerOrder> seneca::g_pending;
std::deque<seneca::CustomerOrder> seneca::g_completed;
std::deque<seneca::CustomerOrder> seneca::g_incomplete;

void seneca::Workstation::fill(std::ostream& os)
{
    if (!m_orders.empty())
    {
		m_orders.front().fillItem(*this, os);
    }
}

bool seneca::Workstation::attemptToMoveOrder()
{
	if (!m_orders.empty())
	{
		if (m_orders.front().isItemFilled(getItemName()) || getQuantity() == 0)
		{
			if (m_pNextStation)										// If the next station is avaialble
			{
				*m_pNextStation += std::move(m_orders.front());
			}
			else if (m_orders.front().isOrderFilled())				// If the order is completed
			{
				g_completed.push_back(std::move(m_orders.front()));
			}
			else													// If the order is incomplete
			{
				g_incomplete.push_back(std::move(m_orders.front()));
			}
			m_orders.pop_front();
			return true;
		}
	}

    return false;
}

void seneca::Workstation::display(std::ostream& os) const
{
	os << getItemName() << " --> ";
	if (m_pNextStation)
	{
		os << m_pNextStation->getItemName();
	}
	else
	{
		os << "End of Line";
	}
	os << std::endl;
}

seneca::Workstation& seneca::Workstation::operator+=(CustomerOrder&& newOrder)
{
	m_orders.push_back(std::move(newOrder));	// Use std::move to avoid copying the object
    return *this;
}
