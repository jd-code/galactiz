
#ifndef INC_SXDREAD_H
#define INC_SXDREAD_H

#include "fakexmlread.h"


//
// ---------------------------------------------- Read_SXD -------------------------------------
//

namespace grapefruit {

class Read_SXD : public Read_XML
{
	typedef void (Read_SXD::*PBaliseFunc) (const string & name, string & content);
	map <string,PBaliseFunc> mbal;

	void p_draw_page (const string & name, string & content);

    public:
	Read_SXD (istream &cin, string name);
	virtual void parse_balise (const string & name, string & content);
};

}

#endif // INC_SXDREAD_H
