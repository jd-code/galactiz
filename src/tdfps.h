
#include <grapefruit/grapefruit.h>

namespace grapefruit
{
    //! this TD displays the number of render call per seconds (fps) and an average for the last nbmoy values

    class TDfps : public TDString, public TDObjSHable
    {
	    Uint32 lasttime;	//!< the last time render was called

	    int nbmoy;		//!< the number of values used for average (moyenne) calculation
	    Uint32 *lt;		//!< the array for the last nbmoy time-intervals
	    int ilt;		//!< the ring-array index
	    int nbval;		//!< number of valid measure stored in the array
	    Uint32 sdt;		//!< the sum of the last nbmoy time intervals

	public:
	    virtual ~TDfps(void);
	    TDfps (int nbmoy=10);
	    int setnbmoy (int nbmoy);	//!< updates the number of nbmoy values used for average

	    virtual void render (void);
	    virtual const string & gettdname (void);
    };

}   // namespace grapefruit

