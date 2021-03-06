/* Sources.cpp */

#include "Sources.h"
/**************************************************************************************/
Charge::Charge()
{
	Charge::init(0, 0,0,0, 0,0,0);
}	// No initialization of rho and Un //

Charge::Charge(ResGrid dd, SizeGrid NN)
{Charge::reset(dd,NN);}

Charge::Charge(double QQ, double XXq, double YYq, double ZZq, double RRq1, double RRq2, double RRq3)
{
	Charge::init(QQ, XXq,YYq,ZZq, RRq1,RRq2,RRq3);
}	// No initialization of rho and Un //

bool Charge::init(char * filename, SizeGrid& NN)
{
	FILE * file = fopen (filename, "r");	
	char	tmp_c, tmp_cc;
	string	tmp_s;
	double	tmp_d;

	Type= "from file";
	Q	= 0;
	Xq	= 0;	Yq	= 0;	Zq	= 0;
	Rq1	= 0;	Rq2	= 0;	Rq3	= 0;
	Un.init(NN.x,NN.y,NN.z);
	rho.init(NN.x,NN.y,NN.z);
	
	NN.x = 0;	NN.y = 0;	NN.z = 0;
	while((tmp_c=fgetc(file)) != EOF)
	{
		if(tmp_c == ' ')
			NN.z++;
		if(tmp_c == '\n')
		{
			NN.y++;
			tmp_cc = fgetc(file);
			if(tmp_cc == '\n' && tmp_cc !=EOF)
				NN.x++;
		}
	}
	NN.x  = NN.y/NN.x;
	NN.y  = NN.z/NN.y;
	NN.z /= NN.x*NN.y;
	
	rewind(file);
	rho.init(NN.x,NN.y,NN.z);
	 Un.init(NN.x,NN.y,NN.z);
	int ii(0),jj(0),kk(0);
	
	while((tmp_c=fgetc(file)) != EOF)
	{
		tmp_s += tmp_c;
		if(tmp_c == ' ')
		{
			tmp_d = atof(tmp_s.c_str());
			rho[ii][jj][kk] = tmp_d*1e-9;
			jj++;
			tmp_s = "";
		}
		if(tmp_c == '\n')
		{
			ii++;
			jj=0;
			tmp_cc = fgetc(file);
			if(tmp_cc == '\n' && tmp_cc !=EOF)
			{
				ii=0;
				jj=0;
				kk++;
			}
		}
	}
	fclose(file);
	return true;
}

bool Charge::init(double QQ, double XXq, double YYq, double ZZq, double RRq1, double RRq2, double RRq3)
{
	Type= "undefined";
	Q	= QQ;
	Xq	= XXq;	Yq	= YYq;	Zq	= ZZq;
	Rq1	= RRq1;	Rq2	= RRq2;	Rq3	= RRq3;
	// No initialization of rho and Un //
	return true;
}

bool Charge::reset(ResGrid dd, SizeGrid NN)
{
	Charge::init(0, 0,0,0, 0,0,0);
	rho.init(NN.x,NN.y,NN.z);
	Un.init(NN.x,NN.y,NN.z);
	return true;
}
// Reset all charge attributes

CMatrix1D Charge::getParams()
{
	CMatrix1D PParams(7);
	PParams[0] = Q;
	PParams[1] = Xq;
	PParams[2] = Yq;
	PParams[3] = Zq;
	PParams[4] = Rq1;
	PParams[5] = Rq2;
	PParams[6] = Rq3;
	return PParams;
}

string	Charge::getType()
{return Type;}

