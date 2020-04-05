// 3Dto2D.h
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DTO2D_H__FE68DEB9_7D69_11D3_B0D5_004005691B12__INCLUDED_)
#define AFX_3DTO2D_H__FE68DEB9_7D69_11D3_B0D5_004005691B12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>
#include <float.h>

const double EPS = 0.0005;			// NC�̌v�Z�덷
const double NCMIN = 0.001;			// NC�̌������덷
const double PI = 3.1415926535897932384626433832795;
const double RAD = PI/180.0;
const double DEG = 180.0/PI;

const double RX = -60.0*RAD;		// ��̫�Ẳ�]�p�x
const double RY =   0.0*RAD;
const double RZ = -35.0*RAD;

//////////////////////////////////////////////////////////////////////
// NCVC���l���Z���ʊ֐�

class	CPointD;
// �Q�̉~�̌�_�����߂�
int		CalcCircleNode(const CPointD&, const CPointD&, double, double, CPointD*, CPointD*);
// ���Ɛ��̒[�_�𒆐S�Ƃ����~�Ƃ̌�_�����߂�
CPointD	CalcCircleLineNode(const CPointD&, double, const CPointD&);
// �Q���������̉������߂�
int		GetKon(double, double, double, double*, double*);
// �������z����f����Ԃ�
UINT	GetPrimeNumber(UINT);
// 1/1000 �l�̌ܓ�
inline	double	RoundUp(double dVal)
{
	return _copysign( floor(fabs(dVal) * 1000.0 + 0.5) / 1000.0, dVal );
	// �؂�グ�͒��~
//	return _copysign( ceil(fabs(dVal) * 1000.0) / 1000.0, dVal );
}

//////////////////////////////////////////////////////////////////////
// double�^�� CPoint�N���X

class CPointD {
public:
	double	x, y;
	// �R���X�g���N�^
	CPointD() {
		SetPoint(0.0, 0.0);
	}
	CPointD(double xy) {
		SetPoint(xy, xy);
	}
	CPointD(double xx, double yy) {
		SetPoint(xx, yy);
	}
	CPointD(const CPoint& pt) {
		SetPoint(pt.x, pt.y);
	}
	// ���Z�q��`
	CPointD&	operator = (double pt) {
		SetPoint(pt, pt);
		return *this;
	}
	CPointD&	operator = (const CPoint& pt) {
		SetPoint(pt.x, pt.y);
		return *this;
	}
	CPointD		operator + (double d) const {
		CPointD		pt(x+d, y+d);
		return pt;
	}
	CPointD		operator + (const CPointD& pt2) const {
		CPointD		pt(x+pt2.x, y+pt2.y);
		return pt;
	}
	CPointD		operator - (double d) const {
		CPointD		pt(x-d, y-d);
		return pt;
	}
	CPointD		operator - (const CPointD& pt2) const {
		CPointD		pt(x-pt2.x, y-pt2.y);
		return pt;
	}
	CPointD		operator * (double d) const {
		CPointD		pt(x*d, y*d);
		return pt;
	}
	CPointD		operator * (const CPointD& pt2) const {
		CPointD		pt(x*pt2.x, y*pt2.y);
		return pt;
	}
	CPointD		operator / (double d) const {
		CPointD		pt(x/d, y/d);
		return pt;
	}
	CPointD		operator / (const CPointD& pt2) const {
		CPointD		pt(x/pt2.x, y/pt2.y);
		return pt;
	}
	CPointD&	operator += (double d) {
		x += d;		y += d;
		return *this;
	}
	CPointD&	operator += (const CPointD& pt) {
		x += pt.x;		y += pt.y;
		return *this;
	}
	CPointD&	operator -= (double d) {
		x -= d;		y -= d;
		return *this;
	}
	CPointD&	operator -= (const CPointD& pt) {
		x -= pt.x;		y -= pt.y;
		return *this;
	}
	CPointD&	operator *= (double d) {
		x *= d;		y *= d;
		return *this;
	}
	CPointD&	operator *= (const CPointD& pt) {
		x *= pt.x;		y *= pt.y;
		return *this;
	}
	CPointD&	operator /= (double d) {
		x /= d;		y /= d;
		return *this;
	}
	CPointD&	operator /= (const CPointD& pt) {
		x /= pt.x;		y /= pt.y;
		return *this;
	}
	BOOL		operator == (const double pt) const {
		return ( x==pt && y==pt ) ? TRUE : FALSE;
	}
	BOOL		operator == (const CPointD& pt) const {
		return ( x==pt.x && y==pt.y ) ? TRUE : FALSE;
	}
	BOOL		operator != (const double pt) const {
		return !(operator ==(pt));
	}
	BOOL		operator != (const CPointD& pt) const {
		return !(operator ==(pt));
	}
	BOOL	IsMatchPoint(const CPointD* pt) const {
		return ( x==pt->x && y==pt->y ) ? TRUE : FALSE;
	}
	double&		operator[] (size_t a) const {
		ASSERT(a>=0 && a<2);
		return *((double *)this+a);
	}
	// �ϊ��֐�
	operator CPoint() const {
		CPoint	pt((int)x, (int)y);
		return pt;
	}
	CPointD	RoundUp(void) const {
		CPointD	pt(::RoundUp(x), ::RoundUp(y));
		return pt;
	}
	// ����֐�
	void	SetPoint(double xx, double yy) {
		x = xx;		y = yy;
	}
	// ���W��]
	void	RoundPoint(double q) {
		double	cosq = cos(q), sinq = sin(q);
		CPointD	pt(x, y);
		x = pt.x * cosq - pt.y * sinq;
		y = pt.x * sinq + pt.y * cosq;
	}
};

