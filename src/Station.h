#ifndef SENECA_STATION_H
#define SENECA_STATION_H

#include <iostream>
#include <string>
#include "Utilities.h"

namespace seneca
{
	class Station
	{
		int m_id{};
		std::string m_name;
		std::string m_desc;
		size_t m_serial{};
		size_t m_quantity{};

		static size_t m_widthField;
		static int id_generator;
	public:
		const std::string& getItemName() const { return m_name; }
		size_t getNextSerialNumber() { return m_serial++; }
		size_t getQuantity() const { return m_quantity; }
		void updateQuantity();

		Station(const std::string& str);
		void display(std::ostream& os, bool full) const;
	};
}

#endif // !SENECA_STATION_H
