#include <list>
#include <bzouz/bzouz.h>
#include <grapefruit/grapefruit.h>

using namespace grapefruit;

class TDGal : public TDObj, public GLReInit
{
	list <Vector3> nuage;
	GLint dl_nuage;

    public:
	class ACDumpItAll : public Action
	{
		TDObj *ptd;
	    public:
		~ACDumpItAll (void) {}
		ACDumpItAll (TDObj & td)
		    {	ptd = &td;
		    }
		virtual void doit (void)
		    {	cerr << "dump of " << ptd->gettdname() << " : " << endl
			     << " coord = " << ptd->pos << endl
			     << " scale = " << ptd->scale << endl
			     << " rm =" << endl
			     << ptd->rm << endl
			     << " det(rm) = " << ptd->rm.det() << endl << endl ;
		    }
	} * pacdumpitall;

	Action * getACDumpItAll (void)
	    {	if (pacdumpitall == NULL)
		    pacdumpitall = new ACDumpItAll (*this);
		return pacdumpitall;
	    }
	
	double scalemult;
	
	TDGal (int nb=256);
	virtual void render (void);
	virtual const string &	gettdname (void);

	// JDJDJDJD those need to be defined !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	inline virtual void renderclickablezone (void) {}
	inline virtual void proj_size(Vector3&, Vector3&) {}
	inline virtual GLfloat diameter() { return 1.0; }
	virtual int perform_reinit (void);
};


