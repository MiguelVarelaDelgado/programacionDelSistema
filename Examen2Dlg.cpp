// Examen2Dlg.cpp: archivo de implementación
//
#ifdef _MSC_VER
/*
 * we do not want the warnings about the old deprecated and unsecure CRT functions
 * since these examples can be compiled under *nix as well
 */
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pch.h"
#include "framework.h"
#include "Examen2.h"
#include "Examen2Dlg.h"
#include "afxdialogex.h"
#include <string>
#include <sstream>

#include <pcap.h>

using namespace std;
#define LINE_LEN 16
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
int vals[17];
int PaquetesInt[1000][1000];
int PaquetesExa[1000][1000];
double valsD[34];

/*


*/

#ifdef WIN32
#include <tchar.h>

BOOL LoadNpcapDlls()
{
	_TCHAR npcap_dir[512];
	UINT len;
	len = GetSystemDirectory(npcap_dir, 480);
	if (!len) {
		fprintf(stderr, "Error in GetSystemDirectory: %x", GetLastError());
		return FALSE;
	}
	_tcscat_s(npcap_dir, 512, _T("\\Npcap"));
	if (SetDllDirectory(npcap_dir) == 0) {
		fprintf(stderr, "Error in SetDllDirectory: %x", GetLastError());
		return FALSE;
	}
	return TRUE;
}
#endif





// Cuadro de diálogo CAboutDlg utilizado para el comando Acerca de

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Datos del cuadro de diálogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV

// Implementación
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchangedList2(NMHDR* pNMHDR, LRESULT* pResult);
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	
END_MESSAGE_MAP()


// Cuadro de diálogo de CExamen2Dlg



CExamen2Dlg::CExamen2Dlg(CWnd* pParent /*= nullptr*/)
	: CDialogEx(IDD_EXAMEN2_DIALOG, pParent)
	, Panel1(_T(""))
	, Panel2(_T(""))
	
	, PanelChar3(_T(""))
	, Panel4(_T(""))
	, Panel5(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExamen2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, Interfaz);
	DDX_Text(pDX, IDC_EDIT1, Panel1);
	DDX_Text(pDX, IDC_EDIT2, Panel2);
	DDX_Control(pDX, IDC_LIST2, Lista);
	DDX_Text(pDX, IDC_EDIT_PANEL_CHAR, PanelChar3);
	DDX_Text(pDX, IDC_EDIT_ENTERO_4, Panel4);
	DDX_Text(pDX, IDC_EDIT_BINARIO, Panel5);
}

BEGIN_MESSAGE_MAP(CExamen2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST2, &CExamen2Dlg::OnCustomDrawList)
	ON_BN_CLICKED(IDC_BUTTON1, &CExamen2Dlg::OnBnClickedButton1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &CExamen2Dlg::OnLvnItemchangedList2)
	
	//ON_BN_CLICKED(IDC_IMAGEN2, &CExamen2Dlg::OnBnClickedImagen2)
	ON_BN_CLICKED(IDC_BUTTON3, &CExamen2Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CExamen2Dlg::OnBnClickedButton2)
END_MESSAGE_MAP()



