#include <math.h>
#include <assert.h>
#include <stdio.h>

#include "FitEllipse.h"

//using namespace yarp::sig;


typedef bool boolean;

#define invL nonspecial_invL

#define MAXP 200

#define A_TperB(_A,_B,_res,_righA,_colA,_righB,_colB) \
      if (1) for (int p=1;p<=_colA;p++) \
        for (int q=1;q<=_colB;q++)                        \
          { _res[p][q]=0.0;                            \
            for (int l=1;l<=_righA;l++)                    \
              _res[p][q]=_res[p][q]+_A[l][p]*_B[l][q];  \
          }

#define AperB(_A,_B,_res,_righA,_colA,_righB,_colB) \
      if (1) for (int p=1;p<=_righA;p++)                \
        for (int q=1;q<=_colB;q++)                       \
          { _res[p][q]=0.0;                           \
            for (int l=1;l<=_colA;l++)                     \
              _res[p][q]=_res[p][q]+_A[p][l]*_B[l][q];  \
          }                                            


#define AperB_T(_A,_B,_res,_righA,_colA,_righB,_colB) \
      if (1) for (int p=1;p<=_colA;p++)         \
        for (int q=1;q<=_colB;q++)               \
          { _res[p][q]=0.0;                       \
            for (int l=1;l<=_righA;l++)            \
              _res[p][q]=_res[p][q]+_A[p][l]*_B[q][l];  \
          }                                            


#define BOOKSTEIN 0
#define TAUBIN 1
#define FPF 2

class EllipseFitter
{
public:
  void ROTATE(double a[7][7], int i, int j, int k, int l, double tau, double s) 
    {
      double g,h;
      g=a[i][j];h=a[k][l];a[i][j]=g-s*(h+g*tau);
      a[k][l]=h+s*(g-h*tau);
    }
    
    void jacobi(double a[7][7], int n, double d[7] , 
		double v[7][7], int nrot)      
      {
        int j,iq,ip,i;
        static double tresh,theta,tau,t,sm,s,h,g,c;
	
	assert(n==6);

        static double b[MAXP+1];
        static double z[MAXP+1];
        
        for (ip=1;ip<=n;ip++) {
          for (iq=1;iq<=n;iq++) v[ip][iq]=0.0;
          v[ip][ip]=1.0;
        }
        for (ip=1;ip<=n;ip++) {
          b[ip]=d[ip]=a[ip][ip];
          z[ip]=0.0;
        }
        nrot=0;
        for (i=1;i<=50;i++) {
          sm=0.0;
          for (ip=1;ip<=n-1;ip++) {
            for (iq=ip+1;iq<=n;iq++)
              sm += fabs(a[ip][iq]);
          }
          if (sm == 0.0) {
            /*    free_vector(z,1,n);
                  free_vector(b,1,n);  */
            return;
          }
          if (i < 4)
            tresh=0.2*sm/(n*n);
          else
            tresh=0.0;
          for (ip=1;ip<=n-1;ip++) {
            for (iq=ip+1;iq<=n;iq++) {
              g=100.0*fabs(a[ip][iq]);
              if (i > 4 && fabs(d[ip])+g == fabs(d[ip])
                  && fabs(d[iq])+g == fabs(d[iq]))
                a[ip][iq]=0.0;
              else if (fabs(a[ip][iq]) > tresh) {
                h=d[iq]-d[ip];
                if (fabs(h)+g == fabs(h))
                  t=(a[ip][iq])/h;
                else {
                  theta=0.5*h/(a[ip][iq]);
                  t=1.0/(fabs(theta)+sqrt(1.0+theta*theta));
                  if (theta < 0.0) t = -t;
                }
                c=1.0/sqrt(1+t*t);
                s=t*c;
                tau=s/(1.0+c);
                h=t*a[ip][iq];
                z[ip] -= h;
                z[iq] += h;
                d[ip] -= h;
                d[iq] += h;
                a[ip][iq]=0.0;
                for (j=1;j<=ip-1;j++) {
                  ROTATE(a,j,ip,j,iq,tau,s);
                  }
                for (j=ip+1;j<=iq-1;j++) {
                  ROTATE(a,ip,j,j,iq,tau,s);
                  }
                for (j=iq+1;j<=n;j++) {
                  ROTATE(a,ip,j,iq,j,tau,s);
                  }
                for (j=1;j<=n;j++) {
                  ROTATE(v,j,ip,j,iq,tau,s);
                  }
                ++nrot;
              }
            }
          }
          for (ip=1;ip<=n;ip++) {
            b[ip] += z[ip];
            d[ip]=b[ip];
            z[ip]=0.0;
          }
        }
        //printf("Too many iterations in routine JACOBI");
      }
    

