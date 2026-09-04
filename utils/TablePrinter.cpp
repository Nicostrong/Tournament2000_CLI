//
// Created by Nicolas Fordoxcel on 04/09/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <format>
#include <iomanip>

#include "../includes/utils/PrintUtils.hpp"
#include "../includes/utils/TablePrinter.hpp"

#include "../includes/Color.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	EXCEPTION																						*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PRIVATE METHOD																					*/
/****************************************************************************************************/

void				TablePrinter::updateWidths(cvString cols)
{
	if (this->_colWidths.size() < cols.size())
		this->_colWidths.resize(cols.size(), 0);

	for (size_t i = 0; i < cols.size(); ++i)
		if (cols[i].length() > this->_colWidths[i])
			this->_colWidths[i] = cols[i].length();
}

void				TablePrinter::printLine(std::ostream& out) const
{
	out << "+";

	for (size_t w : this->_colWidths)
		out << std::string(w + 2, '-') << "+";

	out << "\n";
}

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

void				TablePrinter::setHeaders(cvString headers)
{
	this->_headers = headers;
	updateWidths(headers);
}

void				TablePrinter::addRow(cvString columns, cString color)
{
	this->_rows.push_back({columns, color});
	updateWidths(columns);
}

void				TablePrinter::printTable(std::ostream& out, bool toFile) const
{
	if (this->_colWidths.empty())
		return;

	printLine(out);

	if (!this->_headers.empty())
	{
		out << "|";

		for (size_t i = 0; i < this->_headers.size(); ++i)
			out << " " << std::left << std::setw(this->_colWidths[i]) << this->_headers[i] << " |";

		out << "\n";
		printLine(out);
	}

	for (const auto& row : this->_rows)
	{
		if (!toFile && !row.color.empty())
			out << row.color;

		out << "|";

		for (size_t i = 0; i < this->_colWidths.size(); ++i)
		{
			std::string cell = (i < row.columns.size()) ? row.columns[i] : "";

			out << " " << std::left << std::setw(_colWidths[i]) << cell << " |";
		}

		if (!toFile && !row.color.empty())
			out << Color::RESET;

		out << "\n";
	}

	if (!this->_rows.empty())
		printLine(out);
}