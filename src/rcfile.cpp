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

#include <ctype.h>

#include "bzouz/bzouz.h"    // bzouzerr
#include "rcfile.h"

inline bool iseol (char c)
{   return ((c==10) || (c==13));
}

inline bool isnoteol (char c)
{   return ((c!=10) && (c!=13));
}


// bare import from tokstream.cpp
string& maigre (string& s)
{
    if (s.empty()) return s;

    string::iterator i = s.begin ();
    while ( (i != s.end()) && (isspace (*i)) ) i++;
// before correction jd 2001-04-19
//	    if (i == s.end ())
//	    {	s.erase (s.begin());
//		return s;
//	    }
//	    s.erase (s.begin (), i);
    s.erase (s.begin (), i);
// end of correction...
    if (s.empty()) return s;

    i = s.end ();
    i--;
    while ( (i != s.begin ()) && (isspace (*i)) ) i--;
    i++;
    s.erase (i, s.end());
    return s;
}

//
// --------------------------------------------------------------------------------------
//
//

RCParam::RCParam (void)
{
}

void RCParam::readparams (int nb, char ** cmde)
{   int i;

    for (i=1 ; i<nb ; i++) {
	string s (cmde[i]);
	if (s.substr (0, 6) == "--set_") {
	    size_t p = s.find ('=', 6);
	    if (p != string::npos) {
		string ident = s.substr (6, p-6);
		string value = s.substr (p+1);
		if (ident.size() > 0) {
		    values [ident] = value;
		    //    bzouzerr << "    '" << ident << "' = '" << value << "'"
		    //	     << " bool=" << (getbool(ident)?"true":"false")
		    //	     << " int=" << getint(ident)
		    //	     << endl ;
		}
	    }
	}
    }
}

bool RCParam::readfile (const string & name)
{
    ifstream fin (name.c_str());

    if (!fin) {
	return false;
    }
    
    while (fin) {
	char c;
	// we're looking for the first non space char
	while (fin.get (c) && isspace (c) && isnoteol(c));
	switch (c) {
	    case '#':	// it's a comment, we skip to the end of the line
		while (fin.get (c) && isnoteol(c));
		break;
	    case 10:
	    case 13:	// we're at the end of line
		break;

	    default:
		if (isalpha (c)) {	// it's the begining of some identifier
		    string ident, value;
		    ident += c;
		    while (fin.get (c) && (isalnum (c) || (c=='_')))
			ident += c;
		    fin.unget();
		    
		    // we seek until we have the affectation symbol
		    while (fin.get (c) && isnoteol(c) && (c!='='));
		    if (c != '=')   // end of line/file without equal...
			break;	    // ...nothing performed

		    // we skip some more spaces
		    while (fin.get (c) && isspace (c) && isnoteol(c));
		    if (fin && isnoteol(c))
			fin.unget();
		    char quote = 0;
		    while (fin.get (c) && isnoteol(c)) {
			if (quote) {
			    if (c == quote)
				break;
			    else if (c == '\\') {
				if (fin.get (c) && isnoteol(c))
				    value += c;
				else
				    break;  // we go out of the while
			    } else
				value += c;
			} else {
			    switch (c) {
				case '"':
				case '\'':
				    quote = c;
				    break;

				case '\\':
				    if (fin.get (c) && isnoteol(c))
					value += c;
				    else
					fin.unget();
				    break;

				default:
				    value += c;
			    }
			}
		    }
			
		    if (ident.size() > 0) {
			values [ident] = value;
			//    bzouzerr << "    '" << ident << "' = '" << value << "'"
			//	     << " bool=" << (getbool(ident)?"true":"false")
			//	     << " int=" << getint(ident)
			//	     << endl ;
		    }

		} else 	// it's not a comment nor some identifier, we skip to the end of the line
		    while (fin.get(c) && isnoteol(c));
	}
    }
    return true;
}

int RCParam::getint (const string &ident)
{   map<string,string>::iterator mi = values.find (ident);
    if (mi != values.end())
	return atoi (mi->second.c_str());
    else
	return -1;
}

bool RCParam::getbool (const string &ident)
{   map<string,string>::iterator mi = values.find (ident);
    if (mi != values.end()) {
	string s = mi->second;
	maigre (s);
	int i = s.size();
	while (i>0) {
	    i--;
	    s[i] = tolower(s[i]);
	}
	if ((s == "true") || (s == "on") || (s == "yes") || (s == "1") || (atoi (s.c_str())!=0))
	    return true;
	else
	    return false;
    } else
	return false;
}

const string & RCParam::getstring (const string &ident)
{   
static string emptystring ("");
    map<string,string>::iterator mi = values.find (ident);
    if (mi != values.end())
	return mi->second;
    else
	return emptystring;
}

bool RCParam::isknown (const string &ident)
{
    return values.find (ident) != values.end() ;
}

void RCParam::setvalue (const string &ident, const string &value)
{
    if (ident.size() > 0) {
	values [ident] = value;
    }
}

ostream &  operator<< (ostream &cout, RCParam &rc)
{   map <string,string>::iterator mi;

    for (mi=rc.values.begin() ; mi!=rc.values.end() ; mi++)
	cout << "    " << mi->first << " = " << mi->second << endl ;

    return cout;
}

