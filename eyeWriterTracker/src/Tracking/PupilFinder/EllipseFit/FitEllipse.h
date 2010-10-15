#ifndef FIT_ELLIPSE_INC
#define FIT_ELLIPSE_INC

//#include "ConicPlotter.h"


class FitEllipse  {
	
	public:
		
		double pvec[7];
		int len;
		double xc, yc, xa, ya, la, lb;
		double xx[4], yy[4];
		double Ao, Ax, Ay, Axx, Ayy, Axy;
		int maxlen;

		int A;
		int B;
		int C;
		int D;
		int E;
		int F;
		void assignf(double SCALE, double A, double B, double C, double D, double E, double F);


	
	
	public:
		FitEllipse() {
				maxlen = 1000;
				len = 0;
		}

  void apply(float *x_in, float *y_in, int num_points);

  int generate();

  void getParams(double& xc, double& yc,
		 double& xa, double& ya,
		 double& la, double& lb) {
    xc = this->xc;
    yc = this->yc;
    xa = this->xa;
    ya = this->ya;
    la = this->la;
    lb = this->lb;
  }

};

#endif