//////////////////////////////////////////////////////////////////////
// 3D-CPointD �N���X

class CPoint3D : public CPointD {
public:
	double	z;
	// �������̂��߂̃R���X�g���N�^
	CPoint3D() {
		z = 0.0;
	}
	CPoint3D(double xyz) : CPointD(xyz) {
		z = xyz;
	}
	CPoint3D(double xx, double yy, double zz) : CPointD(xx, yy) {
		z = zz;
	}
	// ���Z�q��`
	CPoint3D&	operator = (double pt) {
		SetPoint(pt, pt, pt);
		return *this;
	}
	CPoint3D	operator + (double d) const {
		CPoint3D	pt(x+d, y+d, z+d);
		return pt;
	}
	CPoint3D	operator + (const CPoint3D& pt2) const {
		CPoint3D	pt(x+pt2.x, y+pt2.y, z+pt2.z);
		return pt;
	}
	CPoint3D	operator - (double d) const {
		CPoint3D	pt(x-d, y-d, z-d);
		return pt;
	}
	CPoint3D	operator - (const CPoint3D& pt2) const {
		CPoint3D	pt(x-pt2.x, y-pt2.y, z-pt2.z);
		return pt;
	}
	CPoint3D	operator * (double d) const {
		CPoint3D	pt(x*d, y*d, z*d);
		return pt;
	}
	CPoint3D	operator * (const CPoint3D& pt2) const {
		CPoint3D	pt(x*pt2.x, y*pt2.y, z*pt2.z);
		return pt;
	}
	CPoint3D	operator / (double d) const {
		CPoint3D	pt(x/d, y/d, z/d);
		return pt;
	}
	CPoint3D	operator / (const CPoint3D& pt2) const {
		CPoint3D	pt(x/pt2.x, y/pt2.y, z/pt2.z);
		return pt;
	}
	CPoint3D&	operator += (double d) {
		x += d;		y += d;		z += d;
		return *this;
	}
	CPoint3D&	operator += (const CPoint3D& pt) {
		x += pt.x;		y += pt.y;		z += pt.z;
		return *this;
	}
	CPoint3D&	operator -= (double d) {
		x -= d;		y -= d;		z -= d;
		return *this;
	}
	CPoint3D&	operator -= (const CPoint3D& pt) {
		x -= pt.x;		y -= pt.y;		z -= pt.z;
		return *this;
	}
	CPoint3D&	operator *= (double d) {
		x *= d;		y *= d;		z *= d;
		return *this;
	}
	CPoint3D&	operator *= (const CPoint3D& pt) {
		x *= pt.x;	y *= pt.y;	z *= pt.z;
		return *this;
	}
	CPoint3D&	operator /= (double d) {
		x /= d;		y /= d;		z /= d;
		return *this;
	}
	CPoint3D&	operator /= (const CPoint3D& pt) {
		x /= pt.x;	y /= pt.y;	z /= pt.z;
		return *this;
	}
	BOOL		operator == (const double pt) const {
		return ( x==pt && y==pt && z==pt ) ? TRUE : FALSE;
	}
	BOOL		operator == (const CPoint3D& pt) const {
		return ( x==pt.x && y==pt.y && z==pt.z ) ? TRUE : FALSE;
	}
	BOOL		operator != (const double pt) const {
		return !(operator ==(pt));
	}
	BOOL		operator != (const CPoint3D& pt) const {
		return !(operator ==(pt));
	}
	BOOL	IsMatchPoint(const CPoint3D* pt) const {
		return ( x==pt->x && y==pt->y && z==pt->z ) ? TRUE : FALSE;
	}
	double&		operator[] (size_t a) const {
		ASSERT(a>=0 && a<3);
		return *((double *)this+a);
	}
	// �ϊ��֐�
	CPointD	GetXY(void) const {
		CPointD	pt(x, y);
		return pt;
	}
	CPointD	GetXZ(void) const {
		CPointD	pt(x, z);
		return pt;
	}
	CPointD	GetYZ(void) const {
		CPointD	pt(y, z);
		return pt;
	}
	// ����֐�
	void	SetPoint(double xx, double yy, double zz) {
		CPointD::SetPoint(xx, yy);
		z = zz;
	}
};

