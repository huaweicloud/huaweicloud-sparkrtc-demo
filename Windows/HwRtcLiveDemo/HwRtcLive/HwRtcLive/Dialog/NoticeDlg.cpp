// .\Dialog\NoticeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HwRtcLive.h"
#include "NoticeDlg.h"
#include "afxdialogex.h"


// CNoticeDlg �Ի���

IMPLEMENT_DYNAMIC(CNoticeDlg, CDialogEx)

CNoticeDlg::CNoticeDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CNoticeDlg::IDD, pParent) {

}

CNoticeDlg::~CNoticeDlg() {
}

void CNoticeDlg::DoDataExchange(CDataExchange* pDX) {
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_NOTICE_TEXT, m_noticeMsg);
}

void CNoticeDlg::MessageBox(CString sMsg) {
    m_msg = sMsg;
}

BEGIN_MESSAGE_MAP(CNoticeDlg, CDialogEx)
    ON_WM_CREATE()
    ON_BN_CLICKED(IDCANCEL, &CNoticeDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

void CNoticeDlg::OnBnClickedCancel() {
    CDialogEx::OnCancel();
}

BOOL CNoticeDlg::OnInitDialog() {
    CDialogEx::OnInitDialog();
    m_noticeMsg.SetWindowTextW(m_msg);
    return 0;
}