BOOL CExamen2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	////// Cabecera de lista
	Lista.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	Lista.InsertColumn(0, _T("#"), LVA_ALIGNLEFT, 30);
	Lista.InsertColumn(1, _T("IP ORIGEN"), LVA_ALIGNLEFT, 100);
	Lista.InsertColumn(2, _T("IP DESTINO"), LVA_ALIGNLEFT, 100);
	Lista.InsertColumn(3, _T("PUERTO"), LVA_ALIGNLEFT, 100);
	Lista.InsertColumn(4, _T("PROTOCOLO"), LVA_ALIGNLEFT, 100);
	
	/*
	Lista.InsertColumn(1, _T("TIEMPO"), LVA_ALIGNLEFT, 100);
	Lista.InsertColumn(2, _T("TAMANO"), LVA_ALIGNLEFT, 100);
	*/
	


	// Agregar el elemento de menú "Acerca de..." al menú del sistema.

	// IDM_ABOUTBOX debe estar en el intervalo de comandos del sistema.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// Establecer el icono para este cuadro de diálogo.  El marco de trabajo realiza esta operación
	//  automáticamente cuando la ventana principal de la aplicación no es un cuadro de diálogo
	SetIcon(m_hIcon, TRUE);			// Establecer icono grande
	SetIcon(m_hIcon, FALSE);		// Establecer icono pequeño

	// TODO: agregar aquí inicialización adicional



	printf("\nNo adapter selected: printing the device list:\n");
	/* The user didn't provide a packet source: Retrieve the local device list */
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		fprintf(stderr, "Error in pcap_findalldevs_ex: %s\n", errbuf);
		exit(1);
	}
	int hi = 1;
	for (d = alldevs; d; d = d->next) {
		CString str;
		str.Format(_T("%d "), hi++);
		CString str2(CA2W(d->description, CP_UTF8));
		//TRACE(str + str2);
		Interfaz.AddString(str + str2);
	}



	







	return TRUE;  // Devuelve TRUE  a menos que establezca el foco en un control
}

void CExamen2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// Si agrega un botón Minimizar al cuadro de diálogo, necesitará el siguiente código
//  para dibujar el icono.  Para aplicaciones MFC que utilicen el modelo de documentos y vistas,
//  esta operación la realiza automáticamente el marco de trabajo.

void CExamen2Dlg::OnPaint()
{	///////////////// COLOR
	Lista.SetTextBkColor(RGB(0,0,0));
	Lista.SetBkColor(RGB(0,0,0));
	Invalidate(true);
	CPaintDC dc(this); // Contexto de dispositivo para dibujo
	CRect rect;


	GetClientRect(&rect);
	
	CBrush myb;
	myb.CreateSolidBrush(RGB(0,0,0));
	//myb.CreateSolidBrush(RGB(73, 106, 92));
	dc.FillRect(&rect, &myb);
	if (IsIconic())
	{
		CPaintDC dc(this); // Contexto de dispositivo para dibujo

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Centrar icono en el rectángulo de cliente
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Dibujar el icono
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();

	}

}

// El sistema llama a esta función para obtener el cursor que se muestra mientras el usuario arrastra
//  la ventana minimizada.
HCURSOR CExamen2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CExamen2Dlg::EnExa() {
	
	
	//CString hola;

	int tam;
	if (header->len > 64)
	{
		pkt_data2 = pkt_data + 64;
	}
	for (i = 0; (i < header->caplen); i++)
	{
		PaquetesExa[contador][i] = pkt_data[i];

		
		
	}
}


void CExamen2Dlg::EnInt(){
	int cont = 0;
	int* arr = (int*)pkt_data;
	for (i = 0; ((i < (header->caplen + 1) / 4)); i++)
	{
 		int profe = arr[i];
		u_char arr4[4];
		arr4[0] = pkt_data[i * 4 + 3];
		arr4[1] = pkt_data[i * 4 + 2];
		arr4[2] = pkt_data[i * 4 + 1];
		arr4[3] = pkt_data[i * 4];
		int* arr5 = (int*)arr4;
		int entero = arr5[0];
		vals[cont] = entero;
		PaquetesInt[contador][i] = vals[i];
		cont++;
	}

}


