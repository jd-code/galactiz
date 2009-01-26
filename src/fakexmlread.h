
#ifndef FAKEXMLREAD_H
#define FAKEXMLREAD_H

#include <iostream>
#include <string>
#include <map>

// #include <bzouz/bzouz.h>

namespace grapefruit {

    using namespace std;

//
// ---------------------------------------------- parse_var2map --------------------------------
//

void parse_var2map (const string & errpref, string & s, map<string,string> &m);

//
// ---------------------------------------------- Read_XML -------------------------------------
//

class Read_XML {

    protected:
	istream& cin;
	string streamname;
	size_t line;

	bool mergedotbalise;
	
	string reach_end_balise ();
	string read_balisename ();
	string read_alnum ();

	virtual void parse_balise (const string & name, string & content) = 0;

	void dumpcontent (const string & name, string & content);
	
    public:
	enum Debug_Level {	DEBUG_NONE,
				DEBUG_ALL_BALISE,
				DEBUG_ALL_BALISE_CONTENT
			 } debug_level;

	Read_XML (istream& is, string & name) :
	    cin (is),
	    streamname(name),
	    line(0),
	    mergedotbalise(false),
	    debug_level (DEBUG_NONE)
	    {}
	virtual ~Read_XML(void) {}

	void parse();
};

}   // namespace grapefruit

#endif // FAKEXMLREAD_H
