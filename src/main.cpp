/* 
 * $Id$
 * Galactiz Copyright (C) 2005 Cristelle Barillon & Jean-Daniel Pauget
 * displaying a galaxy
 *
 * galactiz@disjunkt.com  -  http://galactiz.disjunkt.com/
 *
 * This file is part of Galactiz.
 * 
 * Galactiz is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * Galactiz is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 * You can also try the web at http://www.gnu.org/
 *
 *
 * galactiz@disjunkt.com  -  http://galactiz.disjunkt.com/
 */

#include <math.h>
#include <sys/types.h>	// getpid
#include <unistd.h>	// getpid
#include <stdlib.h>	// setenv

#include <map>
#include <strstream>

#include "SDL.h"

#include "config.h"

#include <grapefruit/grapefruit.h>
#include <caroube/caroube.h>

#include <bzouz/bzouz.h>
#include "rcfile.h"
#include "morespin.h"
#include <matcalc/matcalc.h>
#include "tdgal.h"

using namespace std ;
using namespace grapefruit ;
using namespace caroube ;

// JDJDJD a remettre à sa place plus tard
//RGBAColor   axis_color (0x80, 0x00, 0xFF),
//	    spot_color (0x00, 0xff, 0xFF),
//	    black      (0x00, 0x00, 0xFF),
//	    white      (0xFF, 0xFF, 0xFF),
//	    yellow     (0xDD, 0xAA, 0x00),
//	    myblue     (0xAA, 0xAA, 0xFF),
//
//	    bgcol      (176, 216, 168),
//	    bgcoltrans (0, 0, 0, 128),
//	    bgcollight (186, 226, 178);
//RGBAColor lightblue (0xD2, 0xF4, 0xF3, 0xFF);



// ----- TD stuff ---------------------------------------------------------------------------------


// -------------------------------------------------------------------------------------------------

void maindraw (void)
{
////////    reloader ();
////////    windows_h = (GLfloat) screen->h / (GLfloat) screen->w;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//------------- use to be in the window's reshape JDJDJDJD ----------------------
////////    glMatrixMode(GL_PROJECTION);
//    glPushMatrix();
////////    glLoadIdentity();
////////    glFrustum(-1.0, 1.0, -windows_h, windows_h, 5.0, 60.0);
////////    glMatrixMode(GL_MODELVIEW);
////////    glLoadIdentity();
////////    glTranslatef(0.0, 0.0, -40.0);

////////    redrawer (screen->w, screen->h);
    glEnable(GL_BLEND);
    glShadeModel(GL_SMOOTH);
    TDrender ();
    glDisable(GL_BLEND);
    
    glFinish();
    SDL_GL_SwapBuffers();
}
 
class MainGLinit : public GLReInit
{   private:
	bool enablezbuffer;
    public:
	MainGLinit (bool enablezbuffer)
	    {	MainGLinit::enablezbuffer = enablezbuffer;
	    }
	virtual int perform_reinit (void)
	    {	
		// JDJDJDJD ajout pour activer la gestion d'eclairage de GL
		glEnable(GL_LIGHTING);
		// JDJDJDJD ajout pour activer la gestion du zbuffer ??
		if (enablezbuffer)
		    glEnable(GL_DEPTH_TEST);
		else
		    glDisable(GL_DEPTH_TEST);

		// JDJDJDJD a better object should wrap this one for once (later on..)
		init_circle ();

		loadglvfont ();
		
		return 0;
	    }

};

