/* 
 * $Id$
 * Galactiz Copyright (C) 2005 Cristelle Barillon & Jean-Daniel Pauget
 * displaying a galaxy
 *
 * galactiz@disjunkt.com  -  http://galactiz.disjunkt.com/
 *
 * This file is part of Galactiz.
 * 
 * Galactiz is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * Galactiz is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 * You can also try the web at http://www.gnu.org/
 *
 *
 * galactiz@disjunkt.com  -  http://galactiz.disjunkt.com/
 */

#include <fstream>

#include <map>
#include <string>

using namespace std ;

//! a map of parameters that can be set in rcfile or command line
/*!
 *  readfile is used to extract affectations from an rcfile
 *  readparams is used to extract affectaion from the command-line args
 *  the order in which params are read sets the priority. the last read
 *  overides the previously read.
 */

class RCParam
{
    protected:
	//! the map that contains the values according to varnames
	map <string,string> values;
    public:
	RCParam (void);

	//! reads a set of affectations from an rcfile
	bool readfile (const string & name);

	//! reads a set of affectations from an array of string, typically from the command-line
	void readparams (int nb, char ** cmde);


	//! returns the value matching a varname as an int (-1 if it was undecipherable)
	int getint (const string &ident);

	//! returns the value matching a varname as a boolean
	/*!
	 *  true is returned for anything such as "true" "on" "1" or any non-null integer value 
	 *  false is return for anything such as "false" "off" or anything that is not true (!)
	 */
	bool getbool (const string &ident);

	//! returns the value matching a varname as a string
	const string & getstring (const string &ident);

	//! returns wether a varname has been recorded for a value
	bool isknown (const string &ident);

	//! sets the value for some varname
	void setvalue (const string &ident, const string &value);

    friend ostream &  operator<< (ostream &cout, RCParam &f);
};

