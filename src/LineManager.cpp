#include <algorithm>
#include <fstream>
#include "LineManager.h"

seneca::LineManager::LineManager(const std::string& file, const std::vector<Workstation*>& stations)
{
	std::ifstream fin(file);
	if (!fin.is_open())
	{
		throw std::string("Failed to open [") + file + "]!";
	}

	std::string line;
	while (std::getline(fin, line))
	{
		Utilities util;
		size_t next_pos = 0;
		bool more = true;

		// Extract the current Workstation and the next Workstation if exists
		std::string currentWs = util.extractToken(line, next_pos, more);
		std::string nextWs = more ? util.extractToken(line, next_pos, more) : "";

		// Find out the correct current Workstations in the stations vector based on the extracted station names
		auto itCurrent = std::find_if(stations.begin(), stations.end(),
			[&currentWs](Workstation* ws) { return ws->Station::getItemName() == currentWs; });

		// If currentWs is found in the stations vector
		if (itCurrent != stations.end())
		{
			auto itNext = std::find_if(stations.begin(), stations.end(),
				[&nextWs](Workstation* ws) { return ws->Station::getItemName() == nextWs; });

			// If the next Workstation is not empty and found in the stations vector, set the next Workstation; 
			if (itNext != stations.end())
			{
				(*itCurrent)->setNextStation(*itNext);
			}

			// Add the current Workstation to the activeLine
			m_activeLine.push_back(*itCurrent);
		}
	}

	// Find the first Workstation in the activeLine
	m_firstStation = *std::find_if(stations.begin(), stations.end(),
		[&stations](Workstation* ws)
		{
			return std::none_of(stations.begin(), stations.end(),
				[ws](Workstation* otherWs) { return otherWs->getNextStation() == ws; });
		}
	);

	// Set the order count
	m_cntCustomerOrder = g_pending.size();
}

void seneca::LineManager::reorderStations()
{
	std::vector<Workstation*> temp;
	Workstation* currentStation = m_firstStation;

	// Sort the activeLine based on the order of the Workstations (Like a singly linked list)
	while (currentStation)
	{
		temp.push_back(currentStation);
		currentStation = currentStation->getNextStation();
	}
	m_activeLine = temp;
}

bool seneca::LineManager::run(std::ostream& os)
{
	static size_t iteration = 0;
	++iteration;

	os << "Line Manager Iteration: " << iteration << std::endl;

	// Move the first pending orders to the first Workstation
	if (!g_pending.empty())
	{
		*m_firstStation += std::move(g_pending.front());
		g_pending.pop_front();
	}

	// Fill each Workstation once
	for (auto& ws : m_activeLine)
	{
		ws->fill(os);
	}

	// Move order on the activeLine
	for (auto it = m_activeLine.begin(); it != m_activeLine.end(); ++it)
	{
		(*it)->attemptToMoveOrder();
	}

	// Return true if all orders are completed, otherwise return false
	return g_completed.size() + g_incomplete.size() == m_cntCustomerOrder;
}

void seneca::LineManager::display(std::ostream& os) const
{
	for (auto it = m_activeLine.begin(); it != m_activeLine.end(); ++it)
	{
		(*it)->display(os);
	}
}