int initvideo (int scr_width, int scr_height, bool enablezbuffer)
{
    screen = SDL_SetVideoMode(scr_width, scr_height, 16, SDL_OPENGL|SDL_SWSURFACE|SDL_RESIZABLE);
    if ( screen == NULL ) {
        cerr << "Couldn't set " << scr_width << "x" << scr_height<< " video mode: "
	     << SDL_GetError() << endl ;
        return -1;
    }


//    // JDJDJDJD ajout pour activer la gestion d'eclairage de GL
//    glEnable(GL_LIGHTING);
//    // JDJDJDJD ajout pour activer la gestion du zbuffer ??
//    if (enablezbuffer)
//	glEnable(GL_DEPTH_TEST);
//    else
//	glDisable(GL_DEPTH_TEST);
    GLReInit* mainglinit = new MainGLinit(enablezbuffer);   // JDJDJDJD this one is lacking destruction
    if (mainglinit == NULL)
	bzouzerr << "could not create mainglinit, let's try without it for the fun - we should not go far though" << endl ;
    else {
	mainglinit->subscribe();
    }
    glreinit ();
    
    SDL_WM_SetCaption("our beloved galactiz", "our beloved galactiz");

    // JDJDJDJD maybe this would fit better in an initkeyboard function ?
    SDL_EnableUNICODE(1);

    maindraw ();
    
    return 0;
}


int our_poll (Uint32 delay)
{
    SDL_Event event;
    Uint32 t_begin = SDL_GetTicks();
    Uint32 t_past  = SDL_GetTicks() - t_begin;
    
    while (t_past <= delay) {
	while (SDL_PollEvent (&event)) {
	    if (GrapeRouteEvent (event) != 0) {

	    } else {	    // we have an event that was not handled by galactiz yet
		switch (event.type) {
		    case SDL_VIDEORESIZE:
			screen = SDL_SetVideoMode(event.resize.w, event.resize.h, 16, SDL_OPENGL|SDL_SWSURFACE|SDL_RESIZABLE);
			// screen = SDL_SetVideoMode(event.resize.w, event.resize.h, 16, SDL_OPENGL|SDL_SWSURFACE|SDL_RESIZABLE|SDL_FULLSCREEN);
			glViewport(0, 0, (GLint) event.resize.w, (GLint) event.resize.h);
#ifdef BUILD_FOR_WIN32
			glreinit ();
#endif
			break;
		    case SDL_QUIT:
			maindraw ();
			return 0;
		    default:
			break;
		}
	    }
	}
	t_past = SDL_GetTicks() - t_begin;
	//if (delay-t_past > 10)
	//    SDL_Delay (10);
	//else
	//    break;
    }
    maindraw ();
    return 1;
}




//
// --------- keyboard handling -----------------------------------------------------------
//
//

//! the global key mapping
GrapeKeyMapKey global_keymap_down, global_keymap_up;

bool keybdownroute (SDL_Event const & event)
{
    if (global_keymap_down.trigger_action (event))
	return true;
    else
	return false;
}

bool keybuproute (SDL_Event const & event)
{
    if (global_keymap_up.trigger_action (event))
	return true;
    else
	return false;
}



//
// --------- cycles into full-screen modes and back to window mode and so on -------------
//
//

class ACCycleFullScreen : public Action
{
	int fullscreenon;   // -1 means windowed, else it's some valid SDL fullscreen mode

	int ww, wh; // windowed sizes
	
    public:
	virtual ~ACCycleFullScreen (void) {}
	ACCycleFullScreen (void)
	    {	fullscreenon = -1;
	    }
	virtual const string & getacname (void)
	    {	static string name ("ACCycleFullScreen");
		return name;
	    }
	virtual void doit (void)
	    {	
		SDL_Rect **modes;
		modes = SDL_ListModes (NULL, SDL_OPENGL|SDL_SWSURFACE|SDL_FULLSCREEN);
		if (fullscreenon == -1) {
		    if ((modes == NULL) || (modes == (SDL_Rect **)-1)) {
			bzouzerr << "no suitable video mode returned by SDL_ListModes for fullscreen ?" << endl ;
			return;
		    } else {
			int nbmodes = 0;
			while (modes[nbmodes] != NULL) nbmodes++;
			if (nbmodes == 0) {
			    bzouzerr << "no suitable video mode returned by SDL_ListModes for fullscreen ?" << endl ;
			    return;
			}
			ww = screen->w, wh = screen->h;
			fullscreenon = nbmodes;
		    }
		}
		fullscreenon --;
		if (fullscreenon < 0) {	    // we're back to windowed mode
		    screen = SDL_SetVideoMode(ww, wh, 16, SDL_OPENGL|SDL_SWSURFACE|SDL_RESIZABLE);
		    glViewport(0, 0, (GLint) ww, (GLint) wh);
#ifdef BUILD_FOR_WIN32
		    glreinit ();
#endif
		    fullscreenon = -1;	    // just in case it was not minus one...
		} else {		    // we go to some full screen mode
		    screen = SDL_SetVideoMode(modes[fullscreenon]->w, modes[fullscreenon]->h, 16, SDL_OPENGL|SDL_SWSURFACE|SDL_FULLSCREEN);
		    glViewport(0, 0, (GLint) modes[fullscreenon]->w, (GLint) modes[fullscreenon]->h);
#ifdef BUILD_FOR_WIN32
		    glreinit ();
#endif
		}
	    }
} accyclefullscreen;



