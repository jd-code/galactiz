#include <list>
#include <bzouz/bzouz.h>
#include <grapefruit/grapefruit.h>

using namespace grapefruit;

class TDGal : public TDObj, public GLReInit
{
	list <Vector3> nuage;
	GLint dl_nuage;

	// JDJDJDJD fausse cinematique temporaire !!!!
	double vra;	// vitesse de rotation angulaire
	double angrot;	// angle de rotation
	double vrb;	// vitesse de rotation angulaire
	double bngrot;	// angle de rotation


    public:
	double scalemult;
	
	TDGal (int nb=256);
	virtual void render (void);

	// JDJDJDJD those need to be defined !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	inline virtual void renderclickablezone (void) {}
	inline virtual void proj_size(Vector3&, Vector3&) {}
	inline virtual GLfloat diameter() { return 1.0; }
	virtual int perform_reinit (void);
};