    //  Perform the Cholesky decomposition    
    // Return the lower triangular L  such that L*L'=A  
    void choldc(double a[7][7], int n, double l[7][7])
      {
        int i,j,k;
        double sum;
        static double p[MAXP+1];
        
	assert(n==6);

        for (i=1; i<=n; i++)  {
          for (j=i; j<=n; j++)  {
            for (sum=a[i][j],k=i-1;k>=1;k--) sum -= a[i][k]*a[j][k];
            if (i == j) {
              if (sum<=0.0)  
                // printf("\nA is not poitive definite!");
                {}
              else 
                p[i]=sqrt(sum); }
            else 
              {
                a[j][i]=sum/p[i];
              }
          }
        }       

        for (i=1; i<=n; i++)  
          for (j=i; j<=n; j++)  
            if (i==j)
              l[i][i] = p[i];
            else
              {
                l[j][i]=a[j][i];  
                l[i][j]=0.0;
              }
      }


    /********************************************************************/
    /**    Calcola la inversa della matrice  B mettendo il risultato   **/
    /**    in InvB . Il metodo usato per l'inversione e' quello di     **/
    /**    Gauss-Jordan.   N e' l'ordine della matrice .               **/
    /**    ritorna 0 se l'inversione  corretta altrimenti ritorna     **/
    /**    SINGULAR .                                                  **/
    /********************************************************************/
  static int inverse(double TB[3][3], double InvB[3][3], int N) {  
      int k,i,j,p,q;
      static double mult;
      static double D,temp;
      static double maxpivot;
      int npivot;
      static double B[MAXP+1][MAXP+2];
      static double A[MAXP+1][2*MAXP+2];
      static double C[MAXP+1][MAXP+1];
      static double eps = 10e-20;

      assert(N==2);
      
      for(k=1;k<=N;k++)
        for(j=1;j<=N;j++)
          B[k][j]=TB[k][j];
      
      for (k=1;k<=N;k++)
        {
          for (j=1;j<=N+1;j++)
            A[k][j]=B[k][j];
          for (j=N+2;j<=2*N+1;j++)
            A[k][j]=(float)0;
          A[k][k-1+N+2]=(float)1;
        }
      for (k=1;k<=N;k++)
        {
          maxpivot=fabs((double)A[k][k]);
          npivot=k;
          for (i=k;i<=N;i++)
            if (maxpivot<fabs((double)A[i][k]))
              {
                maxpivot=fabs((double)A[i][k]);
                npivot=i;
              }
          if (maxpivot>=eps)
            {      if (npivot!=k)
                     for (j=k;j<=2*N+1;j++)
                       {
                         temp=A[npivot][j];
                         A[npivot][j]=A[k][j];
                         A[k][j]=temp;
                       } ;
                   D=A[k][k];
                   for (j=2*N+1;j>=k;j--)
                     A[k][j]=A[k][j]/D;
                   for (i=1;i<=N;i++)
                     {
                       if (i!=k)
                         {
                           mult=A[i][k];
                           for (j=2*N+1;j>=k;j--)
                             A[i][j]=A[i][j]-mult*A[k][j] ;
                         }
                     }
                 }
          else
            {  // printf("\n The matrix may be singular !!") ;
               return(-1);
             };
        }
      /**   Copia il risultato nella matrice InvB  ***/
      for (k=1,p=1;k<=N;k++,p++)
        for (j=N+2,q=1;j<=2*N+1;j++,q++)
          InvB[p][q]=A[k][j];
      return(0);
    }            /*  End of INVERSE   */
    
