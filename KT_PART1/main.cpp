//--------------------------------------------------------------------------------------
// File: CustomUI.cpp
//
// Starting point for new Direct3D applications
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "dxstdafx.h"
#include "resource.h"
#include <stdio.h>
#include <atlbase.h>
#include <dshow.h>
#include "D3DHangul.h"
#include "CD3DSprite.h"

//#define DEBUG_VS   // Uncomment this line to debug vertex shaders 
//#define DEBUG_PS   // Uncomment this line to debug pixel shaders 
int ttt = 0;
#define MAX_SCENE 20;
#define MAX_CHARACTER 10;
char g_script[20][MAX_PATH] = { 0 };
char g_name_str[MAX_PATH] = { 0 };
int g_message_length = 0;
int g_selected_extend_message = 0;
char g_message_data[1000][MAX_PATH] = { 0 };
char g_message_data2[1000][MAX_PATH] = { 0 };
void GetText2(float fElapsedTime);
void GetText3(float fElapsedTime);
void InitMenu2();
int g_index = 0;
int g_current_index = 0;
int g_message_count = 0;
int g_selected_char = 0;
int g_act1_auto = 0;
int g_Hero_Scene = 0; // 주인공이 있는 장소 
int g_now_time = 0; //현재의 시각 아이디
int g_KeyBoard_Acess = 0;//키보드 허용 허락 여부
//텍스트 스크립트 처리... 아무리 급하게 만들었다고 하지만 내가 생각해도 너무 형편없다고 생각함
BOOL g_bTextReading = 0;
BOOL g_bProcessKey = 1;
int g_LineRead = 0;
int g_bTextReading_Terminate = 0;
//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
ID3DXFont* g_pFont = NULL;         // Font for drawing text
ID3DXSprite* g_pTextSprite = NULL;   // Sprite for batching draw text calls
ID3DXEffect* g_pEffect = NULL;       // D3DX effect interface
CDXUTMesh               g_Mesh;                 // Background mesh
D3DXMATRIXA16           g_mView;
CModelViewerCamera      g_Camera;               // A model viewing camera
CDXUTDialog             g_HUD;                  // dialog for standard controls
CDXUTDialog             g_SampleUI;             // dialog for sample specific controls
CDXUTDialog             g_GameUI;             // dialog for sample specific controls
CDXUTDialog             g_menu;             // dialog for sample specific controls
IDirect3DDevice9* g_pd3dDevice;
HWND g_Hwnd;
int ACT1_LOC[6] = { 0 };
int g_can_go = 2;
void InitMenu();
void script_analyzer();
void Menu_Extend_Display();
void Text_Parse();
#define IDC_MOVE            30
#define IDC_STAY            31
int g_menu_set = 0;
int g_script_analysis = 0;
int g_count = 0;
int g_extend = -1;
int g_extend_number = -1;
int g_lable = 0;
int g_message_number = 0;
int g_message_extend = 0;
int g_game_state = 0;
BOOL b_Text_Parse = false;
int g_selected_message = 0;
//한글폰트를 이용한 한글출력
CD3DHangul  g_d3dHangul;
CD3DHangul  g_d3dHangul2;
CD3DHangul  g_d3dHangul3;

//--------------------------------------------------------------------------------------
// UI control IDs
//--------------------------------------------------------------------------------------
#define IDC_TOGGLEFULLSCREEN    1
#define IDC_TOGGLEREF           3
#define IDC_CHANGEDEVICE        4
#define IDC_START               5
#define IDC_LOAD                6
#define IDC_EDITBOX1            7
#define IDC_EDITBOX2            8
#define IDC_COMBOBOX            9
#define IDC_STATIC              10
#define IDC_OUTPUT              12
#define IDC_SLIDER              13
#define IDC_CHECKBOX            14
#define IDC_CLEAREDIT           15
#define IDC_RADIO1A             16
#define IDC_RADIO1B             17
#define IDC_RADIO1C             18
#define IDC_RADIO2A             19
#define IDC_RADIO2B             20
#define IDC_RADIO2C             21
#define IDC_LISTBOX             22
#define IDC_LISTBOXM            23

#define IDC_CONFIG              24
#define IDC_EXIT                25
#define WM_GRAPHNOTIFY  WM_USER+13
#define WM_GRAPHNOTIFY2  WM_USER+14

// global game data
// very important, don't crack me...
int qbt = 0, iop = 0;
int g_GameStart = 0;
int g_BgMusic = 0;
float g_width = 640.f;
float g_height = 480.f;
float g_Defaut_Width = 640.0f;
int g_temp2 = 0;
int g_juhang3 = 0;
//implemented for 2d image sprite.. alpha blending fade, and so on
// vertex structure definition, function....

void Create_3DSprite(IDirect3DDevice9* pd3dDevice, LPDIRECT3DTEXTURE9& Test_Texture, LPDIRECT3DVERTEXBUFFER9& Test_Buffer, LPCTSTR lpstr, float x, float y, float width, float height);
void Display(IDirect3DDevice9* pd3dDevice, LPDIRECT3DTEXTURE9& Test_Texture, LPDIRECT3DVERTEXBUFFER9& Test_Buffer, int iUpdate);
void Display2(IDirect3DDevice9* pd3dDevice, LPDIRECT3DTEXTURE9& Test_Texture, LPDIRECT3DVERTEXBUFFER9& Test_Buffer, int iUpdate);

int g_juhang = 0;
int g_juhang2 = 0;
int g_name_counter = 0;
void init_val()
{
	ttt = 0;
	qbt = 0, iop = 0;
	g_index = 0;
	g_KeyBoard_Acess = 0;

	g_current_index = 0;
	g_message_count = 0;
	g_selected_char = 0;
	g_message_length = 0;
	g_selected_extend_message = 0;
	g_bTextReading_Terminate = 0;
	g_menu_set = 0;
	g_juhang3 = 0;
	g_script_analysis = 0;
	g_count = 0;
	g_extend = -1;
	g_extend_number = -1;
	g_lable = 0;
	g_message_number = 0;
	g_message_extend = 0;
	b_Text_Parse = false;
	g_selected_message = 0;
	g_juhang = 0;
	g_juhang2 = 0;
	g_name_counter = 0;
}

LPDIRECT3DVERTEXBUFFER9 g_pVB_Prog = NULL; // Buffer to hold vertices
LPDIRECT3DTEXTURE9      g_pTexture_Prog = NULL; // Our texture


LPDIRECT3DVERTEXBUFFER9 g_pVB_Text_Bar = NULL; // Buffer to hold vertices
LPDIRECT3DTEXTURE9      g_pTexture_Text_Bar = NULL; // Our texture

struct BITMAPVERTEX
{
	BITMAPVERTEX() {}
	BITMAPVERTEX(
		float x, float y, float z, float w,
		float u, float v)
	{
		_x = x;  _y = y;  _z = z; _w = w;
		_u = u;  _v = v;
	}
	float _x, _y, _z, _w;
	float _u, _v; // texture coordinates	
};

LPCTSTR pstrTextureFiles[] =
{
	TEXT("image\\title1.tga"),
		TEXT("image\\opening.jpg"),
		TEXT("image\\text_bar.jpg"),
};


#define D3DFVF_BITMAPVERTEX (D3DFVF_XYZRHW | D3DFVF_TEX1) 


// Character Data
LPCTSTR g_pstrCharater_Name[] =
{
	TEXT("image\\char\\1_Char.tga"),
	TEXT("image\\char\\2_Char.tga"),
	TEXT("image\\char\\3_Char.tga"),
	TEXT("image\\char\\4_Char.tga"),
	TEXT("image\\char\\5_Char.tga"),
	TEXT("image\\char\\6_Char.tga"),
	TEXT("image\\char\\7_Char.tga"),
	TEXT("image\\char\\8_Char.tga"),
	TEXT("image\\char\\9_Char.tga"),
	TEXT("image\\char\\10_Char.tga"),
};
float g_temp = -320.0f;
float g_DownFact = 0.99999999f;
int g_Char_Exchange = 1;
int g_Char_Exchange2 = 1;
int g_Char_Selected_Number = 0;
LPDIRECT3DVERTEXBUFFER9 g_pVB_Character[10] = { NULL }; // Buffer to hold vertices
LPDIRECT3DTEXTURE9     g_pTexture_Character[10] = { NULL }; // Our texture


struct Char_Info
{
	int	Char_ID; // 이름 등에 접근하기 위함
	int location; // 현재 캐릭터가 있는 장소
	int present; // 그 장소에 캐릭터가 보일지의 여부
	int state; // 캐릭터의 심리상태
};


//For Item
struct ItemInfo
{
	int	Item_ID; // 이름 등에 접근하기 위함
	int location; // 현재 아이템이 있는 장소
	int present; // 주인공이 아이템을 취했는지 안했는지의 여부
	int state; // 당장 아이템을 취할 수 있는지의 여부
};
LPCTSTR g_pstrItem_Name[] =
{
	TEXT("image\\item\\Item1.jpg"),
	TEXT("image\\item\\Item2.jpg"),
	TEXT("image\\item\\Item3.tga"),
	TEXT("image\\item\\Item4.tga"),
	TEXT("image\\item\\Item5.tga"),
	TEXT("image\\item\\Item6.tga"),
	TEXT("image\\item\\Item7.tga"),
	TEXT("image\\item\\Item8.tga"),
	TEXT("image\\item\\Item9.tga"),
	TEXT("image\\item\\Item10.tga"),
	TEXT("image\\item\\Item11.tga"),
	TEXT("image\\item\\Item12.tga"),
	TEXT("image\\item\\Item13.tga"),
	TEXT("image\\item\\Item14.tga"),
	TEXT("image\\item\\Item15.tga"),
	TEXT("image\\item\\Item16.tga"),
	TEXT("image\\item\\Item17.tga"),
	TEXT("image\\item\\Item18.tga"),
	TEXT("image\\item\\Item19.tga"),
	TEXT("image\\item\\Item20.tga"),
};
void Show_Inventory_Item(IDirect3DDevice9* pd3dDevice, LPDIRECT3DTEXTURE9& Test_Texture, LPDIRECT3DVERTEXBUFFER9& Test_Buffer, int iUpdate);
BOOL bInventory_Mode = false;
BOOL bCharShow_Mode = false;
int MeetingData[10] = { 0 };
int MeetingDataIndex = 0;
LPDIRECT3DVERTEXBUFFER9 g_pVB_Fade = NULL; // Buffer to hold vertices
LPDIRECT3DTEXTURE9      g_pTexture_Fade = NULL; // Our texture

LPDIRECT3DVERTEXBUFFER9 g_pVB_Item[20] = { NULL }; // Buffer to hold vertices
LPDIRECT3DTEXTURE9     g_pTexture_Item[20] = { NULL }; // Our texture


void Character_Sprite(float fElapsedTime);
//--------------------------------------------------------------------------------------
// Forward declarations 
//--------------------------------------------------------------------------------------
bool    CALLBACK IsDeviceAcceptable(D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed);
void    CALLBACK ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps);
HRESULT CALLBACK OnCreateDevice(IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc);
HRESULT CALLBACK OnResetDevice(IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc);
void    CALLBACK OnFrameMove(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime);
void    CALLBACK OnFrameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime);
LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing);
void    CALLBACK KeyboardProc(UINT nChar, bool bKeyDown, bool bAltDown);
void    CALLBACK OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl);
void    CALLBACK OnLostDevice();
void    CALLBACK OnDestroyDevice();

void    InitApp();
HRESULT LoadMesh(IDirect3DDevice9* pd3dDevice, WCHAR* strFileName, ID3DXMesh** ppMesh);
void    RenderText();

//DirectShow Code
HRESULT GraphInit(void);
HRESULT SwapSourceFilter(void);
HRESULT SwapSourceFilter2(int i);

// DirectShow Graph, Filter & Pins used
IGraphBuilder* g_pGraphBuilder = NULL;
IMediaControl* g_pMediaControl = NULL;
IMediaSeeking* g_pMediaSeeking = NULL;
IBaseFilter* g_pSourceCurrent = NULL;
IBaseFilter* g_pSourceNext = NULL;
IMediaEventEx* g_pME = NULL;

IGraphBuilder* g_pGraphBuilder2 = NULL;
IMediaControl* g_pMediaControl2 = NULL;
IMediaSeeking* g_pMediaSeeking2 = NULL;
IBaseFilter* g_pSourceCurrent2 = NULL;
IBaseFilter* g_pSourceNext2 = NULL;
IMediaEventEx* g_pME2 = NULL;
HRESULT HandleGraphEvent(void);
HRESULT HandleGraphEvent2(void);
int g_effectRunning = 1;
int gPlaying = 0;
TCHAR          g_szCurrentFile[128];
// File names & variables to track current file
// These files are installed with the DirectX SDK into the samples media folder
LPCTSTR pAct1_str[] =
{
	TEXT("이민수의 집"),
	TEXT("민준혁씨의 집"),
	TEXT("노상조씨의 집"),
	TEXT("진한수씨의 집"),
	TEXT("이미나씨"),
	TEXT("마윤진 선배님의 교회"),
};
LPCTSTR pstrFiles[] =
{
	TEXT("music/scene1.mp3\0"),
	TEXT("music/scene2.mp3\0"),
	TEXT("music/scene3.mp3\0"),
	TEXT("music/scene4.mp3\0"),
	TEXT("music/scene4.mp3\0"),
	TEXT("music/scene1.mp3\0"),
};

LPCTSTR pstrEffectFiles[] =
{
	TEXT("music/BLEEP1.WAV\0"),
	TEXT("music/scene1.mp3\0"),
	TEXT("music/scene2.mp3\0"),
	TEXT("music/scene3.mp3\0"),
	TEXT("music/scene4.mp3\0"),
	TEXT("music/scene3.mp3\0"),
	TEXT("music/scene2.mp3\0"),
	TEXT("music/scene1.mp3\0"),
};


int g_iNumFiles = 3, g_iNextFile = 0;




WCHAR g_wFileName[MAX_PATH];
WCHAR g_wFileName2[MAX_PATH];

FILE* g_fp = NULL;
char g_string[MAX_PATH];
char g_string2[MAX_PATH];
void GetFileHandle();
void GetText(float fElapsedTime);

//게임 내용 관련 데이터들, 진행하면서 바뀌는 데이터들 저장 및 상황에 맞는 메시지 출력