bool	Charge::gaussian(double QQ, double XXq, double YYq, double ZZq, double aaq, double bbq, double ccq, ResGrid dd, SizeGrid NN)
{
	Charge::init(QQ, XXq,YYq,ZZq, aaq,bbq,ccq);
	Type = "gaussian";
	rho.init(NN.x,NN.y,NN.z);
	Un.init(NN.x,NN.y,NN.z);
	double rho0 = Q / (pow(M_PI, 1.5) * Rq1 * Rq2 * Rq2);
	for(int ii=0 ; ii<NN.x ; ii++) for(int jj=0 ; jj<NN.y ; jj++) for(int kk=0 ; kk<NN.z ; kk++)
		rho[ii][jj][kk] = rho0 * exp(-(pow((ii*dd.x-Xq)/Rq1,2) + pow((jj*dd.y-Yq)/Rq2,2) + pow((kk*dd.z-Zq)/Rq3,2)));
	return true;
}
// Gaussian charge with tilt along vector (a,b)
/* 
bool	Charge::gaussian(double QQ, double XXq, double YYq, double ZZq, double llambda,double mmu,double nnu, double aa,double bb, ResGrid dd, SizeGrid NN)
{
	Charge::init(QQ, XXq,YYq,ZZq, llambda,mmu,nnu);
	Type = "gaussian";
	rho.init(NN.x,NN.y,NN.z);
	Un.init(NN.x,NN.y,NN.z);
	double rho0 = Q*(pow(aa,2)+pow(bb,2)) / (pow(M_PI, 1.5) * Rq1 * Rq2 * Rq3);
	for(int ii=0 ; ii<NN.x ; ii++) for(int jj=0 ; jj<NN.y ; jj++) for(int kk=0 ; kk<NN.z ; kk++)
		rho[ii][jj][kk] = rho0 * exp(-(pow((ii*dd.x-Xq)/Rq1,2) + pow((aa*(jj*dd.y-Yq)+bb*(kk*dd.z-Zq))/Rq2,2) + pow((-bb*(jj*dd.y-Yq)+aa*(kk*dd.z-Zq))/Rq3,2)));
	return true;
}
*/
// Gaussian charge with tilt, angle phi,theta
/*
bool	Charge::gaussian(double QQ, double XXq, double YYq, double ZZq, double llambda,double mmu,double nnu, double pphi,double ttheta, ResGrid dd, SizeGrid NN)
{
	 double xp,yp,zp;
	 Charge::init(QQ, XXq,YYq,ZZq, llambda,mmu,nnu);
	 Type	 = "gaussian";
	 ttheta	*= M_PI/180;
	 pphi	*= M_PI/180;
	 rho.init(NN.x,NN.y,NN.z);
	 Un.init(NN.x,NN.y,NN.z);
	 double rho0 = Q / (pow(M_PI, 1.5) * Rq1 * Rq2 * Rq3);
	 for(int ii=0 ; ii<NN.x ; ii++) for(int jj=0 ; jj<NN.y ; jj++) for(int kk=0 ; kk<NN.z ; kk++)
	 {
		 xp =  cos(ttheta)*cos(pphi)*(ii*dd.x-XXq) + sin(pphi)*(jj*dd.y-YYq) - sin(ttheta)*cos(pphi)*(kk*dd.z-ZZq);
		 yp = -cos(ttheta)*sin(pphi)*(ii*dd.x-XXq) + cos(pphi)*(jj*dd.y-YYq) + sin(ttheta)*sin(pphi)*(kk*dd.z-ZZq);
		 zp =  sin(ttheta)          *(ii*dd.x-XXq)                           - cos(ttheta)          *(kk*dd.z-ZZq);
		 rho[ii][jj][kk] = rho0 * exp(-(pow(xp/Rq1,2) + pow(yp/Rq2,2) + pow(zp/Rq3,2)));
	 }
	 return true;
}
*/
bool	Charge::gaussian(double QQ, double XXq, double YYq, double ZZq, double llambda,double mmu,double nnu, double pphi,double ttheta, ResGrid dd, SizeGrid NN)
{
	Charge::gaussian(QQ, XXq,YYq,ZZq, llambda,mmu,nnu, dd,NN);
	Charge::rotate(XXq,YYq,ZZq,0,0,1,pphi,dd,NN);
	Charge::rotate(XXq,YYq,ZZq,-sin(pphi*M_PI/180),cos(pphi*M_PI/180),0,ttheta,dd,NN);
	return true;	
}

bool	Charge::disk(double QQ, double XXq,double YYq,double ZZq, double RR, double HH, ResGrid dd, SizeGrid NN)
{
	Charge::init(QQ, XXq,YYq,ZZq, RR,RR,HH);
	Type = "disk";
	Un.init(NN.x,NN.y,NN.z);
	rho.init(NN.x,NN.y,NN.z);
	
	double rho0 = Q / (M_PI * pow(Rq1,2) * Rq3);
	double	ccptPoints	= 0;

	/**********************************************************************************/
	/* This portion of code ensures that whatever the resolution, the charge carried  */
	/* is actually the one inserted. However, the volume might be modified to ensure  */
	/* that.																		  */
	/**********************************************************************************/
	for(int ii=0 ; ii<NN.x ; ii++) for(int jj=0 ; jj<NN.y ; jj++) for(int kk=0 ; kk<NN.z ; kk++)
		if(sqrt(pow(ii*dd.x-Xq,2)+pow(jj*dd.y-Yq,2))<=Rq1 && fabs(kk*dd.z-Zq)<=Rq3/2)
			ccptPoints++;
	rho0 = Q/(ccptPoints*dd.x*dd.y*dd.z);
	/**********************************************************************************/
	/* Endof specified section														  */
	/**********************************************************************************/
	
	for(int ii=0 ; ii<NN.x ; ii++) for(int jj=0 ; jj<NN.y ; jj++) for(int kk=0 ; kk<NN.z ; kk++)
		if(sqrt(pow(ii*dd.x-Xq,2)+pow(jj*dd.y-Yq,2))<=Rq1 && fabs(kk*dd.z-Zq)<=Rq3/2)
			rho[ii][jj][kk] = rho0;
	return true;	
}

