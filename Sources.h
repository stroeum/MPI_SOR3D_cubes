/* Sources.h */
#ifndef SOURCES_H
#define SOURCES_H

#include <list>
#include <fstream>
#include "ResGrid.h"
#include "Matrix.h"
#include "Constants.h"
using namespace std;

/**************************************************************************************/
class Charge
{
protected:
	
public:
	/* Charge Type */
	string Type;
	/* Charge Content */
	double Q;
	/* Charge Position */
	double Xq,Yq,Zq;
	/* Charge Geometrical Parameters */
	double Rq1,Rq2,Rq3;
	CMatrix3D Un;
	CMatrix3D rho;
	
	Charge();											// Default constructor
	Charge(ResGrid dd, SizeGrid NN);					// Initialize Un and rho size
	Charge(double QQ, double XXq, double YYq, double ZZq, double RRq1, double RRq2, double RRq3);
														// Initialize geometrical parameters
	bool		init(char * filename, SizeGrid& N);		// Initiate charge density from a given file
	bool		init(double QQ, double XXq, double YYq, double ZZq, double RRq1, double RRq2, double RRq3);
														// Initiate geometrical parameters for geometrical charges
	bool		reset(ResGrid dd, SizeGrid NN);		// Reset geometrical parameters
	bool		gaussian(double QQ, double XXq, double YYq, double ZZq, double aaq, double bbq, double ccq, ResGrid dd, SizeGrid NN);
														// Distribute charge assuming Gaussian distribution
	bool		gaussian(double QQ, double XXq, double YYq, double ZZq, double llambda,double mmu,double nnu, double pphi,double ttheta, ResGrid dd, SizeGrid NN);
														// Distribute charge assuming Gaussian distribution rotated by pphi degrees in the horizontal plane and ttheta in the vertical plane 
	bool		disk(double QQ, double XXq,double YYq,double ZZq, double RR, double HH, ResGrid dd, SizeGrid NN);
														// Distribute charge assuming disk geometry
	bool		ellipse(double QQ, double XXq,double YYq,double ZZq, double aa, double bb, double hh, ResGrid dd, SizeGrid NN);
														// Distribute charge assuming elliptical geometry
	bool		ellipsoid(double QQ, double XXq,double YYq,double ZZq, double aa, double bb, double cc, ResGrid dd, SizeGrid NN);
														// Distribute charge assuming ellipsoidal geometry
	bool		ellipsoid(double QQ, double XXq,double YYq,double ZZq, double aa, double bb, double cc, double pphi,double ttheta, ResGrid dd, SizeGrid NN);
														// Distribute charge assuming ellipsoidal geometry  rotated by pphi degrees in the horizontal plane and ttheta in the vertical plane 
	bool		rectangle(double QQ, double XXq,double YYq,double ZZq, double llx,double lly,double llz, ResGrid dd, SizeGrid NN);
														// Distribute charge assuming parallelepipedic geometry
	bool		sphere(double QQ, double XXq, double YYq, double ZZq, double RR, ResGrid dd, SizeGrid NN);
														// Distribute charge assuming spherical geometry
	
	// Calculate analytical solution for a SPHERE of radius Rq1 and charge Q at Xq,Yq,Zq
	CMatrix1D	MonopoleAnalyticalSolution(	ResGrid dd, SizeGrid NN);
														// ... in free space
	CMatrix1D	DipoleAnalyticalSolution(	ResGrid dd, SizeGrid NN);
														// ... above a PEC ground plane
	CMatrix1D	MultipoleAnalyticalSolution(ResGrid dd, SizeGrid NN);
														// ... between two PEC ground planes
	CMatrix1D	getParams();							// Get Charge parameters Q, Xq,Yq,Zq, Rq1,Rq2,Rq3
	string		getType();								// Get Charge type
	void		fwrite(char * title);					// Write Charge information to a file
	bool		rotate(double a, double b, double c, double u, double v, double w, double theta, ResGrid dd, SizeGrid NN);
														// Rotate Charge distribution
		
	/*Note: All the surcharged operators below does not consider the position of the 
			centers of the charges, it is basically a shortcut to sum the total charge as
			well as the Status of the lattices. After the sum, the center is affected to
			be 0,0,0 but does not really matter since it won't be used. Our objective while
			defining those operator was to ease the use of several charges.*/
	Charge		operator+=(const Charge&);
	Charge&		operator=(const Charge&);
	Charge		operator+(const Charge&) const;
	friend ostream & operator<< (ostream &, const Charge &);
	~Charge();
};
/**************************************************************************************/

/**************************************************************************************/
class Potential
{
private:
public:
	bool	EquiPotential;							// Type of source
	double	Vo;										// Potential value
	double	Xc,Yc,Zc;								// Center Position
	double	L,W,H;									// Dimensions of the electrode,
	//  * L: Length/Radius
	//  * W: Width
	//  * H: Heigth
	CMatrix3D rho;									// Potential Distribution
	CMatrix3D Un;									// Status of the lattice
													// 0 is modifiable point, 1 if not.
													// Insofar as no point other than the
													// boundary points, Un is no required when
													// source is a charge distribution.

	Potential();									// Defaut constructor
	Potential(CMatrix3D& pphi, CMatrix3D& UUn);		// Constructor by definition of the potential 
													// distribution and Un
	bool init(CMatrix3D& pphi, CMatrix3D& UUn);		// update potential as if constructed in the 
													// previous way
	Potential(double,double,double,double,double,double,double,ResGrid,SizeGrid);
													// Constructor surcharge - Box
	Potential(double,double,double,double,double,double,ResGrid,SizeGrid);
													// Constructor surcharge - Vertical cylinder
	Potential(double,double,double,double,double,ResGrid,SizeGrid);
													// Constructor surcharge - Sphere
	double	getVo()	{return Vo;};					// Return the potential Vo
	bool	getEquiPotential() {return EquiPotential;};	
	// Return EquiPotential
	void	updateUn(const CMatrix3D&);				// Update Un when a new link is added.
	void	fwrite(char * title);					// Write Potential information to a file
	Potential& operator=(const Potential&);			// Equal 2 Potentials distributions.
	friend ostream & operator<< (ostream &, const Potential &);
	~Potential();
};
/**************************************************************************************/

/**************************************************************************************/
typedef list<Charge>   ListCharge;
void write(Charge&,	char *);
void write(Potential&,	char *);
/**************************************************************************************/
#endif SOURCES_H