struct SceneInfo
{
	int	Scene_ID; // 이름 등에 접근하기 위함 또한 현재의 장면위치 확인 아이디
	int location; // 현재 아이템이 있는 장소
	int present; // 주인공이 현재 이 장면으로 갈 수 있는지의 여부
	int script_no; // 해당스크립트 번호(최초 방문시 나오는 스크립트)
	int conversation_script_no; // 대화할시 질문을 위한 텍스트 정보 파일 인덱스
	int access_script_no; //이 scene을 처음 방문하는지의 여부 체크... 그렇다면 최초 스크립트를 전개한다.
	int item_exist; // 현재 장면내에 존재하는 아이템 <-수정할 것임
	int Character; //현재 장면에 존재하는 사람
	int access;
};
SceneInfo Scene[50] = { 0 };
LPCTSTR g_pstrScene[] =
{
	TEXT("image\\scene\\1_1.jpg"),
	TEXT("image\\scene\\1_2.jpg"),
	TEXT("image\\scene\\1_3.jpg"),
	TEXT("image\\scene\\1_4.jpg"),
	TEXT("image\\scene\\1_5.jpg"),
	TEXT("image\\scene\\1_6.jpg"),
};

LPCTSTR g_pstrScenario[] =
{
	TEXT("scenario\\act1\\Act1.txt"),
	TEXT("scenario\\act1\\person1.txt"),
	TEXT("scenario\\act1\\person2.txt"),
	TEXT("scenario\\act1\\person3.txt"),
	TEXT("scenario\\act1\\person4.txt"),
	TEXT("scenario\\act1\\person5.txt"),
	TEXT("scenario\\act1\\person6.txt"),
};
LPCTSTR g_pstrScript[] =
{
	TEXT("scenario\\act1\\Act1.txt"),
	TEXT("scenario\\act1\\person1_text.txt"),
	TEXT("scenario\\act1\\person2_text.txt"),
	TEXT("scenario\\act1\\person3_text.txt"),
	TEXT("scenario\\act1\\person4_text.txt"),
	TEXT("scenario\\act1\\person5_text.txt"),
};


LPDIRECT3DVERTEXBUFFER9 g_pVB_Scene[20] = { NULL }; // Buffer to hold vertices
LPDIRECT3DTEXTURE9      g_pTexture_Scene[20] = { NULL }; // Our texture


LPCTSTR g_pstrTime[] =
{
	TEXT("이민수의 방"),
	TEXT("민준혁씨의 집"),
	TEXT("노상조씨의 집"),
	TEXT("진한수씨의 집"),
	TEXT("대학가. 이미나"),
	TEXT("마수형 선배님의 교회"),
};


//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// Set the callback functions. These functions allow the sample framework to notify
	// the application about device changes, user input, and windows messages.  The 
	// callbacks are optional so you need only set callbacks for events you're interested 
	// in. However, if you don't handle the device reset/lost callbacks then the sample 
	// framework won't be able to reset your device since the application must first 
	// release all device resources before resetting.  Likewise, if you don't handle the 
	// device created/destroyed callbacks then the sample framework won't be able to 
	// recreate your device resources.
	DXUTSetCallbackDeviceCreated(OnCreateDevice);
	DXUTSetCallbackDeviceReset(OnResetDevice);
	DXUTSetCallbackDeviceLost(OnLostDevice);
	DXUTSetCallbackDeviceDestroyed(OnDestroyDevice);
	DXUTSetCallbackMsgProc(MsgProc);
	DXUTSetCallbackKeyboard(KeyboardProc);
	DXUTSetCallbackFrameRender(OnFrameRender);
	DXUTSetCallbackFrameMove(OnFrameMove);

	// Show the cursor and clip it when in full screen
	DXUTSetCursorSettings(true, true);

	InitApp();

	// Initialize the sample framework and create the desired Win32 window and Direct3D 
	// device for the application. Calling each of these functions is optional, but they
	// allow you to set several options which control the behavior of the framework.
	DXUTInit(true, true, true); // Parse the command line, handle the default hotkeys, and show msgboxes

	DXUTCreateWindow(L"Killing Time 2005 Part 1 - For Adventure The Second Anniversary");
	DXUTCreateDevice(D3DADAPTER_DEFAULT, true, 640, 480, IsDeviceAcceptable, ModifyDeviceSettings);
	GraphInit();
	// Pass control to the sample framework for handling the message pump and 
	// dispatching render calls. The sample framework will call your FrameMove 
	// and FrameRender callback when there is idle time between handling window messages.
	DXUTMainLoop();

	// Perform any application-level cleanup here. Direct3D device resources are released within the
	// appropriate callback functions and therefore don't require any cleanup code here.

	return DXUTGetExitCode();
}


//--------------------------------------------------------------------------------------
// Initialize the app 
//--------------------------------------------------------------------------------------
void InitApp()
{

	// Initialize dialogs
	g_HUD.SetCallback(OnGUIEvent); int iY = 10;
	//g_HUD.AddButton( IDC_TOGGLEFULLSCREEN, L"Toggle full screen", 35, iY, 125, 22 );
	//g_HUD.AddButton( IDC_TOGGLEREF, L"Toggle REF (F3)", 35, iY += 24, 125, 22 );
	//g_HUD.AddButton( IDC_CHANGEDEVICE, L"Change device (F2)", 35, iY += 24, 125, 22 );

	g_SampleUI.SetCallback(OnGUIEvent);
	g_SampleUI.SetFont(1, L"Comic Sans MS", 24, FW_NORMAL);
	g_SampleUI.SetFont(2, L"Courier New", 16, FW_NORMAL);

	// Static
	/*g_SampleUI.AddStatic( IDC_STATIC, L"This is a static control.", 0, 0, 200, 30 );
	g_SampleUI.AddStatic( IDC_OUTPUT, L"한글이 control provides feedback for your action.  It will change as you interact with the UI controls.", 20, 50, 620, 300 );
	g_SampleUI.GetStatic( IDC_OUTPUT )->SetTextColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) ); // Change color to red
	g_SampleUI.GetStatic( IDC_STATIC )->SetTextColor( D3DCOLOR_ARGB( 255, 0, 255, 0 ) ); // Change color to green
	g_SampleUI.GetControl( IDC_OUTPUT )->GetElement( 0 )->dwTextFormat = DT_LEFT|DT_TOP|DT_WORDBREAK;
	g_SampleUI.GetControl( IDC_OUTPUT )->GetElement( 0 )->iFont = 2;
	g_SampleUI.GetControl( IDC_STATIC )->GetElement( 0 )->dwTextFormat = DT_CENTER|DT_VCENTER|DT_WORDBREAK;*/


	// Buttons
	g_SampleUI.AddButton(IDC_START, L"게임시작", 30, 390, 80, 35, L'S');
	g_SampleUI.AddButton(IDC_LOAD, L"로드", 30, 50, 80, 35, L'L');
	g_SampleUI.AddButton(IDC_CONFIG, L"환경설정", 30, 390, 80, 35, L'C');
	g_SampleUI.AddButton(IDC_EXIT, L"끝내기", 30, 50, 80, 35, L'E');


	// Edit box
	/*g_SampleUI.AddEditBox( IDC_EDITBOX1, L"Edit control with default styles. Type text here and press Enter", 20, 440, 600, 32 );

	// IME-enabled edit box
	CDXUTIMEEditBox *pIMEEdit;
	if( SUCCEEDED( g_SampleUI.AddIMEEditBox( IDC_EDITBOX2, L"IME-capable edit control with custom styles. Type and press Enter", 20, 390, 600, 45, false, &pIMEEdit ) ) )
	{
		pIMEEdit->GetElement( 0 )->iFont = 1;
		pIMEEdit->GetElement( 1 )->iFont = 1;
		pIMEEdit->GetElement( 9 )->iFont = 1;
		pIMEEdit->GetElement( 0 )->TextureColor.Init( D3DCOLOR_ARGB( 128, 255, 255, 255 ) );  // Transparent center
		pIMEEdit->SetBorderWidth( 7 );
		pIMEEdit->SetTextColor( D3DCOLOR_ARGB( 255, 64, 64, 64 ) );
		pIMEEdit->SetCaretColor( D3DCOLOR_ARGB( 255, 64, 64, 64 ) );
		pIMEEdit->SetSelectedTextColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		pIMEEdit->SetSelectedBackColor( D3DCOLOR_ARGB( 255, 40, 72, 72 ) );
	}*/

	// Slider
	/*g_SampleUI.AddSlider( IDC_SLIDER, 200, 450, 200, 24, 0, 100, 50, false );

	// Checkbox
	g_SampleUI.AddCheckBox( IDC_CHECKBOX, L"This is a checkbox with hotkey. Press 'C' to toggle the check state.",
							170, 450, 350, 24, false, L'C', false );
	g_SampleUI.AddCheckBox( IDC_CLEAREDIT, L"This checkbox controls whether edit control text is cleared when Enter is pressed. (T)",
							170, 460, 450, 24, false, L'T', false );*/

							// Combobox
							/*CDXUTComboBox *pCombo;
							g_SampleUI.AddComboBox( IDC_COMBOBOX, 0, 0, 200, 24, L'O', false, &pCombo );
							if( pCombo )
							{
								pCombo->SetDropHeight( 100 );
								pCombo->AddItem( L"Combobox item (O)", (LPVOID)0x11111111 );
								pCombo->AddItem( L"Placeholder (O)", (LPVOID)0x12121212 );
								pCombo->AddItem( L"One more (O)", (LPVOID)0x13131313 );
								pCombo->AddItem( L"I can't get enough (O)", (LPVOID)0x14141414 );
								pCombo->AddItem( L"Ok, last one, I promise (O)", (LPVOID)0x15151515 );
							}*/

							// Radio buttons
							/*g_SampleUI.AddRadioButton( IDC_RADIO1A, 1, L"Radio group 1 Amy (1)", 0, 50, 220, 24, false, L'1' );
							g_SampleUI.AddRadioButton( IDC_RADIO1B, 1, L"Radio group 1 Brian (2)", 0, 50, 220, 24, false, L'2' );
							g_SampleUI.AddRadioButton( IDC_RADIO1C, 1, L"Radio group 1 Clark (3)", 0, 50, 220, 24, false, L'3' );

							g_SampleUI.AddRadioButton( IDC_RADIO2A, 2, L"Single (4)", 0, 50, 90, 24, false, L'4' );
							g_SampleUI.AddRadioButton( IDC_RADIO2B, 2, L"Double (5)", 0, 50, 90, 24, false, L'5' );
							g_SampleUI.AddRadioButton( IDC_RADIO2C, 2, L"Triple (6)", 0, 50, 90, 24, false, L'6' );*/

							// List box
							/*g_SampleUI.AddListBox( IDC_LISTBOX, 30, 400, 200, 150, 0 );
							for( int i = 0; i < 15; ++i )
							{
								WCHAR wszText[50];
								StringCchPrintf( wszText, 50, L"Single-selection listbox item %d", i );
								g_SampleUI.GetListBox( IDC_LISTBOX )->AddItem( wszText, (LPVOID)(size_t)i );
							}
							g_SampleUI.AddListBox( IDC_LISTBOXM, 30, 400, 200, 150, CDXUTListBox::MULTISELECTION );
							for( int i = 0; i < 30; ++i )
							{
								WCHAR wszText[50];
								StringCchPrintf( wszText, 50, L"Multi-selection listbox item %d", i );
								g_SampleUI.GetListBox( IDC_LISTBOXM )->AddItem( wszText, (LPVOID)(size_t)i );
							}*/
}


//--------------------------------------------------------------------------------------
// Called during device initialization, this code checks the device for some 
// minimum set of capabilities, and rejects those that don't pass by returning false.
//--------------------------------------------------------------------------------------
bool CALLBACK IsDeviceAcceptable(D3DCAPS9* pCaps, D3DFORMAT AdapterFormat,
	D3DFORMAT BackBufferFormat, bool bWindowed)
{
	// Skip backbuffer formats that don't support alpha blending
	IDirect3D9* pD3D = DXUTGetD3DObject();
	if (FAILED(pD3D->CheckDeviceFormat(pCaps->AdapterOrdinal, pCaps->DeviceType,
		AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
		D3DRTYPE_TEXTURE, BackBufferFormat)))
		return false;
	/*
		// Must support pixel shader 1.1
		if( pCaps->PixelShaderVersion < D3DPS_VERSION( 1, 1 ) )
			return false;*/

	return true;
}