bool	Charge::ellipse(double QQ, double XXq,double YYq,double ZZq, double aa, double bb, double hh, ResGrid dd, SizeGrid NN)
{
	Charge::init(QQ, XXq,YYq,ZZq, aa,bb,hh);
	Type = "ellipse";
	Un.init(NN.x,NN.y,NN.z);
	rho.init(NN.x,NN.y,NN.z);
	
	double rho0 = Q / (M_PI * Rq1*Rq2 * Rq3);
	double	ccptPoints	= 0;

	/**********************************************************************************/
	/* This portion of code ensures that whatever the resolution, the charge carried  */
	/* is actually the one inserted. However, the volume might be modified to ensure  */
	/* that.																		  */
	/**********************************************************************************/
	for(int ii=0 ; ii<NN.x ; ii++) for(int jj=0 ; jj<NN.y ; jj++) for(int kk=0 ; kk<NN.z ; kk++)
		if(pow( (ii*dd.x-Xq)/Rq1 ,2) + pow( (jj*dd.y-Yq)/Rq2 ,2) <= 1 && fabs(kk*dd.z-Zq)<=Rq3/2)
			ccptPoints++;	
	rho0 = Q/(ccptPoints*dd.x*dd.y*dd.z);
	/**********************************************************************************/
	/* Endof specified section														  */
	/**********************************************************************************/
	
	for(int ii=0 ; ii<NN.x ; ii++) for(int jj=0 ; jj<NN.y ; jj++) for(int kk=0 ; kk<NN.z ; kk++)
		if(pow( (ii*dd.x-Xq)/Rq1 ,2) + pow( (jj*dd.y-Yq)/Rq2 ,2) <= 1 && fabs(kk*dd.z-Zq)<=Rq3/2)
			rho[ii][jj][kk] = rho0;
	return true;
}

bool	Charge::ellipsoid(double QQ, double XXq,double YYq,double ZZq, double aa, double bb, double cc, ResGrid dd, SizeGrid NN)
{
	Charge::init(QQ, XXq,YYq,ZZq, aa,bb,cc);
	Type = "ellipsoid";
	Un.init(NN.x,NN.y,NN.z);
	rho.init(NN.x,NN.y,NN.z);
	
	double rho0 = Q / (4/3 * M_PI * Rq1*Rq2*Rq3);
	double	ccptPoints	= 0;
	/**********************************************************************************/
	/* This portion of code ensures that whatever the resolution, the charge carried  */
	/* is actually the one inserted. However, the volume might be modified to ensure  */
	/* that.																		  */
	/**********************************************************************************/
	for(int ii=0 ; ii<NN.x ; ii++) for(int jj=0 ; jj<NN.y ; jj++) for(int kk=0 ; kk<NN.z ; kk++)
		if(pow( (ii*dd.x-Xq)/Rq1 ,2) + pow( (jj*dd.y-Yq)/Rq2 ,2) + pow( (kk*dd.z-Zq)/Rq3 ,2) <= 1)
			ccptPoints++;	
	rho0 = Q/(ccptPoints*dd.x*dd.y*dd.z);
	/**********************************************************************************/
	/* Endof specified section														  */
	/**********************************************************************************/
	
	for(int ii=0 ; ii<NN.x ; ii++) for(int jj=0 ; jj<NN.y ; jj++) for(int kk=0 ; kk<NN.z ; kk++)
		if(pow( (ii*dd.x-Xq)/Rq1 ,2) + pow( (jj*dd.y-Yq)/Rq2 ,2) + pow( (kk*dd.z-Zq)/Rq3 ,2) <= 1)
			rho[ii][jj][kk] = rho0;
	return true;	
}

bool	Charge::ellipsoid(double QQ, double XXq,double YYq,double ZZq, double aa, double bb, double cc, double pphi,double ttheta, ResGrid dd, SizeGrid NN)
{
	Charge::ellipsoid(QQ, XXq,YYq,ZZq, aa,bb,cc, dd,NN);
	Charge::rotate(XXq,YYq,ZZq,0,0,1,pphi,dd,NN);
	Charge::rotate(XXq,YYq,ZZq,-sin(pphi*M_PI/180),cos(pphi*M_PI/180),0,ttheta,dd,NN);
	return true;	
}

