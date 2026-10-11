#include <iomanip>
#include "Station.h"

/* Initialization */
size_t seneca::Station::m_widthField = 0;
int seneca::Station::id_generator = 0;

void seneca::Station::updateQuantity()
{
	if (m_quantity > 0)
	{
		--m_quantity;
	}
}

seneca::Station::Station(const std::string& str)
{
	Utilities util;			// Utilities object to extract tokens
	size_t next_pos = 0;	// Position of the next token
	bool more = true;		// Indicate if there are more tokens in the input string

	// Set the ID
	++id_generator;
	m_id = id_generator;
	
	// Extract the token from the input string
	m_name = util.extractToken(str, next_pos, more);
	m_serial = std::stoul(util.extractToken(str, next_pos, more));
	m_quantity = std::stoul(util.extractToken(str, next_pos, more));

	// Update the field width to the maximum value of m_widthField in the two classes
	if (util.getFieldWidth() > m_widthField)
	{
		m_widthField = util.getFieldWidth();
	}
	m_desc = util.extractToken(str, next_pos, more);
}

void seneca::Station::display(std::ostream& os, bool full) const
{
	// ID + Name + Serial
	os << std::setw(3) << std::setfill('0') << std::right << m_id << " | "
		<< std::setw(m_widthField) << std::setfill(' ') << std::left << m_name << " | "
		<< std::setw(6) << std::setfill('0') << std::right << m_serial << " | ";

	// Quantity and Serial if full is true
	if (full)
	{
		os << std::setw(4) << std::setfill(' ') << std::right << m_quantity << " | " << m_desc;
	}
	os << std::endl;
}