//--------------------------------------------------------------------------------------
// This callback function is called immediately before a device is created to allow the 
// application to modify the device settings. The supplied pDeviceSettings parameter 
// contains the settings that the framework has selected for the new device, and the 
// application can make any desired changes directly to this structure.  Note however that 
// the sample framework will not correct invalid device settings so care must be taken 
// to return valid device settings, otherwise IDirect3D9::CreateDevice() will fail.  
//--------------------------------------------------------------------------------------
void CALLBACK ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps)
{
	// If device doesn't support HW T&L or doesn't support 1.1 vertex shaders in HW 
	// then switch to SWVP.
	if ((pCaps->DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
		pCaps->VertexShaderVersion < D3DVS_VERSION(1, 1))
	{
		pDeviceSettings->BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	else
	{
		pDeviceSettings->BehaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}

	// This application is designed to work on a pure device by not using 
	// IDirect3D9::Get*() methods, so create a pure device if supported and using HWVP.
	if ((pCaps->DevCaps & D3DDEVCAPS_PUREDEVICE) != 0 &&
		(pDeviceSettings->BehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING) != 0)
		pDeviceSettings->BehaviorFlags |= D3DCREATE_PUREDEVICE;

	// Debugging vertex shaders requires either REF or software vertex processing 
	// and debugging pixel shaders requires REF.  
#ifdef DEBUG_VS
	if (pDeviceSettings->DeviceType != D3DDEVTYPE_REF)
	{
		pDeviceSettings->BehaviorFlags &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
		pDeviceSettings->BehaviorFlags &= ~D3DCREATE_PUREDEVICE;
		pDeviceSettings->BehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
#endif
#ifdef DEBUG_PS
	pDeviceSettings->DeviceType = D3DDEVTYPE_REF;
#endif
}


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has been 
// created, which will happen during application initialization and windowed/full screen 
// toggles. This is the best location to create D3DPOOL_MANAGED resources since these 
// resources need to be reloaded whenever the device is destroyed. Resources created  
// here should be released in the OnDestroyDevice callback. 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnCreateDevice(IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc)
{
	HRESULT hr;
	g_width = pBackBufferSurfaceDesc->Width;
	g_height = pBackBufferSurfaceDesc->Height;
	g_temp = (pBackBufferSurfaceDesc->Width / 2);
	g_temp = (-g_temp);

	/*Create_3DSprite(pd3dDevice,g_pTexture_Prog,g_pVB_Prog,pstrTextureFiles[0],110,50,416,124);

	Create_3DSprite(pd3dDevice,g_pTexture_Text_Bar,g_pVB_Text_Bar,pstrTextureFiles[2],0,400,640,80);

	//Generate Char
	Create_3DSprite(pd3dDevice,g_pTexture_Character[0],g_pVB_Character[0],g_pstrCharater_Name[0],g_temp,200,g_temp+100,400);
	Create_3DSprite(pd3dDevice,g_pTexture_Character[1],g_pVB_Character[1],g_pstrCharater_Name[1],g_temp,200,g_temp+100,400);
	Create_3DSprite(pd3dDevice,g_pTexture_Character[2],g_pVB_Character[2],g_pstrCharater_Name[2],g_temp,200,g_temp+100,400);
	Create_3DSprite(pd3dDevice,g_pTexture_Character[3],g_pVB_Character[3],g_pstrCharater_Name[3],g_temp,200,g_temp+100,400);
	Create_3DSprite(pd3dDevice,g_pTexture_Character[4],g_pVB_Character[4],g_pstrCharater_Name[4],g_temp,200,g_temp+100,400);
	Create_3DSprite(pd3dDevice,g_pTexture_Character[5],g_pVB_Character[5],g_pstrCharater_Name[5],g_temp,200,g_temp+100,400);
	Create_3DSprite(pd3dDevice,g_pTexture_Character[6],g_pVB_Character[6],g_pstrCharater_Name[6],g_temp,200,g_temp+100,400);
	Create_3DSprite(pd3dDevice,g_pTexture_Character[7],g_pVB_Character[7],g_pstrCharater_Name[7],g_temp,200,g_temp+100,400);
	Create_3DSprite(pd3dDevice,g_pTexture_Character[8],g_pVB_Character[8],g_pstrCharater_Name[8],g_temp,200,g_temp+100,400);
	Create_3DSprite(pd3dDevice,g_pTexture_Character[9],g_pVB_Character[9],g_pstrCharater_Name[9],g_temp,200,g_temp+100,400);*/
	// Initialize the font
	V_RETURN(D3DXCreateFont(pd3dDevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"Arial", &g_pFont));

	// Define DEBUG_VS and/or DEBUG_PS to debug vertex and/or pixel shaders with the 
	// shader debugger. Debugging vertex shaders requires either REF or software vertex 
	// processing, and debugging pixel shaders requires REF.  The 
	// D3DXSHADER_FORCE_*_SOFTWARE_NOOPT flag improves the debug experience in the 
	// shader debugger.  It enables source level debugging, prevents instruction 
	// reordering, prevents dead code elimination, and forces the compiler to compile 
	// against the next higher available software target, which ensures that the 
	// unoptimized shaders do not exceed the shader model limitations.  Setting these 
	// flags will cause slower rendering since the shaders will be unoptimized and 
	// forced into software.  See the DirectX documentation for more information about 
	// using the shader debugger.
	DWORD dwShaderFlags = 0;
#ifdef DEBUG_VS
	dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
	dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif

	// Read the D3DX effect file
	WCHAR str[MAX_PATH];
	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"CustomUI.fx"));

	// If this fails, there should be debug output as to 
	// they the .fx file failed to compile
	V_RETURN(D3DXCreateEffectFromFile(pd3dDevice, str, NULL, NULL, dwShaderFlags,
		NULL, &g_pEffect, NULL));

	g_Mesh.Create(pd3dDevice, L"misc\\cell.x");

	// Setup the camera's view parameters
	D3DXVECTOR3 vecEye(0.0f, 1.5f, -7.0f);
	D3DXVECTOR3 vecAt(0.0f, 0.2f, 0.0f);
	D3DXVECTOR3 vecUp(0.0f, 1.0f, 0.0f);
	g_Camera.SetViewParams(&vecEye, &vecAt);
	D3DXMatrixLookAtLH(&g_mView, &vecEye, &vecAt, &vecUp);

	ACT1_LOC[0] = 1;
	ACT1_LOC[1] = 1;

	Scene[0].Scene_ID = 0;
	Scene[0].access_script_no = 0;
	Scene[0].conversation_script_no = 0;
	Scene[0].item_exist = 0;
	Scene[0].location = 0;
	Scene[0].present = 1;
	Scene[0].script_no = 0;
	Scene[0].Character = 1000;
	Scene[0].access = 0;

	Scene[1].Scene_ID = 1;
	Scene[1].access_script_no = 0;
	Scene[1].conversation_script_no = 1;
	Scene[1].item_exist = 0;
	Scene[1].location = 0;
	Scene[1].present = 1;
	Scene[1].script_no = 1;
	Scene[1].Character = 2;
	Scene[1].access = 0;

	Scene[2].Scene_ID = 2;
	Scene[2].access_script_no = 0;
	Scene[2].conversation_script_no = 2;
	Scene[2].item_exist = 0;
	Scene[2].location = 0;
	Scene[2].present = 1;
	Scene[2].script_no = 2;
	Scene[2].Character = 0;
	Scene[2].access = 0;

	Scene[3].Scene_ID = 3;
	Scene[3].access_script_no = 0;
	Scene[3].conversation_script_no = 3;
	Scene[3].item_exist = 0;
	Scene[3].location = 0;
	Scene[3].present = 1;
	Scene[3].script_no = 3;
	Scene[3].Character = 3;
	Scene[3].access = 0;

	Scene[4].Scene_ID = 4;
	Scene[4].access_script_no = 0;
	Scene[4].conversation_script_no = 4;
	Scene[4].item_exist = 0;
	Scene[4].location = 0;
	Scene[4].present = 1;
	Scene[4].script_no = 4;
	Scene[4].Character = 4;
	Scene[4].access = 0;

	Scene[5].Scene_ID = 5;
	Scene[5].access_script_no = 0;
	Scene[5].conversation_script_no = 4;
	Scene[5].item_exist = 0;
	Scene[5].location = 0;
	Scene[5].present = 1;
	Scene[5].script_no = 5;
	Scene[5].Character = 6;
	Scene[5].access = 0;

	return S_OK;
}


//--------------------------------------------------------------------------------------
// This function loads the mesh and ensures the mesh has normals; it also optimizes the 
// mesh for the graphics card's vertex cache, which improves performance by organizing 
// the internal triangle list for less cache misses.
//--------------------------------------------------------------------------------------
HRESULT LoadMesh(IDirect3DDevice9* pd3dDevice, WCHAR* strFileName, ID3DXMesh** ppMesh)
{
	ID3DXMesh* pMesh = NULL;
	WCHAR str[MAX_PATH];
	HRESULT hr;

	g_pd3dDevice = pd3dDevice;

	// Load the mesh with D3DX and get back a ID3DXMesh*.  For this
	// sample we'll ignore the X file's embedded materials since we know 
	// exactly the model we're loading.  See the mesh samples such as
	// "OptimizedMesh" for a more generic mesh loading example.
	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, strFileName));

	V_RETURN(D3DXLoadMeshFromX(str, D3DXMESH_MANAGED, pd3dDevice, NULL, NULL, NULL, NULL, &pMesh));

	DWORD* rgdwAdjacency = NULL;

	// Make sure there are normals which are required for lighting
	if (!(pMesh->GetFVF() & D3DFVF_NORMAL))
	{
		ID3DXMesh* pTempMesh;
		V(pMesh->CloneMeshFVF(pMesh->GetOptions(),
			pMesh->GetFVF() | D3DFVF_NORMAL,
			pd3dDevice, &pTempMesh));
		V(D3DXComputeNormals(pTempMesh, NULL));

		SAFE_RELEASE(pMesh);
		pMesh = pTempMesh;
	}

	// Optimize the mesh for this graphics card's vertex cache 
	// so when rendering the mesh's triangle list the vertices will 
	// cache hit more often so it won't have to re-execute the vertex shader 
	// on those vertices so it will improve perf.     
	rgdwAdjacency = new DWORD[pMesh->GetNumFaces() * 3];
	if (rgdwAdjacency == NULL)
		return E_OUTOFMEMORY;
	V(pMesh->GenerateAdjacency(1e-6f, rgdwAdjacency));
	V(pMesh->OptimizeInplace(D3DXMESHOPT_VERTEXCACHE, rgdwAdjacency, NULL, NULL, NULL));
	delete[]rgdwAdjacency;

	*ppMesh = pMesh;

	return S_OK;
}


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has been 
// reset, which will happen after a lost device scenario. This is the best location to 
// create D3DPOOL_DEFAULT resources since these resources need to be reloaded whenever 
// the device is lost. Resources created here should be released in the OnLostDevice 
// callback. 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnResetDevice(IDirect3DDevice9* pd3dDevice,
	const D3DSURFACE_DESC* pBackBufferSurfaceDesc)
{
	HRESULT hr;
	g_pd3dDevice = pd3dDevice;
	g_width = pBackBufferSurfaceDesc->Width;
	g_height = pBackBufferSurfaceDesc->Height;

	g_d3dHangul.RestoreDeviceObjects();
	g_d3dHangul2.RestoreDeviceObjects(L"HanFont\\IYAGI.FNT", L"HanFont\\ITALIC.EFT");
	g_d3dHangul3.RestoreDeviceObjects(L"HanFont\\MYUNJO.FNT", L"HanFont\\MYUNJO.EFT");
	//g_d3dHangul3.RestoreDeviceObjects(L"HanFont\\IYAGI.FNT",L"HanFont\\ITALIC.EFT");		
	if (g_pFont)
		V_RETURN(g_pFont->OnResetDevice());
	if (g_pEffect)
		V_RETURN(g_pEffect->OnResetDevice());





	SAFE_RELEASE(g_pTexture_Fade);
	SAFE_RELEASE(g_pVB_Fade);
	Create_3DSprite(pd3dDevice, g_pTexture_Fade, g_pVB_Fade, L"1.tga", 0, 0, pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height);

	SAFE_RELEASE(g_pTexture_Prog);
	SAFE_RELEASE(g_pVB_Prog);
	Create_3DSprite(pd3dDevice, g_pTexture_Prog, g_pVB_Prog, pstrTextureFiles[0], 110 * (g_width / 640), 50 * (g_height / 480), 416 * (g_width / 640), 124 * (g_height / 480));



	SAFE_RELEASE(g_pTexture_Text_Bar);
	SAFE_RELEASE(g_pVB_Text_Bar);
	Create_3DSprite(pd3dDevice, g_pTexture_Text_Bar, g_pVB_Text_Bar, pstrTextureFiles[2], 0, (pBackBufferSurfaceDesc->Height) * (5.0f / 6.0f), pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height / 6);
	// Create a sprite to help batch calls when drawing many lines of text
	V_RETURN(D3DXCreateSprite(pd3dDevice, &g_pTextSprite));

	int i;

	for (i = 0; i < 20; i++)
	{
		SAFE_RELEASE(g_pVB_Scene[i]);
		SAFE_RELEASE(g_pTexture_Scene[i]);
	}

	for (i = 0; i < 20; i++)
	{
		Create_3DSprite(pd3dDevice, g_pTexture_Scene[i], g_pVB_Scene[i], g_pstrScene[i], 0, 0, pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height);
	}


	for (i = 0; i < 10; i++)
	{
		SAFE_RELEASE(g_pTexture_Character[i]);
		SAFE_RELEASE(g_pVB_Character[i]);

		g_temp = (g_width / 2) - (g_width / 4);
	}
	for (i = 0; i < 10; i++)
	{
		Create_3DSprite(pd3dDevice, g_pTexture_Character[i], g_pVB_Character[i], g_pstrCharater_Name[i], g_temp, (pBackBufferSurfaceDesc->Height / 8), (g_width / 2), (pBackBufferSurfaceDesc->Height) * (7.0f / 8.0f));
	}


	for (i = 0; i < 20; i++)
	{
		SAFE_RELEASE(g_pTexture_Item[i]);
		SAFE_RELEASE(g_pVB_Item[i]);
	}
	int b;
	for (i = 0; i < 20; i++)
	{
		if (i > 2)
			b = 2;
		else b = i;
		//Create_3DSprite(pd3dDevice,g_pTexture_Item[i],g_pVB_Item[i],g_pstrItem_Name[0],(g_width/8.0f)*((i%5)+1),(g_height/20.0f)+(g_height/8.0f)*(i/5),g_width/8.0f,g_height/8.0f);				
		Create_3DSprite(pd3dDevice, g_pTexture_Item[b], g_pVB_Item[b], g_pstrItem_Name[b], g_width / 8.0f * i, 0, g_width / 8.0f, g_height / 8.0f);
	}


	// Setup the camera's projection parameters
	float fAspectRatio = pBackBufferSurfaceDesc->Width / (FLOAT)pBackBufferSurfaceDesc->Height;
	g_Camera.SetProjParams(D3DX_PI / 4, fAspectRatio, 0.1f, 1000.0f);
	g_Camera.SetWindow(pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height);

	if (g_GameStart == 0)
	{
		g_HUD.SetLocation(pBackBufferSurfaceDesc->Width - 170, 0);
		g_HUD.SetSize(170, 170);
		g_SampleUI.SetLocation(0, 0);
		g_SampleUI.SetSize(pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height);

		//g_SampleUI.GetControl( IDC_STATIC )->SetSize( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height * 6 / 10 );
		//g_SampleUI.GetControl( IDC_OUTPUT )->SetSize( pBackBufferSurfaceDesc->Width - 170, pBackBufferSurfaceDesc->Height / 4 );
		g_SampleUI.GetControl(IDC_START)->SetLocation((pBackBufferSurfaceDesc->Width / 2) - 40, pBackBufferSurfaceDesc->Height - 220);
		g_SampleUI.GetControl(IDC_LOAD)->SetLocation((pBackBufferSurfaceDesc->Width / 2) - 40, pBackBufferSurfaceDesc->Height - 180);
		g_SampleUI.GetControl(IDC_CONFIG)->SetLocation((pBackBufferSurfaceDesc->Width / 2) - 40, pBackBufferSurfaceDesc->Height - 140);
		g_SampleUI.GetControl(IDC_EXIT)->SetLocation((pBackBufferSurfaceDesc->Width / 2) - 40, pBackBufferSurfaceDesc->Height - 100);

		/*g_SampleUI.GetControl( IDC_EDITBOX1 )->SetLocation( 20, pBackBufferSurfaceDesc->Height - 230 );
		g_SampleUI.GetControl( IDC_EDITBOX1 )->SetSize( pBackBufferSurfaceDesc->Width - 40, 32 );
		g_SampleUI.GetControl( IDC_EDITBOX2 )->SetLocation( 20, pBackBufferSurfaceDesc->Height - 280 );
		g_SampleUI.GetControl( IDC_EDITBOX2 )->SetSize( pBackBufferSurfaceDesc->Width - 40, 45 );

		g_SampleUI.GetControl( IDC_SLIDER )->SetLocation( 10, pBackBufferSurfaceDesc->Height - 140 );
		g_SampleUI.GetControl( IDC_CHECKBOX )->SetLocation( 120, pBackBufferSurfaceDesc->Height - 50 );
		g_SampleUI.GetControl( IDC_CLEAREDIT )->SetLocation( 120, pBackBufferSurfaceDesc->Height - 25 );
		g_SampleUI.GetControl( IDC_COMBOBOX )->SetLocation( 20, pBackBufferSurfaceDesc->Height - 180 );
		g_SampleUI.GetControl( IDC_RADIO1A )->SetLocation( pBackBufferSurfaceDesc->Width - 160, 100 );
		g_SampleUI.GetControl( IDC_RADIO1B )->SetLocation( pBackBufferSurfaceDesc->Width - 160, 124 );
		g_SampleUI.GetControl( IDC_RADIO1C )->SetLocation( pBackBufferSurfaceDesc->Width - 160, 148 );
		g_SampleUI.GetControl( IDC_RADIO2A )->SetLocation( 20, pBackBufferSurfaceDesc->Height - 100 );
		g_SampleUI.GetControl( IDC_RADIO2B )->SetLocation( 20, pBackBufferSurfaceDesc->Height - 76 );
		g_SampleUI.GetControl( IDC_RADIO2C )->SetLocation( 20, pBackBufferSurfaceDesc->Height - 52 );
		g_SampleUI.GetControl( IDC_LISTBOX )->SetLocation( pBackBufferSurfaceDesc->Width - 400, pBackBufferSurfaceDesc->Height - 180 );
		g_SampleUI.GetControl( IDC_LISTBOX )->SetSize( 190, 96 );
		g_SampleUI.GetControl( IDC_LISTBOXM )->SetLocation( pBackBufferSurfaceDesc->Width - 200, pBackBufferSurfaceDesc->Height - 180 );
		g_SampleUI.GetControl( IDC_LISTBOXM )->SetSize( 190, 124 );*/

	}
	g_Mesh.RestoreDeviceObjects(pd3dDevice);
	g_Char_Exchange2 = 0;

	g_temp = g_width / (-2.0f);


	if (g_GameStart == 1 && g_game_state == 4)
	{
		g_menu.RemoveAllControls();
		InitMenu2();
	}

	return S_OK;
}