bool	Charge::rectangle(double QQ, double XXq,double YYq,double ZZq, double llx,double lly,double llz, ResGrid dd, SizeGrid NN)
{
	Charge::init(QQ, XXq,YYq,ZZq, llx,lly,llz);
	Type = "rectangle";
	Un.init(NN.x,NN.y,NN.z);
	rho.init(NN.x,NN.y,NN.z);
	
	double rho0 = Q / (Rq1*Rq2*Rq3);
	double	ccptPoints	= 0;
	
	/**********************************************************************************/
	/* This portion of code ensures that whatever the resolution, the charge carried  */
	/* is actually the one inserted. However, the volume might be modified to ensure  */
	/* that.																		  */
	/**********************************************************************************/
	for(int ii=0 ; ii<NN.x ; ii++) for(int jj=0 ; jj<NN.y ; jj++) for(int kk=0 ; kk<NN.z ; kk++)
		if(fabs(ii*dd.x-Xq)<=Rq1/2 && fabs(jj*dd.y-Yq)<=Rq2/2 && fabs(kk*dd.z-Zq)<=Rq3/2)
			ccptPoints++;
	rho0 = Q/(ccptPoints*dd.x*dd.y*dd.z);
	/**********************************************************************************/
	/* Endof specified section														  */
	/**********************************************************************************/
	
	for(int ii=0 ; ii<NN.x ; ii++) for(int jj=0 ; jj<NN.y ; jj++) for(int kk=0 ; kk<NN.z ; kk++)
		if(fabs(ii*dd.x-Xq)<=Rq1/2 && fabs(jj*dd.y-Yq)<=Rq2/2 && fabs(kk*dd.z-Zq)<=Rq3/2)
			rho[ii][jj][kk] = rho0;
	return true;	
}

bool	Charge::sphere(double QQ, double XXq, double YYq, double ZZq, double RR, ResGrid dd, SizeGrid NN)
{
	Charge::init(QQ, XXq,YYq,ZZq, RR,RR,RR);
	Type = "sphere";
	Un.init(NN.x,NN.y,NN.z);
	rho.init(NN.x,NN.y,NN.z);
	
	double rho0 = Q / (4/3 * M_PI * pow(Rq1,3));
	double	ccptPoints	= 0;
	
	/**********************************************************************************/
	/* This portion of code ensures that whatever the resolution, the charge carried  */
	/* is actually the one inserted. However, the volume might be modified to ensure  */
	/* that.																		  */
	/**********************************************************************************/
	cout<<"!!! Modification of the volume to garantee precision on the implemented charge. !!!\n";
	ccptPoints = 0;
	for(int ii=0 ; ii<NN.x ; ii++) for(int jj=0 ; jj<NN.y ; jj++) for(int kk=0 ; kk<NN.z ; kk++)
		if(sqrt(pow(ii*dd.x-Xq,2)+pow(jj*dd.y-Yq,2)+pow(kk*dd.z-Zq,2))<=Rq1) 
			ccptPoints++;
	rho0 = Q/(ccptPoints*dd.x*dd.y*dd.z);
	cout<<"Error on the volume due to the correction  : "<<
		fabs(4/3 * M_PI * pow(Rq1,3) - ccptPoints*dd.x*dd.y*dd.z)
		/(4/3 * M_PI * pow(Rq1,3)) *100<<" %"<<endl;	
	/**********************************************************************************/
	/* Endof specified section														  */
	/**********************************************************************************/
	
	for(int ii=0 ; ii<NN.x ; ii++) for(int jj=0 ; jj<NN.y ; jj++) for(int kk=0 ; kk<NN.z ; kk++)
		if(sqrt(pow(ii*dd.x-Xq,2)+pow(jj*dd.y-Yq,2)+pow(kk*dd.z-Zq,2))<=Rq1)
			rho[ii][jj][kk] = rho0;
	return true;
}

CMatrix1D	Charge::MonopoleAnalyticalSolution(	ResGrid dd, SizeGrid NN)
{
	CMatrix1D pphiAn(NN.z);
	// int kkq = (int)round(Zq/dd.z);
	
	for(int kk=0 ; kk<NN.z ; kk++)
	{
		if(fabs(kk*dd.z-Zq)<=Rq1)
			pphiAn[kk] = -Q/(4*eps0*M_PI)*(pow(kk*dd.z-Zq,2)/(2*pow(Rq1,3)) -3/(2*Rq1));
		else
			pphiAn[kk] = Q/(4*eps0*M_PI*fabs(kk*dd.z-Zq));
	}
	return pphiAn;	
}// Analytical solution: Monopole case

