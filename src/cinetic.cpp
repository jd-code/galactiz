
#include "cinetic.h"

namespace grapefruit
{
    Mv_Spin::Mv_Spin (TDObj &td, Vector3 &axe, GLfloat speed, Uint32 duration)
    {	speed1k = (2.0 * M_PI * speed) / 1000.0;
	ptd = &td;
	Mv_Spin::axe = axe;
	t_end = curtime + duration;
    }

    int Mv_Spin::step (void)
    {
	if (t_end && (curtime > t_end))	// did we reach end of movement ?
	    return -1;
	
	Uint32 dt = curtime - lasttime;
	ptd->rotate (axe, speed1k * dt);
	return 0;
    }

}