//--------------------------------------------------------------------------------------
// This callback function will be called once at the beginning of every frame. This is the
// best location for your application to handle updates to the scene, but is not 
// intended to contain actual rendering calls, which should instead be placed in the 
// OnFrameRender callback.  
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime)
{
	D3DXMATRIXA16 m;
	D3DXMatrixRotationY(&m, D3DX_PI * fElapsedTime / 40.0f);
	D3DXMatrixMultiply(&g_mView, &m, &g_mView);
	if (g_GameStart == 1)
		Character_Sprite(fElapsedTime);
}


//--------------------------------------------------------------------------------------
// This callback function will be called at the end of every frame to perform all the 
// rendering calls for the scene, and it will also be called if the window needs to be 
// repainted. After this function has returned, the sample framework will call 
// IDirect3DDevice9::Present to display the contents of the next buffer in the swap chain
//--------------------------------------------------------------------------------------

int ghj = 0;
char ggbuf[50];
int g_FirstMeeting = 1;

void Visit_Date_Display()
{
	int k;
	g_game_state = 0;
	if (Scene[g_Hero_Scene].Character != 1000)
	{
		MeetingData[MeetingDataIndex] = Scene[g_Hero_Scene].Character;
		MeetingDataIndex++;
		g_FirstMeeting = 0;
	}

	g_d3dHangul.SetWidthHeight((int)(g_width / 20), (int)(g_height / 16));
	g_d3dHangul.SetColor(0xffffff);
	WCHAR wFileName[MAX_PATH];
	WideCharToMultiByte(CP_ACP, NULL,
		g_pstrTime[g_Hero_Scene], -1,
		ggbuf, sizeof(ggbuf),
		NULL, NULL);
	k = strlen(ggbuf);
	ggbuf[ghj] = 0;
	MultiByteToWideChar(CP_ACP, 0, ggbuf, -1,
		wFileName, MAX_PATH);
	g_d3dHangul.OutHanXY((g_width / 2) - ((g_width / 20) * (sizeof(g_pstrTime[0]) / 2)), (g_height / 2), wFileName);
	if (g_KeyBoard_Acess == 0)
	{
		int t = ghj - 1;
		if (t > 0 && ggbuf[t] == ' ')
			t;
		else SwapSourceFilter2(0);
	}
	ghj++;
	if (ghj > k)
	{
		ghj = k;
		g_KeyBoard_Acess = 1;
	}
	Sleep(100);
}

void Menu_Display()
{
	RenderText();
}
int g_act = 1;
int g_act1_auto2 = 0;
int aaa = 1;
void CALLBACK OnFrameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime)
{

	HRESULT hr;
	D3DXMATRIXA16 mWorld;
	D3DXMATRIXA16 mView;
	D3DXMATRIXA16 mProj;
	D3DXMATRIXA16 mWorldViewProjection;
	char a[] = "박주항은 천재다";

	// Clear the render target and the zbuffer 
	V(pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0));

	// Render the scene
	if (g_GameStart == 0)
	{
		if (SUCCEEDED(pd3dDevice->BeginScene()))
		{
			// Get the projection & view matrix from the camera class       
			mWorld = *g_Camera.GetWorldMatrix();
			mProj = *g_Camera.GetProjMatrix();
			mView = g_mView;
			mWorldViewProjection = mWorld * mView * mProj;
			// Update the effect's variables.  Instead of using strings, it would         
			// be more efficient to cache a handle to the parameter by calling         
			// ID3DXEffect::GetParameterByName        
			V(g_pEffect->SetMatrix("g_mWorldViewProjection", &mWorldViewProjection));
			V(g_pEffect->SetMatrix("g_mWorld", &mWorld));
			V(g_pEffect->SetFloat("g_fTime", (float)fTime));
			g_pEffect->SetTechnique("RenderScene");
			UINT cPasses;

			g_pEffect->Begin(&cPasses, 0);
			ID3DXMesh* pMesh = g_Mesh.GetLocalMesh();
			for (UINT p = 0; p < cPasses; ++p)
			{
				g_pEffect->BeginPass(p);
				for (UINT m = 0; m < g_Mesh.m_dwNumMaterials; ++m)
				{
					g_pEffect->SetTexture("g_txScene", g_Mesh.m_pTextures[m]);
					g_pEffect->CommitChanges();
					pMesh->DrawSubset(m);
				}
				g_pEffect->EndPass();
			}
			g_pEffect->End();
			// V( g_HUD.OnRender( fElapsedTime ) );		
			V(g_SampleUI.OnRender(fElapsedTime));
			V(g_menu.OnRender(fElapsedTime));
			Display(pd3dDevice, g_pTexture_Prog, g_pVB_Prog, 0);
			//RenderText();
			V(pd3dDevice->EndScene());
		}
	}
	else if (g_GameStart == 1)
	{
		if (SUCCEEDED(pd3dDevice->BeginScene()))
		{




			if (g_act == 1)
			{
				//배경장면 
				if (g_act1_auto == 1 && Scene[g_Hero_Scene].Scene_ID == 0)
				{
					init_val();
					Scene[g_Hero_Scene].access = 0;
					Scene[g_Hero_Scene].access_script_no = 0;
					g_act1_auto2 = 1;
					g_act1_auto = 0;
				}
				Display(pd3dDevice, g_pTexture_Scene[g_Hero_Scene], g_pVB_Scene[g_Hero_Scene], 0);

				// 현재 장면에 존재하는 사람 체크
				if (Scene[g_Hero_Scene].Character != 1000 && bCharShow_Mode == false) //1000은 아무도 없다는 걸 의미함										
					Display(pd3dDevice, g_pTexture_Character[Scene[g_Hero_Scene].Character]
						, g_pVB_Character[Scene[g_Hero_Scene].Character], g_Char_Exchange);


				if (bCharShow_Mode == true)
				{
					Display2(pd3dDevice, g_pTexture_Character[MeetingData[g_Char_Selected_Number]]
						, g_pVB_Character[MeetingData[g_Char_Selected_Number]], g_Char_Exchange);
				}

				//대사창 이미지 출력
				Display(pd3dDevice, g_pTexture_Text_Bar, g_pVB_Text_Bar, 0);

				//인벤토리창 출력
				if (bInventory_Mode == true)
				{

					Display(pd3dDevice, g_pTexture_Fade, g_pVB_Fade, 0);
					//for(j=0;j<20;j++)
					//Display( pd3dDevice,g_pTexture_Item[j],g_pVB_Item[j],0);			
					Show_Inventory_Item(pd3dDevice, g_pTexture_Item[0], g_pVB_Item[0], 1);
					Show_Inventory_Item(pd3dDevice, g_pTexture_Item[1], g_pVB_Item[1], 0);
				}

				if (Scene[g_Hero_Scene].access == 0) //처음 방문시 시간을 표시한다.
				{
					Visit_Date_Display();
				}
				else if (Scene[g_Hero_Scene].access_script_no == 0) // 최초로 나오는 텍스트를 출력한다.           
				{
					if (!g_bTextReading)
					{
						GetFileHandle();
						g_bTextReading = 1;
					}
					else
					{
						GetText(fElapsedTime);
					}

				}
				else if (Scene[g_Hero_Scene].access_script_no == 1)
				{
					g_act1_auto2 = 0;
					if (g_Hero_Scene == 0 && g_act1_auto2 == 0)
					{
						/*if(g_menu_set==0)
							InitMenu2();
						V( g_menu.OnRender( fElapsedTime ) );			   */
						if (ttt == 0)
							g_game_state = 4;
						ttt = 1;
						if (g_game_state == 7)
						{
							V(g_menu.OnRender(fElapsedTime));
						}
						else if (g_game_state == 8)
						{
							RenderText();
						}
					}
					else
					{
						if (g_Hero_Scene == 5)
						{
							g_game_state = 0;
						}
						if (g_script_analysis == 0)
							script_analyzer();
						if (g_game_state == 4 && g_Hero_Scene != 5)
							Menu_Display();
						else if (g_game_state == 5)
							Menu_Extend_Display();
						else if (g_game_state == 7)
						{
							V(g_menu.OnRender(fElapsedTime));
						}
						else if (g_game_state == 8)
						{
							RenderText();
						}
						else if (g_game_state == 0)
						{
							if (b_Text_Parse == false)
							{
								Text_Parse();
								if (g_Hero_Scene == 5 && aaa == 1)
								{
									g_bTextReading_Terminate = 0;
									aaa = 0;
								}
								b_Text_Parse = true;
							}
							else
							{
								GetText2(fElapsedTime);
								GetText3(fElapsedTime);
							}
						}
					}

				}

				V(g_GameUI.OnRender(fElapsedTime));
				V(pd3dDevice->EndScene());
			}
			else if (g_act == 2)
			{
				;
			}
		}
	}
}


void GetFileHandle()
{
	char buf[MAX_PATH];
	if (g_act1_auto2 == 1)
	{
		WideCharToMultiByte(CP_ACP, NULL,
			g_pstrScenario[6], -1,
			buf, sizeof(buf),
			NULL, NULL);
		ACT1_LOC[5] = 1;
		g_can_go = 6;

	}
	else
	{
		WideCharToMultiByte(CP_ACP, NULL,
			g_pstrScenario[g_Hero_Scene], -1,
			buf, sizeof(buf),
			NULL, NULL);
	}
	g_fp = fopen(buf, "r");
	if (g_fp == NULL)
		MessageBox(NULL, L"S", L"D", MB_OK);
}

void GetText(float fElapsedTime)
{


	if (g_bProcessKey == 1 && g_bTextReading_Terminate == 0)
	{
		g_KeyBoard_Acess = 0;
		if (!feof(g_fp))
			g_LineRead++;
		fgets(g_string, MAX_PATH, g_fp);
		if (!feof(g_fp))
			g_LineRead++;
		fgets(g_string2, MAX_PATH, g_fp);
		//else fgets(g_string2,MAX_PATH,g_fp);   						


		g_bProcessKey = 0;
		// Output statistics    
	}
	if (g_LineRead != 2)
	{
		g_KeyBoard_Acess = 1;
		Scene[g_Hero_Scene].access_script_no = 1;
		g_bTextReading_Terminate = 1;
	}

	if (g_LineRead == 1)
	{
		MultiByteToWideChar(CP_ACP, 0, g_string, -1,
			g_wFileName, MAX_PATH);
		//MultiByteToWideChar( CP_ACP, 0, "", -1,                          
		//							g_wFileName2, MAX_PATH );
	}
	else
	{
		MultiByteToWideChar(CP_ACP, 0, g_string, -1,
			g_wFileName, MAX_PATH);
		MultiByteToWideChar(CP_ACP, 0, g_string2, -1,
			g_wFileName2, MAX_PATH);
	}










	int k, t;
	char buf[MAX_PATH];
	char buf2[MAX_PATH];
	g_d3dHangul2.SetWidthHeight((int)(g_width / 30), (int)(g_height / 24));
	g_d3dHangul2.SetColor(0x000ff);
	k = strlen(g_string);
	t = strlen(g_string2);
	if (g_LineRead == 1) // 이부분은 한줄을 읽었을 때의 경우가 존재할때를 말한다.(대사의 끝부분의 경우)
	{
		strcpy(buf, g_string);
		buf[qbt] = 0;
		MultiByteToWideChar(CP_ACP, 0, buf, -1,
			g_wFileName, MAX_PATH);
		g_d3dHangul2.OutHanXY(g_width / 20, g_height - (g_height / 10), g_wFileName);
		if (g_KeyBoard_Acess == 0)
		{
			int i = qbt - 1;
			if (i > 0 && buf[i] == ' ')
				;
			else; SwapSourceFilter2(0);
		}
		qbt++;
		if (qbt > k)
		{
			qbt = k;
			g_KeyBoard_Acess = 1;

		}
	}
	if (g_LineRead == 2)
	{
		strcpy(buf, g_string);
		strcpy(buf2, g_string2);
		buf[qbt] = 0;
		buf2[iop] = 0;
		MultiByteToWideChar(CP_ACP, 0, buf, -1,
			g_wFileName, MAX_PATH);
		MultiByteToWideChar(CP_ACP, 0, buf2, -1,
			g_wFileName2, MAX_PATH);
		g_d3dHangul2.OutHanXY(g_width / 20, g_height - (g_height / 8), g_wFileName);
		g_d3dHangul2.OutHanXY(g_width / 20, g_height - (g_height / 12), g_wFileName2);
		if (g_KeyBoard_Acess == 0)
		{
			int i = qbt - 1;
			if (i > 0 && buf[i] == ' ')
				;
			else; SwapSourceFilter2(0);
		}
		qbt++;
		if (qbt > k)
			iop++;
		if (qbt > k && iop > t)
		{
			qbt = k;
			iop = t;
			g_KeyBoard_Acess = 1;

		}
	}

	Sleep(10);












	/*CDXUTTextHelper txtHelper( g_pFont, g_pTextSprite, 15 );

	// Output statistics
	txtHelper.Begin();
	txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 0.0f, 1.0f, 1.0f ) );
	//txtHelper.DrawTextLine( DXUTGetFrameStats() );
	//txtHelper.DrawTextLine( DXUTGetDeviceStats() );
	if(g_LineRead==1)
	{
		txtHelper.SetInsertionPos( g_width/15.0f, g_height-g_height/10.0f );
		txtHelper.DrawTextLine(g_wFileName);
	}
	else
	{

		txtHelper.SetInsertionPos( g_width/15.0f, g_height-g_height/8.0f );
		txtHelper.DrawTextLine(g_wFileName);

		txtHelper.SetInsertionPos( g_width/15.0f, g_height-g_height/12.0f );
		txtHelper.DrawTextLine(g_wFileName2);
	}
	txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );

	// Draw help
	//txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	//txtHelper.DrawTextLine( L"Press ESC to quit" );
	txtHelper.End();*/


	//g_GameUI.GetStatic( IDC_OUTPUT )->SetText(wFileName );

   //fclose(fp);
}




