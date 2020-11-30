
// Examen2Dlg.h: archivo de encabezado
//

#pragma once
#include <pcap.h>
#include <string>
using namespace std;


// Cuadro de diálogo de CExamen2Dlg
class CExamen2Dlg : public CDialogEx
{
// Construcción
public:
	
	CFont m_Font;
	string baseDatos[100];
	CExamen2Dlg(CWnd* pParent = nullptr);	// Constructor estándar
	pcap_if_t* alldevs, * d;
	pcap_t* fp;
	u_int inum, i = 0;
	int cont = 0;
	char errbuf[PCAP_ERRBUF_SIZE];
	int res;
	struct pcap_pkthdr* header;
	const u_char* pkt_data;
	const u_char* pkt_data2;

	int argc = 0;
	int contador = 1;
	

// Datos del cuadro de diálogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXAMEN2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// Compatibilidad con DDX/DDV
	virtual HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCustomDrawList(NMHDR* pNMHDR, LRESULT* pResult);
// Implementación
protected:
	HICON m_hIcon;

	// Funciones de asignación de mensajes generadas
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	

	
	
	DECLARE_MESSAGE_MAP()
public:
	void EnExa();
	int desempaquetar(CExamen2Dlg *dlg1);
	CComboBox Interfaz;
	CString Panel1;
	CString Panel2;
	
	afx_msg void OnLvnItemchangedList2(NMHDR* pNMHDR, LRESULT* pResult);
		void EnInt();
		//UINT Captura(LPVOID pParam);
	void OnBnClickedButton1();
	
	int prueba;
	CString Pru;
	afx_msg void Seleccionar(NMHDR* pNMHDR, LRESULT* pResult);
	CListCtrl Lista;
	
	afx_msg void OnBnClickedImagen();
	
	//CStatic png;
	afx_msg void OnBnClickedImagen2();
	//CStatic IMGPrueba;
	void OnBnClickedButton3();
	int terminarHilo = 0;
	
	CString PanelChar3;
	CString Panel4;
	CString Panel5;
	afx_msg void OnBnClickedButton2();
};
