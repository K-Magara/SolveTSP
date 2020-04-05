// 3Dto2D.cpp
// �R�����ϊ��̸�۰��ي֐�
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "3Dto2D.h"

#include <math.h>
#include <float.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// �Q�̉~�̌�_�����߂�

int CalcCircleNode
	(const CPointD& pts, const CPointD& pte, double r1, double r2, CPointD* lpt1, CPointD* lpt2)
{
	double	xx, yy, rr;
	r1 = fabs(r1);
	r2 = fabs(r2);

	// �n�_�r�����_�ƂȂ�悤���s�ړ�
	xx = pte.x - pts.x;		yy = pte.y - pts.y;
	rr = sqrt(xx*xx + yy*yy);

	// ��_�����߂��Ȃ�(��_���Ȃ�)����
	if ( rr - r1 - r2 > EPS || rr - fabs(r1 - r2) < -EPS )
		return -1;

	// �~�̕��������C(s.x, s.y)�C(e.x, e.y) �𒆐S�Ƃ���
	// �Q�̉~�̌�_�����߂�
	int		nResult;
	double	a, b, c;

	b = (xx*xx + yy*yy + r1*r1 - r2*r2) / 2.0;
	if ( fabs(yy) <= NCMIN ) {		// �r�Ƃd�̂x������
		a = lpt1->x = lpt2->x = b / xx;
		if ( r1 - fabs(a) <= NCMIN ) {
			lpt1->y = lpt2->y = 0.0;
			nResult = 1;
		}
		else {
			lpt1->y = sqrt(r1*r1 - a*a);
			lpt2->y = -lpt1->y;
			nResult = 2;
		}
	}
	else if ( fabs(xx) <= NCMIN ) {	// �r�Ƃd�̂w������
		a = lpt1->y = lpt2->y = b / yy;
		if ( r1 - fabs(a) <= NCMIN ) {
			lpt1->x = lpt2->x = 0.0;
			nResult = 1;
		}
		else {
			lpt1->x = sqrt(r1*r1 - a*a);
			lpt2->x = -lpt1->x;
			nResult = 2;
		}
	}
	else {			// �Q�~�̌�_�����߂邽�߂̎�
		a = -xx/yy;
		b = b / yy;
		c = b*b - r1*r1;
		// ����ό`���� ax^2 + bx + c = 0 �̌`
		// �Q���������̉������߂�
		if ( (nResult=GetKon(a*a+1.0, 2.0*a*b, c, &lpt1->x, &lpt2->x)) <= 0 )
			return nResult;
		lpt1->y = a * lpt1->x + b;
		lpt2->y = a * lpt2->x + b;
	}
	// ���߂����ʂɑ΂��Č��̈ʒu�֕��s�ړ�
	lpt1->x += pts.x;		lpt1->y += pts.y;
	lpt2->x += pts.x;		lpt2->y += pts.y;

	return nResult;
}

//////////////////////////////////////////////////////////////////////
// ���Ɛ��̒[�_�𒆐S�Ƃ����~�Ƃ̌�_�����߂�

CPointD	CalcCircleLineNode(const CPointD& ptOrg, double r, const CPointD& ptSrc)
{
	CPointD	pt;

	// ptOrg �𒆐S�Ƃ����~�� ptSrc �̌�_
	if ( fabs( ptSrc.x - ptOrg.x ) < EPS ) {	// �قړ�����
		pt.x = ptOrg.x;
		pt.y = ptOrg.y + _copysign(r, ptSrc.y - ptOrg.y);
	}
	else if ( fabs( ptSrc.y - ptOrg.y ) < EPS ) {
		pt.x = ptOrg.x + _copysign(r, ptSrc.x - ptOrg.x);
		pt.y = ptOrg.y;
	}
	else {
		double a = (ptSrc.y - ptOrg.y) / (ptSrc.x - ptOrg.x);
		pt.x = r * sqrt( 1 / (1+a*a) );
		pt.y = (ptOrg.x < ptSrc.x ? a : -a) * pt.x;
		pt.x = pt.y / a + ptOrg.x;	// x���Čv�Z(y��a�̕������l��)
		pt.y += ptOrg.y;			// ���_�␳
	}

	return pt;
}

//////////////////////////////////////////////////////////////////////
// �Q���������̉������߂�
// -1:�װ, 0:���Ȃ�, 1:�d��, 2:�Q��

int GetKon
	(double a, double b, double c, double* x1, double* x2)
{
	double	d;
	*x1 = *x2 = 0.0;
	if ( fabs(a) < EPS ) {
		if ( fabs(b) < EPS )
			return -1;
		else {
			*x1 = *x2 = -c / b;		// bx + c = 0
			return 1;
		}
	}
	else {
		b /= a;		c /= a;
		d = b*b - 4.0*c;
		if ( d > EPS ) {
			d = sqrt(d);
			*x1 = b > 0.0 ? (-b - d)/2.0 : (-b + d)/2.0;
			*x2 = c / *x1;
			return 2;
		}
		else if ( fabs(d) < EPS ) {
			*x1 = *x2 = -b / 2.0;
			return 1;
		}
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////
// �������z����f����Ԃ�

UINT GetPrimeNumber(UINT nNum)
{
	UINT	i = 0;		// �����l
	UINT	nLoop = 1;

	while ( i < nLoop ) {
		nLoop = (++nNum >> 1) + 1;
		for ( i=2; i<nLoop; i++ ) {
			if ( nNum % i == 0 )
				break;
		}
	}
	return nNum;
}