//--------------------------------------------------------------------------------------
// Render the help and statistics text. This function uses the ID3DXFont interface for 
// efficient text rendering.
//--------------------------------------------------------------------------------------
void RenderText()
{
	// The helper object simply helps keep track of text position, and color
	// and then it calls pFont->DrawText( m_pSprite, strMsg, -1, &rc, DT_NOCLIP, m_clr );
	// If NULL is passed in as the sprite object, then it will work however the 
	// pFont->DrawText() will not be batched together.  Batching calls will improves performance.
	//CDXUTTextHelper txtHelper( g_pFont, g_pTextSprite, 15 );

	// Output statistics
	//txtHelper.Begin();


	//txtHelper.DrawTextLine( DXUTGetFrameStats() );
	//txtHelper.DrawTextLine( DXUTGetDeviceStats() );
	int t = 0;
	g_d3dHangul3.SetWidthHeight((int)(g_width / 30), (int)(g_height / 24));
	if (g_game_state == 4)
	{

		for (int i = 0; i < g_count; i++)
		{
			WCHAR wFileName[MAX_PATH];
			MultiByteToWideChar(CP_ACP, 0, g_script[i], -1,
				wFileName, MAX_PATH);
			if (i == g_extend_number)
			{
				i = i + g_extend;
			}
			if (t == g_selected_message)
			{

				g_d3dHangul3.SetColor(0x0ffffff);
				//		txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );		
			}
			else
			{
				g_d3dHangul3.SetColor(0xff00ff);

				//txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f ) );
			}
			g_d3dHangul3.OutHanXY(10 * (g_width / 640), (200 * (g_height / 480)) + t * (20 * (g_height / 480)), wFileName);
			//txtHelper.SetInsertionPos( 10, 200+t*20 );			
			t++;
			//txtHelper.DrawTextLine(wFileName);	
		}
	}
	else if (g_game_state == 5)
	{
		for (int i = 0; (i + g_extend_number) <= (g_extend_number + g_extend - 1); i++)
		{

			WCHAR wFileName[MAX_PATH];

			MultiByteToWideChar(CP_ACP, 0, g_script[i + g_extend_number + 1], -1,

				wFileName, MAX_PATH);

			if (i == g_selected_extend_message)
				g_d3dHangul3.SetColor(0x0ffffff);
			//txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );

			else
				g_d3dHangul3.SetColor(0xff00ff);
			//txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f ) );

	//	txtHelper.SetInsertionPos( 10, 200+i*20 );			

		//txtHelper.DrawTextLine(wFileName);
			g_d3dHangul3.OutHanXY(10 * (g_width / 640), (200 * (g_height / 480)) + i * (20 * (g_height / 480)), wFileName);
		}
	}
	else if (g_game_state == 8)
	{
		int t = 0;
		for (int i = 0; i < g_can_go; i++)
		{
		
			while (ACT1_LOC[t] == 0)
			{
				t++;
			}


			if (i == g_selected_char)
				g_d3dHangul3.SetColor(0x0ffffff);
			//	txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
			else
				g_d3dHangul3.SetColor(0xff00ff);
			//	txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f ) );
			//txtHelper.SetInsertionPos( 10, 200+i*20 );				
			g_d3dHangul3.OutHanXY(10 * (g_width / 640), (200 * (g_height / 480)) + i * (20 * (g_height / 480)), pAct1_str[i]);
			//	txtHelper.DrawTextLine(pAct1_str[i]);
			t++;
		}
	}


	//txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );

	// Draw help
	//txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	//txtHelper.DrawTextLine( L"Press ESC to quit" );
//    txtHelper.End();
}


//--------------------------------------------------------------------------------------
// Before handling window messages, the sample framework passes incoming windows 
// messages to the application through this callback function. If the application sets 
// *pbNoFurtherProcessing to TRUE, then the sample framework will not process this message.
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing)
{
	g_Hwnd = hWnd;
	if (uMsg == WM_GRAPHNOTIFY)
		HandleGraphEvent();
	if (uMsg == WM_GRAPHNOTIFY2)
		HandleGraphEvent2();
	if (uMsg == WM_SIZE)
		return 0;
	if (uMsg == WM_RBUTTONUP)
		if (g_game_state == 7)
			return 0;
		else if (g_game_state == 4 || g_game_state == 5)
		{
			InitMenu2();
		}
	// Give the dialogs a chance to handle the message first
	*pbNoFurtherProcessing = g_HUD.MsgProc(hWnd, uMsg, wParam, lParam);
	if (*pbNoFurtherProcessing)
		return 0;
	*pbNoFurtherProcessing = g_SampleUI.MsgProc(hWnd, uMsg, wParam, lParam);
	if (*pbNoFurtherProcessing)
		return 0;
	if (g_menu_set == 1)
		*pbNoFurtherProcessing = g_menu.MsgProc(hWnd, uMsg, wParam, lParam);
	if (*pbNoFurtherProcessing)
		return 0;

	// Pass all remaining windows messages to camera so it can respond to user input
	g_Camera.HandleMessages(hWnd, uMsg, wParam, lParam);

	return 0;
}


//--------------------------------------------------------------------------------------
// As a convenience, the sample framework inspects the incoming windows messages for
// keystroke messages and decodes the message parameters to pass relevant keyboard
// messages to the application.  The framework does not remove the underlying keystroke 
// messages, which are still passed to the application's MsgProc callback.
//--------------------------------------------------------------------------------------
void CALLBACK KeyboardProc(UINT nChar, bool bKeyDown, bool bAltDown)
{

	if (g_GameStart == 1)

		if (g_KeyBoard_Acess == 1)
		{
			g_FirstMeeting = 1;
			if (bKeyDown && nChar == VK_SPACE && g_game_state == 0)
			{
				Scene[g_Hero_Scene].access = 1;
				g_bProcessKey = 1; g_LineRead = 0;
				qbt = 0;
				iop = 0;
				g_KeyBoard_Acess = 0;
				if (g_juhang == 1 && (g_juhang2 == g_message_length - g_name_counter))
				{
					if (g_Hero_Scene == 5)
						exit(0);
					b_Text_Parse = false;
					g_game_state = 4;
					g_juhang = 0;
					g_name_counter = 0;
					g_juhang2 = 0;
					g_KeyBoard_Acess = 1;
				}
			}

			else if (Scene[g_Hero_Scene].access_script_no == 1 && g_game_state == 4)
			{
				if (bKeyDown)
				{
					switch (nChar)
					{
					case VK_SPACE:
						if (g_selected_message == g_extend_number)
						{
							Scene[g_Hero_Scene].access = 1;
							g_game_state = 5;
							g_selected_extend_message = 0;
							g_KeyBoard_Acess = 1;
						}
						else
						{
							if (ttt != 1)
							{
								g_game_state = 0;
								g_bProcessKey = 1; g_LineRead = 0;
								qbt = 0;
								g_KeyBoard_Acess = 1;
								iop = 0;
								g_bTextReading_Terminate = 0;
								g_bTextReading = 1;
							}
						}
						break;
					case VK_DOWN:
						g_selected_message++;
						if (g_selected_message > (g_message_number - 1))
							g_selected_message = 0;
						break;
					case VK_UP:
						g_selected_message--;
						if (g_selected_message < 0)
							g_selected_message = g_message_number - 1;
						break;

					case 'S':
						if (bCharShow_Mode == false)
							bCharShow_Mode = true;
						else bCharShow_Mode = false;
						g_Char_Exchange = 1; g_temp = -(g_width / 2); g_temp2 = 0;
						if (g_Char_Selected_Number >= MeetingDataIndex)
							g_Char_Selected_Number = 0;
						g_Char_Selected_Number++;;
						break;
					case 'A':
						if (bInventory_Mode == false)
							bInventory_Mode = true;
						else bInventory_Mode = false;
						break;
					}
				}
			}

			else if (Scene[g_Hero_Scene].access_script_no == 1 && g_game_state == 5)

			{

				if (bKeyDown)

				{

					switch (nChar)

					{

					case VK_SPACE:
						g_game_state = 0;
						g_bProcessKey = 1; g_LineRead = 0;
						qbt = 0;
						g_KeyBoard_Acess = 0;
						iop = 0;
						g_bTextReading_Terminate = 0;
						g_bTextReading = 1;
						g_juhang3 = 1;
						break;
					case VK_DOWN:
						g_selected_extend_message++;
						if (g_selected_extend_message >= g_extend)
							g_selected_extend_message = 0;
						break;

					case VK_UP:
						g_selected_extend_message--;
						if (g_selected_extend_message < 0)
							g_selected_extend_message = g_extend - 1;
						break;
					case 'S':

						if (bCharShow_Mode == false)

							bCharShow_Mode = true;

						else bCharShow_Mode = false;

						g_Char_Exchange = 1; g_temp = -(g_width / 2); g_temp2 = 0;

						if (g_Char_Selected_Number >= MeetingDataIndex)

							g_Char_Selected_Number = 0;

						g_Char_Selected_Number++;;

						break;

					case 'A':

						if (bInventory_Mode == false)

							bInventory_Mode = true;

						else bInventory_Mode = false;

						break;



					}

				}

			}


			else if (g_game_state == 8)

			{

				if (bKeyDown)

				{

					switch (nChar)

					{

					case VK_SPACE:


						g_Hero_Scene = g_selected_char;

						init_val();
						if (Scene[g_Hero_Scene].access_script_no == 1)
						{
							g_KeyBoard_Acess = 1;
							g_game_state = 4;
						}
						else
						{
							g_game_state = 4;
							g_KeyBoard_Acess = 0;
						}
						g_bProcessKey = 1; g_LineRead = 0;
						ghj = 0;
						g_FirstMeeting = 1;
						qbt = 0;
						iop = 0;
						g_bTextReading = 0;
						g_bTextReading_Terminate = 0;
						SwapSourceFilter();
						break;
					case VK_DOWN:
						g_selected_char++;
						if (g_selected_char >= g_can_go)
							g_selected_char = 0;
						break;

					case VK_UP:
						g_selected_char--;
						if (g_selected_char < 0)
							g_selected_char = g_can_go - 1;
						break;


					}

				}

			}



		}
}


/*case VK_LEFT:
							g_Hero_Scene++;
							if(g_Hero_Scene==5)
								g_Hero_Scene=0;
							g_bProcessKey=0;g_LineRead=0;
							g_bTextReading_Terminate=0;
							if(Scene[g_Hero_Scene].access_script_no==1)
							{
								g_KeyBoard_Acess=1;
								g_bTextReading=1;
							}
							else
							{
								g_bTextReading=0;
								g_KeyBoard_Acess=0;
							}
							ghj=0;
							if(g_fp)
								fclose(g_fp);
							qbt=0;
							iop=0;
							SwapSourceFilter();
							break;					*/

							//--------------------------------------------------------------------------------------
							// Handles the GUI events
							//--------------------------------------------------------------------------------------
