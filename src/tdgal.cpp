#include "tdgal.h"

TDGal::TDGal (int nb)
{
    // JDJDJDJD cinematique temporaire
	vra = 2.0*M_PI / 1000.0;
	angrot = 0.0;
	vrb = 2.0*M_PI / 4142.0;
	bngrot = 0.0;
	scalemult = 1.0;

    int i;
    list <Vector3>::iterator li, lj, lk;

    for (i=0 ; i<nb ; i++) {
	Vector3 v;
	do {	v.x = (randint (200)-100)/100.0,
		v.y = (randint (200)-100)/100.0,
		v.z = (randint (200)-100)/100.0;
	} while (v.norm2() > 1.0);
	nuage.push_back (v);
    }


    for (li=nuage.begin() ; li!=nuage.end() ; li++) {
	for (i=0 ; i<2 ; i++) {
	    GLfloat r = li->norm();
	    (*li) *= r;
	    li->z *= r;
	}
    }

//    li=nuage.begin();
//    Vector3 v = *li;
//    li++;
//    while (li != nuage.end()) {
//	switch (randint(3))
//	{   case 1: li->x = v.x, li->y = v.y; break;
//	    case 2: li->y = v.y, li->z = v.z; break;
//	    case 3: li->z = v.z, li->x = v.x; break;
//	    default:
//		bzouzerr << "is there a bug in randint ?" << endl ;
//	}
//	v = *li;
//	li++;
//    }
    
    li = nuage.begin();
    Vector3 v = *li;
    li++;
    while (li != nuage.end()) {
	if (fabs(v.x) < fabs(li->x)) {
	    lj = nuage.insert (li, Vector3(v.x, li->y, li->z));
	} else {
	    nuage.insert (li, Vector3(li->x, v.y, v.z));
	    lj = li;
	}
	lk = lj;
	lk--;

	if (fabs(lk->y) < fabs(lj->y))
	    nuage.insert (lj, Vector3(lj->x, lk->y, lj->z));
	else
	    nuage.insert (lj, Vector3(lk->x, lj->y, lk->z));


	v = *li;
	li++;
    }
    Vector3 &f = *nuage.begin();
    nuage.push_back (Vector3 (v.x, v.y, f.z));
    nuage.push_back (Vector3 (v.x, f.y, f.z));
    
//    li = nuage.begin();
//    Vector3 v = *li;
//    li++;
//    while (li != nuage.end()) {
//	Vector3 a(  fabs (li->x - v.x),
//		    fabs (li->y - v.y),
//		    fabs (li->z - v.z)
//		 );
//	GLfloat min = (a.x < a.y) ? a.x : a.y;
//	min = (a.z < min) ? a.z : min;
//
//	if (min == a.z)
//	    li->x = v.x, li->y = v.y;
//	else if (min == a.y)
//	    li->z = v.z, li->x = v.x;
//	else
//	    li->y = v.y, li->z = v.z;
//
//	v = *li;
//	li++;
//    }
//    Vector3 &f = *nuage.begin();
//    nuage.push_back (Vector3 (v.x, v.y, f.z));
//    nuage.push_back (Vector3 (v.x, f.y, f.z));
    
//    {
//	double alpha = 2.0*M_PI * rand()/(RAND_MAX+1.0),
//	       beta = M_PI * rand()/(RAND_MAX+1.0) - M_PI/2.0,
//	       ray = rand()/(RAND_MAX+1.0);
//
//	nuage.push_back (Vector3 (cos(alpha)*ray*cos(beta),
//		                  sin(alpha)*ray*cos(beta),
//					     ray*sin(beta)
//				 )
//			);
//    }
	
//	nuage.push_back (Vector3 (  (randint (200)-100)/100.0,
//				    (randint (200)-100)/100.0,
//				    (randint (200)-100)/100.0
//				 )
//			);
    dl_nuage = -1;
    perform_reinit ();
    GLReInit::subscribe ();
}

int TDGal::perform_reinit (void)
{   
    list <Vector3>::iterator li;
    dl_nuage = glGenLists(1);
    glNewList (dl_nuage, GL_COMPILE);
	// glBegin (GL_LINE_LOOP);
	// glBegin (GL_POINTS);
	for (li=nuage.begin() ; li!=nuage.end() ; li++)
	    glVertex3f (li->x, li->y, li->z);
	glEnd ();
    glEndList ();
    return 0;
}

void TDGal::render (void)
{
    // JDJD bad cinematic code
    {
	// rotate (Vector3 (0.0, 1.0, 0.0), vra);
	//Vector3 v(0.1, 0.8, 0.2);
	Vector3 v(1.0, 0.0, 0.0);
	v /= v.norm();
	rotate (v, vra);
	angrot += vra;

	rotate (Vector3 (0.0, 0.0, 1.0), vrb);
	scale *= scalemult;
    }
    // JDJD end of bad cinematic code*

    glPushAttrib(   GL_LIGHTING_BIT |   // glMaterialfv, glShadeModel
		GL_COLOR_BUFFER_BIT |   // glBlendFunc
			GL_LINE_BIT |   // GL_LINE_SMOOTH glLineWidth
		GL_DEPTH_BUFFER_BIT |   // glDepthMask

		     GL_POLYGON_BIT |   // GL_POLYGON_SMOOTH
		       GL_POINT_BIT     // GL_POINT_SMOOTH
		);
    glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (GLfloat *) GLRGBA_BLACK );
    glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR,	     (GLfloat *) GLRGBA_TRANSPBLACK );
    // glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION,	     (GLfloat *) GLRGBA_WHITE);
    glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION,	     (GLfloat *) Vector4(0.2,0.2,0.2,0.0));
    glMateriali  (GL_FRONT_AND_BACK, GL_SHININESS,	     128); 

    glShadeModel (GL_SMOOTH);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable (GL_LINE_SMOOTH);
    glLineWidth (0.1);
    glDepthMask (GL_FALSE);

//    if (dl_nuage == -1) {
//	perform_reinit ();
//    }

    glBegin (GL_LINE_LOOP);
    glCallList (dl_nuage);

    glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION,         (GLfloat *) GLRGBA_WHITE);
    glPointSize (2.0);
    glBegin (GL_POINTS);
    glCallList (dl_nuage);
    
    //	glBegin (GL_POINTS);
    //	list <Vector3>::iterator li;
    //	for (li=nuage.begin() ; li!=nuage.end() ; li++)
    //	    glVertex3f (li->x, li->y, li->z);
    //	glEnd ();

    glPopAttrib();
}


