
// FlowTableNaiveModelDlg.h: 头文件
//

#pragma once

#include "PacketLookup.h"

// CFlowTableNaiveModelDlg 对话框
class CFlowTableNaiveModelDlg : public CDialog
{
// 构造
public:
	CFlowTableNaiveModelDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FLOWTABLENAIVEMODEL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedFileSelector();
	afx_msg void OnEnChangeMessageArea();
	afx_msg void OnEnUpdateMessageArea();

protected:
	UserConfig m_configInfo;
public:
	afx_msg void OnBnClickedResultSave();
};