void CALLBACK OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl)
{
	WCHAR wszOutput[1024];

	switch (nControlID)
	{
	case IDC_CONFIG:DXUTSetShowSettingsDialog(!DXUTGetShowSettingsDialog()); break;
	case IDC_EXIT:        PostQuitMessage(0); break;
	case IDC_TOGGLEFULLSCREEN: DXUTToggleFullScreen(); break;
	case IDC_TOGGLEREF:        DXUTToggleREF(); break;
	case IDC_CHANGEDEVICE:     DXUTSetShowSettingsDialog(!DXUTGetShowSettingsDialog()); break;
	case IDC_START:
		//CDXUTIMEEditBox::EnableImeSystem( true );
		//g_SampleUI.GetStatic( IDC_OUTPUT )->SetText( L"You clicked the 'Enable IME' button.\nIME text input is enabled for IME-capable edit controls." );
		g_SampleUI.RemoveAllControls();
		g_GameStart = 1;
		g_BgMusic = 1;
		g_GameUI.SetCallback(OnGUIEvent);
		g_GameUI.SetFont(1, L"Comic Sans MS", 25, FW_NORMAL);
		g_GameUI.SetFont(2, L"Courier New", 25, FW_NORMAL);
		g_GameUI.AddStatic(IDC_OUTPUT, L"", 0, 270, 620, 200);
		g_GameUI.GetStatic(IDC_OUTPUT)->SetTextColor(D3DCOLOR_ARGB(255, 255, 255, 0)); // Change color to red

		SwapSourceFilter();
		break;
	case IDC_LOAD:
		;
		//CDXUTIMEEditBox::EnableImeSystem( false );
		//g_SampleUI.GetStatic( IDC_OUTPUT )->SetText( L"You clicked the 'Disable IME' button.\nIME text input is disabled for IME-capable edit controls." );
		break;
	case IDC_EDITBOX1:
	case IDC_EDITBOX2:
		switch (nEvent)
		{
		case EVENT_EDITBOX_STRING:
		{
			StringCchPrintf(wszOutput, 1024, L"You have pressed Enter in edit control (ID %u).\nThe content of the edit control is:\n\"%s\"",
				nControlID, ((CDXUTEditBox*)pControl)->GetText());
			g_SampleUI.GetStatic(IDC_OUTPUT)->SetText(wszOutput);

			// Clear the text if needed
			if (g_SampleUI.GetCheckBox(IDC_CLEAREDIT)->GetChecked())
				((CDXUTEditBox*)pControl)->SetText(L"");
			break;
		}

		case EVENT_EDITBOX_CHANGE:
		{
			StringCchPrintf(wszOutput, 1024, L"You have changed the content of an edit control (ID %u).\nIt is now:\n\"%s\"",
				nControlID, ((CDXUTEditBox*)pControl)->GetText());
			g_SampleUI.GetStatic(IDC_OUTPUT)->SetText(wszOutput);

			break;
		}
		}
		break;
	case IDC_SLIDER:
		StringCchPrintf(wszOutput, 1024, L"You adjusted the slider control.\nThe new value reported is %d",
			((CDXUTSlider*)pControl)->GetValue());
		g_SampleUI.GetStatic(IDC_OUTPUT)->SetText(wszOutput);
		break;
	case IDC_CHECKBOX:
		StringCchPrintf(wszOutput, 1024, L"You %s the upper check box.",
			((CDXUTCheckBox*)pControl)->GetChecked() ? L"checked" : L"cleared");
		g_SampleUI.GetStatic(IDC_OUTPUT)->SetText(wszOutput);
		break;
	case IDC_CLEAREDIT:
		StringCchPrintf(wszOutput, 1024, L"You %s the lower check box.\nNow edit controls will %s",
			((CDXUTCheckBox*)pControl)->GetChecked() ? L"checked" : L"cleared",
			((CDXUTCheckBox*)pControl)->GetChecked() ? L"be cleared when you press Enter to send the text" : L"retain the text context when you press Enter to send the text");
		g_SampleUI.GetStatic(IDC_OUTPUT)->SetText(wszOutput);
		break;
	case IDC_COMBOBOX:
	{
		DXUTComboBoxItem* pItem = ((CDXUTComboBox*)pControl)->GetSelectedItem();
		if (pItem)
		{
			StringCchPrintf(wszOutput, 1024, L"You selected a new item in the combobox.\nThe new item is \"%s\" and the associated data value is 0x%p",
				pItem->strText, pItem->pData);
			g_SampleUI.GetStatic(IDC_OUTPUT)->SetText(wszOutput);
		}
		break;
	}
	case IDC_RADIO1A:
	case IDC_RADIO1B:
	case IDC_RADIO1C:
		StringCchPrintf(wszOutput, 1024, L"You selected a new radio button in the UPPER radio group.\nThe new button is \"%s\"",
			((CDXUTRadioButton*)pControl)->GetText());
		g_SampleUI.GetStatic(IDC_OUTPUT)->SetText(wszOutput);
		break;
	case IDC_RADIO2A:
	case IDC_RADIO2B:
	case IDC_RADIO2C:
		StringCchPrintf(wszOutput, 1024, L"You selected a new radio button in the LOWER radio group.\nThe new button is \"%s\"",
			((CDXUTRadioButton*)pControl)->GetText());
		g_SampleUI.GetStatic(IDC_OUTPUT)->SetText(wszOutput);
		break;

	case IDC_LISTBOX:
		switch (nEvent)
		{
		case EVENT_LISTBOX_ITEM_DBLCLK:
		{
			DXUTListBoxItem* pItem = ((CDXUTListBox*)pControl)->GetItem(((CDXUTListBox*)pControl)->GetSelectedIndex(-1));

			StringCchPrintf(wszOutput, 1024, L"You double clicked an item in the left list box.  The item is\n\"%s\"",
				pItem ? pItem->strText : L"");
			g_SampleUI.GetStatic(IDC_OUTPUT)->SetText(wszOutput);
			break;
		}

		case EVENT_LISTBOX_SELECTION:
		{
			StringCchPrintf(wszOutput, 1024, L"You changed the selection in the left list box.  The selected item is %d", ((CDXUTListBox*)pControl)->GetSelectedIndex());
			g_SampleUI.GetStatic(IDC_OUTPUT)->SetText(wszOutput);
			break;
		}
		}
		break;

	case IDC_LISTBOXM:
		switch (nEvent)
		{
		case EVENT_LISTBOX_ITEM_DBLCLK:
		{
			DXUTListBoxItem* pItem = ((CDXUTListBox*)pControl)->GetItem(((CDXUTListBox*)pControl)->GetSelectedIndex(-1));

			StringCchPrintf(wszOutput, 1024, L"You double clicked an item in the right list box.  The item is\n\"%s\"",
				pItem ? pItem->strText : L"");
			g_SampleUI.GetStatic(IDC_OUTPUT)->SetText(wszOutput);
			break;
		}

		case EVENT_LISTBOX_SELECTION:
		{
			StringCchPrintf(wszOutput, 1024, L"You changed the selection in the right list box.  The selected item(s) are\n");
			int nSelected = -1;
			while ((nSelected = ((CDXUTListBox*)pControl)->GetSelectedIndex(nSelected)) != -1)
			{
				StringCchPrintf(wszOutput + lstrlenW(wszOutput), 1024 - lstrlenW(wszOutput), L"%d,", nSelected);
			}
			// Remove the trailing comma if one exists.
			if (wszOutput[lstrlenW(wszOutput) - 1] == L',')
				wszOutput[lstrlenW(wszOutput) - 1] = L'\0';
			g_SampleUI.GetStatic(IDC_OUTPUT)->SetText(wszOutput);
			break;
		}
		}
		break;
	}
}


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has 
// entered a lost state and before IDirect3DDevice9::Reset is called. Resources created
// in the OnResetDevice callback should be released here, which generally includes all 
// D3DPOOL_DEFAULT resources. See the "Lost Devices" section of the documentation for 
// information about lost devices.
//--------------------------------------------------------------------------------------
void CALLBACK OnLostDevice()
{
	g_Mesh.InvalidateDeviceObjects();

	if (g_pFont)
		g_pFont->OnLostDevice();
	if (g_pEffect)
		g_pEffect->OnLostDevice();
	SAFE_RELEASE(g_pTextSprite);
}


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has 
// been destroyed, which generally happens as a result of application termination or 
// windowed/full screen toggles. Resources created in the OnCreateDevice callback 
// should be released here, which generally includes all D3DPOOL_MANAGED resources. 
//--------------------------------------------------------------------------------------
void CALLBACK OnDestroyDevice()
{
	g_Mesh.Destroy();

	SAFE_RELEASE(g_pEffect);
	SAFE_RELEASE(g_pFont);
	g_d3dHangul.SafeRelease();
	g_d3dHangul2.SafeRelease();
	g_d3dHangul3.SafeRelease();
	SAFE_RELEASE(g_pTexture_Prog);
	SAFE_RELEASE(g_pVB_Prog);

	SAFE_RELEASE(g_pTexture_Text_Bar);
	SAFE_RELEASE(g_pVB_Text_Bar);
	SAFE_RELEASE(g_pTexture_Fade);
	SAFE_RELEASE(g_pVB_Fade);

	int i;
	for (i = 0; i < 10; i++)
	{
		SAFE_RELEASE(g_pTexture_Character[i]);
		SAFE_RELEASE(g_pVB_Character[i]);
	}
	for (i = 0; i < 20; i++)
	{
		SAFE_RELEASE(g_pTexture_Item[i]);
		SAFE_RELEASE(g_pVB_Item[i]);
	}
	for (i = 0; i < 20; i++)
	{
		SAFE_RELEASE(g_pTexture_Scene[i]);
		SAFE_RELEASE(g_pVB_Scene[i]);
	}


}



//------------------------------------------------------------------------------
// Name: GraphInit()
// Desc: Initialization of DirectShow components and initial graph
//------------------------------------------------------------------------------

HRESULT GraphInit(void)
{
	HRESULT hr;
	// Initialize COM
	if (FAILED(hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))
		return hr;

	// Create DirectShow Graph
	if (FAILED(hr = CoCreateInstance(CLSID_FilterGraph, NULL,
		CLSCTX_INPROC, IID_IGraphBuilder,
		reinterpret_cast<void**>(&g_pGraphBuilder))))
		return hr;


	// Get the IMediaControl Interface
	if (FAILED(hr = g_pGraphBuilder->QueryInterface(IID_IMediaControl,
		reinterpret_cast<void**>(&g_pMediaControl))))
		return hr;

	// Get the IMediaControl Interface
	if (FAILED(hr = g_pGraphBuilder->QueryInterface(IID_IMediaSeeking,
		reinterpret_cast<void**>(&g_pMediaSeeking))))
		return hr;

	if (FAILED(hr = g_pGraphBuilder->QueryInterface(IID_IMediaEventEx,
		reinterpret_cast<void**>(&g_pME))))
		return hr;
	g_pME->SetNotifyWindow((OAHWND)g_Hwnd, WM_GRAPHNOTIFY, 0);


	if (FAILED(hr = CoCreateInstance(CLSID_FilterGraph, NULL,
		CLSCTX_INPROC, IID_IGraphBuilder,
		reinterpret_cast<void**>(&g_pGraphBuilder2))))
		return hr;
	// Get the IMediaControl Interface
	if (FAILED(hr = g_pGraphBuilder2->QueryInterface(IID_IMediaControl,
		reinterpret_cast<void**>(&g_pMediaControl2))))
		return hr;

	// Get the IMediaControl Interface
	if (FAILED(hr = g_pGraphBuilder2->QueryInterface(IID_IMediaSeeking,
		reinterpret_cast<void**>(&g_pMediaSeeking2))))
		return hr;

	if (FAILED(hr = g_pGraphBuilder2->QueryInterface(IID_IMediaEventEx,
		reinterpret_cast<void**>(&g_pME2))))
		return hr;
	g_pME2->SetNotifyWindow((OAHWND)g_Hwnd, WM_GRAPHNOTIFY2, 0);

	// Create Source Filter for first file
	g_iNextFile = 0;

	// Create the intial graph
	if (FAILED(hr = SwapSourceFilter()))
		return hr;

	// Set a timer for switching the sources
	//if(!SetTimer(g_hwndApp, 0, MEDIA_TIMEOUT, (TIMERPROC) MyTimerProc))
	  //  return E_FAIL;

	return S_OK;
}

//------------------------------------------------------------------------------
// Name: SwapSourceFilter()
// Desc: This routine is used to change the source file in the current graph.
//       First the graph is stopped, then the current source filter is removed.
//       The new source filter is added, the output pin on this filter is
//       rendered, and playback is restarted.
//
//       When this routine is called during initialization, there is no
//       currently running graph. In that case, Stop becomes a no-op. The source
//       filter is added to an empty graph. Then during the render call, all
//       necessary filters required to play this source are added to the graph.
//
//       On subsequent calls, Stopping the graph allows filters to be removed.
//       When the old source filter is removed, all other filters are still
//       left in the graph. The new source filter is added, and then the render
//       operation reconnects the graph. Since all of the necessary filters for
//       playback are already in the graph (if the two files have the same file
//       type), these filters are reused. Existing filters in the graph are
//       always used first, if possible, during a Render operation. This avoids
//       having to create new filter instances with each change.
//------------------------------------------------------------------------------
HRESULT SwapSourceFilter(void)
{
	HRESULT hr = S_OK;
	IPin* pPin = NULL;
	int nNextFileIndex = 0;
	TCHAR szFilename[MAX_PATH];
	WCHAR wFileName[MAX_PATH];

	// Determine the file to load based on DirectX Media path (from SDK)
	//nNextFileIndex = g_iNextFile % g_iNumFiles;
	//DXUtil_FindMediaFileCch( szFilename, MAX_PATH, pstrFiles[nNextFileIndex] );
	//DXUtil_FindMediaFileCch( szFilename, MAX_PATH, "Title.mp3" );
	// Make sure that this file exists
	if (g_GameStart == 0)
	{
		DWORD dwAttr = GetFileAttributes(L"music/title.mp3\0");
	}
	else
	{
		DWORD dwAttr = GetFileAttributes(pstrFiles[g_Hero_Scene]);
	}
	/*if(dwAttr == (DWORD) -1)
	{
		TCHAR sz[MAX_PATH];
		HRESULT hr = StringCchPrintf( sz, NUMELMS(sz), TEXT("Players\\BGMusic\\%s"), pstrFiles[nNextFileIndex] );
		DXUtil_FindMediaFileCch( szFilename, MAX_PATH, sz );

		dwAttr = GetFileAttributes(szFilename);
		if(dwAttr == (DWORD) -1)
		{
			TCHAR szMsg[MAX_PATH + 64];
			hr = StringCchPrintf(szMsg, NUMELMS(szMsg), TEXT("Can't find the media file [%s].\0"), szFilename);
			MessageBox(NULL, szMsg, TEXT("BGMusic Sample Error"), MB_OK | MB_ICONEXCLAMATION);
			return E_FAIL;
		}
	}*/

	if (g_GameStart == 0)
	{
		(void)StringCchCopy(g_szCurrentFile, NUMELMS(g_szCurrentFile), L"music/title.mp3\0");
		(void)StringCchCopy(szFilename, NUMELMS(szFilename), L"music/title.mp3\0");
	}
	else
	{
		(void)StringCchCopy(g_szCurrentFile, NUMELMS(g_szCurrentFile), pstrFiles[g_Hero_Scene]);
		(void)StringCchCopy(szFilename, NUMELMS(szFilename), pstrFiles[g_Hero_Scene]);
	}
	USES_CONVERSION;

	(void)StringCchCopyW(wFileName, MAX_PATH, T2W(szFilename));

	// OPTIMIZATION OPPORTUNITY
	// This will open the file, which is expensive. To optimize, this
	// should be done earlier, ideally as soon as we knew this was the
	// next file to ensure that the file load doesn't add to the
	// filter swapping time & cause a hiccup.
	//
	// Add the new source filter to the graph. (Graph can still be running)
	hr = g_pGraphBuilder->AddSourceFilter(wFileName, wFileName, &g_pSourceNext);

	// Get the first output pin of the new source filter. Audio sources
	// typically have only one output pin, so for most audio cases finding
	// any output pin is sufficient.
	if (SUCCEEDED(hr))
	{
		hr = g_pSourceNext->FindPin(L"Output", &pPin);
	}

	// Stop the graph
	if (SUCCEEDED(hr))
	{
		hr = g_pMediaControl->Stop();
	}

	// Break all connections on the filters. You can do this by adding
	// and removing each filter in the graph
	if (SUCCEEDED(hr))
	{
		IEnumFilters* pFilterEnum = NULL;

		if (SUCCEEDED(hr = g_pGraphBuilder->EnumFilters(&pFilterEnum)))
		{
			int iFiltCount = 0;
			int iPos = 0;

			// Need to know how many filters. If we add/remove filters during the
			// enumeration we'll invalidate the enumerator
			while (S_OK == pFilterEnum->Skip(1))
			{
				iFiltCount++;
			}

			// Allocate space, then pull out all of the
			IBaseFilter** ppFilters = reinterpret_cast<IBaseFilter**>
				(_alloca(sizeof(IBaseFilter*) * iFiltCount));
			pFilterEnum->Reset();

			while (S_OK == pFilterEnum->Next(1, &(ppFilters[iPos++]), NULL));

			SAFE_RELEASE(pFilterEnum);

			for (iPos = 0; iPos < iFiltCount; iPos++)
			{
				g_pGraphBuilder->RemoveFilter(ppFilters[iPos]);

				// Put the filter back, unless it is the old source
				if (ppFilters[iPos] != g_pSourceCurrent)
				{
					g_pGraphBuilder->AddFilter(ppFilters[iPos], NULL);
				}
				SAFE_RELEASE(ppFilters[iPos]);
			}
		}
	}

	// We have the new output pin. Render it
	if (SUCCEEDED(hr))
	{
		// Release the old source filter, if it exists
		SAFE_RELEASE(g_pSourceCurrent)

			hr = g_pGraphBuilder->Render(pPin);
		g_pSourceCurrent = g_pSourceNext;
		g_pSourceNext = NULL;
	}

	SAFE_RELEASE(pPin);
	SAFE_RELEASE(g_pSourceNext); // In case of errors

	// Re-seek the graph to the beginning
	if (SUCCEEDED(hr))
	{
		LONGLONG llPos = 0;
		hr = g_pMediaSeeking->SetPositions(&llPos, AM_SEEKING_AbsolutePositioning,
			&llPos, AM_SEEKING_NoPositioning);
	}

	// Start the graph
	if (SUCCEEDED(hr))
	{
		hr = g_pMediaControl->Run();
	}

	return S_OK;
}