//////////////////////////////////////////////////////////////////////
// double�^�� CRect�׽

class CRectD {
public:
	double	left, top, right, bottom;
	// �������̂��߂̺ݽ�׸�
	CRectD() {
		SetRectEmpty();
	}
	CRectD(double l, double t, double r, double b) {
		left = l;			top = t;
		right = r;			bottom = b;
	}
	CRectD(const CRect& rc) {
		left = rc.left;		top = rc.top;
		right = rc.right;	bottom = rc.bottom;
	}
	// ���ƍ����̐��K��
	void	NormalizeRect() {
		double	dTemp;
		if (left > right) {
			dTemp = left;
			left  = right;
			right = dTemp;
		}
		if (top > bottom) {
			dTemp = top;
			top = bottom;
			bottom = dTemp;
		}
	}
	// �w����W����`���ɂ��邩
	BOOL	PtInRect(const CPointD& pt) const {
		return left<=pt.x && pt.x<=right && top<=pt.y && pt.y<=bottom;
	}
	// 2�̎l�p�`������镔���ɑ�������l�p�`��ݒ�
	BOOL	IntersectRect(const CRectD& rc1, const CRectD& rc2) {
		left	= max(rc1.left, rc2.left);
		top		= max(rc1.top, rc2.top);
		right	= min(rc1.right, rc2.right);
		bottom	= min(rc1.bottom, rc2.bottom);
		if ( rc1.PtInRect(TopLeft()) && rc1.PtInRect(BottomRight()) )
			return TRUE;
		SetRectEmpty();
		return FALSE;
	}
	// ���S���W...��
	CPointD	CenterPoint(void) const {
		CPointD	pt( (left+right)/2, (top+bottom)/2 );
		return pt;
	}
	double	Width(void) const {
		return right - left;
	}
	double	Height(void) const {
		return bottom - top;
	}
	const CPointD&	TopLeft(void) const {
		return *((CPointD *)this);
	}
	CPointD&	TopLeft(void) {
		return *((CPointD *)this);
	}
	const CPointD&	BottomRight(void) const {
		return *((CPointD *)this+1);
	}
	CPointD&	BottomRight(void) {
		return *((CPointD *)this+1);
	}
	// �w�肳�ꂽ�̾�Ă� CRectD ���ړ�
	void	OffsetRect(double x, double y) {
		left	+= x;		right	+= x;
		top		+= y;		bottom	+= y;
	}
	void	OffsetRect(const CPointD& pt) {
		left	+= pt.x;	right	+= pt.x;
		top		+= pt.y;	bottom	+= pt.y;
	}
	// �e�ӂ𒆐S����O���Ɍ������Ĉړ�
	void	InflateRect(double w, double h) {
		left	-= w;		right	+= w;
		top		-= h;		bottom	+= h;
	}
	// ���Z�q��`
	CRectD&	operator /= (double d) {
		left /= d;		right /= d;
		top /= d;		bottom /= d;
		return *this;
	}
	CRectD&	operator |= (const CRectD& rc) {	// ��`�����킹��(����)
		if ( left   > rc.left )		left = rc.left;
		if ( top    > rc.top )		top = rc.top;
		if ( right  < rc.right )	right = rc.right;
		if ( bottom < rc.bottom )	bottom = rc.bottom;
		return *this;
	}
	// �ϊ��֐�
	operator	CRect() const {
		CRect	rc((int)left, (int)top, (int)right, (int)bottom);
		return rc;
	}
	double&		operator[] (size_t a) {
		ASSERT(a>=0 && a<4);
		return *((double *)this+a);
	}
	// ������
	void	SetRectEmpty(void) {
		left = top = right = bottom = 0.0;
	}
};

