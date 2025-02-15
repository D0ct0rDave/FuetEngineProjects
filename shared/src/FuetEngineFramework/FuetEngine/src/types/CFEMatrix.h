// ----------------------------------------------------------------------------
/*! \class CFEMatrix
 *  \brief 3x3 Matrix class
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 *  \TODO: implement operations with 3x2 matrices
 */
// ----------------------------------------------------------------------------
#ifndef CFEMatrixH
#define CFEMatrixH
// ----------------------------------------------------------------------------
#pragma warning( disable : 4244 )	// double to FEReal conversion
// ----------------------------------------------------------------------------
#include "Support/Math/CFEMath.h"
#include "FEBasicTypes.h"
#include "Types/CFEVect2.h"
// ----------------------------------------------------------------------------
class CFEMatrix
{
    public:
        /// Default constructor for the class.
        CFEMatrix(){};
        FEReal m[3][3];

    public:

		/// Loads the identity matrix.
		void LoadIdentity()
		{
		    m[0][0] = _1r;
		    m[0][1] = _0r;
    	    m[0][2] = _0r;

            m[1][0] = _0r;
    	    m[1][1] = _1r;
    	    m[1][2] = _0r;

            m[2][0] = _0r;
            m[2][1] = _0r;
    	    m[2][2] = _1r;
		}

		/// Rotates the matrix by the given angle.
        void Rotate(FEReal _rAngle)
        {
            FEReal rCosA = CFEMath::rCos(_rAngle);
            FEReal rSinA = CFEMath::rSin(_rAngle);

            CFEMatrix oRMat;
            oRMat.m[0][0] = rCosA;
            oRMat.m[0][1] = rSinA;
            oRMat.m[0][2] = _0r;

            oRMat.m[1][0] = -rSinA;
            oRMat.m[1][1] =  rCosA;
            oRMat.m[1][2] = _0r;

            oRMat.m[2][0] = _0r;
            oRMat.m[2][1] = _0r;
            oRMat.m[2][2] = _1r;

            MultiplyBy( oRMat );
        }

		/// Scales the matrix by the given factors.
        void Scale(FEReal _rScaleX,FEReal _rScaleY)
        {
		    m[0][0] *= _rScaleX;
		    m[0][1] *= _rScaleX;
    	    m[0][2] *= _rScaleX;

            m[1][0] *= _rScaleY;
    	    m[1][1] *= _rScaleY;
    	    m[1][2] *= _rScaleY;
        }

		/// Translates the matrix.
        void Translate(FEReal _rX,FEReal _rY)
        {
			m[2][0] = m[0][0] * _rX + m[1][0] * _rY + m[2][0];
			m[2][1] = m[0][1] * _rX + m[1][1] * _rY + m[2][1];
			m[2][2] = m[0][2] * _rX + m[1][2] * _rY + m[2][2];
        }

		/// Stores the result of the multiplication of the given matrices.
        void Multiply(const CFEMatrix& _oA,const CFEMatrix& _oB)
        {
            m[0][0] = _oA.m[0][0]*_oB.m[0][0] + _oA.m[0][1]*_oB.m[1][0] + _oA.m[0][2]*_oB.m[2][0];
            m[0][1] = _oA.m[0][0]*_oB.m[0][1] + _oA.m[0][1]*_oB.m[1][1] + _oA.m[0][2]*_oB.m[2][1];
            m[0][2] = _oA.m[0][0]*_oB.m[0][2] + _oA.m[0][1]*_oB.m[1][2] + _oA.m[0][2]*_oB.m[2][2];
            
            m[1][0] = _oA.m[1][0]*_oB.m[0][0] + _oA.m[1][1]*_oB.m[1][0] + _oA.m[1][2]*_oB.m[2][0];
            m[1][1] = _oA.m[1][0]*_oB.m[0][1] + _oA.m[1][1]*_oB.m[1][1] + _oA.m[1][2]*_oB.m[2][1];
            m[1][2] = _oA.m[1][0]*_oB.m[0][2] + _oA.m[1][1]*_oB.m[1][2] + _oA.m[1][2]*_oB.m[2][2];
            
            m[2][0] = _oA.m[2][0]*_oB.m[0][0] + _oA.m[2][1]*_oB.m[1][0] + _oA.m[2][2]*_oB.m[2][0];
            m[2][1] = _oA.m[2][0]*_oB.m[0][1] + _oA.m[2][1]*_oB.m[1][1] + _oA.m[2][2]*_oB.m[2][1];
            m[2][2] = _oA.m[2][0]*_oB.m[0][2] + _oA.m[2][1]*_oB.m[1][2] + _oA.m[2][2]*_oB.m[2][2];
        }

