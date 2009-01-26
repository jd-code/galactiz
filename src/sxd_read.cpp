
#include "sxd_read.h"

namespace grapefruit {

    using namespace std;

//
// ---------------------------------------------- Read_SXD -------------------------------------
//

Read_SXD::Read_SXD (istream &cin, string name) : Read_XML (cin, name)
{   mergedotbalise = true;
    mbal ["draw:page"] = &Read_SXD::p_draw_page;

    mbal ["/draw:page"] = &Read_SXD::dumpcontent;
    mbal ["draw:path"] = &Read_SXD::dumpcontent;
    mbal ["draw:polygon"] = &Read_SXD::dumpcontent;
    mbal ["draw:g"] = &Read_SXD::dumpcontent;
    mbal ["/draw:g"] = &Read_SXD::dumpcontent;
}

void Read_SXD::parse_balise (const string & name, string & content)
{
    map <string,PBaliseFunc>::iterator mi = mbal.find(name);
    if (mi != mbal.end()) {
	(this->*(mi->second))(name, content);	// Cf page 466 Stroustrup troisieme edition
    }
    else if (debug_level >= DEBUG_ALL_BALISE_CONTENT)
	dumpcontent (name, content);
}

void Read_SXD::p_draw_page (const string & name, string & content)
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

}