CMatrix1D	Charge::DipoleAnalyticalSolution(	ResGrid dd, SizeGrid NN)
{
	CMatrix1D pphiAn(NN.z);
	//	int kkq = (int)round(Zq/dd.z);
	
	for(int kk=0 ; kk<NN.z ; kk++)
	{
		if(fabs(kk*dd.z-Zq)<=Rq1)
			pphiAn[kk] = -Q/(4*eps0*M_PI)*(pow(kk*dd.z-Zq,2)/(2*pow(Rq1,3)) -3/(2*Rq1)) - Q/(4*eps0*M_PI*(kk*dd.z+Zq));
		else
			pphiAn[kk] = Q/(4*eps0*M_PI*fabs(kk*dd.z-Zq))								  - Q/(4*eps0*M_PI*(kk*dd.z+Zq));
	}
	return pphiAn;	
}// Analytical solution: Dipole case

CMatrix1D	Charge::MultipoleAnalyticalSolution(ResGrid dd, SizeGrid NN)
{
	CMatrix1D pphiAn(NN.z);
	/******************************************************************************/
	/* We neglect the ambient Laplacian field on Earth (100 V/m = 1e-3kV/cm)	  */
	/* The potential due to the ambient Laplacian field VL = 0.					  */
	/* To take into account this potential, VL = 100 * z (in V)					  */
	/******************************************************************************/
	double	Eambient = 0;
	double	VL;
	
	/******************************************************************************/
	/* Define number of images and store their positions						  */
	/******************************************************************************/
	int		M(1000);						// Account for M ground images and M ionospheric images
	double	z_GndImg = 0;				// Altitude of ground images
	double	z_IonImg = 0;				// Altitude of iono/electrosphere images
	double	z_Ion    = (NN.z-1)*dd.z;	// Altitude coordinate of the iono/electrosphere
	
	/******************************************************************************/
	/* Derive potential on the central axis										  */
	/******************************************************************************/
	for(int kk=0 ; kk<NN.z ; kk++)
	{
		VL = Eambient * kk *dd.z;
		pphiAn[kk] = VL;		
	}
	for(int kk=0 ; kk<NN.z ; kk++)
	{
		if(fabs(kk*dd.z-Zq)<=Rq1)
			pphiAn[kk] += -Q/(4*eps0*M_PI)*(pow(kk*dd.z-Zq,2)/(2*pow(Rq1,3)) -3/(2*Rq1));
		else
			pphiAn[kk] += Q/(4*eps0*M_PI*fabs(kk*dd.z-Zq));
		
		z_GndImg = Zq; // Altitude of ground images			    //
		z_IonImg = Zq; // Altitude of iono/electrosphere images //
					   //		cout<<"m = "<<setw(3)<<0<<"; z_Ion = "<<setw(8)<<z_Ion<<"; z_GndImg = "<<setw(8)<<z_GndImg<<"; z_IonImg = "<<setw(8)<<z_IonImg<<endl;
		for(int mm=1; mm<=M; mm++)
		{
			z_GndImg	= z_GndImg - ( mm%2*2*Zq + (mm-1)%2*2*(z_Ion-Zq) );
			z_IonImg	= z_IonImg + ( (mm-1)%2*2*Zq + mm%2*2*(z_Ion-Zq) );
			//			cout<<"m = "<<setw(3)<<mm<<"; z_Ion = "<<setw(8)<<z_Ion<<"; z_GndImg = "<<setw(8)<<z_GndImg<<"; z_IonImg = "<<setw(8)<<z_IonImg<<endl;
			
			pphiAn[kk] += pow(-1.0,mm)*
				(Q/(4*eps0*M_PI*fabs(kk*dd.z-z_GndImg)) +	// Ground Images
				 Q/(4*eps0*M_PI*fabs(kk*dd.z-z_IonImg)) ); // Ionosphere Images
		};
	}
	return pphiAn;
}// Analytical solution: Multipole case

Charge Charge::operator+=(const Charge& CC) 
{
	Type = "undefined";
	Q	+= CC.Q;
	Xq	 = 0;
	Yq	 = 0;
	Zq	 = 0;
	rho += CC.rho;
	Un	+= CC.Un;
	return *this;
} // operator +=

Charge& Charge::operator=(const Charge& CC)
{
	Type= "undefined";	
	Q	= CC.Q;
	Xq	= 0;
	Yq	= 0;
	Zq	= 0;
	rho = CC.rho;
	Un	= CC.Un;
	return *this;
} // operator +=

Charge Charge::operator+(const Charge& CC) const
{
	Charge result(*this);
	result+=CC;
	return result;
} // operator =