int CExamen2Dlg::desempaquetar(CExamen2Dlg *dlg1) {
	inum = Interfaz.GetCurSel();
	/* Jump to the selected adapter */
	for (d = alldevs, i = 0; i < inum - 1; d = d->next, i++);

	/* Open the adapter */
	if ((fp = pcap_open_live(d->name,	// name of the device
		65536,							// portion of the packet to capture. 
										// 65536 grants that the whole packet will be captured on all the MACs.
		1,								// promiscuous mode (nonzero means promiscuous)
		1000,							// read timeout
		errbuf							// error buffer
	)) == NULL)
	{
		fprintf(stderr, "\nError opening adapter\n");
		//return -1;
	}
	/* Read the packets */
	CString hola;

	hola.Format(_T("inum= %d"), inum);
	
	while ((res = pcap_next_ex(fp, &header, &pkt_data)) >= 0)
	{
			
			EnInt();
			EnExa();
			
			
			int hi = (PaquetesInt[contador][5]) & 0xff;
			CString Proto;
			if (hi == 6 || hi == 1 || hi == 2 || hi == 9 || hi == 17 || hi == 47 || hi == 50 || hi == 51 || hi == 57)
			{
				if (hi == 6) {
					Proto = "UTF";
				}
				else if (hi == 1) {
					Proto = "ICMP";
				}
				else if (hi == 2) {
					Proto = "IGMP";
				}
				else if (hi == 9) {
					Proto = "TCP";
				}
				else if (hi == 17) {
					Proto = "UDP";
				}
				else if (hi == 47) {
					Proto = "GRE";
				}
				else if (hi == 50) {
					Proto = "ESP";
				}
				else if (hi == 51) {
					Proto = "ah";
				}
				else if (hi == 57) {
					Proto = "skip";
				}

			}
			else
			{
				Proto.Format(_T("%d"), hi);
			}


			/// IP ORIGEN EN LA LISTA
			
			int ip2 = ((PaquetesInt[contador][6]) & 0xff);
			int ip1 = ((PaquetesInt[contador][6]) >> 8 & 0xff);
			int ip4 = ((PaquetesInt[contador][7]) >> 16 & 0xff);
			int ip3 = ((PaquetesInt[contador][7]) >> 24 & 0xff);
			CString IpOrigen;
			IpOrigen.Format(_T("%d.%d.%d.%d"), ip1, ip2, ip3, ip4);

			/// IP DESTINO EN LA LISTA
			
			ip2 = ((PaquetesInt[contador][7]) & 0xff);
			ip1 = ((PaquetesInt[contador][7]) >> 8 & 0xff);
			ip4 = ((PaquetesInt[contador][8]) >> 16 & 0xff);
			ip3 = ((PaquetesInt[contador][8]) >> 24 & 0xff);
			CString IpDestino;
			IpDestino.Format(_T("%d.%d.%d.%d"), ip1, ip2, ip3, ip4);

			/// PUERTO EN LA LISTA
			
			int Puer = ((PaquetesInt[contador][8]) & 0xffff);
			CString Puerto;
			Puerto.Format(_T("%d"), Puer);


			CString count;
			count.Format(_T("%d"), contador);
			int Items = Lista.InsertItem(0, count);
			Lista.SetItemText(Items, 1, IpOrigen);//
			Lista.SetItemText(Items, 2, IpDestino);//
			Lista.SetItemText(Items, 3, Puerto);///
			Lista.SetItemText(Items, 4, Proto);

			contador++;
			if (terminarHilo == 1)
			{
				AfxEndThread(0);
			}
			

		}
	//}
		pcap_close(fp);
		return 0;
}

UINT Captura(LPVOID pParam) {
	CExamen2Dlg* pObject = (CExamen2Dlg*)pParam;

	if (pObject == NULL ||
		!pObject->IsKindOf(RUNTIME_CLASS(CExamen2Dlg)))
		return 1;
	pObject->desempaquetar(pObject);

	return 0;
}

void CExamen2Dlg::OnBnClickedButton1()
{
	terminarHilo = 0;	
	AfxBeginThread(Captura, this);	
}