//
// --------- travail temporaire menu hideable --------------------------------------------
//
//

class MainMenu : public grapefruit::TDObj //  public TDObjHideable, public TDObjShowable
{
	ActionTest nullact; // JDJDJDJD definir ca ici est plutot tres mauvais...
	TDMenu menu;

    public:
	MainMenu (void);
	virtual void render (void);
	virtual void renderclickablezone (void);
	virtual void gotclicked (SDL_Event const &event);
	virtual void proj_size (Vector3 &offset, Vector3 &size) {}
	virtual GLfloat diameter (void) {return 0.0;}
	void change_staytype (TDMenu::MenuStayType newstaytype) 
	{
	    menu.change_staytype (newstaytype);
	}
	virtual const string & gettdname (void);
};

class ACQuit : public Action
{
	SDL_Event event_quit;

    public:
	virtual ~ACQuit (void) {}
	virtual void doit (void)
	    {
		event_quit.type = SDL_QUIT;
		SDL_PushEvent(&event_quit);
	    }
	virtual const string & getacname (void)
	    {	static string name ("ACQuit");
		return name;
	    }
} acquit;

MainMenu::MainMenu (void)
{
    menu.change_name ("MainMenu") ;
    menu.push_back ( * new TDMenuItemString (nullact, "open"));
    menu.push_back ( * new TDMenuItemString (nullact, ""));
    menu.push_back ( * new TDMenuItemString (nullact, "save"));
    menu.push_back ( * new TDMenuItemString (nullact, "save as"));
    menu.push_back ( * new TDMenuItemString (nullact, ""));
    menu.push_back ( * new TDMenuItemString (acquit, "quit"));

    menu.push_back ( * new TDMenuItemString (nullact, ""));

    //menu.push_back ( * new TDMenuItemString (*(TDObjHideable *)(&menu), "hide menu"));
    menu.push_back ( * new TDMenuItemString (*menu.getactdhide(), "hide menu"));

    
    ////menu.push_back ( * new TDMenuItemString (*(TDObjShowable *)(&menu), "show"));
}

void MainMenu::render (void)
{
glPushAttrib(   GL_LIGHTING_BIT |   // glMaterialfv, glShadeModel
	    GL_COLOR_BUFFER_BIT |   // glBlendFunc
		    GL_LINE_BIT |   // GL_LINE_SMOOTH glLineWidth
	    GL_DEPTH_BUFFER_BIT |   // glDepthMask

		 GL_POLYGON_BIT |   // GL_POLYGON_SMOOTH
		   GL_POINT_BIT     // GL_POINT_SMOOTH
	    );
    glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (GLfloat *) GLRGBA_BLACK );
    glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR,	     (GLfloat *) GLRGBA_TRANSPBLACK );
    glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION,	     (GLfloat *) GLRGBA_RED);
    glMateriali  (GL_FRONT_AND_BACK, GL_SHININESS,	     128); 

    glShadeModel (GL_SMOOTH);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable (GL_LINE_SMOOTH);
    glLineWidth (0.5);
    glDepthMask (GL_FALSE);

    glBegin (GL_LINE_LOOP);
    render_circle ();
