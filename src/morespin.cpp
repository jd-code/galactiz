
#include <bzouz/bzouz.h>
#include <morespin.h>

namespace grapefruit
{
//    Mv_Spin2::Mv_Spin2 (TDObj &td, Vector3 &axis, GLfloat speed, Uint32 duration) : Mvmt (td)
//    {	speed1k = (2.0 * M_PI * speed) / 1000.0;
//	Mv_Spin2::axis = axis;
//	if (duration != 0)
//	    t_end = curtime + duration;
//	else
//	    t_end = 0;
//    }
//
//    int Mv_Spin2::step (void)
//    {
//	if (t_end && (curtime > t_end))	// did we reach end of movement ?
//	    return -1;
//	
//	Uint32 dt = curtime - lasttime;
//	ptd->rotate (axis, speed1k * dt);
//	lasttime = curtime;
//	return 0;
//    }


    SpinForEver::SpinForEver (TDObj &td, Uint32 duration)
    {	ptd = &td;
	SpinForEver::duration = duration;
	pmvspin = NULL;
    }

    void SpinForEver::doit (void)
    {	
	if (pmvspin != NULL)
	    delete (pmvspin);
	Vector3 axe (randint(11)-1, randint(11)-1, randint(11)-1);
	if (axe.norm() < 0.0001) {
	    //    bzouzerr << "norme trop petite " << endl
	    //	     << "||axe|| = " << axe.norm() << endl
	    //	     << "axe = " << axe << endl ;
	    axe = Vector3 (1,0,0);
	}
	axe /= axe.norm();
	pmvspin = new Mv_Spin (*ptd, axe, 0.25, duration);
	if (pmvspin != NULL) {
	    pmvspin->pa_finish += *this;
	    pmvspin->start();
	}
    }

    string SpinForEver::getacname (void)
    {	string name = ptd->gettdname() + "->SpinForEver";
	return name;
    }
}

