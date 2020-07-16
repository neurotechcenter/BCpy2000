// MFCdemoDlg.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "MFCdemo.h"
#include "MFCdemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMFCdemoDlg Dialogfeld

CMFCdemoDlg::CMFCdemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMFCdemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMFCdemoDlg)
		// HINWEIS: Der Klassenassistent fügt hier Member-Initialisierung ein
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCdemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMFCdemoDlg)
		// HINWEIS: Der Klassenassistent fügt an dieser Stelle DDX- und DDV-Aufrufe ein
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMFCdemoDlg, CDialog)
	//{{AFX_MSG_MAP(CMFCdemoDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// CMFCdemoDlg Nachrichten-Handler

BOOL CMFCdemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Großes Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden
	
	// ZU ERLEDIGEN: Hier zusätzliche Initialisierung einfügen
  mCursorIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
  mCursorX = 0.5;
  mCursorY = 0.5;
	
	return TRUE;  // Geben Sie TRUE zurück, außer ein Steuerelement soll den Fokus erhalten
}

// Wollen Sie Ihrem Dialogfeld eine Schaltfläche "Minimieren" hinzufügen, benötigen Sie 
//  den nachstehenden Code, um das Symbol zu zeichnen. Für MFC-Anwendungen, die das 
//  Dokument/Ansicht-Modell verwenden, wird dies automatisch für Sie erledigt.

void CMFCdemoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Gerätekontext für Zeichnen

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Symbol in Client-Rechteck zentrieren
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Symbol zeichnen
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		// Draw the BCI cursor icon.
		CPaintDC dc( this );
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect( &rect );
		int xPos = static_cast<int>( rect.left + rect.Width() * mCursorX - cxIcon / 2 ),
			yPos = static_cast<int>( rect.top + rect.Height() * mCursorY - cyIcon / 2 );
		dc.DrawIcon( xPos, yPos, mCursorIcon );
		CDialog::OnPaint();
	}
}

HCURSOR CMFCdemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
