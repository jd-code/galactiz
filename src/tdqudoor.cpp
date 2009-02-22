#include "tdqudoor.h"

TDQuDoor::TDQuDoor ()
{
    pacdumpitall = NULL;
    scalemult = 1.0;

    aperture= 1.0;

    update_nuage ();

    dl_wheel = -1;
    perform_reinit ();
    GLReInit::subscribe ();
}

const string & TDQuDoor::gettdname (void)
{
static string tdname ("tdqudoor");
    return tdname;
}

void TDQuDoor::update_nuage (void) {
    int nb = 17;

    int i;
    list <Vector3>::iterator li, lj, lk;

    nuage.erase (nuage.begin(), nuage.end());

    for (i=0 ; i<nb ; i++) {
	Vector3 v, w, z;
	v.x = cos ((2.0 * M_PI * i) / nb);
	v.y = sin ((2.0 * M_PI * i) / nb);
	v.z = 0;

	w.x = -sin ((2.0 * M_PI * i) / nb);
	w.y = cos ((2.0 * M_PI * i) / nb);
	w.z = 0;

	z.x = 0;
	z.y = 0;
	z.z = 1;

	GLfloat a = M_PI * 0.5 * aperture;

	nuage.push_back (0.9*v + 0.1*v                    + 0.1*w                 );
	nuage.push_back (0.9*v - 0.1*v * (2.0*cos(a)-1.0) + 0.1*w + 0.2*z * sin(a));
	nuage.push_back (0.9*v - 0.1*v * (2.0*cos(a)-1.0) - 0.1*w + 0.2*z * sin(a));
	nuage.push_back (0.9*v + 0.1*v                    - 0.1*w                 );
    }
}

int TDQuDoor::perform_reinit (void)
{   
    list <Vector3>::iterator li;
    if (dl_wheel != -1) {
	glDeleteLists (dl_wheel, 1);
	dl_wheel = -1;
    }
    dl_wheel = glGenLists(1);
    glNewList (dl_wheel, GL_COMPILE);
	glBegin (GL_QUADS);
	// glBegin (GL_POINTS);
	for (li=nuage.begin() ; li!=nuage.end() ; li++) {
	    glVertex3f (li->x, li->y, li->z); li++;
	    glVertex3f (li->x, li->y, li->z); li++;
	    glVertex3f (li->x, li->y, li->z); li++;
	    glVertex3f (li->x, li->y, li->z);
	}
	glEnd ();
    glEndList ();
    return 0;
}

void TDQuDoor::render (void)
{
    scale *= scalemult;

    glPushAttrib(   GL_LIGHTING_BIT |   // glMaterialfv, glShadeModel
		GL_COLOR_BUFFER_BIT |   // glBlendFunc
			GL_LINE_BIT |   // GL_LINE_SMOOTH glLineWidth
		GL_DEPTH_BUFFER_BIT |   // glDepthMask

		     GL_POLYGON_BIT |   // GL_POLYGON_SMOOTH
		       GL_POINT_BIT     // GL_POINT_SMOOTH
		);
    glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (GLfloat *) GLRGBA_BLACK );
    glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (GLfloat *) GLRGBA_BLACK );
    glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR,	     (GLfloat *) GLRGBA_TRANSPBLACK );
    // glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION,	     (GLfloat *) GLRGBA_WHITE);
    glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION,	     (GLfloat *) Vector4(0.8,0.0,0.0,0.0));
    glMateriali  (GL_FRONT_AND_BACK, GL_SHININESS,	     128); 

    glShadeModel (GL_SMOOTH);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable (GL_LINE_SMOOTH);
    glLineWidth (0.1);
    glDepthMask (GL_FALSE);

//    if (dl_nuage == -1) {
//	perform_reinit ();
//    }

    glCallList (dl_wheel);

//    glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION,         (GLfloat *) GLRGBA_WHITE);
//    glPointSize (2.0);
//    glBegin (GL_POINTS);
//    glCallList (dl_nuage);
    
    //	glBegin (GL_POINTS);
    //	list <Vector3>::iterator li;
    //	for (li=nuage.begin() ; li!=nuage.end() ; li++)
    //	    glVertex3f (li->x, li->y, li->z);
    //	glEnd ();

    glPopAttrib();
}

MvQuDoorSelfSpin::MvQuDoorSelfSpin (TDQuDoor &td, Uint32 duration) : Mvmt (td) {
    MvQuDoorSelfSpin::duration = duration;
    MvQuDoorSelfSpin::ptdqdoor = &td;
}

void MvQuDoorSelfSpin::firststep (void) {
    t_start = curtime;
    t_end = curtime + duration;
}

int MvQuDoorSelfSpin::step (void) {
    Uint32 dt = curtime - t_start;

    ptdqdoor->aperture = ((GLfloat)dt) / (t_end-t_start);
    ptdqdoor->update_nuage ();
    ptdqdoor->perform_reinit ();
    lasttime = curtime;

    if (t_end && (curtime > t_end))	// did we reach end of movement ?
	return -1;
    else
	return 0;
}
