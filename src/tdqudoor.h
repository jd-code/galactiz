#include <list>
#include <bzouz/bzouz.h>
#include <grapefruit/grapefruit.h>

using namespace grapefruit;

class TDQuDoor : public TDObj, public GLReInit
{
	GLint dl_wheel;
	list <Vector3> nuage;

    public:
	GLfloat aperture;

	void update_nuage (void);

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
	
	TDQuDoor ();
	virtual void render (void);
	virtual const string &	gettdname (void);

	// JDJDJDJD those need to be defined !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	inline virtual void renderclickablezone (void) {}
	inline virtual void proj_size(Vector3&, Vector3&) {}
	inline virtual GLfloat diameter() { return 1.0; }
	virtual int perform_reinit (void);
};

class MvQuDoorSelfSpin : public Mvmt
{
	Uint32 duration;    //!< the movement duration if any or zero
	Uint32 t_end;	    //!< when the movement must end or 0 for non-ending
	Uint32 t_start;	    //!< when the movement starts
	TDQuDoor *ptdqdoor;
    public:
	MvQuDoorSelfSpin (TDQuDoor &td, Uint32 duration);
	virtual void firststep (void);
	virtual int step (void);
};