  static int inverse7(double TB[7][7], double InvB[7][7], int N) {  
      int k,i,j,p,q;
      static double mult;
      static double D,temp;
      static double maxpivot;
      int npivot;
      static double B[MAXP+1][MAXP+2];
      static double A[MAXP+1][2*MAXP+2];
      static double C[MAXP+1][MAXP+1];
      double eps = 10e-20;

      assert(N==6);
      
      for(k=1;k<=N;k++)
        for(j=1;j<=N;j++)
          B[k][j]=TB[k][j];
      
      for (k=1;k<=N;k++)
        {
          for (j=1;j<=N+1;j++)
            A[k][j]=B[k][j];
          for (j=N+2;j<=2*N+1;j++)
            A[k][j]=(float)0;
          A[k][k-1+N+2]=(float)1;
        }
      for (k=1;k<=N;k++)
        {
          maxpivot=fabs((double)A[k][k]);
          npivot=k;
          for (i=k;i<=N;i++)
            if (maxpivot<fabs((double)A[i][k]))
              {
                maxpivot=fabs((double)A[i][k]);
                npivot=i;
              }
          if (maxpivot>=eps)
            {      if (npivot!=k)
                     for (j=k;j<=2*N+1;j++)
                       {
                         temp=A[npivot][j];
                         A[npivot][j]=A[k][j];
                         A[k][j]=temp;
                       } ;
                   D=A[k][k];
                   for (j=2*N+1;j>=k;j--)
                     A[k][j]=A[k][j]/D;
                   for (i=1;i<=N;i++)
                     {
                       if (i!=k)
                         {
                           mult=A[i][k];
                           for (j=2*N+1;j>=k;j--)
                             A[i][j]=A[i][j]-mult*A[k][j] ;
                         }
                     }
                 }
          else
            {  // printf("\n The matrix may be singular !!") ;
               return(-1);
             };
        }
      /**   Copia il risultato nella matrice InvB  ***/
      for (k=1,p=1;k<=N;k++,p++)
        for (j=N+2,q=1;j<=2*N+1;j++,q++)
          InvB[p][q]=A[k][j];
      return(0);
    }            /*  End of INVERSE   */
 
    

  static void get_param(double *pvec, 
		 int npts,
		 double xx[], double yy[]) {
      static double u[3][MAXP+1];
      static double Aiu[3][MAXP+1];
      static double L[3][MAXP+1];
      static double B[3][MAXP+1];
      static double Xpos[3][MAXP+1];
      static double Xneg[3][MAXP+1];
      static double ss1[3][MAXP+1];
      static double ss2[3][MAXP+1];
      static double lambda[MAXP+1];
      static double uAiu[3][MAXP+1];
      static double A[3][3];
      static double Ai[3][3];
      static double Aib[3][2];
      static double b[3][2];
      static double r1[2][2];
      static double Ao, Ax, Ay, Axx, Ayy, Axy;
             
      double pi = 3.14781;      
      double theta;
      int i;
      int j;
      double kk;
      
      Ao = pvec[6];
      Ax = pvec[4];
      Ay = pvec[5];
      Axx = pvec[1];
      Ayy = pvec[3];
      Axy = pvec[2];

      A[1][1] = Axx;    A[1][2] = Axy/2;
      A[2][1] = Axy/2;  A[2][2] = Ayy;
      b[1][1] = Ax; b[2][1] = Ay;  

      // Generate normals linspace
      for (i=0, theta=0.0; i<=npts; i++, theta=(i*2*M_PI/npts)) {
        u[1][i] = cos(theta);
        u[2][i] = sin(theta); 
      }
      
      inverse(A,Ai,2);
      
      AperB(Ai,b,Aib,2,2,2,1);
      A_TperB(b,Aib,r1,2,1,2,1);      
      r1[1][1] = r1[1][1] - 4*Ao;

      AperB(Ai,u,Aiu,2,2,2,npts);
      for (i=1; i<=2; i++)
        for (j=1; j<=npts; j++)
          uAiu[i][j] = u[i][j] * Aiu[i][j];

      for (j=1; j<=npts; j++) {
        if ( (kk=(r1[1][1] / (uAiu[1][j]+uAiu[2][j]))) >= 0.0)
          lambda[j] = sqrt(kk);
        else
          lambda[j] = -1.0; }

      // Builds up B and L
      for (j=1; j<=npts; j++)
        L[1][j] = L[2][j] = lambda[j];      
      for (j=1; j<=npts; j++) {
        B[1][j] = b[1][1];
        B[2][j] = b[2][1]; }
      
      for (j=1; j<=npts; j++) {
        ss1[1][j] = 0.5 * (  L[1][j] * u[1][j] - B[1][j]);
        ss1[2][j] = 0.5 * (  L[2][j] * u[2][j] - B[2][j]);
        ss2[1][j] = 0.5 * ( -L[1][j] * u[1][j] - B[1][j]);
        ss2[2][j] = 0.5 * ( -L[2][j] * u[2][j] - B[2][j]); }

      AperB(Ai,ss1,Xpos,2,2,2,npts);
      AperB(Ai,ss2,Xneg,2,2,2,npts);

      for (j=1; j<=npts; j++) {
        if (lambda[j]==-1.0) {
	  //          points[1][j] = -1.0;
	  //          points[2][j] = -1.0;
	  //          points[1][j+npts] = -1.0;
	  //          points[2][j+npts] = -1.0;
	  // printf("Hrmm\n");
        }
        else {
	  //printf("(%g %g)\n", Xpos[1][j], Xpos[2][j]);
	  //          points[1][j] = Xpos[1][j];
	  //          points[2][j] = Xpos[2][j];
	  //          points[1][j+npts] = Xneg[1][j];
	  //          points[2][j+npts] = Xneg[2][j];
          }                              
      }
      for (int i=0; i<npts; i++) {
	xx[i] = Xpos[1][i+1];
	yy[i] = Xpos[2][i+1];
	//printf(">>>> %d %g %g\n", i, xx[i], yy[i]);
      }
    }