void CExamen2Dlg::OnLvnItemchangedList2(NMHDR* pNMHDR, LRESULT* pResult)
{	
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	 int indi=Lista.GetNextItem(-1,LVNI_SELECTED);
	CString rowstr = Lista.GetItemText(indi, 0);
	int row = _ttoi(rowstr);
	UpdateData(true);
	Panel1 = _T(" ");
	Panel2 = _T(" ");
	PanelChar3 = _T(" ");
	Panel4 = _T(" ");
	Panel5 = _T(" ");
	for (int i = 0; i < 17; i++)
	{	
		CString hola; 
		hola.Format(_T(" %u\n"), PaquetesInt[row][i]);

		Panel4 += hola;
	}
	for (int i = 0; i < 1000; i++)
	{
		CString hola;
		CString hola2;
		CString hola3;
		
		char p  = PaquetesExa[row][i];
		hola3 += (p & 0x80 ? "1" : "0");
		hola3 += (p & 0x40 ? "1" : "0");
		hola3 += (p & 0x20 ? "1" : "0");
		hola3 += (p & 0x10 ? "1" : "0");
		hola3 += (p & 0x08 ? "1" : "0");
		hola3 += (p & 0x04 ? "1" : "0");
		hola3 += (p & 0x02 ? "1" : "0");
		Panel5 += hola3;
		hola.Format(_T(" %.2x"), PaquetesExa[row][i]);
		//TRACE(_T("INT \n%d"),PaquetesExa[row][i]);
		Panel2 += hola;
		hola2.Format(_T("%c"), PaquetesExa[row][i]);
		PanelChar3 += hola2;
		//hola3.Format(_T("%c%c%c%c%c%c%c%c"), PaquetesExa[row][i]);
	}


			int hi = (PaquetesInt[row][3] >> 12) & 0xf;
			CString wi;
			wi.Format(_T("Version:\n %d \n"), hi);
			Panel1 = wi;
			Panel1 += ("\r\n");
			hi = (PaquetesInt[row][3] >> 8) & 0xf;
			wi.Format(_T("Header Length: %d Bits\n"), hi * 4); //printf(" Bits");
			Panel1 += wi;
			Panel1 += ("\r\n");
			hi = ((PaquetesInt[row][3]) & ((1 << (8))) - 1);
			wi.Format(_T("\nDifferentiatedServices Field: 0x%x"), hi);
			Panel1 += wi;
			Panel1 += ("\r\n");
			hi = (PaquetesInt[row][4] >> 16) & 0xff;
			wi.Format(_T("\nTotal lenght: %d"), hi);
			Panel1 += wi;
			Panel1 += ("\r\n");
			hi = ((PaquetesInt[row][4]) & ((1 << (16))) - 1);
			wi.Format(_T("\nDifferentiatedServices Field: 0x%x"), hi);
			Panel1 += wi;
			Panel1 += ("\r\n");
			hi = ((PaquetesInt[row][5]) >> 16 & ((1 << (16))) - 1);
			wi.Format(_T("\nFlags: 0x%x Don't fragment"), hi); //printf(" Don't fragment ");
			Panel1 += wi;
			Panel1 += ("\r\n");
			hi = (PaquetesInt[row][5] >> 16) & 0xfff;
			wi.Format(_T("\nFragment offset: %x"), hi);
			Panel1 += wi;
			Panel1 += ("\r\n");
			hi = (PaquetesInt[row][5]) & 0xff;
			CString Proto;
			


			if (hi == 6 || hi == 1 || hi == 2 || hi == 9 || hi == 17 || hi == 47 || hi == 50 || hi == 51 || hi == 57)
			{
				if (hi == 6) {
					wi.Format(_T("\nProtoco: UTF 6"));
					Panel1 += wi;
					Panel1 += ("\r\n");
					Proto = "UTF 6";
				}
				else if (hi == 1) {
					wi.Format(_T("\nProtocol: ICMP 1 "));
					Panel1 += wi;
					Panel1 += ("\r\n");
					Proto = "ICMP 1";
				}
				else if (hi == 2) {
					wi.Format(_T("\nProtocol: IGMP 2"));
					Panel1 += wi;
					Panel1 += ("\r\n");
					Proto = "IGMP 2";
				}
				else if (hi == 9) {
					wi.Format(_T("\nProtocol: TCP 9"));
					Panel1 += wi;
					Panel1 += ("\r\n");
					Proto = "TCP 9";
				}
				else if (hi == 17) {
					wi.Format(_T("\nProtocol: UDP 17"));
					Panel1 += wi;
					Panel1 += ("\r\n");
					Proto = "UDP 17";
				}
				else if (hi == 47) {
					wi.Format(_T("\nProtocol: GRE 47"));
					Panel1 += wi;
					Panel1 += ("\r\n");
					Proto = "GRE 47";
				}
				else if (hi == 50) {
					wi.Format(_T("\nProtocol: ESP 50"));
					Panel1 += wi;
					Panel1 += ("\r\n");
					Proto = "ESP 50";
				}
				else if (hi == 51) {
					wi.Format(_T("\nProtocol: ah 51"));
					Panel1 += wi;
					Panel1 += ("\r\n");
					Proto = "ah 51";
				}
				else if (hi == 57) {
					wi.Format(_T("\nProtocol: skip 57"));
					Panel1 += wi;
					Panel1 += ("\r\n");
					Proto = "skip 57";
				}

			}
			else
			{
				wi.Format(_T("Protocol:\n %d \n"), hi);
				Proto.Format(_T("%d"), hi);
				Panel1 += wi;
				Panel1 += ("\r\n");
			}

			hi = ((PaquetesInt[row][6]) >> 16 & 0xffff);
			wi.Format(_T("\nHeader checksum: 0x%x"), hi);
			Panel1 += wi;
			Panel1 += ("\r\n");
			////// SOURCE
			int ip2 = ((PaquetesInt[row][6]) & 0xff);
			int ip1 = ((PaquetesInt[row][6]) >> 8 & 0xff);
			int ip4 = ((PaquetesInt[row][7]) >> 16 & 0xff);
			int ip3 = ((PaquetesInt[row][7]) >> 24 & 0xff);
			wi.Format(_T("\nIP Source: %d.%d.%d.%d"), ip1, ip2, ip3, ip4);
			CString IpOrigen;
			IpOrigen.Format(_T("%d.%d.%d.%d"), ip1, ip2, ip3, ip4);
			Panel1 += wi;
			Panel1 += ("\r\n");
			////// DESTINIATION
			ip2 = ((PaquetesInt[row][7]) & 0xff);
			ip1 = ((PaquetesInt[row][7]) >> 8 & 0xff);
			ip4 = ((PaquetesInt[row][8]) >> 16 & 0xff);
			ip3 = ((PaquetesInt[row][8]) >> 24 & 0xff);

			wi.Format(_T("\nIP Destination: %d.%d.%d.%d"), ip1, ip2, ip3, ip4);
			Panel1 += wi;
			//string ipv4D = to_string( ip1) + "." + to_string(ip2) + "." + to_string(ip3) + "." + to_string(ip4);   //+ "." + ip3 + "." + ip4;
			CString IpDestino;
			IpDestino.Format(_T("%d.%d.%d.%d"), ip1, ip2, ip3, ip4);
			Panel1 += ("\r\n");
			hi = ((PaquetesInt[row][8]) & 0xffff);
			wi.Format(_T("\nSource Port1: %d"), hi);
			Panel1 += wi;
			CString Puerto;
			Puerto.Format(_T("%d"), hi);
			Panel1 += ("\r\n");
			int go = PaquetesInt[row][9] << 16 | PaquetesInt[row][10] >> 16;
			wi.Format(_T("\n Sequencenumber (raw): %d"), go);
			Panel1 += wi;
			Panel1 += ("\r\n");
			int  go2 = PaquetesInt[row][10] << 16 | (PaquetesInt[row][11] >> 16 & 0xffff);
			wi.Format(_T("\n Acknowledgment number (raw): %x "), go2);
			Panel1 += wi;
			Panel1 += ("\r\n");
	UpdateData(false);
	//TRACE(_T("\n\n lolololol %d"),row);
	// TODO: Agregue aquí su código de controlador de notificación de control
	*pResult = 0;
}