void	Charge::fwrite(char * title)
{
	FILE * file = fopen (title, "a");	
	char * type = &Type[0];
	if(file)
	{
		fprintf(file,"Type: %s\n",type);
		fprintf(file," [Q]             = [%f]\n",Q);
		fprintf(file," [Xq,  Yq,  Zq ] = [%f %f %f]\n",Xq, Yq, Zq );		
		fprintf(file," [Rq1, Rq2, Rq3] = [%f %f %f]\n",Rq1,Rq2,Rq3);		
	}
	fclose(file);
} // fwrite

bool Charge::rotate(double a, double b, double c, double u, double v, double w, double theta, ResGrid dd, SizeGrid NN)
{
	double	xp, yp, zp;
	double	x(0),	y(0),	z(0);
	bool	flag = 0;
	int		ip_inf,	jp_inf,	kp_inf;
	int		ip_sup,	jp_sup,	kp_sup;
	CMatrix3D rhop(NN.x,NN.y,NN.z);
	
	theta *= M_PI/180;	
	for (int ii = 0; ii<NN.x ; ii++) for (int jj = 0 ; jj<NN.y ; jj++) for (int kk = 0 ; kk<NN.z ; kk++)
		if (rho(ii,jj,kk)!=0) //If all the rotated points are on the mesh
		{
			x = ii*dd.x;
			y = jj*dd.y;
			z = kk*dd.z;
		
			xp = ( a*(v*v + w*w) + u*(-b*v - c*w + u*x + v*y + w*z) + ( (x-a)*(v*v + w*w) + u*(b*v + c*w - v*y - w*z) )*cos(theta) + sqrt(u*u + v*v + w*w)*( b*w - c*v -w*y + v*z)*sin(theta) ) / (u*u + v*v + w*w);
			yp = ( b*(u*u + w*w) + v*(-a*u - c*w + u*x + v*y + w*z) + ( (y-b)*(u*u + w*w) + v*(a*u + c*w - u*x - w*z) )*cos(theta) + sqrt(u*u + v*v + w*w)*(-a*w + c*u +w*x - u*z)*sin(theta) ) / (u*u + v*v + w*w);
			zp = ( c*(u*u + v*v) + w*(-a*u - b*v + u*x + v*y + w*z) + ( (z-c)*(u*u + v*v) + w*(a*u + b*v - u*x - v*y) )*cos(theta) + sqrt(u*u + v*v + w*w)*( a*v - b*u -v*x + u*y)*sin(theta) ) / (u*u + v*v + w*w);
			
			ip_inf	= floor(xp/dd.x);	jp_inf	= floor(yp/dd.y);	kp_inf	= floor(zp/dd.z);
			ip_sup	=  ceil(xp/dd.x);	jp_sup	=  ceil(yp/dd.y);	kp_sup	=  ceil(zp/dd.z);
			
			if(ip_inf == ip_sup && jp_inf == jp_sup && kp_inf == kp_sup ) // case: rotated point is on the mesh
			{
				if (ip_inf >= 0 && ip_inf < NN.x && jp_inf >= 0 && jp_inf < NN.y && kp_inf >= 0 && kp_inf < NN.z) 
					rhop(ip_inf,jp_inf,kp_inf) += rho(ii,jj,kk);
			}
			else
			{
				flag = 1;
				break;
			}
		};
	if (flag == 1)
	{
		rhop.init(NN.x,NN.y,NN.z);
		for (int ii = 0; ii<NN.x ; ii++) for (int jj = 0 ; jj<NN.y ; jj++) for (int kk = 0 ; kk<NN.z ; kk++)
			if (rho(ii,jj,kk)!=0) //If at least one of the rotated points is NOT on the mesh
			{
				x = ii*dd.x;
				y = jj*dd.y;
				z = kk*dd.z;
				
				xp = ( a*(v*v + w*w) + u*(-b*v - c*w + u*x + v*y + w*z) + ( (x-a)*(v*v + w*w) + u*(b*v + c*w - v*y - w*z) )*cos(theta) + sqrt(u*u + v*v + w*w)*( b*w - c*v -w*y + v*z)*sin(theta) ) / (u*u + v*v + w*w);
				yp = ( b*(u*u + w*w) + v*(-a*u - c*w + u*x + v*y + w*z) + ( (y-b)*(u*u + w*w) + v*(a*u + c*w - u*x - w*z) )*cos(theta) + sqrt(u*u + v*v + w*w)*(-a*w + c*u +w*x - u*z)*sin(theta) ) / (u*u + v*v + w*w);
				zp = ( c*(u*u + v*v) + w*(-a*u - b*v + u*x + v*y + w*z) + ( (z-c)*(u*u + v*v) + w*(a*u + b*v - u*x - v*y) )*cos(theta) + sqrt(u*u + v*v + w*w)*( a*v - b*u -v*x + u*y)*sin(theta) ) / (u*u + v*v + w*w);
				
				ip_inf	= floor(xp/dd.x);	jp_inf	= floor(yp/dd.y);	kp_inf	= floor(zp/dd.z);
				ip_sup	=  ceil(xp/dd.x);	jp_sup	=  ceil(yp/dd.y);	kp_sup	=  ceil(zp/dd.z);
				if (ip_inf == ip_sup) ip_sup = ip_inf+1;
				if (jp_inf == jp_sup) jp_sup = jp_inf+1;
				if (kp_inf == kp_sup) kp_sup = kp_inf+1;
				
				if (ip_inf >= 0 && ip_inf < NN.x && jp_inf >= 0 && jp_inf < NN.y && kp_inf >= 0 && kp_inf < NN.z) 
				{	
					// Reset Charge Density if two points are moved to the same gridpoints //
					/*rhop(ip_inf,jp_inf,kp_inf)  = rho(ii,jj,kk)/8;
					rhop(ip_inf,jp_sup,kp_inf)  = rho(ii,jj,kk)/8;
					rhop(ip_inf,jp_inf,kp_sup)  = rho(ii,jj,kk)/8;
					rhop(ip_inf,jp_sup,kp_sup)  = rho(ii,jj,kk)/8;
					rhop(ip_sup,jp_inf,kp_inf)  = rho(ii,jj,kk)/8;
					rhop(ip_sup,jp_inf,kp_sup)  = rho(ii,jj,kk)/8;
					rhop(ip_sup,jp_sup,kp_inf)  = rho(ii,jj,kk)/8;
					rhop(ip_sup,jp_sup,kp_sup)  = rho(ii,jj,kk)/8;*/
					
					// Add Charge Density if two points are moved to the same gridpoints //
					rhop(ip_inf,jp_inf,kp_inf) += rho(ii,jj,kk)/8;
					rhop(ip_inf,jp_sup,kp_inf) += rho(ii,jj,kk)/8;
					rhop(ip_inf,jp_inf,kp_sup) += rho(ii,jj,kk)/8;
					rhop(ip_inf,jp_sup,kp_sup) += rho(ii,jj,kk)/8;
					rhop(ip_sup,jp_inf,kp_inf) += rho(ii,jj,kk)/8;
					rhop(ip_sup,jp_inf,kp_sup) += rho(ii,jj,kk)/8;
					rhop(ip_sup,jp_sup,kp_inf) += rho(ii,jj,kk)/8;
					rhop(ip_sup,jp_sup,kp_sup) += rho(ii,jj,kk)/8;
				}			
			};
	}
	rho = rhop;
	return true;
}
// rotate