  //  double points[MAXP][4];
  //  double x_in[MAXP];
  //  double y_in[MAXP];
  //  int num_points;

  //double pvec[7];
  
  void Apply(float *x_in, float *y_in, int num_points, 
	     double pvec[7],
	     double& xc, double& yc,
	     double& xa, double& ya,
	     double& la, double& lb,
	     double* xx, double* yy)
    {
    int np = num_points;
    static double invL[7][7];
    static double D[MAXP+1][7];
    static double S[7][7];
    static double Const[7][7];
    static double temp[7][7];
    static double L[7][7]; 
    static double C[7][7]; 
    
    static double d [7];
    static double V[7][7]; 
    static double sol[7][7];
    static double tx,ty;
    int nrot=0;
        int npts=50;

        static double XY[3][MAXP+1];
        
	assert(num_points<MAXP);

        int mode = FPF;

	for (int i=0; i<7; i++)
	  {
	    d[i] = 0;
	    for (int j=0; j<MAXP+1; j++)
	      {
		D[j][i] = 0;
	      }
	    for (int j=0; j<7; j++)
	      {
		S[i][j] = 0;
		Const[i][j] = 0;
		temp[i][j] = 0;
		L[i][j] = 0;
		C[i][j] = 0;
		invL[i][j] = 0;
		V[i][j] = 0;
		sol[i][j] = 0;
	      }
	  }

        switch (mode) {
           case(FPF):
              //System.out.println("FPF mode");
              Const[1][3]=-2;
              Const[2][2]=1;
              Const[3][1]=-2;   
              break;
           case(TAUBIN):
	     // g.drawString(warn_taub_str,size().width/18 , size().height/18 );
              break;
           case(BOOKSTEIN):
              //System.out.println("BOOK mode");
              Const[1][1]=2;
              Const[2][2]=1;
              Const[3][3]=2;    

          }

        if (np<6)
          return;


	// Now first fill design matrix
        for (int i=1; i <= np; i++)
          { 
            tx = x_in[i-1];
            ty = y_in[i-1];
//	    printf("%d %d, %g, %g\n", i, np, tx, ty);
            D[i][1] = tx*tx;
            D[i][2] = tx*ty;
            D[i][3] = ty*ty;
            D[i][4] = tx;
            D[i][5] = ty;
            D[i][6] = 1.0;
          }

//      printf("Done\n");
      

        //pm(Const,"Constraint");
        // Now compute scatter matrix  S
        A_TperB(D,D,S,np,6,np,6);
        //pm(S,"Scatter");

        choldc(S,6,L);    
        //pm(L,"Cholesky");


        inverse7(L,invL,6);
        //pm(invL,"inverse");
        
        AperB_T(Const,invL,temp,6,6,6,6);
        AperB(invL,temp,C,6,6,6,6);
        //pm(C,"The C matrix");


        jacobi(C,6,d,V,nrot);
        //pm(V,"The Eigenvectors");  
        //pv(d,"The eigevalues");
        
        A_TperB(invL,V,sol,6,6,6,6);
        //pm(sol,"The GEV solution unnormalized"); 

        // Now normalize them 
        for (int j=1;j<=6;j++) 
          {
            double mod = 0.0;
            for (int i=1;i<=6;i++)
              mod += sol[i][j]*sol[i][j];
            for (int i=1;i<=6;i++)
              sol[i][j] /=  sqrt(mod); 
          }

        //pm(sol,"The GEV solution"); 
        
        double zero=10e-20;
        double minev=10e+20;
        int  solind=0;
        switch (mode) {
           case(BOOKSTEIN):  // smallest eigenvalue                
              for (int i=1; i<=6; i++)
                 if (d[i]<minev && fabs(d[i])>zero) 
                   solind = i;
              break;
           case(FPF):
              for (int i=1; i<=6; i++)
                 if (d[i]<0 && fabs(d[i])>zero)     
                   solind = i;
          }
        // Now fetch the right solution
        for (int j=1;j<=6;j++)
	  {
	    pvec[j] = sol[j][solind];
	    //params[j-1] = sol[j][solind];
	  }
        //pv(pvec,"the solution");

	//	double xc, yc, xa, ya, a, b;
	get_param(pvec,4,xx,yy);

	/*
	  In fact this la and lb are WRONG
	  for any but horizontal/vertical case
	 */
	la = 0.5*sqrt((xx[0]-xx[2])*(xx[0]-xx[2])+
		      (yy[0]-yy[2])*(yy[0]-yy[2]));
	lb = 0.5*sqrt((xx[1]-xx[3])*(xx[1]-xx[3])+
		      (yy[1]-yy[3])*(yy[1]-yy[3]));
	xc = (xx[0]+xx[1]+xx[2]+xx[3])/4;
	yc = (yy[0]+yy[1]+yy[2]+yy[3])/4;
	xa = (xx[0]-xc);
	ya = (yy[0]-yc);
	/*
	for (int i=0; i<4; i++) {
	  this->xx[i] = xx[i];
	  this->yy[i] = yy[i];
	}
	*/
	double dv = sqrt(xa*xa+ya*ya);
	if (dv>0.0001)
	  {
	    xa /= dv;
	    ya /= dv;
	  }

	}
};