		/// Stores the result of the multiplication of the given matrices.
        void MultiplyBy(const CFEMatrix& _oA)
        {
            CFEMatrix oM = *this;
            // Multiply(_oA,oM);
            Multiply(_oA,oM);
        }

		/// Transform the given vector by the matrix.
        CFEVect2 Transform(const CFEVect2& _oV) const
        {
            CFEVect2 v;

            // Hacer dot product del vector y la fila
            v.x = m[0][0] * _oV.x + m[1][0] * _oV.y + m[2][0];
            v.y = m[0][1] * _oV.x + m[1][1] * _oV.y + m[2][1];

            return(v);
        }

        /// Computes the trasposed matrix.
        void Transpose()
        {
			CFEMatrix oMat = *this;
			
			for (uint j=0;j<3;j++)
				for (uint i=0;i<3;i++)
					m[j][i] = oMat.m[i][j];	
        }

        /// Computes the inverse of the given matrix
        void Inverse()
        {
			// Transpose();
			// return;

            /*
            CFEMatrix oMat = *this;

            FEReal rDet = m[0][0]*m[1][1] - m[1][0]*m[0][1];
            if (rDet != 0.0)
            {
                FEReal rInvDet = 1.0 / rDet;
                m[0][0] =  oMat.m[1][1]*rInvDet;
                m[0][1] = -oMat.m[0][1]*rInvDet;
                m[1][0] = -oMat.m[1][0]*rInvDet;
                m[1][1] =  oMat.m[0][0]*rInvDet;

                // do the translation part
                m[2][0] = -(oMat.m[2][0]*m[0][0] + oMat.m[2][1]*m[1][0]);
                m[2][1] = -(oMat.m[2][0]*m[0][1] + oMat.m[2][1]*m[1][1]);                               
            }
            */


			// http://stackoverflow.com/questions/983999/simple-3x3-matrix-inverse-code-c
			CFEMatrix oMat = *this;

			FEReal rDet = m[0][0]*m[1][1]*m[2][2] + m[1][0]*m[2][1]*m[0][2] + m[2][0]*m[0][1]*m[1][2] 
			            - m[0][0]*m[2][1]*m[1][2] - m[1][0]*m[0][1]*m[2][2] - m[2][0]*m[1][1]*m[0][2];

			if (rDet != _0r) 
			{			
				FEReal rInvDet = _1r / rDet;
				m[0][0] =  (oMat.m[1][1]*oMat.m[2][2] - oMat.m[2][1]*oMat.m[1][2])*rInvDet;
				m[0][1] =  (oMat.m[0][2]*oMat.m[2][1] - oMat.m[0][1]*oMat.m[2][2])*rInvDet;
				m[0][2] =  (oMat.m[0][1]*oMat.m[1][2] - oMat.m[0][2]*oMat.m[1][1])*rInvDet;
				
				m[1][0] =  (oMat.m[1][2]*oMat.m[2][0] - oMat.m[1][0]*oMat.m[2][2])*rInvDet;
				m[1][1] =  (oMat.m[0][0]*oMat.m[2][2] - oMat.m[0][2]*oMat.m[2][0])*rInvDet;
				m[1][2] =  (oMat.m[1][0]*oMat.m[0][2] - oMat.m[0][0]*oMat.m[1][2])*rInvDet;
				
				m[2][0] =  (oMat.m[1][0]*oMat.m[2][1] - oMat.m[2][0]*oMat.m[1][1])*rInvDet;
				m[2][1] =  (oMat.m[2][0]*oMat.m[0][1] - oMat.m[0][0]*oMat.m[2][1])*rInvDet;
				m[2][2] =  (oMat.m[0][0]*oMat.m[1][1] - oMat.m[1][0]*oMat.m[0][1])*rInvDet;				
			}

			// http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/threeD/index.htm
			/*
			double det = m00*m11*m22 + m01*m12*m20 + m02*m10*m21 - m00*m12*m21 - m01*m10*m22 - m02*m11*m20;

			 m00 = (m11*m22 - m12*m21)/det;
			 m01 = (m02*m21 - m01*m22)/det;
			 m02 = (m01*m12 - m02*m11)/det;
			 m10 = (m12*m20 - m10*m22)/det;
			 m11 = (m00*m22 - m02*m20)/det;
			 m12 = (m02*m10 - m00*m12)/det;
			 m20 = (m10*m21 - m11*m20)/det;
			 m21 = (m01*m20 - m00*m21)/det;
			 m22 = (m00*m11 - m01*m10)/det;
			*/			
        }
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
