
//#ifndef   GRAPEFRUIT_H_INC
//#warning cinetic.h should not be included as is. grapefruit.h should be included instead
//#else  // GRAPEFRUIT_H_INC
//#ifdef	    GRAPEFRUIT_H_GLOBINST
//#define	    AGCINETIC_H_GLOBINST
//#endif //   GRAPEFRUIT_H_GLOBINST
//#endif // GRAPEFRUIT_H_INC

#ifndef MORESPIN_H_HEADER
#define MORESPIN_H_HEADER


#include <matcalc/matcalc.h>

#include <grapefruit/grapefruit.h>

namespace grapefruit
{
    class Mv_Spin2 : public Mvmt
    {
	    GLfloat speed1k;	//!< the spin-speed in radian per millisecond
	    Uint32 t_end;	//!< when the movement must end or 0 for non-ending
	    Vector3 axis;	//!< the axis of the spin movement

	public:
	    Mv_Spin2 (TDObj &td, Vector3 &axe, GLfloat speed, Uint32 duration);
	    virtual int step (void);
    };

    class SpinForEver : public Action
    {
	    TDObj * ptd;
	    Mv_Spin * pmvspin;
	    Uint32 duration;

	public:
	    SpinForEver (TDObj &td, Uint32 duration);
	    virtual void doit (void);

    };

}   // namespace grapefruit

#endif	// MORESPIN_H_HEADER
