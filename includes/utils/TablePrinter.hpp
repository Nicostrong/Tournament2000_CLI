//
// Created by Nicolas Fordoxcel on 04/09/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <string>
# include <vector>
# include <iomanip>

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;
using				vString			=	std::vector<String>;
using				cvString		=	const std::vector<String>&;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

/**
 * Gestion du paramettrage et l affichage d un tableau
 */
class				TablePrinter
{
	private:

		struct		Row
		{
					vString			columns;
					String			color;
		};

		vString						_headers;
		std::vector<Row>			_rows;
		std::vector<size_t>			_colWidths;

		void						updateWidths(cvString cols);
		void						printLine(std::ostream& out) const;

	public:

		void						setHeaders(cvString headers);
		void						addRow(cvString columns, cString color = "");
		void						printTable(std::ostream& out, bool toFile = false) const;

};