HRESULT SwapSourceFilter2(int i)
{
	HRESULT hr = S_OK;
	IPin* pPin = NULL;
	int nNextFileIndex = 0;
	TCHAR szFilename[MAX_PATH];
	WCHAR wFileName[MAX_PATH];
	if (gPlaying == 0)
	{
		DWORD dwAttr = GetFileAttributes(pstrEffectFiles[i]);
		(void)StringCchCopy(g_szCurrentFile, NUMELMS(g_szCurrentFile), pstrEffectFiles[i]);
		(void)StringCchCopy(szFilename, NUMELMS(szFilename), pstrEffectFiles[i]);
		USES_CONVERSION;

		(void)StringCchCopyW(wFileName, MAX_PATH, T2W(szFilename));
		hr = g_pGraphBuilder2->AddSourceFilter(wFileName, wFileName, &g_pSourceNext2);
		if (SUCCEEDED(hr))
		{
			hr = g_pSourceNext2->FindPin(L"Output", &pPin);
		}

		// Stop the graph
		if (SUCCEEDED(hr))
		{
			hr = g_pMediaControl2->Stop();
		}
		if (SUCCEEDED(hr))
		{
			IEnumFilters* pFilterEnum = NULL;

			if (SUCCEEDED(hr = g_pGraphBuilder2->EnumFilters(&pFilterEnum)))
			{
				int iFiltCount = 0;
				int iPos = 0;

				// Need to know how many filters. If we add/remove filters during the
				// enumeration we'll invalidate the enumerator
				while (S_OK == pFilterEnum->Skip(1))
				{
					iFiltCount++;
				}

				// Allocate space, then pull out all of the
				IBaseFilter** ppFilters = reinterpret_cast<IBaseFilter**>
					(_alloca(sizeof(IBaseFilter*) * iFiltCount));
				pFilterEnum->Reset();

				while (S_OK == pFilterEnum->Next(1, &(ppFilters[iPos++]), NULL));

				SAFE_RELEASE(pFilterEnum);

				for (iPos = 0; iPos < iFiltCount; iPos++)
				{
					g_pGraphBuilder2->RemoveFilter(ppFilters[iPos]);

					// Put the filter back, unless it is the old source
					if (ppFilters[iPos] != g_pSourceCurrent2)
					{
						g_pGraphBuilder2->AddFilter(ppFilters[iPos], NULL);
					}
					SAFE_RELEASE(ppFilters[iPos]);
				}
			}
		}

		// We have the new output pin. Render it
		if (SUCCEEDED(hr))
		{
			// Release the old source filter, if it exists
			SAFE_RELEASE(g_pSourceCurrent2)

				hr = g_pGraphBuilder2->Render(pPin);
			g_pSourceCurrent2 = g_pSourceNext2;
			g_pSourceNext2 = NULL;
		}

		SAFE_RELEASE(pPin);
		SAFE_RELEASE(g_pSourceNext2); // In case of errors


	}
	// Re-seek the graph to the beginning


	// Start the graph

	if (g_effectRunning == 1)
	{
		LONGLONG llPos = 0;
		hr = g_pMediaSeeking2->SetPositions(&llPos, AM_SEEKING_AbsolutePositioning,
			&llPos, AM_SEEKING_NoPositioning);

		gPlaying = 1;
		hr = g_pMediaControl2->Run();
		g_effectRunning = 0;

	}
	return S_OK;
}


HRESULT HandleGraphEvent(void)
{
	LONG evCode;
	LONG_PTR evParam1, evParam2;
	HRESULT hr = S_OK;

	// Make sure that we don't access the media event interface
	// after it has already been released.
	if (!g_pME)
		return S_OK;

	// Process all queued events
	while (SUCCEEDED(g_pME->GetEvent(&evCode, &evParam1, &evParam2, 0)))
	{
		// Free memory associated with callback, since we're not using it
		hr = g_pME->FreeEventParams(evCode, evParam1, evParam2);

		// If this is the end of the clip, reset to beginning
		if (EC_COMPLETE == evCode)
		{

			LONGLONG llPos = 0;
			hr = g_pMediaSeeking->SetPositions(&llPos, AM_SEEKING_AbsolutePositioning,
				&llPos, AM_SEEKING_NoPositioning);
			if (SUCCEEDED(hr))
			{
				hr = g_pMediaControl->Run();
			}
		}
	}

	return hr;
}

HRESULT HandleGraphEvent2(void)
{
	LONG evCode;
	LONG_PTR evParam1, evParam2;
	HRESULT hr = S_OK;

	// Make sure that we don't access the media event interface
	// after it has already been released.
	if (!g_pME)
		return S_OK;

	// Process all queued events
	while (SUCCEEDED(g_pME2->GetEvent(&evCode, &evParam1, &evParam2, 0)))
	{
		// Free memory associated with callback, since we're not using it
		hr = g_pME->FreeEventParams(evCode, evParam1, evParam2);

		// If this is the end of the clip, reset to beginning
		if (EC_COMPLETE == evCode)
		{

			g_effectRunning = 1;
		}
	}

	return hr;
}







void Create_3DSprite(IDirect3DDevice9* pd3dDevice, LPDIRECT3DTEXTURE9& Test_Texture, LPDIRECT3DVERTEXBUFFER9& Test_Buffer, LPCTSTR lpstr, float x, float y, float width, float height)
{
	TCHAR szFilename[MAX_PATH];
	WCHAR wFileName[MAX_PATH];
	if (Test_Buffer == NULL)
	{
		pd3dDevice->CreateVertexBuffer(
			6 * sizeof(BITMAPVERTEX),
			D3DUSAGE_WRITEONLY,
			D3DFVF_BITMAPVERTEX,
			D3DPOOL_MANAGED,
			&Test_Buffer,
			0);
	}

	BITMAPVERTEX* v;
	Test_Buffer->Lock(0, 0, (void**)&v, 0);

	// quad built from two triangles, note texture coordinates:

	v[0] = BITMAPVERTEX(x, y, 1.f, 1.f, 0.0f, 0.01f);

	v[1] = BITMAPVERTEX(x + width, y, 1.f, 1.f, 1.0f, 0.01f);

	v[2] = BITMAPVERTEX(x, y + height, 1.f, 1.f, 0.0f, 1.0f);


	v[3] = BITMAPVERTEX(x, y + height, 1.f, 1.f, 0.0f, 1.0f);

	v[4] = BITMAPVERTEX(x + width, y, 1.f, 1.f, 1.0f, 0.01f);
	v[5] = BITMAPVERTEX(x + width, y + height, 1.f, 1.f, 1.0f, 1.0f);

	Test_Buffer->Unlock();

	//
	// Create the texture and set filters.
	//

	(void)StringCchCopy(szFilename, NUMELMS(szFilename), lpstr);
	USES_CONVERSION;

	(void)StringCchCopyW(wFileName, MAX_PATH, T2W(szFilename));

	D3DXCreateTextureFromFile(
		pd3dDevice,
		wFileName,
		&Test_Texture);
}


void Display2(IDirect3DDevice9* pd3dDevice, LPDIRECT3DTEXTURE9& Test_Texture, LPDIRECT3DVERTEXBUFFER9& Test_Buffer, int iUpdate)
{
	if (iUpdate == 1 || g_Char_Exchange2 == 0)
	{
		BITMAPVERTEX* v;

		Test_Buffer->Lock(0, 0, (void**)&v, 0);

		v[0] = BITMAPVERTEX(g_temp, g_height / 8, 1.f, 1.f, 0.0f, 0.0f);
		v[1] = BITMAPVERTEX(g_temp + (g_width / 2), g_height / 8, 1.f, 1.f, 1.0f, 0.0f);
		v[2] = BITMAPVERTEX(g_temp, g_height, 1.f, 1.f, 0.0f, 1.0f);
		v[3] = BITMAPVERTEX(g_temp, g_height, 1.f, 1.f, 0.0f, 1.0f);
		v[4] = BITMAPVERTEX(g_temp + (g_width / 2), g_height / 8, 1.f, 1.f, 1.0f, 0.0f);
		v[5] = BITMAPVERTEX(g_temp + (g_width / 2), g_height, 1.f, 1.f, 1.0f, 1.0f);


		Test_Buffer->Unlock();
		//g_temp2=1;
	}
	g_Char_Exchange2 = 1;

	pd3dDevice->SetTexture(0, Test_Texture);
	//pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);


	pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


	//
	// Don't use lighting for this sample.
	//
	pd3dDevice->SetRenderState(D3DRS_LIGHTING, false);
	/*
		pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );

		pd3dDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
		pd3dDevice->SetRenderState( D3DRS_ZENABLE,      TRUE );
		*/



		// Alpha
	pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//
	// Set the projection matrix.
	//

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f, // 90 - degree
		(float)640 / (float)480,
		1.0f,
		1000.0f);
	pd3dDevice->SetTransform(D3DTS_PROJECTION, &proj);
	pd3dDevice->SetStreamSource(0, Test_Buffer, 0, sizeof(BITMAPVERTEX));
	pd3dDevice->SetFVF(D3DFVF_BITMAPVERTEX);
	pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
	// pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );	
}
int first_start = 0;

void Character_Sprite(float fElapsedTime)
{
	float i = g_width / 2;
	if (first_start == 0)
	{
		first_start = 1;
		return;
	}
	if (g_temp < ((i)-(i / 2)))
	{
		g_temp = g_temp + ((fElapsedTime * 900.0f) * ((g_width * 2.f) / g_Defaut_Width));
		if (g_temp >= ((i)-(i / 2)))
			g_temp = (i)-(i / 2);
	}
	else
	{
		g_Char_Exchange = 0;
	}
	g_DownFact *= g_DownFact;
}

void Display(IDirect3DDevice9* pd3dDevice, LPDIRECT3DTEXTURE9& Test_Texture, LPDIRECT3DVERTEXBUFFER9& Test_Buffer, int iUpdate) {
	pd3dDevice->SetTexture(0, Test_Texture);
	pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);


	pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	/*

		//
		// Don't use lighting for this sample.
		//
		pd3dDevice->SetRenderState(D3DRS_LIGHTING, false);

		pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );

		pd3dDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
		pd3dDevice->SetRenderState( D3DRS_ZENABLE,      TRUE );*/




		// Alpha
	pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//
	// Set the projection matrix.
	//

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f, // 90 - degree
		(float)640 / (float)480,
		1.0f,
		1000.0f);
	pd3dDevice->SetTransform(D3DTS_PROJECTION, &proj);
	pd3dDevice->SetStreamSource(0, Test_Buffer, 0, sizeof(BITMAPVERTEX));
	pd3dDevice->SetFVF(D3DFVF_BITMAPVERTEX);
	pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
	// pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );	
}

void Show_Inventory_Item(IDirect3DDevice9* pd3dDevice, LPDIRECT3DTEXTURE9& Test_Texture, LPDIRECT3DVERTEXBUFFER9& Test_Buffer, int iUpdate)
{

	pd3dDevice->SetTexture(0, Test_Texture);
	pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pd3dDevice->SetRenderState(D3DRS_LIGHTING, false);
	pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f, // 90 - degree
		(float)640 / (float)480,
		1.0f,
		1000.0f);
	pd3dDevice->SetTransform(D3DTS_PROJECTION, &proj);
	pd3dDevice->SetStreamSource(0, Test_Buffer, 0, sizeof(BITMAPVERTEX));
	pd3dDevice->SetFVF(D3DFVF_BITMAPVERTEX);
	if (iUpdate == 1)
	{
		for (int i = 0; i < 8; i++)
		{
			BITMAPVERTEX* v;
			Test_Buffer->Lock(0, 0, (void**)&v, 0);
			v[0] = BITMAPVERTEX(g_width / 8.0f * i, 0, 1.f, 1.f, 0.0f, 0.0f);
			v[1] = BITMAPVERTEX((g_width / 8.0f) + (g_width / 8.0f * i), 0, 1.f, 1.f, 1.0f, 0.0f);
			v[2] = BITMAPVERTEX(g_width / 8.0f * i, g_height / 8.0f, 1.f, 1.f, 0.0f, 1.0f);
			v[3] = BITMAPVERTEX(g_width / 8.0f * i, g_height / 8.0f, 1.f, 1.f, 0.0f, 1.0f);
			v[4] = BITMAPVERTEX((g_width / 8.0f) + (g_width / 8.0f * i), 0, 1.f, 1.f, 1.0f, 0.0f);
			v[5] = BITMAPVERTEX((g_width / 8.0f) + (g_width / 8.0f * i), g_height / 8.0f, 1.f, 1.f, 1.0f, 1.0f);
			Test_Buffer->Unlock();
			pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
		}
	}
	else
	{
		BITMAPVERTEX* v;
		Test_Buffer->Lock(0, 0, (void**)&v, 0);
		v[0] = BITMAPVERTEX(5.f, 5.f, 1.f, 1.f, 0.0f, 0.0f);
		v[1] = BITMAPVERTEX((g_width / 8.0f) - 10.f, 5.f, 1.f, 1.f, 1.0f, 0.0f);
		v[2] = BITMAPVERTEX(5.f, (g_height / 8.0f) - 10.f, 1.f, 1.f, 0.0f, 1.0f);
		v[3] = BITMAPVERTEX(5.f, (g_height / 8.0f) - 10.f, 1.f, 1.f, 0.0f, 1.0f);
		v[4] = BITMAPVERTEX((g_width / 8.0f) - 10.f, 5.f, 1.f, 1.f, 1.0f, 0.0f);
		v[5] = BITMAPVERTEX((g_width / 8.0f) - 10.f, (g_height / 8.0f) - 10.f, 1.f, 1.f, 1.0f, 1.0f);
		Test_Buffer->Unlock();
		pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
	}
}