//////////////////////////////////////////////////////////////////////
// double�^�� CRect�R������`�׽

class CRect3D : public CRectD {
public:
	double	high, low;		// ����
	// �������̂��߂̺ݽ�׸�
	CRect3D() {
		high = low = 0.0;	// CRectD::SetRectEmpty() �͎����ŌĂ΂��
	}
	CRect3D(double l, double t, double r, double b, double h, double w) :
			CRectD(l, t, r, b) {
		high = h;	low = w;
	}
	// ���ƍ����̐��K��
	void	NormalizeRect() {
		CRectD::NormalizeRect();
		if ( high < low ) {
			double dTemp = high;
			high = low;
			low = dTemp;
		}
	}
	// �w�肳�ꂽ�̾�Ă� CRectD ���ړ�
	void	OffsetRect(double x, double y, double z) {
		CRectD::OffsetRect(x, y);
		high	+= z;		low		+= z;
	}
	void	OffsetRect(const CPointD& pt) {
		CRectD::OffsetRect(pt);
	}
	void	OffsetRect(const CPoint3D& pt) {
		CRectD::OffsetRect(pt);
		high	+= pt.z;	low		+= pt.z;
	}
	// ���Z�q��`
	CRect3D&	operator |= (const CRect3D& rc) {
		CRectD::operator |= (rc);
		if ( low  > rc.low )	low = rc.low;
		if ( high < rc.high )	high = rc.high;
		return *this;
	}
	// �ϊ��֐�
	operator	CRectD() const {
		CRectD	rc(left, top, right, bottom);
		return rc;
	}
	// ������
	void	SetRectEmpty(void) {
		CRectD::SetRectEmpty();
		high = low = 0.0;
	}
};

// �ެ��ߌv�Z�̲�ײ݊֐�
inline	double	GAPCALC(double x, double y)
{
	return	x * x + y * y;		// ����������炸�Q��ł悢
//	return	hypot(x, y);		// ���Ԃ������肷��
}
inline	double	GAPCALC(const CPointD& pt)
{
	return pt.x * pt.x + pt.y * pt.y;
}

#endif // !defined(AFX_3DTO2D_H__FE68DEB9_7D69_11D3_B0D5_004005691B12__INCLUDED_)
