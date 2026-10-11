#include <iomanip>
#include <vector>
#include "CustomerOrder.h"

size_t seneca::CustomerOrder::m_widthField = 0;

seneca::CustomerOrder::CustomerOrder(const std::string& str)
{
    Utilities util;
    size_t next_pos = 0;
    bool more = true;

	// Extract the customer name and product name
	m_name = util.extractToken(str, next_pos, more);
	m_product = util.extractToken(str, next_pos, more);

	// Create a vector to store the items
	std::vector<std::string> items;
	// Continue extracting tokens until there are no more tokens
	while (more)
	{
		items.push_back(util.extractToken(str, next_pos, more));
	}
	// Manage the list of items (Composition)
	m_cntItem = items.size();
	m_lstItem = new Item* [m_cntItem];
	for (size_t i = 0; i < m_cntItem; ++i)
	{
		m_lstItem[i] = new Item(items[i]);
	}

	// Update the field width if the token length is greater than the width in the Utilities object
	if (m_widthField < util.getFieldWidth())
	{
		m_widthField = util.getFieldWidth();
	}
}

seneca::CustomerOrder::CustomerOrder(CustomerOrder&& src) noexcept
{
	*this = std::move(src);
}

seneca::CustomerOrder& seneca::CustomerOrder::operator=(CustomerOrder&& src) noexcept
{
	if (this != &src)
	{
		for (size_t i = 0; i < m_cntItem; ++i)
		{
			delete m_lstItem[i];
		}
		delete[] m_lstItem;

		m_name = src.m_name;
		m_product = src.m_product;
		m_cntItem = src.m_cntItem;
		m_lstItem = src.m_lstItem;

		src.m_cntItem = 0;
		src.m_lstItem = nullptr;
	}

    return *this;
}

seneca::CustomerOrder::~CustomerOrder()
{
	for (size_t i = 0; i < m_cntItem; ++i)
	{
		delete m_lstItem[i];
	}
	delete[] m_lstItem;
}

bool seneca::CustomerOrder::isOrderFilled() const
{
	for (size_t i = 0; i < m_cntItem; ++i)
	{
		// If any of the items in the order is not filled, return false
		if (!m_lstItem[i]->m_isFilled)
		{
			return false;
		}
	}

	return true;
}

bool seneca::CustomerOrder::isItemFilled(const std::string& item) const
{
	for (size_t i = 0; i < m_cntItem; ++i)
	{
		if (m_lstItem[i]->m_itemName == item && !m_lstItem[i]->m_isFilled)
		{
			return false;
		}
	}

	return true;
}

void seneca::CustomerOrder::fillItem(Station& station, std::ostream& os)
{
	for (size_t i = 0; i < m_cntItem; ++i)
	{
		if (m_lstItem[i]->m_itemName == station.getItemName() && !m_lstItem[i]->m_isFilled)
		{
			if (station.getQuantity() > 0)
			{
				m_lstItem[i]->m_serialNumber = station.getNextSerialNumber();	// Update serial number
				m_lstItem[i]->m_isFilled = true;								// Update the isFilled
				station.updateQuantity();										// Update quantity
				os << "    Filled " << m_name << ", " << m_product << " [" << m_lstItem[i]->m_itemName << "]" << std::endl;
				break;															// If one item is filled, exit the loop
			}
			else
			{
				os << "    Unable to fill " << m_name << ", " << m_product << " [" << m_lstItem[i]->m_itemName << "]" << std::endl;
			}
		}
	}
}

void seneca::CustomerOrder::display(std::ostream& os) const
{
	os << m_name << " - " << m_product << std::endl;
	for (size_t i = 0; i < m_cntItem; ++i)
	{
		os << "[" << std::setw(6) << std::setfill('0') << std::right << m_lstItem[i]->m_serialNumber << "] "
			<< std::setw(m_widthField) << std::setfill(' ') << std::left << m_lstItem[i]->m_itemName << " - "
			<< (m_lstItem[i]->m_isFilled ? "FILLED" : "TO BE FILLED") << std::endl;
	}
}