void CALLBACK OnGUIMenuEvent(UINT nEvent, int nControlID, CDXUTControl* pControl)
{
	switch (nControlID)
	{
	case IDC_MOVE:
		g_Hero_Scene = 1;
		g_menu.RemoveAllControls();
		g_bProcessKey = 0; g_LineRead = 0;

		g_bTextReading_Terminate = 0;
		if (Scene[g_Hero_Scene].access_script_no == 1)
		{
			g_KeyBoard_Acess = 1;
			g_bTextReading = 1;
		}
		else
		{
			g_bTextReading = 0;
			g_KeyBoard_Acess = 0;
		}
		ghj = 0;

		SwapSourceFilter();
		break;

	}
}




void InitMenu()
{
	g_menu.SetCallback(OnGUIMenuEvent);
	g_menu.SetFont(1, L"Comic Sans MS", 24, FW_NORMAL);
	g_menu.SetFont(2, L"Courier New", 16, FW_NORMAL);
	g_menu.AddButton(IDC_MOVE, L"이동", 560, 360, 80, 35, L'S');
	g_menu.SetLocation(0, 0);
	g_menu.SetSize(640, 480);
	g_menu.GetControl(IDC_MOVE)->SetLocation(560, 360);
	g_menu_set = 1;
}






void CALLBACK OnGUIMenuEvent2(UINT nEvent, int nControlID, CDXUTControl* pControl)
{
	switch (nControlID)
	{
	case IDC_MOVE:
		g_KeyBoard_Acess = 1;
		g_game_state = 8;
		g_menu.RemoveAllControls();
		break;

	case IDC_STAY:
		g_game_state = 4;
		g_KeyBoard_Acess = 1;
		g_menu.RemoveAllControls();
		g_menu_set = 0;
		break;

	}
}




void InitMenu2()
{
	g_menu.SetCallback(OnGUIMenuEvent2);
	g_menu.SetFont(1, L"Comic Sans MS", 24, FW_NORMAL);
	g_menu.SetFont(2, L"Courier New", 16, FW_NORMAL);
	g_menu.AddButton(IDC_MOVE, L"이동", g_width - ((g_width / 8) * 1), g_height - ((g_height / 6) * 2), (g_width / 8), (g_height / 13.7f), L'S');
	g_menu.AddButton(IDC_STAY, L"머뭄", g_width - ((g_width / 8) * 2), g_height - ((g_height / 6) * 2), (g_width / 8), (g_height / 13.7f), L'S');
	g_menu.SetLocation(0, 0);
	g_menu.GetControl(IDC_MOVE)->SetLocation(g_width - ((g_width / 8) * 1), g_height - ((g_height / 6) * 2));
	g_menu.GetControl(IDC_STAY)->SetLocation(g_width - ((g_width / 8) * 2), g_height - ((g_height / 6) * 2));
	g_menu.SetLocation(0, 0);
	g_menu.SetSize(640, 480);
	g_menu_set = 1;
	g_KeyBoard_Acess = 0;
	g_game_state = 7;



}


void script_analyzer()
{
	char buf[MAX_PATH];
	int count = 0;
	g_extend = 0;
	g_extend_number = -1;


	WideCharToMultiByte(CP_ACP, NULL,
		g_pstrScript[g_Hero_Scene], -1,
		buf, sizeof(buf),
		NULL, NULL);



	g_fp = fopen(buf, "r");
	if (g_fp == NULL)
		MessageBox(NULL, L"S", L"D", MB_OK);

	fgets(g_string, MAX_PATH, g_fp);
	while (!feof(g_fp))
	{
		char seps[] = " "; //seperator입니다					
		char seps2[] = "\n"; //seperator입니다					
		char seps3[] = "@"; //seperator입니다					
		char* token;

		token = strtok(g_string, seps);
		if ((!strcmp(token, "START")) && count == 0)
		{
			fgets(g_string, MAX_PATH, g_fp);
			continue;
		}
		if ((!strcmp(token, "TEXT")) && count == 0)
		{
			token = strtok(NULL, seps);
			if (!strcmp(token, "LABLE"))
			{
				token = strtok(NULL, seps);
				if (!strcmp(token, "REPEAT"))
				{
					g_lable = 3;
					token = strtok(NULL, seps3);
					strcpy(g_script[0], token);
					token = strtok(NULL, seps3);
					strcpy(g_script[18], token);
					token = strtok(NULL, seps2);
					strcpy(g_script[19], token);
				}
				else
				{

					g_lable = 2;
					token = strtok(NULL, seps3);
					strcpy(g_script[0], token);
					token = strtok(NULL, seps2);
					strcpy(g_script[18], token);
				}
			}
			else
			{
				token = strtok(NULL, seps);
				if (!strcmp(token, "REPEAT"))
				{
					g_lable = 1;
					token = strtok(NULL, seps3);
					strcpy(g_script[0], token);
					token = strtok(NULL, seps2);
					strcpy(g_script[19], token);
				}
				else
				{
					g_lable = 0;
					token = strtok(NULL, seps);
					strcpy(g_script[0], token);
				}
			}
			count++;
			fgets(g_string, MAX_PATH, g_fp);
			continue;
		}
		if (!strcmp(token, "TEXT") && count != 0)
		{
			token = strtok(NULL, seps2);
			strcpy(g_script[count], token);
			count++;
		}
		if (!strcmp(token, "TEXT_EXTEND") && count != 0)
		{
			token = strtok(NULL, seps);
			g_extend = atoi(token);
			g_extend_number = count;
			token = strtok(NULL, seps2);
			strcpy(g_script[count], token);
			count++;
		}
		fgets(g_string, MAX_PATH, g_fp);
	}
	g_message_number = count - g_extend;
	g_message_extend = g_extend;
	g_script_analysis = 1;
	g_count = count;
	g_game_state = 4;
	fclose(g_fp);
}

void Text_Parse()
{
	char buf[MAX_PATH];
	int count = 0;
	char seps[] = " "; //seperator입니다					
	char seps2[] = "\n"; //seperator입니다					
	char seps3[] = "@"; //seperator입니다					

	char* token;
	WideCharToMultiByte(CP_ACP, NULL,
		g_pstrScript[g_Hero_Scene], -1,
		buf, sizeof(buf),
		NULL, NULL);
	if (g_fp)
		fclose(g_fp);
	g_fp = fopen(buf, "r");
	if (g_fp == NULL)
		MessageBox(NULL, L"S", L"D", MB_OK);
	if (g_juhang3 == 1)
	{
		count = g_extend_number + g_selected_extend_message + 2;
		g_juhang3 = 0;
	}
	else
	{
		count = g_selected_message + 1;
	}
	fgets(g_string, MAX_PATH, g_fp);
	while (count)
	{
		token = strtok(g_string, seps);

		if ((!strcmp(token, "START")) && count == 0)
		{
			fgets(g_string, MAX_PATH, g_fp);
			continue;
		}

		if (!strcmp(token, "TEXT"))
		{
			count--;
		}
		if (!strcmp(token, "TEXT_EXTEND") && count != 0)
		{
			count--;
		}
		fgets(g_string, MAX_PATH, g_fp);
	}
	int i = 0;
	strcpy(g_message_data[i], g_string);
	strcpy(g_message_data2[i], g_string);
	i++;
	fgets(g_string, MAX_PATH, g_fp);
	while (!feof(g_fp))
	{
		strcpy(g_message_data[i], g_string);
		strcpy(g_message_data2[i], g_string);
		i++;
		token = strtok(g_string, seps);
		if (!strcmp(token, "TEXT"))
			break;
		if (!strcmp(token, "TEXT_END"))
			break;
		if (!strcmp(token, "TEXT_EXTEND"))
			break;

		fgets(g_string, MAX_PATH, g_fp);
	}
	i--;
	g_message_length = i;
	g_index = 0;
	g_current_index = 0;
	g_message_count = 0;
	fclose(g_fp);
}




void GetText2(float fElapsedTime)
{

	char seps[] = " "; //seperator입니다					
	char seps2[] = "\n"; //seperator입니다					
	char seps3[] = "@"; //seperator입니다					
	char* token;
	if (g_bProcessKey == 1 && g_bTextReading_Terminate == 0 && g_message_count == 0)
	{
		g_KeyBoard_Acess = 0;
		token = strtok(g_message_data[g_index], seps);
		if (!strcmp(token, "SPEAKING"))
		{
			g_name_counter++;
			token = strtok(NULL, seps2);
			strcpy(g_name_str, token);
			g_index++;
			g_current_index = g_index;
		}
		while (1)
		{
			token = strtok(g_message_data[g_index], seps);
			if (!strcmp(token, "SPEAKING"))
			{
				strcpy(g_message_data[g_index], g_message_data2[g_index]);
				break;
			}
			if (!strcmp(token, "ENABLE"))
			{
				token = strtok(NULL, seps2);

				int b = atoi(token);
				if (ACT1_LOC[b] != 1)
				{
					g_can_go++;
					ACT1_LOC[b] = 1;
				}
				g_juhang = 1;
				g_name_counter++;
				break;
			}
			if (!strcmp(token, "AUTO"))
			{
				g_act1_auto = 1;
				g_juhang = 1;
				g_name_counter++;
				break;
			}
			if (!strcmp(token, "TEXT_EXTEND"))
			{
				g_name_counter++;
				g_juhang = 1;
				break;
			}
			if (!strcmp(token, "TEXT_END"))
			{
				g_name_counter++;
				g_juhang = 1;
				break;
			}
			g_index++;
			if (g_index >= g_message_length)
			{
				g_juhang = 1;
				break;
			}

		}

		g_message_count = g_index - g_current_index;
	}
}

void GetText3(float fElapsedTime)
{
	if (g_bProcessKey == 1 && g_bTextReading_Terminate == 0)
	{
		if (g_message_count > 0)
		{
			g_LineRead++;
			g_juhang2++;
			strcpy(g_string, g_message_data2[g_current_index]);
			g_current_index++;
			g_message_count--;
		}
		if (g_message_count > 0)
		{
			g_juhang2++;
			g_LineRead++;
			strcpy(g_string2, g_message_data2[g_current_index]);
			g_current_index++;
			g_message_count--;
		}

		g_bProcessKey = 0;

	}


	if (g_LineRead == 1)
	{
		MultiByteToWideChar(CP_ACP, 0, g_string, -1,
			g_wFileName, MAX_PATH);
		//MultiByteToWideChar( CP_ACP, 0, "", -1,                          
		//							g_wFileName2, MAX_PATH );
	}
	else
	{
		MultiByteToWideChar(CP_ACP, 0, g_string, -1,
			g_wFileName, MAX_PATH);
		MultiByteToWideChar(CP_ACP, 0, g_string2, -1,
			g_wFileName2, MAX_PATH);
	}










	int k, t;
	char buf[MAX_PATH];
	char buf2[MAX_PATH];
	g_d3dHangul2.SetWidthHeight((int)(g_width / 30), (int)(g_height / 24));
	g_d3dHangul2.SetColor(0x000ff);
	k = strlen(g_string);
	t = strlen(g_string2);
	if (g_LineRead == 1) // 이부분은 한줄을 읽었을 때의 경우가 존재할때를 말한다.(대사의 끝부분의 경우)
	{
		strcpy(buf, g_string);
		buf[qbt] = 0;
		MultiByteToWideChar(CP_ACP, 0, buf, -1,
			g_wFileName, MAX_PATH);
		g_d3dHangul2.OutHanXY(g_width / 20, g_height - (g_height / 10), g_wFileName);
		if (g_KeyBoard_Acess == 0)
		{
			int i = qbt - 1;
			if (i > 0 && buf[i] == ' ')
				;
			else; SwapSourceFilter2(0);
		}
		qbt++;
		if (qbt > k)
		{
			qbt = k;
			g_KeyBoard_Acess = 1;

		}
	}
	if (g_LineRead == 2)
	{
		strcpy(buf, g_string);
		strcpy(buf2, g_string2);
		buf[qbt] = 0;
		buf2[iop] = 0;
		MultiByteToWideChar(CP_ACP, 0, buf, -1,
			g_wFileName, MAX_PATH);
		MultiByteToWideChar(CP_ACP, 0, buf2, -1,
			g_wFileName2, MAX_PATH);
		g_d3dHangul2.OutHanXY(g_width / 20, g_height - (g_height / 8), g_wFileName);
		g_d3dHangul2.OutHanXY(g_width / 20, g_height - (g_height / 12), g_wFileName2);
		if (g_KeyBoard_Acess == 0)
		{
			int i = qbt - 1;
			if (i > 0 && buf[i] == ' ')
				;
			else; SwapSourceFilter2(0);
		}
		qbt++;
		if (qbt > k)
			iop++;
		if (qbt > k && iop > t)
		{
			qbt = k;
			iop = t;
			g_KeyBoard_Acess = 1;


		}
	}

	MultiByteToWideChar(CP_ACP, 0, g_name_str, -1,
		g_wFileName, MAX_PATH);
	g_d3dHangul2.OutHanXY(20 * (g_width / 640), 390 * (g_height / 480), g_wFileName);

	Sleep(10);
}


void Menu_Extend_Display()
{
	RenderText();
}

