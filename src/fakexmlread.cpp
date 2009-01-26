
#include "fakexmlread.h"
//#include <bzouz/bzouz.h>

namespace grapefruit {

//
// ---------------------------------------------- parse_var2map --------------------------------
//

void parse_var2map (const string & errpref, string & s, map<string,string> &m)
{
    size_t i = 0, size = s.size();

    while (i<size) {
	string name, content;
	while ((i<size) && isspace (s[i])) i++;
	while ((i<size) && (s[i] != '=')) {
	    name += s[i];
	    i++;
	}
	i++;
	if ((i<size) && (s[i] != '"')) {
	    cerr << errpref << " a double quote(\") was expected." << endl;
	    return;	// JDJDJDJD c'est un poil abrupt
	}
	i++;
	while ((i<size) && (s[i] != '"')) {
	    content += s[i];
	    i++;
	}
	i++;
	m[name] = content;
    }
}

//
// ---------------------------------------------- Read_XML -------------------------------------
//

string Read_XML::reach_end_balise ()
{
    string buf;
    char c;

    while (cin.get(c) && (c!='>')) {
	if ((c==10) || (c==13)) line++;
	buf += c;
    }
    return buf;
}

string Read_XML::read_alnum ()
{
    string buf;
    char c;

    while (cin.get(c) && isalnum (c)) {
	buf += c;
    }
    if (cin)
	cin.unget();
    return buf;
}

string Read_XML::read_balisename ()
{
    string buf;
    char c;

    if (mergedotbalise)
	while (cin.get(c) && (isalnum (c) || (c=='/') || (c==':') || (c=='-')))    buf += c;
    else
	while (cin.get(c) && (isalnum (c) || (c=='/')                        ))    buf += c;
    if (cin)
	cin.unget();
    return buf;
}

void Read_XML::parse ()
{
    char c;

    while (cin.get(c)) {
	if (isspace (c)) {
	    if ((c==10) || (c==13)) {
		line ++;
	    }
	    continue;
	}
	if (c == '<') {
	    string balisename, content;
	    if (!cin.get(c)) break;
	    switch (c) {
		case '?':
		case '!':
		    reach_end_balise ();
		    break;
		default:
		    cin.unget();
		    balisename = read_balisename ();
		    if (debug_level >= DEBUG_ALL_BALISE) {
			if ((!balisename.empty()) && (balisename[0]=='/')) {
			    cout << "close: " << balisename << endl ;
			} else {
			    cout << endl
				 << "open:  " << balisename << endl ;
			}
		    }
		    content = reach_end_balise ();
		    parse_balise (balisename, content);
	    }

	} else {
	    cerr << streamname << ":" << line << ": unexpected char: " << c << endl ;
	}
    }
}

void Read_XML::dumpcontent (const string & name, string & content)
{
    string errpref(streamname);
    errpref += ":";
    errpref += line;
    errpref += ":";

    map<string,string> param;

    parse_var2map (errpref, content, param);

    cout << name << " : " << endl;

    map<string,string>::iterator mi;

    for (mi=param.begin() ; mi!=param.end() ; mi++) {
	cout << "    " << mi->first << " =" << endl
	     << "        " << mi->second << endl;
    }
}

}   // namespace std