void FitEllipse::apply(float *x_in, float *y_in, int num_points) {
  EllipseFitter fitter;
  fitter.Apply(x_in,y_in,num_points,pvec,xc,yc,xa,ya,la,lb,
	       xx,yy);
	
	Ao = pvec[6];
	Ax = pvec[4];
	Ay = pvec[5];
	Axx = pvec[1];
	Ayy = pvec[3];
	Axy = pvec[2];

	assignf(1e8,Axx,Axy,Ayy,Ax,Ay,Ao);	
	 
}


inline int rnd(double x) { return (x>=0.0)?(int)(x + 0.5):(int)(x - 0.5); }

void FitEllipse::assignf(double scale,double A_, double B_, double C_, double D_, double E_, double F_){
	A = rnd(A_ * scale);
	B = rnd(B_ * scale);
	C = rnd(C_ * scale);
	D = rnd(D_ * scale);
	E = rnd(E_ * scale);
	F = rnd(F_ * scale);
}


int FitEllipse::generate() {
  Ao = pvec[6];
  Ax = pvec[4];
  Ay = pvec[5];
  Axx = pvec[1];
  Ayy = pvec[3];
  Axy = pvec[2];


   
      
   assignf(1e8,Axx,Axy,Ayy,Ax,Ay,Ao);
	// }
      

  return len;
}


// simple test...
//
//#define TEST_FIT
//
//#ifdef TEST_FIT
//
//#define MYN 40
//
//void main()
//{
//  float x[MYN], y[MYN];
//  double p[6];
//  float theta;
//  EllipseFitter fitter;
//  for (int i=0; i<MYN; i++)
//    {
//      theta = i*2*M_PI/MYN;
//      x[i] = 20 + 40*cos(theta);
//      y[i] = 80*sin(theta);
//    }
//  double xc, yc;
//  double xa, ya;
//  double la, lb;
//  fitter.Apply(x,y,MYN,xc,yc,xa,ya,la,lb);
//  printf("(%g,%g) %gx%g (%g,%g)\n", xc, yc, la, lb, xa, ya);
//}
//
//#endif