/*void CAboutDlg::OnLvnItemchangedList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Agregue aquí su código de controlador de notificación de control
	*pResult = 0;
}*/

/*
void CExamen2Dlg::OnBnClickedImagen2()
{
	//TRACE(_T("hola entré"));
	// TODO: Agregue aquí su código de controlador de notificación de control
	// TODO: Agregue aquí su código de controlador de notificación de control
	CImage cImg;
	CBitmap cBmp;

	cImg.Load(_T("C:\\Users\\migue\\Desktop\\ipv4.png"));
	cBmp.Attach(cImg.Detach());

	//IMGPrueba.SetBitmap((HBITMAP)cBmp);
}*/

/*void CMFCMultithreadingDlg::OnBnClickedButtonStop() {
   // TODO: Add your control notification handler code here
   stopNow = TRUE;
   KillTimer(1234);
   m_ctrlStatus.SetWindowText(L"Stopped");
}*/

void CExamen2Dlg::OnBnClickedButton3()
{
	// TODO: Agregue aquí su código de controlador de notificación de control

	terminarHilo = 1;

}



// Controladores de mensajes de CExamen2Dlg
HBRUSH CExamen2Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{

	CBrush brush(RGB(0, 255, 0));
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	CFont* pPrevFont;

	switch (nCtlColor)
	{
		// process my edit controls by ID.
	case CTLCOLOR_EDIT:
	case CTLCOLOR_MSGBOX:
	case CTLCOLOR_STATIC:
		switch (pWnd->GetDlgCtrlID())
		{
			// first CEdit control ID
		case IDC_STATIC:

			//pWnd->SetFont(&m_Font, TRUE);
			pPrevFont = pDC->SelectObject(&m_Font);
			// put your own CONTROL ID here
			pDC->SetBkMode(TRANSPARENT);  // make text
			//pDC->SetBkMode(OPAQUE);  // make text
									 // background transparent
			pDC->SetTextColor(RGB(108, 236, 108));
			pDC->SetBkColor(RGB(100, 200, 30));
			pDC->SetBkColor(LTGRAY_BRUSH);



			//AfxMessageBox(TEXT("Hello"));
			// change the text color to red.
			//hbr = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
			//hbr = brush;
			hbr = CreateSolidBrush(RGB(0, 0, 0));
			//hbr = CreateSolidBrush(RGB(73, 106, 92));

			// apply a null brush, so control's rectangle
			// isn't filled.
			//hbr = brush;
			break;
		case IDC_EDIT2://////////// Exa

			//pWnd->SetFont(&m_Font, TRUE);
			pPrevFont = pDC->SelectObject(&m_Font);
			// put your own CONTROL ID here
			pDC->SetBkMode(TRANSPARENT);  // make text
			//pDC->SetBkMode(OPAQUE);  // make text
									 // background transparent
			pDC->SetTextColor(RGB(108, 236, 108));
			pDC->SetBkColor(RGB(73, 106, 92));
			pDC->SetBkColor(LTGRAY_BRUSH);



			//AfxMessageBox(TEXT("Hello"));
			// change the text color to red.
			//hbr = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
			//hbr = brush;
			hbr = CreateSolidBrush(RGB(0,0,0));
			//hbr = CreateSolidBrush(RGB(73, 106, 92));

			// apply a null brush, so control's rectangle
			// isn't filled.
			//hbr = brush;
			break;

		case IDC_EDIT1://////////// Exa

			//pWnd->SetFont(&m_Font, TRUE);
			pPrevFont = pDC->SelectObject(&m_Font);
			// put your own CONTROL ID here
			pDC->SetBkMode(TRANSPARENT);  // make text
			//pDC->SetBkMode(OPAQUE);  // make text
									 // background transparent
			pDC->SetTextColor(RGB(108, 236, 108));
			pDC->SetBkColor(RGB(73, 106, 92));
			pDC->SetBkColor(LTGRAY_BRUSH);



			//AfxMessageBox(TEXT("Hello"));
			// change the text color to red.
			//hbr = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
			//hbr = brush;
			hbr = CreateSolidBrush(RGB(0, 0, 0));
			//hbr = CreateSolidBrush(RGB(73, 106, 92));

			// apply a null brush, so control's rectangle
			// isn't filled.
			//hbr = brush;
			break;
		case IDC_EDIT_PANEL_CHAR://////////// Exa

			//pWnd->SetFont(&m_Font, TRUE);
			pPrevFont = pDC->SelectObject(&m_Font);
			// put your own CONTROL ID here
			pDC->SetBkMode(TRANSPARENT);  // make text
			//pDC->SetBkMode(OPAQUE);  // make text
									 // background transparent
			pDC->SetTextColor(RGB(108, 236, 108));
			pDC->SetBkColor(RGB(73, 106, 92));
			pDC->SetBkColor(LTGRAY_BRUSH);



			//AfxMessageBox(TEXT("Hello"));
			// change the text color to red.
			//hbr = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
			//hbr = brush;
			hbr = CreateSolidBrush(RGB(0, 0, 0));
			//hbr = CreateSolidBrush(RGB(73, 106, 92));

			// apply a null brush, so control's rectangle
			// isn't filled.
			//hbr = brush;
			break;
		case IDC_EDIT_ENTERO_4://////////// Exa

			//pWnd->SetFont(&m_Font, TRUE);
			pPrevFont = pDC->SelectObject(&m_Font);
			// put your own CONTROL ID here
			pDC->SetBkMode(TRANSPARENT);  // make text
			//pDC->SetBkMode(OPAQUE);  // make text
									 // background transparent
			pDC->SetTextColor(RGB(108, 236, 108));
			pDC->SetBkColor(RGB(73, 106, 92));
			pDC->SetBkColor(LTGRAY_BRUSH);



			//AfxMessageBox(TEXT("Hello"));
			// change the text color to red.
			//hbr = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
			//hbr = brush;
			hbr = CreateSolidBrush(RGB(0, 0, 0));
			//hbr = CreateSolidBrush(RGB(73, 106, 92));

			// apply a null brush, so control's rectangle
			// isn't filled.
			//hbr = brush;
			break;
		case IDC_EDIT_BINARIO://////////// Exa

			//pWnd->SetFont(&m_Font, TRUE);
			pPrevFont = pDC->SelectObject(&m_Font);
			// put your own CONTROL ID here
			pDC->SetBkMode(TRANSPARENT);  // make text
			//pDC->SetBkMode(OPAQUE);  // make text
									 // background transparent
			pDC->SetTextColor(RGB(108, 236, 108));
			pDC->SetBkColor(RGB(73, 106, 92));
			pDC->SetBkColor(LTGRAY_BRUSH);



			//AfxMessageBox(TEXT("Hello"));
			// change the text color to red.
			//hbr = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
			//hbr = brush;
			hbr = CreateSolidBrush(RGB(0, 0, 0));
			//hbr = CreateSolidBrush(RGB(73, 106, 92));

			// apply a null brush, so control's rectangle
			// isn't filled.
			//hbr = brush;
			break;
			// otherwise, do default handling of OnCtlColor
		default:

			hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		}

		return hbr;    // return brush


	}
}

void CExamen2Dlg::OnCustomDrawList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLV = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	int rowIndex = static_cast<int>(pLV->nmcd.dwItemSpec);
	*pResult = 0;

	TRACE("OndrawList\n");

	switch (pLV->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT:
		if (rowIndex % 2 == 0) {
			pLV->clrTextBk = RGB(0,0,0);
			//pLV->clrTextBk = RGB(73, 106, 92);
			pLV->clrText = RGB(108, 236, 108);
		}
		else {
			pLV->clrTextBk = RGB(0,0,0);
			//pLV->clrTextBk = RGB(73, 106, 92);
			pLV->clrText = RGB(108, 236, 108);
		}

		*pResult = CDRF_DODEFAULT;
		break;
	}
}

void CExamen2Dlg::OnBnClickedButton2()
{
	valsD[34]=0;
	contador = 1;
	Lista.DeleteAllItems();
	UpdateData(true);
	Panel1 = _T(" ");
	Panel2 = _T(" ");
	PanelChar3 = _T(" ");
	Panel4 = _T(" ");
	Panel5 = _T(" ");
	UpdateData(false);

}