glPopAttrib();
}

void MainMenu::renderclickablezone (void)
{   glBegin (GL_POLYGON);
    render_circle ();
}

void MainMenu::gotclicked (SDL_Event const &event)
{   menu.setxypos (pos);
    if (!menu.isshown ())
	menu.show ();
    if (!menu.isactivated ())
	menu.activate ();
}

const string & MainMenu::gettdname (void)
{   static string name ("mainmenu_redcircle");
    return name;
}

//---------- test de profondeur de champs ------------------------------------------------
//

class TDFieldDepth : public TDObj
{
	TDString * pnums;
    public:
	class ACmovetofront : public Action
	{
		TDObj *ptd;
	    public:
		ACmovetofront (TDObj & td)
		    {	ptd = &td;
		    }
		virtual void doit (void)
		    {	ptd->pos.z += 0.1;
		    }
	} * pacmovetofront;

	class ACmovetoback : public Action
	{
		TDObj *ptd;
	    public:
		ACmovetoback (TDObj & td)
		    {	ptd = &td;
		    }
		virtual void doit (void)
		    {	ptd->pos.z -= 0.1;
		    }
	} * pacmovetoback;

	TDFieldDepth (void)
	    {	pacmovetofront = NULL,
		pacmovetoback = NULL;
		pnums = new TDString ("", 0.3);
		pnums->setcolor (GLRGBA_GREEN);
	    }
	virtual ~TDFieldDepth (void)
	    {	if (pacmovetofront != NULL)
		    delete (pacmovetofront);
		if (pacmovetoback != NULL)
		    delete (pacmovetoback);
	    }
	
	void render (void)
	    {	
		glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (GLfloat *) GLRGBA_BLACK );
		glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR,            (GLfloat *) GLRGBA_TRANSPBLACK );
		glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION,            (GLfloat *) GLRGBA_GREEN);
		glMateriali  (GL_FRONT_AND_BACK, GL_SHININESS,           128);
		glShadeModel (GL_SMOOTH);

		glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (GLfloat *)Vector4 (0.0, 0.0, 0.0, 0.5) );
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION,		(GLfloat *)Vector4 (0.1, 0.1, 0.4, 0.0));
		glDisable (GL_LINE_SMOOTH);
		glDisable (GL_POLYGON_SMOOTH);
		glLineWidth (1.0);
		glPushMatrix();
glTranslatef (0.0, 0.0, -0.1);
		glBegin (GL_POLYGON);
		render_circle ();
		glPopMatrix();
		glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION,            (GLfloat *) GLRGBA_TRANSPBLACK );
		glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (GLfloat *) GLRGBA_BLACK );

		glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION,            (GLfloat *) GLRGBA_GREEN);
		glEnable (GL_LINE_SMOOTH);
		glLineWidth (3.0);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glBegin (GL_LINE_LOOP);
		render_circle ();

		strstream s;
		s << setprecision (4) << setw(10) << pos.z << ends ;
		pnums->update (s.str());
		pnums->render();
	    }
	Action * getACmovetofront (void)
	    {	if (pacmovetofront == NULL)
		    pacmovetofront = new ACmovetofront (*this);
		return pacmovetofront;
	    }
	Action * getACmovetoback (void)
	    {	if (pacmovetoback == NULL)
		    pacmovetoback = new ACmovetoback (*this);
		return pacmovetoback;
	    }

	virtual void renderclickablezone (void) {}
	virtual void proj_size (Vector3 &offset, Vector3 &size) {}
	virtual GLfloat diameter (void)
	    {	return scale;
	    }

} tdfielddepth;

//---------- fin test de profondeur de champs --------------------------------------------


class SDLRCParam : public RCParam
{
    public:
	bool putenv (const string &ident)
	    {
		if (isknown (ident)) {
		    string s (ident);
		    s += '=';
		    s += values[ident];
		    char * ps = (char *) malloc (s.size() + 1);	// this (wich is part of the environment) will be freed at program exit
		    if (ps == NULL) {
			bzouzerr << "could not allocate a string for " << s << endl;
			return false;
		    }
		    strcpy (ps, s.c_str());
		    return (::putenv (ps) == 0);
		} else
		    return false;
	    }

};

