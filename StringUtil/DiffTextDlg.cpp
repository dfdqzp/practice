
// DiffTextDlg.cpp : implementation file
//

#include "DiffText.h"
#include "DiffTextDlg.h"
#include "afxdialogex.h"
#include <math.h>
#include <map>
#include "util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

struct ll_compare
{
	bool operator() (const long long & v1, const long long & v2) const
	{
		return v1 - v2 > 0;
	}
};

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDiffTextDlg dialog




CDiffTextDlg::CDiffTextDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDiffTextDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDiffTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BT_SRC, m_bt_srcfile);
	DDX_Control(pDX, IDC_BT_DES, m_bt_desfile);
	DDX_Control(pDX, IDC_EDIT_SPLITSTRINGS, m_edit_splitchars);
	DDX_Control(pDX, IDOK, m_BT_Diff);
}

BEGIN_MESSAGE_MAP(CDiffTextDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CDiffTextDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BT_SRC, &CDiffTextDlg::OnBnClickedBtSrc)
	ON_BN_CLICKED(IDC_BT_DES, &CDiffTextDlg::OnBnClickedBtDes)
END_MESSAGE_MAP()


// CDiffTextDlg message handlers

BOOL CDiffTextDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_splitString.push_back(",");
	m_splitString.push_back(" ");
	m_splitString.push_back("#");
	m_splitString.push_back("_");
	m_splitString.push_back("+");
	m_splitString.push_back("-");
	
	m_edit_splitchars.SetWindowText(",space # _ + -");
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDiffTextDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDiffTextDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDiffTextDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDiffTextDlg::OnBnClickedOk()
{
	// add customized split char
	m_splitString.clear();
	CString split_chars;
	m_edit_splitchars.GetWindowText(split_chars);
	char* splits = split_chars.GetBuffer();
	string split_tmp;
	for(int i = 0; i < split_chars.GetLength(); i++)
	{
		split_tmp = splits[i];
		m_splitString.push_back(split_tmp);
	}
	// strings in to be diff file
	std::map<long long, string, ll_compare> vecSrc;
	std::map<long long, string, ll_compare> vecDes;
	
	// store diff result
	ofstream fResult;
	fResult.open("Diff.txt", ofstream::out);

	ifstream fileSrc, fileDes;
	fileSrc.open(m_file_src.c_str(), ifstream::in);
	fileDes.open(m_file_des.c_str(), ifstream::in);

	int idx = 0;
	fResult << "+-----------------------------------------------------+" 
		    << "\n+\tDuplicate item in " << m_file_src;
	while(!fileSrc.eof())
	{
		string tmp;
		getline(fileSrc, tmp);
		if(tmp.length() == 0)
			continue;

		long long key = StringToHashValue(tmp);
		if(vecSrc.end() != vecSrc.find(key))
		{
			if (0 < idx)
				fResult <<", ";
			if (0 == idx++ % 10)
				fResult <<"\n";

			fResult << tmp;
		}
		else
			vecSrc[key] = tmp;
	}
	fResult << "\n+\tTotal: "<< idx
		    << "\n+-----------------------------------------------------+";
	
	idx = 0;
	fResult << "\n\n\n+-----------------------------------------------------+" 
		    << "\n+\tDuplicate item in " << m_file_des;
	while(!fileDes.eof())
	{
		string tmp;
		getline(fileDes, tmp);
		if(tmp.length() == 0)
			continue;

		long long key = StringToHashValue(tmp);
		if(vecDes.end() != vecDes.find(key))
		{
			if (0 < idx)
				fResult <<", ";
			if (0 == idx++ % 10)
				fResult <<"\n";

			fResult << tmp;
		}
		else
			vecDes[key] = tmp;
	}
	fResult << "\n+\tTotal: "<< idx
		    << "\n+-----------------------------------------------------+";

	// delete same items in two map
	for(auto des = vecDes.begin(); des != vecDes.end(); )
	{
		long long key = des->first;
		des++;
		if(vecSrc.end() != vecSrc.find(key))
		{
			vecSrc.erase(key);
			vecDes.erase(key);
		}
	}

	idx = 0;
	
	fResult << "\n\n\n+-----------------------------------------------------+" 
		    << "\n+\tOnly exist in " << m_file_src
			<< "\n+\tTotal: "<< vecSrc.size()
		    << "\n+-----------------------------------------------------+\n";

	for(auto src = vecSrc.begin(); src != vecSrc.end(); src++)
	{
		if (0 < idx)
			fResult <<", ";
		if (0 == idx++ % 10)
			fResult <<"\n";
		fResult << src->second;
	}

	idx = 0;	
	fResult << "\n\n\n+-----------------------------------------------------+" 
		    << "\n+\tOnly exist in " << m_file_des
			<< "\n+\tTotal: "<< vecDes.size()
		    << "\n+-----------------------------------------------------+\n";

	for(auto des = vecDes.begin(); des != vecDes.end(); des++)
	{
		if (0 < idx)
			fResult <<", ";
		if (0 == idx++ % 10)
			fResult <<"\n";
		fResult << des->second;
	}
	
	fResult.close();

	ShellExecute(NULL,"open","Diff.txt", NULL, NULL, SW_SHOW);
}


void CDiffTextDlg::OnBnClickedBtSrc()
{
	CFileDialog dlg(TRUE);
	if(dlg.DoModal() == IDOK)
	{
		m_file_src = file_sed(dlg.GetPathName().GetBuffer(), m_splitString, "\n");
		m_bt_srcfile.SetWindowText(dlg.GetPathName());
	}

	if(m_file_src.length() > 0 && m_file_des.length() > 0)
		GetDlgItem(IDOK)->EnableWindow(TRUE);

	if(0 == m_file_src.compare(m_file_des))
		MessageBox("You selected the same file with File2.","Warning", MB_ICONEXCLAMATION );

}


void CDiffTextDlg::OnBnClickedBtDes()
{
	CFileDialog dlg(TRUE);
	if(dlg.DoModal() == IDOK)
	{
		m_file_des = file_sed(dlg.GetPathName().GetBuffer(), m_splitString, "\n");
		m_bt_desfile.SetWindowText(dlg.GetPathName());
	}

	if(m_file_src.length() > 0 && m_file_des.length() > 0)
		GetDlgItem(IDOK)->EnableWindow(TRUE);

	if(0 == m_file_src.compare(m_file_des))
		MessageBox("You selected the same file with File1.","Warning", MB_ICONEXCLAMATION );
}

long long StringToHashValue(string & s)
{
	char *start = (char*)s.c_str();
	int idx =  0;
	long long hashValue = -1;
	while(*start)
	{
		hashValue += (*start++) * pow(10.0, idx++);
	}

	return hashValue;
}