#ifndef _UNITVECTOR_
#define _UNITVECTOR_

//########################### TUnitVec ##############################
class TUnitVec{  // ��������� ������
 public:
   double  x,  y,  z;   // ������
   double ox, oy, oz;   // ������������ ��������
  // ------------------------ Designers ------------------------
   TUnitVec( void );
   TUnitVec( double x_, double y_, double z_, double ox_, double oy_, double oz_ );
  // ------------------------ Selectors ------------------------
   void Move( double t );   // �������� ����� �����������
   void MoveTo( double x_, double y_, double z_ ); // ����������� � �����
   void DirectTo( double ox_, double oy_, double oz_ ); // ��������� �� �������
   void Back( void );   // ��������� �� ���������������
   double Radius( void );   // �������� ������ �� 0
   void Rotate( double ct, double cf, double sf );
   void Copy( TUnitVec &V );
   void Set( double x_, double y_, double z_, double ox_, double oy_, double oz_ );
   double PointToPlane( double x_, double y_, double z_ );
   double PointToLine( double x_, double y_, double z_ );
   double LineToLine( TUnitVec &V );
  // ------------------------ Controls ------------------------
  private:
};

void   Tran01( double *X, double *Y, double *TRM );
void   Tan01( double *X, double *Y, double *TRM );
void   Tran10( double *X, double *Y, double *TRM );
void   Tan10( double *X, double *Y, double *TRM );
void   VectorMult( TUnitVec *VecM, TUnitVec &V1, TUnitVec &V2 );
void   VectorSum( TUnitVec *VecM, TUnitVec &V1, TUnitVec &V2 );
void   GetParaPlane( TUnitVec *VecM, TUnitVec &V1, TUnitVec &V2 );
double ScalarMult( TUnitVec &V1, TUnitVec &V2 );
double GetDistPoints( TUnitVec *VR1, TUnitVec *VR2, TUnitVec &V1, TUnitVec &V2 );

#endif