ostream & operator<< (ostream & os, const Charge & C)
{
	return os<<"Type: "<<C.Type<<"\n [Q]             = ["<<C.Q<<"]\n [Xq,  Yq,  Zq ] = ["<<C.Xq<<" "<<C.Yq<<" "<<C.Zq<<"]\n [Rq1, Rq2, Rq3] = ["<<C.Rq1<<" "<<C.Rq2<<" "<<C.Rq3<<"]\n"/*<<"rho: "<<C.rho*/;
}

Charge::~Charge(){}
/**************************************************************************************/

/**************************************************************************************/
Potential::Potential()
{
	EquiPotential	= true;
	Vo				= 0;
	Xc				= 0;
	Yc				= 0;
	Zc				= 0;
	L				= 0;
	W				= 0;
	H				= 0;
}

Potential::Potential(CMatrix3D& pphi, CMatrix3D& UUn)
{
	EquiPotential	= false;
	Vo				= 0;
	Xc				= 0;
	Yc				= 0;
	Zc				= 0;
	L				= 0;
	W				= 0;
	H				= 0;
	
	Un	= UUn;
	rho	= pphi; // potential distribution
}

bool Potential::init(CMatrix3D& pphi, CMatrix3D& UUn)
{
	EquiPotential	= false;
	Vo				= 0;
	Xc				= 0;
	Yc				= 0;
	Zc				= 0;
	L				= 0;
	W				= 0;
	H				= 0;
	
	Un	= UUn;
	rho	= pphi; // potential distribution
	return true;
}