class ACStartZoomIn : public Action
{
	TDGal *ptd;
    public:
	virtual ~ACStartZoomIn (void) {}
	ACStartZoomIn (TDGal & td)
	    {   ptd = &td;
	    }
	virtual void doit (void)
	    {	ptd->scalemult = 1.05;
	    }
	virtual const string & getacname (void)
	    {	static string name ("ACStartZoomIn");
		return name;
	    }
};

class ACStartZoomOUT : public Action
{
	TDGal *ptd;
    public:
	virtual ~ACStartZoomOUT (void) {}
	ACStartZoomOUT (TDGal & td)
	    {   ptd = &td;
	    }
	virtual void doit (void)
	    {	ptd->scalemult = 1.0/1.05;
	    }
	virtual const string & getacname (void)
	    {	static string name ("ACStartZoomOUT");
		return name;
	    }
};

class ACStopZoomIn : public Action
{
	TDGal *ptd;
    public:
	virtual ~ACStopZoomIn (void) {}
	ACStopZoomIn (TDGal & td)
	    {   ptd = &td;
	    }
	virtual void doit (void)
	    {	ptd->scalemult = 1.0;
	    }
	virtual const string & getacname (void)
	    {	static string name ("ACStopZoomIn");
		return name;
	    }
};

class ACStopZoomOUT : public Action
{
	TDGal *ptd;
    public:
	virtual ~ACStopZoomOUT (void) {}
	ACStopZoomOUT (TDGal & td)
	    {   ptd = &td;
	    }
	virtual void doit (void)
	    {	ptd->scalemult = 1.0;
	    }
	virtual const string & getacname (void)
	    {	static string name ("ACStopZoomOUT");
		return name;
	    }
};

//
// --------- main ------------------------------------------------------------------------
//
//

