#include "Utilities.h"


/* Initialization */
char seneca::Utilities::m_delimiter = ' ';

void seneca::Utilities::setFieldWidth(size_t newWidth)
{
	m_widthField = newWidth;
}

size_t seneca::Utilities::getFieldWidth() const
{
    return m_widthField;
}

std::string seneca::Utilities::extractToken(const std::string& str, size_t& next_pos, bool& more)
{
    std::string token;
	size_t pos_delimiter = str.find(m_delimiter, next_pos);

	// Check if a delimiter is found at the next position
	if (next_pos == pos_delimiter)
	{
		more = false;
		throw "Error! Delimiter is found at the next position.";
	}

	// If no delimiter is found, return the remaining string
	if (pos_delimiter == std::string::npos)
	{
		more = false;
		token = trim(str.substr(next_pos));
	}
	else
	// Extract the token
	{
		token = str.substr(next_pos, pos_delimiter - next_pos);
		next_pos = pos_delimiter + 1;
		more = true;
		token = trim(token);
	}

	// Update m_widthField if the token length is greater than the current m_widthField
	if (token.length() > m_widthField)
	{
		setFieldWidth(token.length());
	}

	return token;
}

/**
 * Static Methods
 */
void seneca::Utilities::setDelimiter(char newDelimiter)
{
    m_delimiter = newDelimiter;
}

char seneca::Utilities::getDelimiter()
{
	return m_delimiter;
}