Potential::Potential(double VVo, double XXc, double YYc, double ZZc, double LL, double WW, double HH, ResGrid dd, SizeGrid NN)
{
	EquiPotential	= true;
	Vo				= VVo;
	Xc				= XXc;
	Yc				= YYc;
	Zc				= ZZc;
	L				= LL;
	W				= WW;
	H				= HH;
	
	// Derivation of the potential distribution //
	//	int iic = (int)round(Xc/dd.x);
	//  int jjc = (int)round(Yc/dd.y);
	//  int kkc = (int)round(Zc/dd.z);
	
	rho.init(NN.x, NN.y, NN.z);
	Un.init(NN.x, NN.y, NN.z);
	for(int ii=0 ; ii<NN.x ; ii++) for(int jj=0 ; jj<NN.y ; jj++) for(int kk=0 ; kk<NN.z ; kk++)
		if(fabs(ii*dd.x-Xc)<=L/2 && fabs(jj*dd.y-Yc)<=W/2 && fabs(kk*dd.z-Zc)<=H/2)
		{
			rho[ii][jj][kk] = Vo;
			Un[ii][jj][kk]  = 1;
		};
}

Potential::Potential(double VVo, double XXc, double YYc, double ZZc, double RR, double HH, ResGrid dd, SizeGrid NN)
{
	EquiPotential	= true;
	Vo				= VVo;
	Xc				= XXc;
	Yc				= YYc;
	Zc				= ZZc;
	L				= 0;
	W				= RR;								// Radius of the cylinder
	H				= HH;								// Height of the cylinder
	
	// Derivation of the potential distribution //
	//	int iic = (int)round(Xc/dd.x);
	//  int jjc = (int)round(Yc/dd.y);
	//  int kkc = (int)round(Zc/dd.z);
	
	rho.init(NN.x, NN.y, NN.z);
	Un.init(NN.x, NN.y, NN.z);
	for(int ii=0 ; ii<NN.x ; ii++) for(int jj=0 ; jj<NN.y ; jj++) for(int kk=0 ; kk<NN.z ; kk++)
		if(sqrt(pow(ii*dd.x-Xc,2) + pow(jj*dd.y-Yc,2))<=RR && fabs(kk*dd.z-Zc)<=H/2)
		{
			rho[ii][jj][kk] = Vo;
			Un[ii][jj][kk]  = 1;
		};	
}

Potential::Potential(double VVo, double XXc, double YYc, double ZZc, double RR, ResGrid dd, SizeGrid NN)
{
	EquiPotential	= true;
	Vo				= VVo;
	Xc				= XXc;
	Yc				= YYc;
	Zc				= ZZc;
	L				= RR;								// Radius of the sphere
	W				= 0;
	H				= 0;
	
	// Derivation of the potential distribution //
	//	int iic = (int)round(Xc/dd.x);
	//  int jjc = (int)round(Yc/dd.y);
	//  int kkc = (int)round(Zc/dd.z);
	
	rho.init(NN.x, NN.y, NN.z);
	Un.init(NN.x, NN.y, NN.z);
	for(int ii=0 ; ii<NN.x ; ii++) for(int jj=0 ; jj<NN.y ; jj++) for(int kk=0 ; kk<NN.z ; kk++)
		if(sqrt(pow(ii*dd.x-Xc,2) + pow(jj*dd.y-Yc,2) + pow(kk*dd.z-Zc,2))<=RR)
		{
			rho[ii][jj][kk] = Vo;
			Un[ii][jj][kk]  = 1;
		};
}

ostream & operator<< (ostream & os, const Potential & P)
{
	return os<<" Vo = "<<P.Vo<<"\n [Xc, Yc, Zc] = ["<<P.Xc<<" "<<P.Yc<<" "<<P.Zc<<"]\n [L, W, H] = ["<<P.L<<" "<<P.W<<" "<<P.H<<"]\n"/*<<"rho: "<<C.rho*/;
}

void Potential::updateUn(const CMatrix3D& UUn)
{
	Un = UUn;
}

void Potential::fwrite(char * title)
{
	FILE * file = fopen (title, "a");	
	if(file)
	{
		fprintf(file,"Type: %s\n","Potential");
		fprintf(file," [Vo]            = [%f]\n",Vo);
		fprintf(file," [Xq,  Yq,  Zq ] = [%f %f %f]\n",Xc, Yc, Zc );		
		fprintf(file," [Rq1, Rq2, Rq3] = [%f %f %f]\n",L,H,W);		
	}
	fclose(file);
} // fwrite


Potential& Potential::operator=(const Potential& PP)
{
	Vo	= PP.Vo;
	Xc	= PP.Xc;
	Yc	= PP.Yc;
	Zc	= PP.Zc;
	L	= PP.L;
	W	= PP.W;
	H	= PP.H;
	rho	= PP.rho;
	Un	= PP.Un;
	return *this;
} // operator =

Potential::~Potential(){}
/**************************************************************************************/

void write(Charge& CC,	char * fname)
{CC.fwrite(fname);}

void write(Potential& PP, char * fname)
{PP.fwrite(fname);}