int main (int nb, char ** cmde)
{
    int poll_delay = 1;

    {
#ifdef BZOUTOTALOCAL
	cerr	 << "     bzouz = " << get_bzouz_version () << endl
		 << "   matcalc = " << get_matcalc_version () << endl
		 << "   caroube = " << caroube::get_caroube_version () << endl
		 << "grapefruit = " << grapefruit::get_grapefruit_version () << endl
		 << "  galactiz = " << "Total-Local® " PACKAGE "-" VERSION " $Id$" << endl ;
#endif
	SDLRCParam rcparam; //!< the map of the Params that can be set in rcfile or command-line

	// --- here in this part, we first set the default Params that can be overriden later on ---------

#ifdef BUILD_FOR_WIN32
	// on win32 we go against the SDL default
	rcparam.setvalue ("SDL_AUDIODRIVER", "dsound");	// directX sound
	// rcparam.setvalue ("SDL_AUDIODRIVER", "waveout");	// win32 regular sound
#endif

	rcparam.setvalue ("scr_width", "800");		// initial window size
	rcparam.setvalue ("scr_height", "600");
	rcparam.setvalue ("soundbuffer_size", "2048");	// in bytes
	rcparam.setvalue ("refresh_rate", "25");	// in milliseconds
	rcparam.setvalue ("zbuffer", "yes");		// enable/disable zbuffer

	// --- the local rcfile is read, overriding default settings, if any -----------------------------
	{   string rcfilename;
#ifdef BUILD_FOR_WIN32
	    char * home = getenv ("USERPROFILE");
	    if (home != NULL) {
		rcfilename += home;
		rcfilename += "/Application Data/Galactiz/galactiz.rc";	// Win32 do not allow filenames beginning with '.'
	    }
#else
	    char * home = getenv ("HOME");
	    if (home != NULL) {
		rcfilename += home;
		rcfilename += "/.galactiz.rc";
	    }
#endif
	    rcparam.readfile (rcfilename);
	}

	// --- the command-line is parsed for changes in Params, overriding anything previously set ------
	rcparam.readparams (nb, cmde);


	// --- we now take care of applying consequences of each possible Params -------------------------

	rcparam.putenv ("SDL_AUDIODRIVER");	// this environment-var is used by SDL in initaudio
	rcparam.putenv ("DISPLAY");		// this environment-var is used by SDL with X11

	if (rcparam.getbool("debugenv"))	// do we want a dump of the environment before any init call ?
	{
#ifdef BUILD_FOR_WIN32
	    bzouzerr << "no way to browse blindly through the whole (fake) environment provided by lib_SDL on win32" << endl ;
#else
	    extern char **environ;
	    int i=0;
	    while (environ[i] != NULL) {
		bzouzerr << i << "   - " << environ[i] << endl ;
		i++;
	    }
#endif
	}
	
	// ----------- video initialization --------------------------------------

	if (SDL_Init ( SDL_INIT_VIDEO) < 0) {
	    cerr << "SDL_Init (SDL_INIT_VIDEO) failed : " << SDL_GetError() << endl ;
	    return -1;
	}

	if ( ! initvideo (rcparam.getint("scr_width"), rcparam.getint("scr_height"), rcparam.getbool("zbuffer")) )
	    cerr << "... video initialization done" << endl;
	else
	    cerr << "... video initialization failed" << endl;


	// ---------- dumps OpenGL implementation details ------------------------
	if (rcparam.getbool("glinfo")) {
	    cerr << "-------------------------------------------------------------" << endl
		 << "GL_VERSION = " << ((const char *)glGetString(GL_VERSION)) << endl
		 << "GL_VENDOR = " << ((const char *)glGetString(GL_VENDOR)) << endl
		 << "GL_RENDERER = " << ((const char *)glGetString(GL_RENDERER)) << endl
		 << "GL_EXTENSIONS = " << ((const char *)glGetString(GL_EXTENSIONS)) << endl
		 << "-------------------------------------------------------------" << endl ;
	}
	

	// ----------- we initialize the sound -----------------------------------
	if (rcparam.getbool ("enablesound")) {    

	    cerr << "calling initaudio..." << flush;
	    if (initaudio(rcparam.getint("soundbuffer_size"))) cerr << "... audio initialization succeeded" << endl;
	    else {
		cerr << "...audio initialization failed" << endl;
		SDL_Quit ();
		return -1;
	    }

	    // ------- we start the sound - this also starts the seq -------------

	    SDL_PauseAudio(0);
	}

	// ----------- galactiz keyboard initialisation ------------------------
	advertise_unmapped_keys = rcparam.getbool ("warn_unmapped_keys");
	global_keydownhandler = keybdownroute;
	global_keyuphandler = keybuproute;

	// Vector4 coul1 (0.7, 0.9, 1.0, 1.0);	// bleu sarrazin ???

	// ---------- affichage de la gpl ----------------------------------------
	double r = 0.05;
	string test (   "Galactiz Copyright ©2002,©2005 Cristelle Barillon & Jean-Daniel Pauget\n"
			"displaying a galaxy\n"
			"\n"
			"galactiz@disjunkt.com  -  http://galactiz.disjunkt.com/\n"
			"\n"
			"Galactiz is free software; you can redistribute it and/or\n"
			"modify it under the terms of the GNU General Public License\n"
			"as published by the Free Software Foundation; either version 2\n"
			"of the License, or (at your option) any later version.\n"
			"\n"
			"Galactiz is distributed in the hope that it will be useful,\n"
			"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
			"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
			"GNU General Public License for more details.\n"
			"\n"
			"You should have received a copy of the GNU General Public License\n"
			"along with this program; if not, write to the Free Software\n"
			"Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.\n"
			"You can also try the web at http://www.gnu.org/"
			"\n"
			"galactiz@disjunkt.com  -  http://galactiz.disjunkt.com/\n"
		    );
		 
static	TDString tds (test, r);
	
	if (rcparam.getbool("gplshow")) {
	    tds.setxypos (0.0, 0.0);
	    tds.show ();
	    tds.activate ();
	}


	// ---------- affichage-test de notre fonte ------------------------------

	string s2;
	{	int i;
	    for (i=0 ; i<256 ; i++) {
		if ((i==10) || (i==13))
		    i = 32;
		s2 += (char) i;
		if ((i%32) == 31)
		    s2 += '\n';
	    }
	}
static	TDString tds2 (s2, r);
	if (rcparam.getbool("showfonttest")) {
	    tds2.setxypos (0.0, 0.5);
	    tds2.show ();
	    tds2.activate ();
	}

	// ---------- instanciation du menu principal (JDJDJDJD pas tres propre) -

static	MainMenu mainmenu;
	mainmenu.change_staytype (TDMenu::ONECLICK);
	mainmenu.scale =0.1;
	mainmenu.setxypos (-0.70, -0.7);
	mainmenu.show();
	mainmenu.activate();

	// ---------- instanciations de differentes actions et affectations a des touches de raccourci
	
	global_keymap_down.map_sdlkey_action (SDLK_ESCAPE, acquit);
	global_keymap_down.map_sdlkey_action (SDLK_F2, accyclefullscreen);

	// dump de td_displayed pour debuggage
static	ACDump_td_displayed acdump_td_displayed;
	global_keymap_down.map_sdlkey_action (SDLK_F1, KMOD_NONE, acdump_td_displayed);

	// shuffle de la liste des TD affichés, pour debuggage
static	ACScramble_td_displayed acscramble_td_displayed;
	global_keymap_down.map_sdlkey_action (SDLK_F1, KMOD_LSHIFT, acscramble_td_displayed);
	global_keymap_down.map_sdlkey_action (SDLK_F1, KMOD_RSHIFT, acscramble_td_displayed);
	global_keymap_down.map_sdlkey_action (SDLK_HOME, acscramble_td_displayed);
	global_keymap_down.map_sdlkey_action (SDLK_PAGEUP, acscramble_td_displayed);

	// instanciations and mappings for tdfielddepth field-depth testings
	if (rcparam.getbool("showtestfield")) {
	    tdfielddepth.show ();
	    global_keymap_down.map_sdlkey_action (SDLK_F5, *tdfielddepth.getACmovetofront());
	    global_keymap_down.map_sdlkey_action (SDLK_F6, *tdfielddepth.getACmovetoback());
	}
	poll_delay = rcparam.getint("refresh_rate");
    }

    TDGal unegalaxy (1000);
    // TDGal unegalaxy (175000);
    unegalaxy.show ();

    Vector3 axe (0.6, 0.7, 0.1);
    axe/=axe.norm();
    Mv_Spin unegalaxy_spin (unegalaxy, axe, 0.3333333333333, 10000);
    SpinForEver sfe (unegalaxy, 1000);
    unegalaxy_spin.pa_finish += sfe;
    unegalaxy_spin.start ();

    ACStartZoomIn  acstartzoomin (unegalaxy);
    ACStartZoomOUT acstartzoomout (unegalaxy);
    ACStopZoomIn   acstopzoomin (unegalaxy);
    ACStopZoomOUT  acstopzoomout (unegalaxy);
    global_keymap_down.map_unicode_action (SDLK_PLUS, acstartzoomin);
    global_keymap_up.map_unicode_action (SDLK_PLUS, acstopzoomin);
    global_keymap_down.map_unicode_action (SDLK_MINUS, acstartzoomout);
    global_keymap_up.map_unicode_action (SDLK_MINUS, acstopzoomout);

    TDfps tdfps (100);
    global_keymap_down.map_sdlkey_action (SDLK_f, KMOD_LCTRL, *tdfps.getactdtoggle());

    while (our_poll (poll_delay)) {
	// JDJDJDJD on ne fait rien ?
	//	int n = randint(5);
	//	testmenu.change_viewtype (TDMenu::INARRAY,n);
    }

    SDL_Quit ();

    return 0;
}

