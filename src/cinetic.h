
namespace grapefruit
{

    class Mvmt
    {
     static Uint32 curtime;
	    Uint32 lasttime;

	public:
	    virtual int step (void) = 0;
    };

    class Mv_Spin : public Mvmt
    {
	    TDObj * ptd;	// the td we spin
	    GLfloat speed1k;	// the spin-speed in radian per millisecond
	    Uint32 t_end;	// when the movement must end or 0 for non-ending

	public:
	    Mv_Spin (TDObj &td, Vector3 &axe, GLfloat speed, Uint32 duration);
	    virtual int step (void);
    };
    
}   // namespace grapefruit

