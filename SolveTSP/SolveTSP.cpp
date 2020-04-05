// SolveTSP.cpp : DLL �p�̏����������̒�`���s���܂��B
//

#include "stdafx.h"
#include <afxdllx.h>
#include "NCVCaddin.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static AFX_EXTENSION_MODULE SolveTSPDLL = { NULL, NULL };

// ���W�\����
typedef	struct	tagTSPPT {
	CPointD		m_pt;
	BOOL		m_OriginFlag;
	BOOL		m_CheckedFlag;
} TSPPT, *LPTSPPT;

// ��{���W�z��
static	CSortArray<CPtrArray, LPTSPPT>	g_arPoint,	// ���W�\���̂��i�[����z��i���C���j
										g_arTmp,	// ���W�\���̂��i�[����z��i�ꎞ�j
										g_arBest;	// ���W�\���̂��i�[����z��i���̂Ƃ��x�X�g�ȁj

// ��Čv�Z�֐�
static double	GetCost(int);
// �z�񏉊���
static void		ClearArray(void);

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// lpReserved ���g���ꍇ�͂������폜���Ă�������
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("SOLVETSP.DLL Initializing!\n");
		
		// �g�� DLL ���P�񂾂����������܂��B
		if (!AfxInitExtensionModule(SolveTSPDLL, hInstance))
			return 0;

		// ���� DLL �����\�[�X �`�F�C���֑}�����܂��B
		// ����: �g�� DLL �� MFC �A�v���P�[�V�����ł͂Ȃ�
		//   MFC �W�� DLL (ActiveX �R���g���[���̂悤��)
		//   �ɈÖٓI�Ƀ����N�����ꍇ�A���̍s�� DllMain
		//   ����폜���āA���̊g�� DLL ����G�N�X�|�[�g
		//   ���ꂽ�ʂ̊֐����֒ǉ����Ă��������B  
		//   ���̊g�� DLL ���g�p����W�� DLL �͂��̊g�� DLL
		//   �����������邽�߂ɖ����I�ɂ��̊֐����Ăяo���܂��B 
		//   ����ȊO�̏ꍇ�́ACDynLinkLibrary �I�u�W�F�N�g��
		//   �W�� DLL �̃��\�[�X �`�F�C���փA�^�b�`���ꂸ�A
		//   ���̌��ʏd��Ȗ��ƂȂ�܂��B

		new CDynLinkLibrary(SolveTSPDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("SOLVETSP.DLL Terminating!\n");
		// �f�X�g���N�^���Ăяo�����O�Ƀ��C�u�������I�����܂�
		AfxTermExtensionModule(SolveTSPDLL);
	}
	return 1;   // ok
}


/////////////////////////////////////////////////////////////////////////////
// NCVC ��޲݊֐�

NCADDIN BOOL NCVC_Initialize(NCVCINITIALIZE* nci)
{
	// ��޲݂̕K�v���
	nci->dwSize = sizeof(NCVCINITIALIZE);
	nci->dwType = NCVCADIN_FLG_DXFEDIT;
//	nci->nToolBar = 0;
	nci->lpszMenuName[NCVCADIN_ARY_DXFEDIT] = "�����H�̍œK��...";
	nci->lpszFuncName[NCVCADIN_ARY_DXFEDIT] = "SolveTSP";
	nci->lpszAddinName	= "SolveTSP";
	nci->lpszCopyright	= "MNCT Yoshiro Nose";
	nci->lpszSupport	= "http://s-gikan2.maizuru-ct.ac.jp/";

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// NCVC ����Ċ֐�

NCADDIN void SolveTSP(void)
{
	DXFDATA		dxfData;

	// ���݃A�N�e�B�u�ȃh�L�������g�n���h�����擾
	NCVCHANDLE	hDoc = NCVC_GetDocument(NULL);

	// �e����
	char	szBuf[256];
	CString	strTmp;
	NCVC_GetDocumentFileName(hDoc, szBuf, sizeof(szBuf));

	LPTSPPT		lpTsp;		// �\���̂ւ��߲��
	int	nLayerCnt = NCVC_GetDXFLayerSize(hDoc), nDataCnt, i, j, k;
	// �z��T�C�Y�ݒ�
	ClearArray();
	g_arPoint.SetSize(0, 1024);
	g_arTmp.SetSize(0, 1024);
	g_arBest.SetSize(0, 1024);

	// NCVC�{�̂���CAD�f�[�^�̎擾
	for ( i=0; i<nLayerCnt; i++ ) {
		NCVC_GetDXFLayerData(hDoc, i, szBuf, sizeof(szBuf));
		nDataCnt = NCVC_GetDXFDataSize(hDoc, szBuf);

		strTmp=szBuf;
		if(strTmp == "TSP_MOVE"){
			NCVC_DelDXFData(hDoc, szBuf, 0, nDataCnt);
			continue;
		}

		for ( j=0; j<nDataCnt; j++ ) {
			dxfData.dwSize = sizeof(DXFDATA);
			lstrcpy(dxfData.szLayer, szBuf);
			NCVC_GetDXFData(hDoc, j, &dxfData);
			if ( dxfData.enType != DXFPOINTDATA )
				continue;
			// �_�f�[�^�����O�̔z��ɒǉ�
			lpTsp = new TSPPT;
			lpTsp->m_pt.x = dxfData.ptS.x;
			lpTsp->m_pt.y = dxfData.ptS.y;
			lpTsp->m_OriginFlag = 0;		// �׸ނ̏�����
			lpTsp->m_CheckedFlag = 0;		// �׸ނ̏�����
			g_arPoint.Add( lpTsp );
			g_arTmp.Add( lpTsp );
			g_arBest.Add( lpTsp );
		}
	}
	// �؍팴�_���z��ɒǉ�
	LPDPOINT	lpptOrg = NULL;
	lpptOrg = new DPOINT;
	NCVC_GetDXFCutterOrigin(hDoc, lpptOrg);
	lpTsp = new TSPPT;
	lpTsp->m_pt.x = lpptOrg->x;
	lpTsp->m_pt.y = lpptOrg->y;
	lpTsp->m_OriginFlag = 1;	// ���_�f�[�^�Ƀ`�F�b�N
	lpTsp->m_CheckedFlag = 0;	// �׸ނ̏�����
	g_arPoint.Add( lpTsp );
	g_arTmp.Add( lpTsp );
	g_arBest.Add( lpTsp );


	//////////////////////////////////////////////
	// �s�r�o�œK����@�ɂ����H���̕��בւ�
	//////////////////////////////////////////////

	int nNumOfData = g_arPoint.GetSize();				// �؍팴�_�܂߂��ް���
	double nInitCost = 0, nTotalCost = 0, nTmpCost = 0; // ������Ԃ̺�āC�œK����̺�āC�v�Z���̺��

	// ������Ԃ̺�āi�z�񏇂ɺ�đ����Ă����j
	nInitCost = GetCost(nNumOfData);
#ifdef _DEBUG
	strTmp.Format("nNumOfData�i�ް����j = %d\nnInitCost�i�z�񏇂ɑ�������āj = %f", nNumOfData, nInitCost);
	AfxMessageBox(strTmp);
#endif

//	for ( i=0; i<=nNumOfData; i++){
//		strTmp.Format("(x, y) = (%f, %f)", g_arBest[i]->m_pt.x, g_arBest[i]->m_pt.y);
//		AfxMessageBox(strTmp);
//	}

	////////////////////////////////////////////// ����ѻ���i�������l����̫�Ă̒l+1�j
	CPointD	pt;
	int nRand1, nRand2;

	// �o�H������тɂ��ĺ�Čv�Z
	for (j=0, nTotalCost=nInitCost; nTotalCost>=nInitCost && j<10000; j++){
		srand( (unsigned)time( NULL ) );	// �����ު�ڰ��̏������i���ԂŁj
		for ( i=0; i<nNumOfData; i++){
			nRand1 = rand() % nNumOfData;	// �����̔���
			nRand2 = rand() % nNumOfData;
			ASSERT( nRand1>=0 && nRand1<g_arBest.GetSize() );
			ASSERT( nRand2>=0 && nRand2<g_arBest.GetSize() );
			// nRand1��nRand2�̓���ւ�
			lpTsp = g_arBest.GetAt(nRand1);					// nRand1�̒l��ޔ����Ă�����
			g_arBest.SetAt(nRand1, g_arBest.GetAt(nRand2));	// nRand1��nRand2������
			g_arBest.SetAt(nRand2, lpTsp);					// nRand2�ɑޔ������l����
		}
		// ����ѻ���̺�Čv�Z
		nTotalCost = GetCost(nNumOfData);
	}

#ifdef _DEBUG
	strTmp.Format("nTotalCost�i����ѻ���̺�āj = %f", nTotalCost);
	AfxMessageBox(strTmp);
#endif

	//////////////////////////////////////////////  �R�o��@�i����ѻ���̌��ʂ������l�Ɂj
/*	for( g_nBestTotalCost; 1; nInitCost = g_nBestTotalCost){
		// �Q�_�����ւ����o�H���쐬
		for ( i=0; i<nNumOfData-2; i++){
			for ( int j=i+1; j<nNumOfData-1; j++){
				g_arTmp.Copy(g_arBest);					// g_arTmp��g_arBest���߰
				lpTsp = g_arTmp.GetAt(j);				// j�̒l��ޔ����Ă�����
				g_arTmp.SetAt(j, g_arTmp.GetAt(i));		// j��i������
				g_arTmp.SetAt(i, lpTsp);				// i�ɑޔ������l����
				// ��Ă̌v�Z
				nTmpCost = 0;
				for ( i=0; i<nNumOfData-1; i++){
					pt = g_arTmp[i+1]->m_pt - g_arTmp[i]->m_pt;
					nTmpCost += GAPCALC(pt);
				}
				// �D�G�Ȍo�H���L�^
				if ( nTmpCost < g_nBestTotalCost ){
					g_arPoint.Copy(g_arTmp);			// g_arPoint��g_arTmp���R�s�[
					g_nBestTotalCost = nTmpCost;
				}
			}
		}
		// �����Ƃ��D�G�Ȍo�H�����̎�l����
		if ( g_nBestTotalCost < nInitCost ){
			g_arBest.Copy(g_arPoint);	// g_arBest��g_arPoint���R�s�[
		}else{
			break;						// �D�G�Ȍo�H���Ȃ���ΏI��
		}
	}
	strTmp.Format("g_nBestTotalCost�i�R�o��@�̍œK���j = %f", g_nBestTotalCost);
	AfxMessageBox(strTmp);
*/
	//////////////////////////////////////////////  2-opt�Z�@�i����ѻ���̌��ʂ������l�Ɂj

	int i0 = 0;
	double nDifCost = 0;
	CPointD	pt1, pt2, pt3, pt4, pt5, pt6;

	for (i=0; i<i0+nNumOfData; i++){
		for (j=i+2; j<i+nNumOfData-1; j++){
			pt1 =  g_arBest[i % nNumOfData]->m_pt - g_arBest[j % nNumOfData]->m_pt;
			pt2 =  g_arBest[(i+1) % nNumOfData]->m_pt - g_arBest[(j+1) % nNumOfData]->m_pt;
			pt3 =  g_arBest[i % nNumOfData]->m_pt - g_arBest[(i+1) % nNumOfData]->m_pt;
			pt4 =  g_arBest[j % nNumOfData]->m_pt - g_arBest[(j+1) % nNumOfData]->m_pt;
			nDifCost = GAPCALC(pt1) + GAPCALC(pt2) - GAPCALC(pt3) - GAPCALC(pt4);	// �����̑���
			if (nDifCost < -0.0001){		// ���ǉ��̔���
				for (k=0; k<(j-i)/2; k++){
					lpTsp = g_arBest.GetAt((j-k) % nNumOfData);									// j-k �̒l��ޔ����Ă�����
					g_arBest.SetAt((j-k) % nNumOfData, g_arBest.GetAt((i+1+k) % nNumOfData));	// j-k �� i+1+k ������
					g_arBest.SetAt((i+1+k) % nNumOfData, lpTsp);								// i+1+k �ɑޔ������l����
				}
				i0 = i + 1; 
				break;
			}
		}
	}

#ifdef _DEBUG
	nTotalCost = GetCost(nNumOfData);
	strTmp.Format("nTotalCost�i2-opt�Z�@�̍œK���j = %f", nTotalCost);
	AfxMessageBox(strTmp);
#endif

/*	//////////////////////////////////////////////  Or-opt�Z�@�i2-opt�Z�@�̌��ʂ������l�Ɂj
	int h;
	i0 = 0;
	for (i=i0; i<i0+nNumOfData; i++){
//	for (i=0; i<i0+nNumOfData; i++){
		for (k=i+1; k<=i+3; k++){
			for (j=k+1; j<i+nNumOfData-1; j++){
				pt1 =  g_arBest[i % nNumOfData]->m_pt - g_arBest[(i+1) % nNumOfData]->m_pt;
				pt2 =  g_arBest[j % nNumOfData]->m_pt - g_arBest[(j+1) % nNumOfData]->m_pt;
				pt3 =  g_arBest[k % nNumOfData]->m_pt - g_arBest[(k+1) % nNumOfData]->m_pt;
				pt4 =  g_arBest[i % nNumOfData]->m_pt - g_arBest[(k+1) % nNumOfData]->m_pt;
				pt5 =  g_arBest[j % nNumOfData]->m_pt - g_arBest[k % nNumOfData]->m_pt;
				pt6 =  g_arBest[(i+1) % nNumOfData]->m_pt - g_arBest[(j+1) % nNumOfData]->m_pt;
				nDifCost = GAPCALC(pt1) + GAPCALC(pt2) + GAPCALC(pt3) - GAPCALC(pt4) - GAPCALC(pt5) - GAPCALC(pt6);	// �����̑���
				if (nDifCost < -0.0001){
					for (h=i+1; h<=k; h++) g_arTmp.SetAt((h-i-1), g_arBest.GetAt(h % nNumOfData));
					for (h=k+1; h<=j; h++) g_arBest.SetAt((h-k+i) % nNumOfData, g_arBest.GetAt(h % nNumOfData));
					for (h=0; h<k-i; h++) g_arBest.SetAt((j-k+i+1+h) % nNumOfData, g_arTmp.GetAt(k-i-1-h));
					i0 = i + 1;
					break;
				}
				if (k == i+1) continue;
				pt4 =  g_arBest[i % nNumOfData]->m_pt - g_arBest[(k+1) % nNumOfData]->m_pt;
				pt5 =  g_arBest[j % nNumOfData]->m_pt - g_arBest[(i+1) % nNumOfData]->m_pt;
				pt6 =  g_arBest[k % nNumOfData]->m_pt - g_arBest[(j+1) % nNumOfData]->m_pt;
				nDifCost = GAPCALC(pt1) + GAPCALC(pt2) + GAPCALC(pt3) - GAPCALC(pt4) - GAPCALC(pt5) - GAPCALC(pt6);	// �����̑���
				if (nDifCost < -0.0001){
					for (h=i+1; h<=k; h++) g_arTmp.SetAt((h-i-1), g_arBest.GetAt(h % nNumOfData));
					for (h=k+1; h<=j; h++) g_arBest.SetAt((h-k+i) % nNumOfData, g_arBest.GetAt(h % nNumOfData));
					for (h=0; h<k-i; h++) g_arBest.SetAt((j-k) % nNumOfData, g_arTmp.GetAt(k-i-1-h));
					i0 = i + 1;
					break;
				}
			}
//			if (nDifCost < -0.0001){
//				break;
//			}
		}
	}
	
	nTotalCost = GetCost(nNumOfData);
	strTmp.Format("nTotalCost�iOr-opt�Z�@�̍œK���j = %f", nTotalCost);
	AfxMessageBox(strTmp);
*/

	// ���ꂼ����ړ�ڲԂłȂ���悤���ް���o�^
	dxfData.dwSize = sizeof(DXFDATA);
	dxfData.ptS.x = g_arBest[0]->m_pt.x;
	dxfData.ptS.y = g_arBest[0]->m_pt.y;

	// �Q�ڈȍ~����̈ړ��f�[�^�Ƃ��ēo�^
	nDataCnt = g_arBest.GetSize();
	dxfData.enType = DXFLINEDATA;
	lstrcpy(dxfData.szLayer, "TSP_MOVE");
	dxfData.nLayer = DXFMOVLAYER;

	for ( i=1; i<nDataCnt; i++ ) {
		dxfData.de.ptE.x = g_arBest[i]->m_pt.x;
		dxfData.de.ptE.y = g_arBest[i]->m_pt.y;
		// �ړ����C���̐��f�[�^�ǉ�
		NCVC_AddDXFData(hDoc, &dxfData);
		// ���̏I�_�͎��̎n�_
		dxfData.ptS.x = dxfData.de.ptE.x;
		dxfData.ptS.y = dxfData.de.ptE.y;
	}

	// ����H�ɂȂ�悤��
	dxfData.de.ptE.x = g_arBest[0]->m_pt.x;
	dxfData.de.ptE.y = g_arBest[0]->m_pt.y;
	NCVC_AddDXFData(hDoc, &dxfData);

	// ��ؗ̈���
	ClearArray();
	if ( lpptOrg )
		delete lpptOrg;

	// �{�̂̍ĕ`��
	NCVC_ReDraw(hDoc);
}

// ��Čv�Z�֐�
double GetCost(int nNumOfData)
{
	int			i;
	CPointD		pt;
	double		nTotalCost = 0;

	for ( i=0; i<nNumOfData-1; i++){
		pt = g_arBest[i+1]->m_pt - g_arBest[i]->m_pt;
		nTotalCost += GAPCALC(pt);
	}
	// �I�_���猴�_�܂ł̺�Ă��ǉ�
	pt = g_arBest[0]->m_pt - g_arBest[nNumOfData-1]->m_pt;
	nTotalCost += GAPCALC(pt);

	return nTotalCost;
}
//
void ClearArray(void)
{
	for ( int i=0; i<g_arBest.GetSize(); i++ ) {
		delete	g_arBest[i];
	}
	g_arPoint.RemoveAll();
	g_arTmp.RemoveAll();
	g_arBest.RemoveAll();
}
