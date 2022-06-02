//--------------------------------------------------------------------------------------
// File: CustomUI.cpp
//
// Starting point for new Direct3D applications
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "dxstdafx.h"
#include "constant.h"
#include "resource.h"
#include <stdio.h>
#include <atlbase.h>
#include <dshow.h>
#include "D3DHangul.h"
#include "CD3DSprite.h"
int g_point = 0;
bool bspecial = false;
LPCTSTR g_gameover[] =
{
TEXT("GAME OVER"),
};
LPCTSTR g_pstrTime[] =
{
	TEXT("고등학교 학교 정문 : 밤11시"),
	TEXT("3층 교실2 : 밤 11시 30분"),
	TEXT("4층계단 : 밤 11시 55분"),
	TEXT("1층 복도2 : 새벽 00시 10분"),
	TEXT("3층 복도 : 새벽 00시 20분"),
	TEXT("1층 교무실 : 새벽 00시 30분"),
	TEXT("3층 화장실 : 새벽 00시 35분"),
	TEXT("1층 복도2 : 새벽 00시 45분"),
	TEXT("2층 교실: 새벽 00시 55분"),
	TEXT("4층 복도2 : 새벽 01시 00분"),
	TEXT("옥상 : 새벽 01시 10분"),
	TEXT("교회 : 사건 이틀후"),
	TEXT("4층 복도2 : 새벽 01시 30분"),
};
void Item_Set();
void Init_Script_Var();
bool Gnerate_Next_Scene(int i);
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
#define IDC_CONV             22
#define IDC_SEARCH            23

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


bool Create_3DSprite(IDirect3DDevice9* pd3dDevice, LPDIRECT3DTEXTURE9& Test_Texture, LPDIRECT3DVERTEXBUFFER9& Test_Buffer, LPCTSTR lpstr, float x, float y, float width, float height, bool a, D3DCOLOR TransparencyColor);
void Display(IDirect3DDevice9* pd3dDevice, LPDIRECT3DTEXTURE9& Test_Texture, LPDIRECT3DVERTEXBUFFER9& Test_Buffer, int iUpdate);
void Display2(IDirect3DDevice9* pd3dDevice, LPDIRECT3DTEXTURE9& Test_Texture, LPDIRECT3DVERTEXBUFFER9& Test_Buffer, int iUpdate);

int g_juhang = 0;
int g_juhang2 = 0;
int g_name_counter = 0;
void Menu_Display2();
void Check_Move_Mode();
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

//LPDIRECT3DVERTEXBUFFER9 g_pVB_Opening        = NULL; // Buffer to hold vertices
//LPDIRECT3DTEXTURE9      g_pTexture_Opening   = NULL; // Our texture

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
int GetText_1(float fElapsedTime);
void GetText_2(float fElapsedTime);
LPCTSTR g_pstrCharater_Name[] =
{
	TEXT("image\\char\\7_Char.png"),
	TEXT("image\\char\\3_Char.png"),
	TEXT("image\\char\\1_Char.png"),
	TEXT("image\\char\\4_Char.png"),
	TEXT("image\\char\\5_Char.png"),
	TEXT("image\\char\\10_Char.png"),
	TEXT("image\\char\\6_Char.png"),
	TEXT("image\\char\\2_Char.png"),
	TEXT("image\\char\\8_Char.png"),
	TEXT("image\\char\\10_Char.png"),
};
float g_temp = -320.0f;
float g_DownFact = 0.99999999f;
int g_Char_Exchange = 1;
int g_Char_Exchange2 = 1;
int g_Char_Selected_Number = 0;
LPDIRECT3DVERTEXBUFFER9 g_pVB_Character[10] = { NULL }; // Buffer to hold vertices
LPDIRECT3DTEXTURE9     g_pTexture_Character[10] = { NULL }; // Our texture


LPCTSTR g_pstrItem[] =
{
	TEXT("연필"),
	TEXT("악보"),
	TEXT("카메라"),
	TEXT("플로피 디스크"),
	TEXT("시디"),
	TEXT("인형"),
	TEXT("나이프"),
	TEXT("음악실 열쇠"),
	TEXT("실습실 열쇠"),
	TEXT("빵"),
	TEXT("한장의 문서"),
	TEXT("라이터"),
	TEXT("담배"),
	TEXT("3층 교실1 열쇠"),
	TEXT("실험실 열쇠"),
	TEXT("물리책"),
	TEXT("안경"),
	TEXT("유리거울"),
	TEXT("예비"),
	TEXT("예비"),
};
LPCTSTR g_pstrItem2[] =
{
	TEXT("연필을 얻었다"),
	TEXT("악보를 얻었다"),
	TEXT("카메라를 얻었다"),
	TEXT("플로피 디스크를 얻었다."),
	TEXT("시디를 얻었다"),
	TEXT("인형을 얻었다."),
	TEXT("칼을 얻었다"),
	TEXT("음악실 열쇠를 얻었다."),
	TEXT("실습실 열쇠를 얻었다."),
	TEXT("빵을 얻었다"),
	TEXT("한장의 문서를 얻었다."),
	TEXT("라이터를 얻었다."),
	TEXT("담배를 얻었다."),
	TEXT("3층 교실 1열쇠를 얻었다."),
	TEXT("실험실 열쇠를 얻었다."),
	TEXT("물리책을 얻었다."),
	TEXT("안경을 얻었다."),
	TEXT("유리거울을 얻었다"),
	TEXT("예비"),
	TEXT("예비"),
};

LPCTSTR g_pstrItem3[] =
{
	TEXT("깎고 나서 한번도 사용되지 않은 것 같다."),
	TEXT("어떤 의미의 악보일까.. 시도레미"),
	TEXT("안에 어떤 내용이 찍혀 있는지 궁금하다."),
	TEXT("요즘 시대에도 플로피디스크 쓰는 사람이?."),
	TEXT("요즘 시대에도 시디 쓰는 사람이?"),
	TEXT("인형을 얻었다. 예쁜 아이 인형이다."),
	TEXT("식칼용 칼인 것 같다. 피가 묻어있다."),
	TEXT("열쇠가 다 똑같겠지만.. 라벨이 있다."),
	TEXT("열쇠가 다 똑같겠지만.. 라벨이 있다."),
	TEXT("이걸보니 배가 점점 고파진다."),
	TEXT("어떤 내용인지는.. 어두워서 볼수 없다."),
	TEXT("기름이 다 떨어진 라이터이다.."),
	TEXT("2개피밖에 들어 있지 않다.."),
	TEXT("열쇠가 다 똑같겠지만.. 라벨이 있다."),
	TEXT("열쇠가 다 똑같겠지만.. 라벨이 있다."),
	TEXT("역학 수식으로 가득차 있다. 그런데 역학은.."),
	TEXT("특별한 점은 보이지 않는다."),
	TEXT("형광물질이 있어 거울에 내 모습이 조금은 보인다."),
	TEXT("예비"),
	TEXT("예비"),
};

LPCTSTR g_pstrItem_Name[] =
{
	TEXT("image\\item\\Item1.jpg"),
	TEXT("image\\item\\Item2.jpg"),
	TEXT("image\\item\\Item3.jpg"),
	TEXT("image\\item\\Item4.tga"),
	TEXT("image\\item\\Item5.tga"),
	TEXT("image\\item\\Item6.tga"),
	TEXT("image\\item\\Item7.tga"),
	TEXT("image\\item\\Item8.tga"),
	TEXT("image\\item\\display.jpg"),
	TEXT("image\\item\\warning.tga"),
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
void Show_Inventory_Item(IDirect3DDevice9* pd3dDevice, LPDIRECT3DTEXTURE9& Test_Texture, LPDIRECT3DVERTEXBUFFER9& Test_Buffer, int iUpdate, int a);
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

TCHAR          g_szCurrentFile[128];
// File names & variables to track current file
// These files are installed with the DirectX SDK into the samples media folder
LPCTSTR pAct1_str[] =
{
	TEXT(""),
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
	TEXT("music/scene5.wav\0"),
	TEXT("music/scene6.mp3\0"),
	TEXT("music/scene7.wav\0"),
	TEXT("music/scene2.mp3\0"),
	TEXT("music/scene3.mp3\0"),
	TEXT("music/scene8.mp3\0"),
	TEXT("music/ending.mp3\0"),
	TEXT("music/scene1.mp3\0"),
	TEXT("music/scene1.mp3\0"),
	TEXT("music/scene2.mp3\0"),
	TEXT("music/scene3.mp3\0"),
	TEXT("music/scene4.mp3\0"),
	TEXT("music/scene4.mp3\0"),
	TEXT("music/scene1.mp3\0"),
	TEXT("music/scene1.mp3\0"),
	TEXT("music/scene2.mp3\0"),
	TEXT("music/scene3.mp3\0"),
	TEXT("music/scene4.mp3\0"),
	TEXT("music/scene4.mp3\0"),
	TEXT("music/scene1.mp3\0"),
	TEXT("music/scene1.mp3\0"),
	TEXT("music/scene2.mp3\0"),
	TEXT("music/scene3.mp3\0"),
	TEXT("music/scene4.mp3\0"),
	TEXT("music/scene4.mp3\0"),
	TEXT("music/scene1.mp3\0"),
	TEXT("music/scene1.mp3\0"),
	TEXT("music/scene2.mp3\0"),
	TEXT("music/scene3.mp3\0"),
	TEXT("music/scene4.mp3\0"),
	TEXT("music/scene4.mp3\0"),
	TEXT("music/scene1.mp3\0"),
};

LPCTSTR pstrEffectFiles[] =
{
	TEXT("effect/effect1.WAV\0"),
	TEXT("effect/effect2.mp3\0"),
	TEXT("effect/effect3.mp3\0"),
	TEXT("effect/effect4.WAV\0"),
	TEXT("effect/effect5.wav\0"),
	TEXT("effect/effect6.wav\0"),
	TEXT("effect/effect7.wav\0"),
	TEXT("effect/effect8.wav\0"),
	TEXT("effect/effect9.wav\0"),//8
	TEXT("effect/effect10.wav\0"),
	TEXT("effect/effect11.wav\0"),
	TEXT("effect/effect12.wav\0"),
};


int g_iNumFiles = 3, g_iNextFile = 0;


LPCTSTR pAct2_str[] =
{

	TEXT("1층 여자화장실 바깥"),
	TEXT("3층 여자화장실 바깥"),
	TEXT("학교 출구 앞"),
	TEXT("1층 입구"),
	TEXT("1층 계단"),
	TEXT("2층 계단"),
	TEXT("3층 계단"),
	TEXT("4층 계단"),
	TEXT("학교 옥상"),
	TEXT("실험실"),
	TEXT("실습실"),
	TEXT("양호실"),
	TEXT("교무실"),
	TEXT("매점"),
	TEXT("1층 교실"),
	TEXT("2층 교실"),
	TEXT("3층 교실1"),
	TEXT("3층 교실2"),
	TEXT("4층 교실"),
	TEXT("음악실"),
	TEXT("미술실"),
	TEXT("1층 여자화장실 내부"),
	TEXT("3층 여자화장실 내부"),
	TEXT("1층 남자화장실 외부"),
	TEXT("3층 남자화장실 외부"),
	TEXT("1층 남자화장실 내부"),
	TEXT("3층 남자화장실 내부"),
	TEXT("1층 복도1"),
	TEXT("2층 복도1"),
	TEXT("3층 복도"),
	TEXT("4층 복도1"),
	TEXT("1층 복도2"),
	TEXT("2층 복도2"),
	TEXT("4층 복도2"),
	TEXT("체육관"),
	TEXT("교회"),
};

WCHAR g_wFileName[MAX_PATH];
WCHAR g_wFileName2[MAX_PATH];
void Scene_Initialize();
FILE* g_fp = NULL;
char g_string[MAX_PATH];
char g_string2[MAX_PATH];
void GetFileHandle();
void GetText(float fElapsedTime);
bool LoadTexture(LPDIRECT3DTEXTURE9* ppTexture, LPDIRECT3DVERTEXBUFFER9& Test_Buffer, LPCTSTR strFileName, bool bUseTransparency, D3DCOLOR TransparencyColor);
//게임 내용 관련 데이터들, 진행하면서 바뀌는 데이터들 저장 및 상황에 맞는 메시지 출력

struct Char_Info
{
	int	Char_ID; // 이름 등에 접근하기 위함
	int location; // 현재 캐릭터가 있는 장소
	int present; // 그 장소에 캐릭터가 보일지의 여부
	int state; // 캐릭터의 심리상태
};

struct Char_Info2
{
	int location; // 현재 주인공이 있는 장소
	int item[30]; // 아이템 소유 목록
	int item_num; //가지고 있는 아이템의 수...
	int state; // 캐릭터의 심리상태... 미래를 위해.. 쓰이지 않음
	int game_mode;
	int sub_game_mode;
};
Char_Info2 g_Hero;

struct Global_State
{
	bool Menu_open;
	bool Key_Access;
	bool Error_Message;
	int Next_Move;
	int Messge_Number;
	int Messge_INDEX;
	int Procees_script;
	int Text_Out;
	int Conv_State;
	int Script_State;
	int Script_Count;
	int Script_Current_Count;
	int Script_Length;
	int Script_No;
	POINT mouse_loc;
	bool Get_Item;
	bool Date_Display;
	bool script_copy;
	bool Get_Messge_Data;
	bool bProcessKey;
	int Sound;
	int Char_Present;
	int Script_Check_Point;
	int Item_Messge_INDEX;
	int Get_Item_Message;
	int Music_Count;
	int effectRunning;
	int Playing;
	int end_mode;
};
Global_State g_Global_State = { 0 };

//For Item
struct ItemInfo
{
	int	Item_ID; // 이름 등에 접근하기 위함
	RECT rect; // 아이템이 있는 위치

	int state; // 당장 아이템을 취할 수 있는지의 여부
};
ItemInfo Item[20];

struct SceneInfo
{
	int	Scene_ID; // 이름 등에 접근하기 위함 또한 현재의 장면위치 확인 아이디	
	bool bscript; // 대화스크립트가 존재?
	int script_no; // 대화스크립트가 존재한다면 몇번째 대화스크립트인가?
	int item_num; //현재장소에 존재하는 아이템의 수..
	int item[5]; //현재장소에 존재하는 아이템의 수 다섯개가 한개이다.
	int Character_Num; //현재장소에 있는 캐릭터 수
	int Character[10]; //현재장소에 있는 캐릭터 리스트
	bool access; // 이 장소에 접근여부...
	int can_go[35];
	int can_go_num;
};
SceneInfo Scene[35] = { 0 };

LPCTSTR g_pstrScene[] =
{
	TEXT("image\\scene2\\0.png"),
	TEXT("image\\scene2\\2.png"),
	TEXT("image\\scene2\\3.png"),
	TEXT("image\\scene2\\4.png"),
	TEXT("image\\scene2\\8.png"),
	TEXT("image\\scene2\\9.png"),
	TEXT("image\\scene2\\10.png"),
	TEXT("image\\scene2\\11.png"),
	TEXT("image\\scene2\\12.png"),
	TEXT("image\\scene2\\13.png"),
	TEXT("image\\scene2\\14.png"),
	TEXT("image\\scene2\\19.png"),
	TEXT("image\\scene2\\20.png"),
	TEXT("image\\scene2\\21.png"),
	TEXT("image\\scene2\\23.png"),
	TEXT("image\\scene2\\25.png"),
	TEXT("image\\scene2\\27.png"),
	TEXT("image\\scene2\\31.png"),
	TEXT("image\\scene2\\34.png"),
	TEXT("image\\scene2\\end.jpg"),
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
	TEXT("scenario\\act2\\Act2_1.txt"),
	TEXT("scenario\\act2\\script1.txt"),
	TEXT("scenario\\act2\\script2.txt"),
	TEXT("scenario\\act2\\script3.txt"),
	TEXT("scenario\\act2\\script3_5.txt"),
	TEXT("scenario\\act2\\script4.txt"),
	TEXT("scenario\\act2\\script5.txt"),
	TEXT("scenario\\act2\\script6.txt"),
	TEXT("scenario\\act2\\script7.txt"),
	TEXT("scenario\\act2\\script8.txt"),
	TEXT("scenario\\act2\\script9.txt"),
	TEXT("scenario\\act2\\script10.txt"),
	TEXT("scenario\\act2\\script11.txt"),
	TEXT("scenario\\act2\\script12.txt"),
	TEXT("scenario\\act2\\script13.txt"),
	TEXT("scenario\\act2\\script14.txt"),
	TEXT("scenario\\act2\\script15.txt"),
	TEXT("scenario\\act2\\script16.txt"),
	TEXT("scenario\\act2\\script17.txt"),
	TEXT("scenario\\act2\\script18.txt"),
	TEXT("scenario\\act2\\script19.txt"),
	TEXT("scenario\\act2\\script20.txt"),
	TEXT("scenario\\act2\\script21.txt"),
	TEXT("scenario\\act2\\script22.txt"),
	TEXT("scenario\\act2\\script23.txt"),
	TEXT("scenario\\act2\\script24.txt"),
	TEXT("scenario\\act2\\script25.txt"),
	TEXT("scenario\\act2\\script26.txt"),
	TEXT("scenario\\act2\\script27.txt"),
	TEXT("scenario\\act2\\script28.txt"),
	TEXT("scenario\\act2\\script29.txt"),
	TEXT("scenario\\act2\\script30.txt"),
	TEXT("scenario\\act2\\script31.txt"),
	TEXT("scenario\\act2\\script32.txt"),
	TEXT("scenario\\act2\\script33.txt"),
};


LPDIRECT3DVERTEXBUFFER9 g_pVB_Scene[20] = { NULL }; // Buffer to hold vertices
LPDIRECT3DTEXTURE9      g_pTexture_Scene[20] = { NULL }; // Our texture




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

	DXUTCreateWindow(L"Killing Time 2005 Part 2 - For Adventure The Second Anniversary");
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

	g_HUD.SetCallback(OnGUIEvent); int iY = 10;
	g_SampleUI.SetCallback(OnGUIEvent);
	g_SampleUI.SetFont(1, L"Comic Sans MS", 24, FW_NORMAL);
	g_SampleUI.SetFont(2, L"Courier New", 16, FW_NORMAL);

	// Buttons
	g_SampleUI.AddButton(IDC_START, L"게임시작", 30, 390, 80, 35, L'S');
	g_SampleUI.AddButton(IDC_LOAD, L"로드", 30, 50, 80, 35, L'L');
	g_SampleUI.AddButton(IDC_CONFIG, L"환경설정", 30, 390, 80, 35, L'C');
	g_SampleUI.AddButton(IDC_EXIT, L"끝내기", 30, 50, 80, 35, L'E');
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

	Scene_Initialize();

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

void Item_Set2();
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
	Create_3DSprite(pd3dDevice, g_pTexture_Fade, g_pVB_Fade, L"1.tga", 0, 0, pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height, false, D3DCOLOR_ARGB(0, 0, 255, 0));

	SAFE_RELEASE(g_pTexture_Prog);
	SAFE_RELEASE(g_pVB_Prog);
	Create_3DSprite(pd3dDevice, g_pTexture_Prog, g_pVB_Prog, pstrTextureFiles[0], 110 * (g_width / 640), 50 * (g_height / 480), 416 * (g_width / 640), 124 * (g_height / 480), false, D3DCOLOR_ARGB(0, 0, 255, 0));
	//SAFE_RELEASE(g_pTexture_Opening);
	//SAFE_RELEASE(g_pVB_Opening);
	//Create_3DSprite(pd3dDevice,g_pTexture_Opening,g_pVB_Opening,pstrTextureFiles[1],0,0,pBackBufferSurfaceDesc->Width,pBackBufferSurfaceDesc->Height,false,D3DCOLOR_ARGB(0, 0, 255, 0));

	SAFE_RELEASE(g_pTexture_Text_Bar);
	SAFE_RELEASE(g_pVB_Text_Bar);
	Create_3DSprite(pd3dDevice, g_pTexture_Text_Bar, g_pVB_Text_Bar, pstrTextureFiles[2], 0, (pBackBufferSurfaceDesc->Height) * (5.0f / 6.0f), pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height / 6, false, D3DCOLOR_ARGB(0, 0, 255, 0));
	// Create a sprite to help batch calls when drawing many lines of text
	V_RETURN(D3DXCreateSprite(pd3dDevice, &g_pTextSprite));

	int i;

	for (i = 0; i < 20; i++)
	{
		SAFE_RELEASE(g_pVB_Scene[i]);
		SAFE_RELEASE(g_pTexture_Scene[i]);
	}

	D3DCOLOR rt;

	for (i = 0; i < 20; i++)
	{

		Create_3DSprite(pd3dDevice, g_pTexture_Scene[i], g_pVB_Scene[i], g_pstrScene[i], 0, 0, pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height, false, rt);
	}


	for (i = 0; i < 10; i++)
	{
		SAFE_RELEASE(g_pTexture_Character[i]);
		SAFE_RELEASE(g_pVB_Character[i]);

		g_temp = (g_width / 2) - (g_width / 4);
	}
	for (i = 0; i < 10; i++)
	{
		if (i == 3)
			rt = D3DCOLOR_ARGB(0, 6, 255, 0);
		else rt = D3DCOLOR_ARGB(0, 0, 255, 0);
		Create_3DSprite(pd3dDevice, g_pTexture_Character[i], g_pVB_Character[i], g_pstrCharater_Name[i], g_temp, (pBackBufferSurfaceDesc->Height / 8), (g_width / 2), (pBackBufferSurfaceDesc->Height) * (7.0f / 8.0f), true, rt);
	}


	for (i = 0; i < 10; i++)
	{
		SAFE_RELEASE(g_pTexture_Item[i]);
		SAFE_RELEASE(g_pVB_Item[i]);
	}

	for (i = 0; i < 9; i++)
	{
		//Create_3DSprite(pd3dDevice,g_pTexture_Item[i],g_pVB_Item[i],g_pstrItem_Name[0],(g_width/8.0f)*((i%5)+1),(g_height/20.0f)+(g_height/8.0f)*(i/5),g_width/8.0f,g_height/8.0f);				
		Create_3DSprite(pd3dDevice, g_pTexture_Item[i], g_pVB_Item[i], g_pstrItem_Name[i], g_width / 8.0f * i, 0, g_width / 8.0f, g_height / 8.0f, false, D3DCOLOR_ARGB(0, 0, 255, 0));
	}
	LoadTexture(&g_pTexture_Item[9], g_pVB_Item[9], g_pstrItem_Name[9], true, D3DCOLOR_ARGB(0, 255, 255, 255));

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

	if (g_GameStart == 1 && g_Hero.game_mode == MENU_DISPLAY_MODE)
	{
		g_menu.RemoveAllControls();
		InitMenu2();
	}

	Item_Set2();

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

void Visit_Date_Display()
{
	int k;
	g_d3dHangul.SetWidthHeight((int)(g_width / 20), (int)(g_height / 16));
	g_d3dHangul.SetColor(0xffffff);
	WCHAR wFileName[MAX_PATH];
	WideCharToMultiByte(CP_ACP, NULL,
		g_pstrTime[g_Global_State.Text_Out], -1,
		ggbuf, sizeof(ggbuf),
		NULL, NULL);
	k = strlen(ggbuf);
	ggbuf[ghj] = 0;
	MultiByteToWideChar(CP_ACP, 0, ggbuf, -1,
		wFileName, MAX_PATH);
	g_d3dHangul.OutHanXY((g_width / 2) - ((g_width / 6) * (sizeof(g_pstrTime[0]) / 2)), (g_height / 2), wFileName);
	if (g_Global_State.Key_Access == false)
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
		g_Global_State.Key_Access = true;
	}
	Sleep(100);
}

int image_map();
void Check_CanGetItem();
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

			//Display( pd3dDevice,g_pTexture_Opening,g_pVB_Opening,0);
			int image_mapping = 0;
			int u = 0;
			image_mapping = image_map();
			Display(pd3dDevice, g_pTexture_Scene[image_mapping], g_pVB_Scene[image_mapping], 0);
			switch (g_Hero.game_mode)
			{

			case SEARCH_MODE:
				if (Scene[g_Hero.location].item_num > 0)
				{
					for (int i = 0; i < Scene[g_Hero.location].item_num; i++)
						Show_Inventory_Item(pd3dDevice, g_pTexture_Item[9], g_pVB_Item[9], 1, i);
				}
				break;

			case INVENTORY_MODE:
				RenderText();
				break;

			case MENU_DISPLAY_MODE:
				if (Scene[g_Hero.location].Character_Num > 0) //일단 한사람만 나오도록 하고 차후 엔진 개량시 여러사람이
				{ //나올 수 있도록 고려한다.

					Display(pd3dDevice, g_pTexture_Character[Scene[g_Hero.location].Character[0]]
						, g_pVB_Character[Scene[g_Hero.location].Character[0]], g_Char_Exchange);
				}
				Menu_Display2();
				V(g_menu.OnRender(fElapsedTime));
				break;
			case MOVE_MODE:
				g_Global_State.Key_Access = true;
				g_Global_State.Messge_Number = Scene[g_Hero.location].can_go_num;
				RenderText();
				break;

			case CONV_MODE:
				if (Scene[g_Hero.location].Character_Num > 0) //일단 한사람만 나오도록 하고 차후 엔진 개량시 여러사람이
				{ //나올 수 있도록 고려한다.

					Display(pd3dDevice, g_pTexture_Character[Scene[g_Hero.location].Character[0]]
						, g_pVB_Character[Scene[g_Hero.location].Character[0]], g_Char_Exchange);
				}
				break;
			case CHECK_MODE:
				Check_Move_Mode();
				break;
			case CANT_GO:
				RenderText();
				break;

			case ITEM_DISCRIPT_MODE:
				RenderText();
				break;


			case GAMEOVER_MODE:
				RenderText();
				break;
			case SPECIAL_SCRIPT:
				if (g_Global_State.script_copy == false)
					script_analyzer();
				GetText_1(fElapsedTime);
				GetText_2(fElapsedTime);
				break;

			case END_MODE:
				if (g_Global_State.script_copy == false)
					script_analyzer();
				GetText_1(fElapsedTime);
				GetText_2(fElapsedTime);
				break;

			case SCRIPT_MODE:
				if (g_Global_State.Date_Display == true)
				{
					Visit_Date_Display();
					g_LineRead = 0;
					qbt = 0;
					iop = 0;
					g_Global_State.script_copy = false;
					g_Global_State.Script_State = SCRIPT_READING;
				}
				else
				{

					if (g_Global_State.script_copy == false)
						script_analyzer();

					if (u != 0 || g_Global_State.Char_Present != 0)
					{
						Display(pd3dDevice, g_pTexture_Character[g_Global_State.Char_Present]
							, g_pVB_Character[g_Global_State.Char_Present], g_Char_Exchange);
					}

					u = GetText_1(fElapsedTime);
					GetText_2(fElapsedTime);
					if (u != 0)
					{
						g_Global_State.Char_Present = u;
					}

				}
				break;

			case CONV_MODE2:
				break;

			case ITEM_CHECK:
				Check_CanGetItem();
				break;

			case DISPLAY_GET_ITEM:
				RenderText();
				break;
			}
		}


		g_d3dHangul3.SetWidthHeight((int)(g_width / 30), (int)(g_height / 24));
		g_d3dHangul3.SetColor(0x0ffffff);

		g_d3dHangul3.OutHanXY(10, 20, pAct2_str[g_Hero.location]);
		TCHAR szDuration[64];
		StringCchPrintf(szDuration, NUMELMS(szDuration), _T("POINT : %d / 227"), g_point);

		g_d3dHangul3.OutHanXY(450 * (g_width / 640), 20, szDuration);
		V(g_GameUI.OnRender(fElapsedTime));
		V(pd3dDevice->EndScene());
	}
}
//배경장면 
/*	  if(g_act1_auto==1&&Scene[g_Hero_Scene].Scene_ID==0)
	  {
				init_val();
				Scene[g_Hero_Scene].access=0;
				Scene[g_Hero_Scene].access_script_no=0;
				g_act1_auto2=1;
				g_act1_auto=0;
	  }


	// 현재 장면에 존재하는 사람 체크
	if(Scene[g_Hero_Scene].Character!=1000&&bCharShow_Mode==false) //1000은 아무도 없다는 걸 의미함



	if(bCharShow_Mode==true)
	{
		Display2( pd3dDevice,g_pTexture_Character[MeetingData[g_Char_Selected_Number]]
				,g_pVB_Character[MeetingData[g_Char_Selected_Number]],g_Char_Exchange);
	}

	//대사창 이미지 출력
	Display( pd3dDevice,g_pTexture_Text_Bar,g_pVB_Text_Bar,0);

	//인벤토리창 출력
	if(bInventory_Mode==true)
	{

		Display( pd3dDevice,g_pTexture_Fade,g_pVB_Fade,0);
		//for(j=0;j<20;j++)
		//Display( pd3dDevice,g_pTexture_Item[j],g_pVB_Item[j],0);
		Show_Inventory_Item( pd3dDevice,g_pTexture_Item[0],g_pVB_Item[0],1);
		Show_Inventory_Item( pd3dDevice,g_pTexture_Item[1],g_pVB_Item[1],0);
	}

	if(Scene[g_Hero_Scene].access==0) //처음 방문시 시간을 표시한다.
	{
		Visit_Date_Display();
	}
	else if(Scene[g_Hero_Scene].access_script_no==0) // 최초로 나오는 텍스트를 출력한다.
	{
		if(!g_bTextReading)
		{
			GetFileHandle();
			g_bTextReading=1;
		}
		else
		{
			GetText(fElapsedTime);
		}

	}else if(Scene[g_Hero_Scene].access_script_no==1)
	{             				g_act1_auto2=0;
	   if(g_Hero_Scene==0&&g_act1_auto2==0)
	   {				   */
	   /*if(g_menu_set==0)
		   InitMenu2();
	   V( g_menu.OnRender( fElapsedTime ) );			   */
	   /*if(ttt==0)
		   g_game_state=4;
	   ttt=1;
	   if(g_game_state==7)
	   {V( g_menu.OnRender( fElapsedTime ) );}
		else if(g_game_state==8)
	   {RenderText();}
   }
   else
   {
	   if(g_Hero_Scene==5)
	   {
		   g_game_state=0;
	   }
	   if(g_script_analysis==0)
		   script_analyzer();
	   if(g_game_state==4&&g_Hero_Scene!=5)
		   Menu_Display();
	   else if(g_game_state==5)
		   Menu_Extend_Display();
	   else if(g_game_state==7)
	   {V( g_menu.OnRender( fElapsedTime ) );}
	   else if(g_game_state==8)
	   {RenderText();}
	   else if(g_game_state==0)
	   {
		   if(b_Text_Parse==false)
		   {
			   Text_Parse();
			   if(g_Hero_Scene==5&&aaa==1)
			   {
				   g_bTextReading_Terminate=0;
				   aaa=0;
			   }
			   b_Text_Parse=true;
		   }
		   else
		   {
			   GetText2(fElapsedTime);
			   GetText3(fElapsedTime);
		   }
	   }
   }

}*/



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
		//g_KeyBoard_Acess=1;			
		//Scene[g_Hero_Scene].access_script_no=1;
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




}



int nani = 0;
int nani2 = 0;
int nani3 = 0;
//--------------------------------------------------------------------------------------
// Render the help and statistics text. This function uses the ID3DXFont interface for 
// efficient text rendering.
//--------------------------------------------------------------------------------------
void RenderText()
{

	int t = 0;
	g_d3dHangul3.SetWidthHeight((int)(g_width / 30), (int)(g_height / 24));
	if (g_Hero.game_mode == CONV_MODE)
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
			g_d3dHangul3.OutHanXY(10, (200 * (g_height / 480)) + t * (20 * (g_height / 480)), wFileName);
			//txtHelper.SetInsertionPos( 10, 200+t*20 );			
			t++;
			//txtHelper.DrawTextLine(wFileName);	
		}
	}
	else if (g_Hero.game_mode == CONV_MODE2)
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

			g_d3dHangul3.OutHanXY(10, 200 + i * 20, wFileName);
		}
	}
	else if (g_Hero.game_mode == GAMEOVER_MODE)
	{
		g_d3dHangul.SetWidthHeight((int)(g_width / 15), (int)(g_height / 12));
		g_d3dHangul.SetColor(0xffffff);
		g_d3dHangul.OutHanXY((g_width / 2) - ((g_width / 15) * (sizeof(g_gameover[0]) / 2)), (g_height / 2), g_gameover[0]);
	}
	else if (g_Hero.game_mode == INVENTORY_MODE)
	{

		for (int i = 0; i < g_Hero.item_num; i++)
		{
			if (i == g_Global_State.Item_Messge_INDEX)
				g_d3dHangul3.SetColor(0x0ffffff);

			else
				g_d3dHangul3.SetColor(0xff00ff);

			g_d3dHangul3.OutHanXY(10 * (g_width / 640), (100 * (g_height / 480)) + i * (20 * (g_height / 480)), g_pstrItem[g_Hero.item[i]]);

		}
	}
	else if (g_Hero.game_mode == MOVE_MODE)
	{

		for (int i = 0; i < Scene[g_Hero.location].can_go_num; i++)
		{
			if (i == g_Global_State.Messge_INDEX)
				g_d3dHangul3.SetColor(0x0ffffff);

			else
				g_d3dHangul3.SetColor(0xff00ff);

			g_d3dHangul3.OutHanXY(10 * (g_width / 640), (200 * (g_height / 480)) + i * (20 * (g_height / 480)), pAct2_str[Scene[g_Hero.location].can_go[i]]);
		}
	}
	else if (g_Hero.game_mode == DISPLAY_GET_ITEM)
	{

		if (g_Global_State.Get_Item == true)
		{
			g_d3dHangul3.SetColor(0x0ffffff);
			g_d3dHangul3.OutHanXY(g_width / 20, g_height - (g_height / 10), g_pstrItem2[g_Global_State.Get_Item_Message]);
		}
		else
		{
			g_d3dHangul3.SetColor(0x0ffffff);

			if (Scene[g_Hero.location].Scene_ID == MUSIC_CLASS && Scene[g_Hero.location].item_num > 0 && nani == 1)
				g_d3dHangul3.OutHanXY(g_width / 20, g_height - (g_height / 10), L"아무런 이유없이 피아노를 칠 필요가 없다.");
			else g_d3dHangul3.OutHanXY(g_width / 20, g_height - (g_height / 10), L"특별한 것은 없다");

		}

	}
	else if (g_Hero.game_mode == CANT_GO)
	{
		g_d3dHangul3.SetColor(0x0ffffff);
		if (nani2 == 1)
		{
			g_Global_State.Sound = 2008;
			g_Global_State.effectRunning = 1;
			g_Global_State.Playing = 0;
			if (nani3 == 1)
			{
				g_point += 5;
				SwapSourceFilter2(7);
				Sleep(1000);
				nani3 = 0;
			}
			g_d3dHangul3.OutHanXY(g_width / 20, g_height - (g_height / 10), L"가지고 있던 열쇠로 문을 열었다.");
		}
		else g_d3dHangul3.OutHanXY(g_width / 20, g_height - (g_height / 10), L"문이 잠겨서 열리지가 않는다.");

	}
	else if (g_Hero.game_mode == ITEM_DISCRIPT_MODE)
	{

		g_d3dHangul3.SetColor(0x0ffffff);
		g_d3dHangul3.OutHanXY(g_width / 20, g_height - (g_height / 10), g_pstrItem3[g_Hero.item[g_Global_State.Item_Messge_INDEX]]);

	}
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
	{
		if (g_Hero.game_mode == INVENTORY_MODE || g_Hero.game_mode == MOVE_MODE ||
			g_Hero.game_mode == SEARCH_MODE || g_Hero.game_mode == CONV_MODE)
		{
			g_Hero.game_mode = MENU_DISPLAY_MODE;
		}
	}
	else if (uMsg == WM_LBUTTONUP)
	{

		if (g_Hero.game_mode == SEARCH_MODE)
		{
			g_Global_State.mouse_loc.x = (int)(LOWORD(lParam));      // 마우스 커서의 x 좌표    		        
			g_Global_State.mouse_loc.y = (int)(HIWORD(lParam)); // 마우스 커서의 y 좌표    		        
			g_Hero.game_mode = ITEM_CHECK;
		}
		else if (g_Hero.game_mode == DISPLAY_GET_ITEM)
		{
			g_Global_State.Get_Item = false;
			nani = 0;
			g_Hero.game_mode = SEARCH_MODE;
		}
	}
	// Give the dialogs a chance to handle the message first
	*pbNoFurtherProcessing = g_HUD.MsgProc(hWnd, uMsg, wParam, lParam);
	if (*pbNoFurtherProcessing)
		return 0;
	*pbNoFurtherProcessing = g_SampleUI.MsgProc(hWnd, uMsg, wParam, lParam);
	if (*pbNoFurtherProcessing)
		return 0;
	if (g_Hero.game_mode == MENU_DISPLAY_MODE)
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
int jklp = 0;
void CALLBACK KeyboardProc(UINT nChar, bool bKeyDown, bool bAltDown)
{

	if (g_GameStart == 1)
	{
		if (g_Global_State.Key_Access == true)
		{
			if (g_Hero.game_mode == MOVE_MODE)
			{
				if (bKeyDown)
				{
					switch (nChar)
					{
					case VK_SPACE:
						g_Global_State.Next_Move = Scene[g_Hero.location].can_go[g_Global_State.Messge_INDEX];
						if (Scene[g_Global_State.Next_Move].access == false)
						{

							g_Global_State.Error_Message = true;
							g_Hero.game_mode = CHECK_MODE;
						}
						else
						{
							g_Global_State.Error_Message = false;
							g_Hero.location = g_Global_State.Next_Move;
							g_Global_State.Messge_INDEX = 0;
							g_Hero.game_mode = CHECK_MODE;
						}
						break;
					case VK_DOWN:
						g_Global_State.Messge_INDEX++;
						if (g_Global_State.Messge_INDEX >= g_Global_State.Messge_Number)
							g_Global_State.Messge_INDEX = 0;
						break;
					case VK_UP:
						g_Global_State.Messge_INDEX--;
						if (g_Global_State.Messge_INDEX < 0)
							g_Global_State.Messge_INDEX = g_Global_State.Messge_Number - 1;

					}

				}

			}

			else if (g_Hero.game_mode == CANT_GO)
			{
				if (bKeyDown)
				{
					switch (nChar)
					{
					case VK_SPACE:
						nani2 = 0;
						nani3 = 0;
						g_Hero.game_mode = MENU_DISPLAY_MODE;
						break;
					}
				}
			}

			else if (g_Hero.game_mode == ITEM_DISCRIPT_MODE)
			{
				if (bKeyDown)
				{
					switch (nChar)
					{
					case VK_SPACE:
						g_Hero.game_mode = INVENTORY_MODE;
						break;
					}
				}
			}


			else if (g_Hero.game_mode == SPECIAL_SCRIPT)
			{
				if (bKeyDown)
				{
					switch (nChar)
					{
					case VK_SPACE:
						g_Global_State.Date_Display = false;
						ghj = 0;
						g_Global_State.bProcessKey = true;
						g_Global_State.Key_Access = false;
						g_LineRead = 0;
						qbt = 0;
						iop = 0;
						if (g_Global_State.Procees_script == 1)
						{
							g_Global_State.Key_Access = true;
							g_Global_State.Get_Messge_Data = true;
							g_Global_State.Procees_script = 0;
							if (g_Global_State.Script_State == SCRIPT_END)

							{
								Scene[g_Hero.location].item_num = 1;
								jklp = 1;
								Item[ITEM_유리거울].rect.left = 490 * (g_width / 640);
								Item[ITEM_유리거울].rect.top = 375 * (g_height / 480);
								Item[ITEM_유리거울].rect.right = 510 * (g_width / 640);
								Item[ITEM_유리거울].rect.bottom = 395 * (g_height / 480);
								Scene[g_Hero.location].item[0] = ITEM_유리거울;
								g_Global_State.script_copy = false;
								Init_Script_Var();
								g_Hero.game_mode = MENU_DISPLAY_MODE;
							}

						}
						break;
					}
				}
			}
			else if (g_Hero.game_mode == END_MODE)
			{
				if (bKeyDown)
				{
					switch (nChar)
					{
					case VK_SPACE:
						g_Global_State.Date_Display = false;
						ghj = 0;
						g_Global_State.bProcessKey = true;
						g_Global_State.Key_Access = false;
						g_LineRead = 0;
						qbt = 0;
						iop = 0;
						if (g_Global_State.Procees_script == 1)
						{
							g_Global_State.Key_Access = true;
							g_Global_State.Get_Messge_Data = true;
							g_Global_State.Procees_script = 0;
							if (g_Global_State.Script_State == SCRIPT_END)
							{
								g_Hero.game_mode = GAMEOVER_MODE;
							}

						}
						break;
					}
				}
			}
			else if (g_Hero.game_mode == SCRIPT_MODE)
			{
				if (bKeyDown)
				{
					switch (nChar)
					{
					case VK_SPACE:
						g_Global_State.Date_Display = false;
						ghj = 0;
						g_Global_State.bProcessKey = true;
						g_Global_State.Key_Access = false;
						g_LineRead = 0;
						qbt = 0;
						iop = 0;
						if (g_Global_State.Procees_script == 1)
						{
							g_Global_State.Key_Access = true;
							g_Global_State.Get_Messge_Data = true;
							g_Global_State.Procees_script = 0;
							if (g_Global_State.Script_State == SCRIPT_END)
							{
								if (g_Global_State.Script_No == 27)
								{
									g_Hero.location = GAME_END;
									Gnerate_Next_Scene(1);
								}
								else if (g_Global_State.Script_No >= 28)
								{
									g_Hero.game_mode = GAMEOVER_MODE;
								}
								else Gnerate_Next_Scene(1);
							}

						}
						break;
					}
				}
			}
			else if (g_Hero.game_mode == INVENTORY_MODE)
			{
				if (bKeyDown)
				{
					switch (nChar)
					{
					case VK_SPACE:
						//g_Hero.location=g_Global_State.Next_Move;
						g_Hero.game_mode = ITEM_DISCRIPT_MODE;
						break;
					case VK_DOWN:
						g_Global_State.Item_Messge_INDEX++;
						if (g_Global_State.Item_Messge_INDEX >= g_Hero.item_num)
							g_Global_State.Item_Messge_INDEX = 0;
						break;
					case VK_UP:
						g_Global_State.Item_Messge_INDEX--;
						if (g_Global_State.Item_Messge_INDEX < 0)
							g_Global_State.Item_Messge_INDEX = g_Hero.item_num - 1;
						break;
					}
				}
			}

		}

	}
}

/*
void CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown )
{

	if(g_GameStart==1)
		if(g_Global_State)
		{
			if(g_Global_State)
			{
				if( bKeyDown )
				{
					switch( nChar )
					{
						case VK_SPACE:
							if(g_selected_message==g_extend_number)
							{
								Scene[g_Hero_Scene].access=1;
								g_game_state=5;
								g_selected_extend_message=0;
								g_KeyBoard_Acess=1;
							}
							else
							{
								if(ttt!=1)
								{g_game_state=0;
								g_bProcessKey=1;g_LineRead=0;
								qbt=0;
								g_KeyBoard_Acess=1;
								iop=0;
								g_bTextReading_Terminate=0;
								g_bTextReading=1;}
							}
							break;
						case VK_DOWN:
							g_selected_message++;
							if(g_selected_message>(g_message_number-1))
								g_selected_message=0;
							break;
						case VK_UP:
							g_selected_message--;
							if(g_selected_message<0)
								g_selected_message=g_message_number-1;
							break;
			}

			else if(Scene[g_Hero_Scene].access_script_no==1&&g_game_state==4)
			{
				if( bKeyDown )
				{
					switch( nChar )
					{
						case VK_SPACE:
							if(g_selected_message==g_extend_number)
							{
								Scene[g_Hero_Scene].access=1;
								g_game_state=5;
								g_selected_extend_message=0;
								g_KeyBoard_Acess=1;
							}
							else
							{
								if(ttt!=1)
								{g_game_state=0;
								g_bProcessKey=1;g_LineRead=0;
								qbt=0;
								g_KeyBoard_Acess=1;
								iop=0;
								g_bTextReading_Terminate=0;
								g_bTextReading=1;}
							}
							break;
						case VK_DOWN:
							g_selected_message++;
							if(g_selected_message>(g_message_number-1))
								g_selected_message=0;
							break;
						case VK_UP:
							g_selected_message--;
							if(g_selected_message<0)
								g_selected_message=g_message_number-1;
							break;

						case 'S':
							if(bCharShow_Mode==false)
								bCharShow_Mode=true;
							else bCharShow_Mode=false;
							g_Char_Exchange=1;g_temp=-(g_width/2);g_temp2=0;
							if(g_Char_Selected_Number>=MeetingDataIndex)
								g_Char_Selected_Number=0;
							g_Char_Selected_Number++;;
							break;
						case 'A':
							if(bInventory_Mode==false)
								bInventory_Mode=true;
							else bInventory_Mode=false;
							break;
					}
				}
			}

			else if(Scene[g_Hero_Scene].access_script_no==1&&g_game_state==5)

			{

				if( bKeyDown )

				{

					switch( nChar )

					{

					case VK_SPACE:
						g_game_state=0;
						g_bProcessKey=1;g_LineRead=0;
						qbt=0;
						g_KeyBoard_Acess=0;
						iop=0;
						g_bTextReading_Terminate=0;
						g_bTextReading=1;
						g_juhang3=1;
						break;
					case VK_DOWN:
						g_selected_extend_message++;
						if(g_selected_extend_message>=g_extend)
							g_selected_extend_message=0;
						break;

					case VK_UP:
						g_selected_extend_message--;
						if(g_selected_extend_message<0)
							g_selected_extend_message=g_extend-1;
						break;
					case 'S':

						if(bCharShow_Mode==false)

							bCharShow_Mode=true;

						else bCharShow_Mode=false;

						g_Char_Exchange=1;g_temp=-(g_width/2);g_temp2=0;

						if(g_Char_Selected_Number>=MeetingDataIndex)

							g_Char_Selected_Number=0;

						g_Char_Selected_Number++;;

						break;

					case 'A':

						if(bInventory_Mode==false)

							bInventory_Mode=true;

						else bInventory_Mode=false;

						break;



					}

				}

			}


			else if(g_game_state==8)

			{

				if( bKeyDown )

				{

					switch( nChar )

					{

					case VK_SPACE:


						g_Hero_Scene=g_selected_char;

						init_val();
						if(Scene[g_Hero_Scene].access_script_no==1)
						{
							g_KeyBoard_Acess=1;
							g_game_state=4;
						}
						else
						{
							g_game_state=4;
							g_KeyBoard_Acess=0;
						}
						g_bProcessKey=1;g_LineRead=0;
						ghj=0;

						qbt=0;
						iop=0;
						g_bTextReading=0;
						g_bTextReading_Terminate=0;
						SwapSourceFilter();
						break;
					case VK_DOWN:
						g_selected_char++;
						if(g_selected_char>=g_can_go)
							g_selected_char=0;
						break;

					case VK_UP:
						g_selected_char--;
						if(g_selected_char<0)
							g_selected_char=g_can_go-1;
						break;


					}

				}

			}



}
}
*/

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

		g_Hero.location = FRONT_DOOR;
		g_Global_State.Date_Display = true;
		g_Global_State.Key_Access = false;
		g_bProcessKey = 0; g_LineRead = 0;
		g_bTextReading_Terminate = 0;
		ghj = 0;
		g_Hero.game_mode = SCRIPT_MODE;

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
	//SAFE_RELEASE(g_pTexture_Opening);
	//SAFE_RELEASE(g_pVB_Opening);
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
	for (i = 0; i < 10; i++)
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
		DWORD dwAttr = GetFileAttributes(pstrFiles[g_Global_State.Music_Count]);

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
		(void)StringCchCopy(g_szCurrentFile, NUMELMS(g_szCurrentFile), pstrFiles[g_Global_State.Music_Count]);
		(void)StringCchCopy(szFilename, NUMELMS(szFilename), pstrFiles[g_Global_State.Music_Count]);
		g_Global_State.Music_Count++;
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
	if (g_Global_State.Playing == 0)
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

	if (g_Global_State.effectRunning == 1)
	{
		LONGLONG llPos = 0;
		hr = g_pMediaSeeking2->SetPositions(&llPos, AM_SEEKING_AbsolutePositioning,
			&llPos, AM_SEEKING_NoPositioning);


		hr = g_pMediaControl2->Run();
		if (i != 0)
		{

			g_Global_State.Playing = 0;
			g_Global_State.effectRunning = 1;
		}
		else
		{
			g_Global_State.Playing = 1;
			g_Global_State.effectRunning = 0;
		}


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

			g_Global_State.effectRunning = 1;
		}
	}

	return hr;
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

void Show_Inventory_Item(IDirect3DDevice9* pd3dDevice, LPDIRECT3DTEXTURE9& Test_Texture, LPDIRECT3DVERTEXBUFFER9& Test_Buffer, int iUpdate, int a)
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
	if (g_Hero.game_mode == SEARCH_MODE)
	{
		//for(int i=0;i<8;i++)	
		//{
		BITMAPVERTEX* v;
		Test_Buffer->Lock(0, 0, (void**)&v, 0);
		v[0] = BITMAPVERTEX(Item[Scene[g_Hero.location].item[a]].rect.left, Item[Scene[g_Hero.location].item[a]].rect.top, 1.f, 1.f, 0.0f, 0.0f);
		v[1] = BITMAPVERTEX(Item[Scene[g_Hero.location].item[a]].rect.right, Item[Scene[g_Hero.location].item[a]].rect.top, 1.f, 1.f, 1.0f, 0.0f);
		v[2] = BITMAPVERTEX(Item[Scene[g_Hero.location].item[a]].rect.left, Item[Scene[g_Hero.location].item[a]].rect.bottom, 1.f, 1.f, 0.0f, 1.0f);
		v[3] = BITMAPVERTEX(Item[Scene[g_Hero.location].item[a]].rect.left, Item[Scene[g_Hero.location].item[a]].rect.bottom, 1.f, 1.f, 0.0f, 1.0f);
		v[4] = BITMAPVERTEX(Item[Scene[g_Hero.location].item[a]].rect.right, Item[Scene[g_Hero.location].item[a]].rect.top, 1.f, 1.f, 1.0f, 0.0f);
		v[5] = BITMAPVERTEX(Item[Scene[g_Hero.location].item[a]].rect.right, Item[Scene[g_Hero.location].item[a]].rect.bottom, 1.f, 1.f, 1.0f, 1.0f);
		Test_Buffer->Unlock();
		pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
		//}
	}
	/*else
	{
	BITMAPVERTEX* v;
	Test_Buffer->Lock(0, 0, (void**)&v, 0);
	v[0] = BITMAPVERTEX(5.f, 5.f, 1.f, 1.f, 0.0f, 0.0f);
	v[1] = BITMAPVERTEX((g_width/8.0f)-10.f, 5.f, 1.f, 1.f, 1.0f, 0.0f);
	v[2] = BITMAPVERTEX(5.f,  (g_height/8.0f)-10.f, 1.f, 1.f,0.0f, 1.0f);
	v[3] = BITMAPVERTEX(5.f,  (g_height/8.0f)-10.f, 1.f, 1.f,0.0f, 1.0f);
	v[4] = BITMAPVERTEX((g_width/8.0f)-10.f, 5.f, 1.f, 1.f, 1.0f, 0.0f);
	v[5] = BITMAPVERTEX((g_width/8.0f)-10.f, (g_height/8.0f)-10.f, 1.f, 1.f, 1.0f, 1.0f);
	Test_Buffer->Unlock();
	pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
	}*/
}

void CALLBACK OnGUIMenuEvent(UINT nEvent, int nControlID, CDXUTControl* pControl)
{
	switch (nControlID)
	{
	case IDC_MOVE:
		g_Hero.location = FRONT_DOOR;
		g_Global_State.Date_Display = true;
		g_Global_State.Key_Access = false;
		g_menu.RemoveAllControls();
		g_bProcessKey = 0; g_LineRead = 0;
		g_bTextReading_Terminate = 0;
		ghj = 0;
		g_Hero.game_mode = SCRIPT_MODE;
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
		g_menu.RemoveAllControls();
		g_Global_State.Key_Access = true;
		g_Global_State.Menu_open = false;
		g_Global_State.Item_Messge_INDEX = 0;
		g_Hero.game_mode = MOVE_MODE;
		break;

	case IDC_CONV:
		g_menu.RemoveAllControls();
		g_Global_State.Key_Access = true;
		g_Global_State.Menu_open = false;
		g_Hero.game_mode = INVENTORY_MODE;
		break;

	case IDC_SEARCH:
		g_menu.RemoveAllControls();
		g_Global_State.Key_Access = false;
		g_Global_State.Menu_open = false;
		g_Hero.game_mode = SEARCH_MODE;
		break;

	}
}




void InitMenu2()
{
	g_menu.SetCallback(OnGUIMenuEvent2);
	g_menu.SetFont(1, L"Comic Sans MS", 24, FW_NORMAL);
	g_menu.SetFont(2, L"Courier New", 16, FW_NORMAL);
	g_menu.AddButton(IDC_MOVE, L"이동", g_width - ((g_width / 8) * 1), g_height - ((g_height / 6) * 2), (g_width / 8), (g_height / 13.7f), L'S');
	g_menu.AddButton(IDC_CONV, L"아이템", g_width - ((g_width / 8) * 2), g_height - ((g_height / 6) * 2), (g_width / 8), (g_height / 13.7f), L'S');
	g_menu.AddButton(IDC_SEARCH, L"조사", g_width - ((g_width / 8) * 3), g_height - ((g_height / 6) * 2), (g_width / 8), (g_height / 13.7f), L'S');
	g_menu.SetLocation(0, 0);
	g_menu.SetSize(640, 480);
	g_menu.GetControl(IDC_MOVE)->SetLocation(g_width - ((g_width / 8) * 1), g_height - ((g_height / 6) * 2));
	g_menu.GetControl(IDC_CONV)->SetLocation(g_width - ((g_width / 8) * 2), g_height - ((g_height / 6) * 2));
	g_menu.GetControl(IDC_SEARCH)->SetLocation(g_width - ((g_width / 8) * 3), g_height - ((g_height / 6) * 2));
}

void script_analyzer()
{
	char buf[MAX_PATH];
	int count = 0;
	char seps[] = " \n"; //seperator입니다						
	char seps2[] = "\n"; //seperator입니다					
	char seps3[] = "@"; //seperator입니다					
	char* token;
	if (g_Hero.game_mode == SPECIAL_SCRIPT)
	{

		WideCharToMultiByte(CP_ACP, NULL,
			g_pstrScript[29], -1,
			buf, sizeof(buf),
			NULL, NULL);
	}
	else if (g_Hero.game_mode == END_MODE && g_Global_State.end_mode == 0)
	{

		WideCharToMultiByte(CP_ACP, NULL,
			g_pstrScript[30], -1,
			buf, sizeof(buf),
			NULL, NULL);
	}
	else if (g_Hero.game_mode == END_MODE && g_Global_State.end_mode == 1)
	{

		WideCharToMultiByte(CP_ACP, NULL,
			g_pstrScript[31], -1,
			buf, sizeof(buf),
			NULL, NULL);
	}
	else if (g_Hero.game_mode == END_MODE && g_Global_State.end_mode == 2)
	{

		WideCharToMultiByte(CP_ACP, NULL,
			g_pstrScript[32], -1,
			buf, sizeof(buf),
			NULL, NULL);
	}
	else if (g_Hero.game_mode == END_MODE && g_Global_State.end_mode == 3)
	{

		WideCharToMultiByte(CP_ACP, NULL,
			g_pstrScript[33], -1,
			buf, sizeof(buf),
			NULL, NULL);
	}
	else
	{
		WideCharToMultiByte(CP_ACP, NULL,
			g_pstrScript[g_Global_State.Script_No], -1,
			buf, sizeof(buf),
			NULL, NULL);
	}
	g_fp = fopen(buf, "r");

	if (g_fp == NULL)
		MessageBox(NULL, L"S", L"D", MB_OK);
	fgets(g_string, MAX_PATH, g_fp);

	while (!feof(g_fp))
	{
		strcpy(g_message_data[count], g_string);
		strcpy(g_message_data2[count], g_string);
		token = strtok(g_string, seps);
		if ((!strcmp(token, "START")) && count == 0)
		{
			fgets(g_string, MAX_PATH, g_fp);
			continue;
		}
		if (!strcmp(token, "TEXT"))
		{
			fgets(g_string, MAX_PATH, g_fp);
			continue;
		}


		count++;
		fgets(g_string, MAX_PATH, g_fp);

	}

	g_Global_State.script_copy = true;
	g_Global_State.Script_Count = count;
	fclose(g_fp);
}



void script_analyzer2()
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
	g_d3dHangul2.OutHanXY(20, 390, g_wFileName);

	Sleep(10);
}


void Menu_Extend_Display()
{
	RenderText();
}
void Scene_Initialize()
{
	int i;
	for (i = 0; i < 10; i++)
		g_Hero.item[i] = 0;
	g_Hero.location = FRONT_DOOR;
	g_Hero.state = 0;
	g_Hero.game_mode = MENU_DISPLAY_MODE;
	g_Hero.sub_game_mode = 0;
	g_Hero.item_num = 1;
	g_Hero.item[0] = ITEM_BREAD;

	g_Global_State.Menu_open = false;
	g_Global_State.Key_Access = false;
	g_Global_State.Next_Move = -1;
	g_Global_State.Procees_script = 0;
	g_Global_State.Text_Out = 0;
	g_Global_State.Conv_State = 0;
	g_Global_State.Script_State = SCRIPT_READING;
	g_Global_State.Script_No = 0;
	g_Global_State.Script_Count = 0;
	g_Global_State.script_copy = false;
	g_Global_State.Get_Messge_Data = true;
	g_Global_State.bProcessKey = true;
	g_Global_State.Item_Messge_INDEX = 0;
	g_Global_State.effectRunning = 1;
	g_Global_State.Playing = 0;

	Scene[FRONT_DOOR].Scene_ID = FRONT_DOOR;
	Scene[FRONT_DOOR].item_num = 0;
	Scene[FRONT_DOOR].access = true;
	Scene[FRONT_DOOR].bscript = false;
	Scene[FRONT_DOOR].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[FRONT_DOOR].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[FRONT_DOOR].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[FRONT_DOOR].can_go[i] = 0;
	Scene[FRONT_DOOR].Character_Num = 0;
	//Scene[FRONT_DOOR].Character[0]=MINJUNHYUK;
	//Scene[FRONT_DOOR].Character[1]=SINYURA;
	Scene[FRONT_DOOR].can_go_num = 1;
	Scene[FRONT_DOOR].can_go[0] = EXIT;

	Scene[EXIT].Scene_ID = EXIT;
	Scene[EXIT].item_num = 0;
	Scene[EXIT].access = true;
	Scene[EXIT].bscript = false;
	Scene[EXIT].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[EXIT].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[EXIT].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[EXIT].can_go[i] = 0;
	Scene[EXIT].Character_Num = 0;
	Scene[EXIT].can_go_num = 2;
	Scene[EXIT].can_go[0] = FRONT_DOOR;
	Scene[EXIT].can_go[1] = FIRST_STAIR;

	Scene[FIRST_STAIR].Scene_ID = FIRST_STAIR;
	Scene[FIRST_STAIR].item_num = 0;
	Scene[FIRST_STAIR].access = true;
	Scene[FIRST_STAIR].bscript = false;
	Scene[FIRST_STAIR].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[FIRST_STAIR].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[FIRST_STAIR].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[FIRST_STAIR].can_go[i] = 0;
	Scene[FIRST_STAIR].Character_Num = 0;
	Scene[FIRST_STAIR].can_go_num = 4;
	Scene[FIRST_STAIR].can_go[0] = EXIT;
	Scene[FIRST_STAIR].can_go[1] = SECOND_STAIR;
	Scene[FIRST_STAIR].can_go[2] = FIRST_BOKDO1;
	Scene[FIRST_STAIR].can_go[3] = MAN_TOILET_OUT1;

	Scene[FIRST_BOKDO1].Scene_ID = FIRST_BOKDO1;
	Scene[FIRST_BOKDO1].item_num = 0;
	Scene[FIRST_BOKDO1].access = true;
	Scene[FIRST_BOKDO1].bscript = false;
	Scene[FIRST_BOKDO1].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[FIRST_BOKDO1].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[FIRST_BOKDO1].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[FIRST_BOKDO1].can_go[i] = 0;
	Scene[FIRST_BOKDO1].Character_Num = 0;
	Scene[FIRST_BOKDO1].can_go_num = 3;
	Scene[FIRST_BOKDO1].can_go[0] = FIRST_STAIR;
	Scene[FIRST_BOKDO1].can_go[1] = FIRST_CLASS;
	Scene[FIRST_BOKDO1].can_go[2] = FIRST_BOKDO2;

	Scene[FIRST_CLASS].Scene_ID = FIRST_CLASS;
	Scene[FIRST_CLASS].item_num = 0;
	Scene[FIRST_CLASS].access = false;
	Scene[FIRST_CLASS].bscript = false;
	Scene[FIRST_CLASS].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[FIRST_CLASS].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[FIRST_CLASS].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[FIRST_CLASS].can_go[i] = 0;
	Scene[FIRST_CLASS].Character_Num = 0;
	Scene[FIRST_CLASS].can_go_num = 1;
	Scene[FIRST_CLASS].can_go[0] = FIRST_BOKDO1;

	Scene[FIRST_BOKDO2].Scene_ID = FIRST_BOKDO2;
	Scene[FIRST_BOKDO2].item_num = 0;
	Scene[FIRST_BOKDO2].access = true;
	Scene[FIRST_BOKDO2].bscript = false;
	Scene[FIRST_BOKDO2].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[FIRST_BOKDO2].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[FIRST_BOKDO2].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[FIRST_BOKDO2].can_go[i] = 0;
	Scene[FIRST_BOKDO2].Character_Num = 0;
	Scene[FIRST_BOKDO2].can_go_num = 2;
	Scene[FIRST_BOKDO2].can_go[0] = FIRST_BOKDO1;
	Scene[FIRST_BOKDO2].can_go[1] = CHEYUK;

	Scene[CHEYUK].Scene_ID = CHEYUK;
	Scene[CHEYUK].item_num = 0;
	Scene[CHEYUK].access = false;
	Scene[CHEYUK].bscript = false;
	Scene[CHEYUK].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[CHEYUK].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[CHEYUK].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[CHEYUK].can_go[i] = 0;
	Scene[CHEYUK].Character_Num = 0;
	Scene[CHEYUK].can_go_num = 1;
	Scene[CHEYUK].can_go[0] = FIRST_BOKDO2;

	Scene[MAN_TOILET_OUT1].Scene_ID = MAN_TOILET_OUT1;
	Scene[MAN_TOILET_OUT1].item_num = 0;
	Scene[MAN_TOILET_OUT1].access = true;
	Scene[MAN_TOILET_OUT1].bscript = false;
	Scene[MAN_TOILET_OUT1].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[MAN_TOILET_OUT1].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[MAN_TOILET_OUT1].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[MAN_TOILET_OUT1].can_go[i] = 0;
	Scene[MAN_TOILET_OUT1].Character_Num = 0;
	Scene[MAN_TOILET_OUT1].can_go_num = 3;
	Scene[MAN_TOILET_OUT1].can_go[0] = MAN_TOILET_IN1;
	Scene[MAN_TOILET_OUT1].can_go[1] = WOMAN_TOILET_OUT1;
	Scene[MAN_TOILET_OUT1].can_go[2] = FIRST_STAIR;

	Scene[WOMAN_TOILET_OUT1].Scene_ID = WOMAN_TOILET_OUT1;
	Scene[WOMAN_TOILET_OUT1].item_num = 0;
	Scene[WOMAN_TOILET_OUT1].access = true;
	Scene[WOMAN_TOILET_OUT1].bscript = false;
	Scene[WOMAN_TOILET_OUT1].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[WOMAN_TOILET_OUT1].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[WOMAN_TOILET_OUT1].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[WOMAN_TOILET_OUT1].can_go[i] = 0;
	Scene[WOMAN_TOILET_OUT1].Character_Num = 0;
	Scene[WOMAN_TOILET_OUT1].can_go_num = 4;
	Scene[WOMAN_TOILET_OUT1].can_go[0] = WOMAN_TOILET_IN1;
	Scene[WOMAN_TOILET_OUT1].can_go[1] = MAN_TOILET_OUT1;
	Scene[WOMAN_TOILET_OUT1].can_go[2] = MAE_JUM;
	Scene[WOMAN_TOILET_OUT1].can_go[3] = KYOMU;

	Scene[MAN_TOILET_IN1].Scene_ID = MAN_TOILET_IN1;
	Scene[MAN_TOILET_IN1].access = true;
	Scene[MAN_TOILET_IN1].bscript = false;
	Scene[MAN_TOILET_IN1].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[MAN_TOILET_IN1].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[MAN_TOILET_IN1].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[MAN_TOILET_IN1].can_go[i] = 0;
	Scene[MAN_TOILET_IN1].item_num = 0;
	Scene[MAN_TOILET_IN1].Character_Num = 0;
	Scene[MAN_TOILET_IN1].can_go_num = 1;
	Scene[MAN_TOILET_IN1].can_go[0] = MAN_TOILET_OUT1;

	Scene[WOMAN_TOILET_IN1].Scene_ID = WOMAN_TOILET_IN1;
	Scene[WOMAN_TOILET_IN1].item_num = 0;
	Scene[WOMAN_TOILET_IN1].access = true;
	Scene[WOMAN_TOILET_IN1].bscript = false;
	Scene[WOMAN_TOILET_IN1].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[WOMAN_TOILET_IN1].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[WOMAN_TOILET_IN1].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[WOMAN_TOILET_IN1].can_go[i] = 0;
	Scene[WOMAN_TOILET_IN1].Character_Num = 0;
	Scene[WOMAN_TOILET_IN1].can_go_num = 1;
	Scene[WOMAN_TOILET_IN1].can_go[0] = WOMAN_TOILET_OUT1;

	Scene[SECOND_STAIR].Scene_ID = SECOND_STAIR;
	Scene[SECOND_STAIR].item_num = 0;
	Scene[SECOND_STAIR].access = true;
	Scene[SECOND_STAIR].bscript = false;
	Scene[SECOND_STAIR].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[SECOND_STAIR].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[SECOND_STAIR].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[SECOND_STAIR].can_go[i] = 0;
	Scene[SECOND_STAIR].Character_Num = 0;
	Scene[SECOND_STAIR].can_go_num = 4;
	Scene[SECOND_STAIR].can_go[0] = FIRST_STAIR;
	Scene[SECOND_STAIR].can_go[1] = THIRD_STAIR;
	Scene[SECOND_STAIR].can_go[2] = SECOND_BOKDO1;
	Scene[SECOND_STAIR].can_go[3] = SECOND_BOKDO2;

	Scene[SECOND_BOKDO1].Scene_ID = SECOND_BOKDO1;
	Scene[SECOND_BOKDO1].item_num = 0;
	Scene[SECOND_BOKDO1].access = true;
	Scene[SECOND_BOKDO1].bscript = false;
	Scene[SECOND_BOKDO1].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[SECOND_BOKDO1].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[SECOND_BOKDO1].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[SECOND_BOKDO1].can_go[i] = 0;
	Scene[SECOND_BOKDO1].Character_Num = 0;
	Scene[SECOND_BOKDO1].can_go_num = 3;
	Scene[SECOND_BOKDO1].can_go[0] = SECOND_STAIR;
	Scene[SECOND_BOKDO1].can_go[1] = SECOND_CLASS;
	Scene[SECOND_BOKDO1].can_go[2] = EXER;

	Scene[EXER].Scene_ID = EXER;
	Scene[EXER].item_num = 0;
	Scene[EXER].access = false;
	Scene[EXER].bscript = false;
	Scene[EXER].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[EXER].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[EXER].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[EXER].can_go[i] = 0;
	Scene[EXER].Character_Num = 0;
	Scene[EXER].can_go_num = 1;
	Scene[EXER].can_go[0] = SECOND_BOKDO1;

	Scene[SECOND_CLASS].Scene_ID = SECOND_CLASS;
	Scene[SECOND_CLASS].item_num = 0;
	Scene[SECOND_CLASS].access = false;
	Scene[SECOND_CLASS].bscript = false;
	Scene[SECOND_CLASS].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[SECOND_CLASS].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[SECOND_CLASS].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[SECOND_CLASS].can_go[i] = 0;
	Scene[SECOND_CLASS].Character_Num = 0;
	Scene[SECOND_CLASS].can_go_num = 1;
	Scene[SECOND_CLASS].can_go[0] = SECOND_BOKDO1;

	Scene[SECOND_BOKDO2].Scene_ID = SECOND_BOKDO2;
	Scene[SECOND_BOKDO2].item_num = 0;
	Scene[SECOND_BOKDO2].access = true;
	Scene[SECOND_BOKDO2].bscript = false;
	Scene[SECOND_BOKDO2].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[SECOND_BOKDO2].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[SECOND_BOKDO2].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[SECOND_BOKDO2].can_go[i] = 0;
	Scene[SECOND_BOKDO2].Character_Num = 0;
	Scene[SECOND_BOKDO2].can_go_num = 3;
	Scene[SECOND_BOKDO2].can_go[0] = SECOND_STAIR;
	Scene[SECOND_BOKDO2].can_go[1] = MUSIC_CLASS;
	Scene[SECOND_BOKDO2].can_go[2] = EXPERIMENT;

	Scene[MUSIC_CLASS].Scene_ID = MUSIC_CLASS;
	Scene[MUSIC_CLASS].item_num = 0;
	Scene[MUSIC_CLASS].item[0] = 1000;
	Scene[MUSIC_CLASS].access = false;
	Scene[MUSIC_CLASS].bscript = false;
	Scene[MUSIC_CLASS].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[MUSIC_CLASS].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[MUSIC_CLASS].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[MUSIC_CLASS].can_go[i] = 0;
	Scene[MUSIC_CLASS].Character_Num = 0;
	Scene[MUSIC_CLASS].can_go_num = 2;
	Scene[MUSIC_CLASS].can_go[0] = SECOND_BOKDO2;
	Scene[MUSIC_CLASS].can_go[1] = ART_CLASS;

	Scene[EXPERIMENT].Scene_ID = EXPERIMENT;
	Scene[EXPERIMENT].item_num = 0;
	Scene[EXPERIMENT].access = false;
	Scene[EXPERIMENT].bscript = false;
	Scene[EXPERIMENT].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[EXPERIMENT].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[EXPERIMENT].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[EXPERIMENT].can_go[i] = 0;
	Scene[EXPERIMENT].Character_Num = 0;
	Scene[EXPERIMENT].can_go_num = 1;
	Scene[EXPERIMENT].can_go[0] = SECOND_BOKDO2;

	Scene[THIRD_STAIR].Scene_ID = THIRD_STAIR;
	Scene[THIRD_STAIR].item_num = 0;
	Scene[THIRD_STAIR].access = true;
	Scene[THIRD_STAIR].bscript = false;
	Scene[THIRD_STAIR].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[THIRD_STAIR].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[THIRD_STAIR].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[THIRD_STAIR].can_go[i] = 0;
	Scene[THIRD_STAIR].Character_Num = 0;
	Scene[THIRD_STAIR].can_go_num = 4;
	Scene[THIRD_STAIR].can_go[0] = SECOND_STAIR;
	Scene[THIRD_STAIR].can_go[1] = FORTH_STAIR;
	Scene[THIRD_STAIR].can_go[2] = MAN_TOILET_OUT3;
	Scene[THIRD_STAIR].can_go[3] = THIRD_BOKDO1;


	Scene[MAN_TOILET_OUT3].Scene_ID = MAN_TOILET_OUT3;
	Scene[MAN_TOILET_OUT3].item_num = 0;
	Scene[MAN_TOILET_OUT3].access = true;
	Scene[MAN_TOILET_OUT3].bscript = false;
	Scene[MAN_TOILET_OUT3].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[MAN_TOILET_OUT3].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[MAN_TOILET_OUT3].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[MAN_TOILET_OUT3].can_go[i] = 0;
	Scene[MAN_TOILET_OUT3].Character_Num = 0;
	Scene[MAN_TOILET_OUT3].can_go_num = 3;
	Scene[MAN_TOILET_OUT3].can_go[0] = MAN_TOILET_IN3;
	Scene[MAN_TOILET_OUT3].can_go[1] = WOMAN_TOILET_OUT3;
	Scene[MAN_TOILET_OUT3].can_go[2] = THIRD_STAIR;

	Scene[WOMAN_TOILET_OUT3].Scene_ID = WOMAN_TOILET_OUT3;
	Scene[WOMAN_TOILET_OUT3].item_num = 0;
	Scene[WOMAN_TOILET_OUT3].access = true;
	Scene[WOMAN_TOILET_OUT3].bscript = false;
	Scene[WOMAN_TOILET_OUT3].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[WOMAN_TOILET_OUT3].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[WOMAN_TOILET_OUT3].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[WOMAN_TOILET_OUT3].can_go[i] = 0;
	Scene[WOMAN_TOILET_OUT3].Character_Num = 0;
	Scene[WOMAN_TOILET_OUT3].can_go_num = 2;
	Scene[WOMAN_TOILET_OUT3].can_go[0] = WOMAN_TOILET_IN3;
	Scene[WOMAN_TOILET_OUT3].can_go[1] = MAN_TOILET_OUT3;

	Scene[MAN_TOILET_IN3].Scene_ID = MAN_TOILET_IN3;
	Scene[MAN_TOILET_IN3].item_num = 0;
	Scene[MAN_TOILET_IN3].access = true;
	Scene[MAN_TOILET_IN3].bscript = false;
	Scene[MAN_TOILET_IN3].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[MAN_TOILET_IN3].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[MAN_TOILET_IN3].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[MAN_TOILET_IN3].can_go[i] = 0;
	Scene[MAN_TOILET_IN3].Character_Num = 0;
	Scene[MAN_TOILET_IN3].can_go_num = 1;
	Scene[MAN_TOILET_IN3].can_go[0] = MAN_TOILET_OUT3;

	Scene[WOMAN_TOILET_IN3].Scene_ID = WOMAN_TOILET_IN3;
	Scene[WOMAN_TOILET_IN3].item_num = 0;
	Scene[WOMAN_TOILET_IN3].access = true;
	Scene[WOMAN_TOILET_IN3].bscript = false;
	Scene[WOMAN_TOILET_IN3].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[WOMAN_TOILET_IN3].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[WOMAN_TOILET_IN3].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[WOMAN_TOILET_IN3].can_go[i] = 0;
	Scene[WOMAN_TOILET_IN3].Character_Num = 0;
	Scene[WOMAN_TOILET_IN3].can_go_num = 1;
	Scene[WOMAN_TOILET_IN3].can_go[0] = WOMAN_TOILET_OUT3;


	Scene[THIRD_BOKDO1].Scene_ID = THIRD_BOKDO1;
	Scene[THIRD_BOKDO1].item_num = 0;
	Scene[THIRD_BOKDO1].access = true;
	Scene[THIRD_BOKDO1].bscript = false;
	Scene[THIRD_BOKDO1].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[THIRD_BOKDO1].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[THIRD_BOKDO1].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[THIRD_BOKDO1].can_go[i] = 0;
	Scene[THIRD_BOKDO1].Character_Num = 0;
	Scene[THIRD_BOKDO1].can_go_num = 3;
	Scene[THIRD_BOKDO1].can_go[0] = THIRD_STAIR;
	Scene[THIRD_BOKDO1].can_go[1] = THIRD_CLASS;
	Scene[THIRD_BOKDO1].can_go[2] = THIRD_CLASS2;

	Scene[THIRD_CLASS].Scene_ID = THIRD_CLASS;
	Scene[THIRD_CLASS].item_num = 0;
	Scene[THIRD_CLASS].access = false;
	Scene[THIRD_CLASS].bscript = false;
	Scene[THIRD_CLASS].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[THIRD_CLASS].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[THIRD_CLASS].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[THIRD_CLASS].can_go[i] = 0;
	Scene[THIRD_CLASS].Character_Num = 0;
	Scene[THIRD_CLASS].can_go_num = 1;
	Scene[THIRD_CLASS].can_go[0] = THIRD_BOKDO1;

	Scene[THIRD_CLASS2].Scene_ID = THIRD_CLASS2;
	Scene[THIRD_CLASS2].item_num = 0;
	Scene[THIRD_CLASS2].access = true;
	Scene[THIRD_CLASS2].bscript = false;
	Scene[THIRD_CLASS2].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[THIRD_CLASS2].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[THIRD_CLASS2].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[THIRD_CLASS2].can_go[i] = 0;
	Scene[THIRD_CLASS2].Character_Num = 0;
	Scene[THIRD_CLASS2].can_go_num = 1;
	Scene[THIRD_CLASS2].can_go[0] = THIRD_BOKDO1;

	Scene[FORTH_STAIR].Scene_ID = FORTH_STAIR;
	Scene[FORTH_STAIR].item_num = 0;
	Scene[FORTH_STAIR].access = true;
	Scene[FORTH_STAIR].bscript = false;
	Scene[FORTH_STAIR].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[FORTH_STAIR].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[FORTH_STAIR].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[FORTH_STAIR].can_go[i] = 0;
	Scene[FORTH_STAIR].Character_Num = 0;
	Scene[FORTH_STAIR].can_go_num = 4;
	Scene[FORTH_STAIR].can_go[0] = THIRD_STAIR;
	Scene[FORTH_STAIR].can_go[1] = OK_SANG;
	Scene[FORTH_STAIR].can_go[2] = FORTH_BOKDO1;
	Scene[FORTH_STAIR].can_go[3] = FORTH_BOKDO2;

	Scene[OK_SANG].Scene_ID = OK_SANG;
	Scene[OK_SANG].item_num = 0;
	Scene[OK_SANG].access = false;
	Scene[OK_SANG].bscript = false;
	Scene[OK_SANG].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[OK_SANG].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[OK_SANG].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[OK_SANG].can_go[i] = 0;
	Scene[OK_SANG].Character_Num = 0;
	Scene[OK_SANG].can_go_num = 1;
	Scene[OK_SANG].can_go[0] = FORTH_STAIR;

	Scene[FORTH_BOKDO1].Scene_ID = FORTH_BOKDO1;
	Scene[FORTH_BOKDO1].item_num = 0;
	Scene[FORTH_BOKDO1].access = true;
	Scene[FORTH_BOKDO1].bscript = false;
	Scene[FORTH_BOKDO1].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[FORTH_BOKDO1].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[FORTH_BOKDO1].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[FORTH_BOKDO1].can_go[i] = 0;
	Scene[FORTH_BOKDO1].Character_Num = 0;
	Scene[FORTH_BOKDO1].can_go_num = 3;
	Scene[FORTH_BOKDO1].can_go[0] = FORTH_STAIR;
	Scene[FORTH_BOKDO1].can_go[1] = FORTH_CLASS;
	Scene[FORTH_BOKDO1].can_go[2] = YANG_HO;


	Scene[FORTH_BOKDO2].Scene_ID = FORTH_BOKDO2;
	Scene[FORTH_BOKDO2].item_num = 0;
	Scene[FORTH_BOKDO2].access = true;
	Scene[FORTH_BOKDO2].bscript = false;
	Scene[FORTH_BOKDO2].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[FORTH_BOKDO2].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[FORTH_BOKDO2].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[FORTH_BOKDO2].can_go[i] = 0;
	Scene[FORTH_BOKDO2].Character_Num = 0;
	Scene[FORTH_BOKDO2].can_go_num = 1;
	Scene[FORTH_BOKDO2].can_go[0] = FORTH_STAIR;



	Scene[FORTH_CLASS].Scene_ID = FORTH_CLASS;
	Scene[FORTH_CLASS].item_num = 0;
	Scene[FORTH_CLASS].access = false;
	Scene[FORTH_CLASS].bscript = false;
	Scene[FORTH_CLASS].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[FORTH_CLASS].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[FORTH_CLASS].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[FORTH_CLASS].can_go[i] = 0;
	Scene[FORTH_CLASS].Character_Num = 0;
	Scene[FORTH_CLASS].can_go_num = 1;
	Scene[FORTH_CLASS].can_go[0] = FORTH_BOKDO1;

	Scene[YANG_HO].Scene_ID = YANG_HO;
	Scene[YANG_HO].item_num = 0;
	Scene[YANG_HO].access = false;
	Scene[YANG_HO].bscript = false;
	Scene[YANG_HO].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[YANG_HO].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[YANG_HO].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[YANG_HO].can_go[i] = 0;
	Scene[YANG_HO].Character_Num = 0;
	Scene[YANG_HO].can_go_num = 1;
	Scene[YANG_HO].can_go[0] = FORTH_BOKDO1;




	Scene[MAE_JUM].Scene_ID = MAE_JUM;
	Scene[MAE_JUM].item_num = 0;
	Scene[MAE_JUM].access = false;
	Scene[MAE_JUM].bscript = false;
	Scene[MAE_JUM].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[MAE_JUM].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[MAE_JUM].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[MAE_JUM].can_go[i] = 0;
	Scene[MAE_JUM].Character_Num = 0;
	Scene[MAE_JUM].can_go_num = 1;
	Scene[MAE_JUM].can_go[0] = WOMAN_TOILET_OUT1;

	Scene[KYOMU].Scene_ID = KYOMU;
	Scene[KYOMU].item_num = 0;
	Scene[KYOMU].access = false;
	Scene[KYOMU].bscript = false;
	Scene[KYOMU].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[KYOMU].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[KYOMU].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[KYOMU].can_go[i] = 0;
	Scene[KYOMU].Character_Num = 0;
	Scene[KYOMU].can_go_num = 1;
	Scene[KYOMU].can_go[0] = WOMAN_TOILET_OUT1;

	Scene[ART_CLASS].Scene_ID = ART_CLASS;
	Scene[ART_CLASS].item_num = 0;
	Scene[ART_CLASS].access = false;
	Scene[ART_CLASS].bscript = false;
	Scene[ART_CLASS].script_no = 0;
	for (i = 0; i < 5; i++)
		Scene[ART_CLASS].item[i] = 0;
	for (i = 0; i < 10; i++)
		Scene[ART_CLASS].Character[i] = 0;
	for (i = 0; i < 35; i++)
		Scene[ART_CLASS].can_go[i] = 0;
	Scene[ART_CLASS].Character_Num = 0;
	Scene[ART_CLASS].can_go_num = 1;
	Scene[ART_CLASS].can_go[0] = MUSIC_CLASS;


}
void Menu_Display2()
{
	if (g_Global_State.Menu_open == false)
	{
		g_Global_State.Menu_open = true;
		InitMenu2();
		g_Global_State.Key_Access = false;
	}
}
void Check_Move_Mode()
{
	if (g_Global_State.Error_Message == true)
	{
		int i;
		g_Global_State.Error_Message = false;
		if (g_Global_State.Next_Move == MUSIC_CLASS)
		{
			for (i = 0; i < g_Hero.item_num; i++)
			{
				if (g_Hero.item[i] == ITEM_KEY)
				{
					Scene[g_Global_State.Next_Move].access = true;
					nani2 = 1;
					nani3 = 1;
				}
			}
		}
		else if (g_Global_State.Next_Move == EXER)
		{
			for (i = 0; i < g_Hero.item_num; i++)
			{
				if (g_Hero.item[i] == ITEM_KEY2)
				{
					Scene[g_Global_State.Next_Move].access = true;
					nani2 = 1;
					nani3 = 1;
				}
			}
		}

		else if (g_Global_State.Next_Move == THIRD_CLASS)
		{
			for (i = 0; i < g_Hero.item_num; i++)
			{
				if (g_Hero.item[i] == ITEM_KEY4)
				{
					Scene[g_Global_State.Next_Move].access = true;
					nani2 = 1;
					nani3 = 1;
				}
			}
		}

		else if (g_Global_State.Next_Move == EXPERIMENT)
		{
			for (i = 0; i < g_Hero.item_num; i++)
			{
				if (g_Hero.item[i] == ITEM_KEY3)
				{
					Scene[g_Global_State.Next_Move].access = true;
					nani3 = 1;
					nani2 = 1;
				}
			}
		}
		else g_Global_State.Error_Message = true;
		g_Hero.game_mode = CANT_GO;
	}


	else if (!Gnerate_Next_Scene(0))
		g_Hero.game_mode = MENU_DISPLAY_MODE;
}
int image_map()
{
	if (g_Hero.location == WOMAN_TOILET_OUT1 || g_Hero.location == WOMAN_TOILET_OUT3)
		return 0;
	if (g_Hero.location == FRONT_DOOR)
		return 1;
	if (g_Hero.location == EXIT)
		return 2;
	if (g_Hero.location == FIRST_STAIR || g_Hero.location == SECOND_STAIR || g_Hero.location == THIRD_STAIR || g_Hero.location == FORTH_STAIR)
		return 3;
	if (g_Hero.location == OK_SANG)
		return 4;
	if (g_Hero.location == EXPERIMENT)
		return 5;
	if (g_Hero.location == EXER)
		return 6;
	if (g_Hero.location == YANG_HO)
		return 7;
	if (g_Hero.location == KYOMU)
		return 8;
	if (g_Hero.location == MAE_JUM)
		return 9;
	if (g_Hero.location == FIRST_CLASS || g_Hero.location == SECOND_CLASS || g_Hero.location == THIRD_CLASS || g_Hero.location == THIRD_CLASS2 || g_Hero.location == FORTH_CLASS)
		return 10;
	if (g_Hero.location == MUSIC_CLASS)
		return 11;
	if (g_Hero.location == ART_CLASS)
		return 12;
	if (g_Hero.location == WOMAN_TOILET_IN1 || g_Hero.location == WOMAN_TOILET_IN3)
		return 13;
	if (g_Hero.location == MAN_TOILET_OUT1 || g_Hero.location == MAN_TOILET_OUT3)
		return 14;
	if (g_Hero.location == MAN_TOILET_IN1 || g_Hero.location == MAN_TOILET_IN3)
		return 15;
	if (g_Hero.location == FIRST_BOKDO1 || g_Hero.location == SECOND_BOKDO1 || g_Hero.location == THIRD_BOKDO1 || g_Hero.location == FORTH_BOKDO1)
		return 16;
	if (g_Hero.location == FIRST_BOKDO2 || g_Hero.location == SECOND_BOKDO2 || g_Hero.location == FORTH_BOKDO2)
		return 17;
	if (g_Hero.location == CHEYUK)
		return 18;
	if (g_Hero.location == GAME_END)
		return 19;
	return 0;
}
void Check_CanGetItem()
{
	int top, bottom, left, right, j, i;
	if (Scene[g_Hero.location].item_num > 0)
	{


		if (Scene[g_Hero.location].Scene_ID == MUSIC_CLASS && bspecial == false)
		{
			left = Item[Scene[g_Hero.location].item[0]].rect.left;
			right = Item[Scene[g_Hero.location].item[0]].rect.right;
			top = Item[Scene[g_Hero.location].item[0]].rect.top;
			bottom = Item[Scene[g_Hero.location].item[0]].rect.bottom;
			if (g_Global_State.mouse_loc.x > left && g_Global_State.mouse_loc.x<right
				&& g_Global_State.mouse_loc.y>top && g_Global_State.mouse_loc.y < bottom)
			{
				for (i = 0; i < g_Hero.item_num; i++)
				{
					left = Item[Scene[g_Hero.location].item[i]].rect.left;
					right = Item[Scene[g_Hero.location].item[i]].rect.right;
					top = Item[Scene[g_Hero.location].item[i]].rect.top;
					bottom = Item[Scene[g_Hero.location].item[i]].rect.bottom;
					if (g_Hero.item[i] == ITEM_OAKBO)
					{
						Scene[g_Hero.location].item_num--;
						g_point += 30;
						bspecial = true;
						g_Hero.game_mode = SPECIAL_SCRIPT;
						g_Global_State.script_copy = false;
						g_Global_State.Script_State = SCRIPT_READING;
						return;
					}
					nani = 1;
				}
			}
		}
		else
		{

			for (i = 0; i < Scene[g_Hero.location].item_num; i++)

			{
				left = Item[Scene[g_Hero.location].item[i]].rect.left;

				right = Item[Scene[g_Hero.location].item[i]].rect.right;

				top = Item[Scene[g_Hero.location].item[i]].rect.top;

				bottom = Item[Scene[g_Hero.location].item[i]].rect.bottom;


				if (g_Global_State.mouse_loc.x > left && g_Global_State.mouse_loc.x<right
					&& g_Global_State.mouse_loc.y>top && g_Global_State.mouse_loc.y < bottom)
				{
					g_Global_State.Get_Item_Message = Scene[g_Hero.location].item[i];
					g_Hero.item[g_Hero.item_num] = Scene[g_Hero.location].item[i];
					g_Hero.item_num++;
					j = i;
					while (j < (Scene[g_Hero.location].item_num - 1))
					{
						Scene[g_Hero.location].item[j] = Scene[g_Hero.location].item[j + 1];
						j++;
					}

					g_Global_State.effectRunning = 1;
					g_Global_State.Playing = 0;
					SwapSourceFilter2(1);
					Scene[g_Hero.location].item_num--;
					g_point += 3;
					g_Global_State.Get_Item = true;
					break;

				}
				else g_Global_State.Get_Item = false;
			}
		}
	}
	g_Hero.game_mode = DISPLAY_GET_ITEM;
}


int GetText_1(float fElapsedTime)
{

	char seps[] = " \n"; //seperator입니다					
	char seps2[] = "\n"; //seperator입니다					
	char seps3[] = "@"; //seperator입니다					
	char* token;
	int return_value = 0;
	int count = 0;
	if (g_Global_State.Get_Messge_Data == true && g_Global_State.Script_State == SCRIPT_READING)
	{
		g_Global_State.Key_Access = false;
		g_Global_State.Get_Messge_Data = false;
		g_Global_State.Script_Check_Point = g_Global_State.Script_Current_Count + 1;
		token = strtok(g_message_data[g_Global_State.Script_Current_Count], seps);
		if (!strcmp(token, "SPEAKING"))
		{
			token = strtok(NULL, seps);
			strcpy(g_name_str, token);
			g_Global_State.Script_Current_Count++;
			token = strtok(NULL, seps);
			return_value = atoi(token);
			if (token)
			{
				int j;
				token = strtok(NULL, seps);
				if (token)
				{
					j = atoi(token);
					if (j == 1000)
						g_Global_State.Char_Present = 0;
					if (j == 2000)
					{
						g_Global_State.Sound = 2000;
						g_Global_State.effectRunning = 1;
						g_Global_State.Playing = 0;
						SwapSourceFilter2(2);
						Sleep(4000);
					}
					if (j == 2001)
					{
						g_Global_State.Sound = 2001;
						g_Global_State.effectRunning = 1;
						g_Global_State.Playing = 0;
						SwapSourceFilter2(3);
						Sleep(16000);
					}
					if (j == 2002)
					{
						g_Global_State.Sound = 2002;
						g_Global_State.effectRunning = 1;
						g_Global_State.Playing = 0;
						g_pMediaControl->Stop();
						SwapSourceFilter2(4);
						Sleep(10000);
						SwapSourceFilter();
					}
					if (j == 2003)
					{
						g_Global_State.Sound = 2003;
						g_Global_State.effectRunning = 1;
						g_Global_State.Playing = 0;
						g_pMediaControl->Stop();
						SwapSourceFilter2(5);
						Sleep(4000);
						g_pMediaControl->Run();
					}
					if (j == 2004)
					{
						g_Global_State.Sound = 2004;
						g_Global_State.effectRunning = 1;
						g_Global_State.Playing = 0;
						g_pMediaControl->Stop();
						SwapSourceFilter2(6);
						Sleep(2000);
						g_pMediaControl->Run();
					}
					if (j == 2006)
					{
						g_Global_State.Sound = 2006;
						g_Global_State.effectRunning = 1;
						g_Global_State.Playing = 0;
						g_pMediaControl->Stop();
						SwapSourceFilter2(8);
						Sleep(2000);
						g_pMediaControl->Run();
					}
					if (j == 2007)
					{
						g_Global_State.Sound = 2007;
						g_Global_State.effectRunning = 1;
						g_Global_State.Playing = 0;
						g_pMediaControl->Stop();
						SwapSourceFilter2(9);
						Sleep(2000);
						g_pMediaControl->Run();
					}
					if (j == 2008)
					{
						g_Global_State.Sound = 2008;
						g_Global_State.effectRunning = 1;
						g_Global_State.Playing = 0;
						g_pMediaControl->Stop();
						SwapSourceFilter2(10);
						Sleep(2000);
						g_pMediaControl->Run();
					}
				}
			}

		}
		while (1)
		{
			token = strtok(g_message_data[g_Global_State.Script_Current_Count], seps);
			if (!strcmp(token, "SPEAKING"))
			{
				strcpy(g_message_data[g_Global_State.Script_Current_Count], g_message_data2[g_Global_State.Script_Current_Count]);
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

			if (!strcmp(token, "TEXT_END"))
			{
				g_Global_State.Script_State = SCRIPT_TER;
				break;
			}
			g_Global_State.Script_Current_Count++;
			count++;

		}
		g_Global_State.Script_Length = count;
	}

	return return_value;
}

void GetText_2(float fElapsedTime)
{
	char* token;
	char seps[] = " \n"; //seperator입니다					
	if (g_Global_State.bProcessKey == true && (g_Global_State.Script_State == SCRIPT_READING || (g_Global_State.Script_State == SCRIPT_TER)))
	{
		token = strtok(g_message_data[g_Global_State.Script_Check_Point], seps);
		if (!strcmp(token, "TEXT_END"))
		{
			/*token=strtok(NULL,seps);
			g_Global_State.Script_State=atoi(token);*/
			g_LineRead = 0;
			g_Global_State.Procees_script = 1;
			g_Global_State.Key_Access = true;
			if (g_Global_State.Script_State == SCRIPT_TER)
				g_Global_State.Script_State = SCRIPT_END;
		}
		else
		{
			if (g_Global_State.Script_Length > 0)

			{

				g_LineRead++;

				strcpy(g_string, g_message_data2[g_Global_State.Script_Check_Point]);

				g_Global_State.Script_Check_Point++;

				g_Global_State.Script_Length--;

			}

			if (g_Global_State.Script_Length > 0)

			{

				g_LineRead++;

				strcpy(g_string2, g_message_data2[g_Global_State.Script_Check_Point]);

				g_Global_State.Script_Check_Point++;

				g_Global_State.Script_Length--;

			}

		}
		g_Global_State.bProcessKey = false;
	}

	if (g_LineRead == 1)
	{
		MultiByteToWideChar(CP_ACP, 0, g_string, -1,
			g_wFileName, MAX_PATH);
	}
	else if (g_LineRead == 2)
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
	g_d3dHangul2.SetColor(0x0ffffff);
	k = strlen(g_string);
	t = strlen(g_string2);
	if (g_LineRead == 1) // 이부분은 한줄을 읽었을 때의 경우가 존재할때를 말한다.(대사의 끝부분의 경우)
	{
		MultiByteToWideChar(CP_ACP, 0, g_name_str, -1,
			g_wFileName, MAX_PATH);
		//g_d3dHangul2.OutHanXY(20, 390, g_wFileName);											
		g_d3dHangul2.OutHanXY(g_width / 30, g_height - (g_height / 5.4), g_wFileName);
		strcpy(buf, g_string);
		buf[qbt] = 0;
		MultiByteToWideChar(CP_ACP, 0, buf, -1,
			g_wFileName, MAX_PATH);
		g_d3dHangul2.OutHanXY(g_width / 20, g_height - (g_height / 10), g_wFileName);
		if (g_Global_State.Key_Access == false)
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
			g_Global_State.Key_Access = true;
			if (g_Global_State.Script_Length == 0)
				g_Global_State.Procees_script = 1;
			if (g_Global_State.Script_State == SCRIPT_TER)
				g_Global_State.Script_State == SCRIPT_END;
		}
	}
	if (g_LineRead == 2)
	{
		MultiByteToWideChar(CP_ACP, 0, g_name_str, -1,
			g_wFileName, MAX_PATH);
		//g_d3dHangul2.OutHanXY(20, 390, g_wFileName);											
		g_d3dHangul2.OutHanXY(g_width / 30, g_height - (g_height / 5.4), g_wFileName);
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
		if (g_Global_State.Key_Access == false)
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
			g_Global_State.Key_Access = true;
			if (g_Global_State.Script_Length == 0)
				g_Global_State.Procees_script = 1;
			if (g_Global_State.Script_State == SCRIPT_TER)
				g_Global_State.Script_State == SCRIPT_END;
		}
	}


	Sleep(10);
}
void Init_Script_Var()
{
	g_Global_State.Menu_open = false;
	g_Global_State.Key_Access = false;
	g_Global_State.Next_Move = -1;
	g_Global_State.Procees_script = 0;
	g_Global_State.Conv_State = 0;
	g_Global_State.Script_Current_Count = 0;
	g_Global_State.Script_Check_Point = 0;
	g_Global_State.Script_State = SCRIPT_READING;
	g_Global_State.Script_Count = 0;
	g_Global_State.script_copy = false;
	g_Global_State.Char_Present = 0;
	g_Global_State.Get_Messge_Data = true;
	g_Global_State.bProcessKey = true;
	ghj = 0;

}
bool Gnerate_Next_Scene(int i)
{
	bool val;
	val = false;
	if (i)
	{
		g_Global_State.Script_No++;
		//g_Global_State.Script_No=10; 
		Init_Script_Var();
	}
	switch (g_Global_State.Script_No)
	{
	case 1:
		g_Hero.game_mode = SCRIPT_MODE;
		break;
	case 2:
		g_Hero.game_mode = MENU_DISPLAY_MODE;
		if (g_Hero.location == FIRST_STAIR)
		{
			g_point += 5;
			g_Hero.game_mode = SCRIPT_MODE;
			val = true;
		}
		break;

	case 3:
		g_Hero.game_mode = MENU_DISPLAY_MODE;
		if (g_Hero.location == THIRD_CLASS2)
		{
			g_point += 5;
			Scene[FRONT_DOOR].access = false;
			g_Hero.game_mode = SCRIPT_MODE;
			val = true;
		}
		break;

	case 4:
		g_Hero.game_mode = MENU_DISPLAY_MODE;
		if (g_Hero.location == THIRD_CLASS2)
		{

			SwapSourceFilter();
			g_Global_State.Text_Out++;
			g_Global_State.Date_Display = true;
			g_Hero.game_mode = SCRIPT_MODE;
			val = true;
		}
		break;

	case 5:
		g_Hero.game_mode = MENU_DISPLAY_MODE;
		if (g_Hero.location == EXIT)
		{
			g_point += 5;
			Item_Set();
			g_Hero.game_mode = SCRIPT_MODE;
			val = true;
		}
		break;

	case 6:
		g_Hero.game_mode = MENU_DISPLAY_MODE;
		if (g_Hero.location == MAN_TOILET_IN1)
		{
			g_point += 5;
			g_Hero.game_mode = SCRIPT_MODE;
			val = true;
		}
		break;

	case 7:
		g_Hero.game_mode = MENU_DISPLAY_MODE;
		if (g_Hero.location == SECOND_BOKDO2)
		{
			g_point += 5;
			Scene[OK_SANG].access = true;
			g_Hero.game_mode = SCRIPT_MODE;
			val = true;
		}
		break;

	case 8:
		g_Hero.game_mode = MENU_DISPLAY_MODE;
		if (g_Hero.location == FORTH_STAIR)
		{
			g_point += 5;
			SwapSourceFilter();
			Scene[THIRD_CLASS2].access = false;
			g_Global_State.Text_Out++;
			g_Global_State.Key_Access = false;
			g_Global_State.Date_Display = true;
			g_Hero.game_mode = SCRIPT_MODE;
			val = true;
		}
		break;
	case 9:
		g_Hero.game_mode = MENU_DISPLAY_MODE;
		if (g_Hero.location == FIRST_BOKDO2)
		{
			g_point += 5;
			g_Global_State.Text_Out++;
			Scene[FIRST_BOKDO2].Character_Num++;
			Scene[FIRST_BOKDO2].Character[0] = MASUCHEN;
			g_Global_State.Key_Access = false;
			g_Global_State.Date_Display = true;
			g_Hero.game_mode = SCRIPT_MODE;
			val = true;
		}
		break;
	case 10:
		g_Hero.game_mode = MENU_DISPLAY_MODE;
		if (g_Hero.location == WOMAN_TOILET_IN3)
		{
			g_point += 5;
			SwapSourceFilter();
			Scene[OK_SANG].access = false;
			g_Hero.game_mode = SCRIPT_MODE;
			val = true;
		}
		break;
	case 11:
		g_Hero.game_mode = MENU_DISPLAY_MODE;
		if (g_Hero.location == FORTH_STAIR)
		{
			g_point += 5;
			g_Hero.game_mode = SCRIPT_MODE;
			val = true;
		}
		break;
	case 12:
		g_Hero.game_mode = MENU_DISPLAY_MODE;
		if (g_Hero.location == THIRD_BOKDO1)
		{
			g_point += 5;
			g_Global_State.Text_Out++;
			g_Global_State.Key_Access = false;
			g_Global_State.Date_Display = true;
			g_Hero.game_mode = SCRIPT_MODE;
			val = true;
		}
		break;
	case 13:
		g_Hero.game_mode = MENU_DISPLAY_MODE;
		if (g_Hero.location == FORTH_BOKDO1)
		{
			g_point += 5;
			g_Hero.game_mode = SCRIPT_MODE;
			val = true;
		}
		break;
	case 14:
		g_Hero.game_mode = MENU_DISPLAY_MODE;
		if (g_Hero.location == WOMAN_TOILET_OUT1)
		{
			g_point += 5;
			Scene[KYOMU].access = true;
			g_Hero.game_mode = SCRIPT_MODE;
			val = true;
		}
		break;
	case 15:
		g_Hero.game_mode = MENU_DISPLAY_MODE;
		if (g_Hero.location == KYOMU)
		{
			g_point += 5;
			SwapSourceFilter();
			g_Global_State.Text_Out++;
			g_Global_State.Key_Access = false;
			g_Global_State.Date_Display = true;
			g_Hero.game_mode = SCRIPT_MODE;
			val = true;
		}
		break;
	case 16:
		g_Hero.game_mode = MENU_DISPLAY_MODE;
		if (g_Hero.location == WOMAN_TOILET_OUT3)
		{
			g_point += 5;
			g_Global_State.Text_Out++;
			g_Global_State.Key_Access = false;
			g_Global_State.Date_Display = true;
			g_Hero.game_mode = SCRIPT_MODE;
			val = true;
		}
		break;
	case 17:
		g_Hero.game_mode = MENU_DISPLAY_MODE;
		if (g_Hero.location == FIRST_BOKDO2)
		{
			Scene[FIRST_BOKDO2].Character_Num = 0;
			Scene[CHEYUK].access = true;
			g_Hero.game_mode = SCRIPT_MODE;
			val = true;
		}
		break;
	case 18:
		g_Hero.game_mode = MENU_DISPLAY_MODE;
		if (g_Hero.location == CHEYUK)
		{
			g_point += 5;
			g_Hero.game_mode = SCRIPT_MODE;
			val = true;
		}
		break;
	case 19:
		g_Hero.game_mode = MENU_DISPLAY_MODE;
		if (g_Hero.location == FIRST_BOKDO2)
		{

			SwapSourceFilter();
			g_Global_State.Text_Out++;
			g_Global_State.Key_Access = false;
			g_Global_State.Date_Display = true;
			g_Hero.game_mode = SCRIPT_MODE;
			val = true;
		}
		break;
	case 20:
		g_Hero.game_mode = MENU_DISPLAY_MODE;
		if (g_Hero.location == FIRST_BOKDO1)
		{
			g_Hero.game_mode = SCRIPT_MODE;
			val = true;
		}
		break;
	case 21:
		g_Hero.game_mode = MENU_DISPLAY_MODE;
		if (g_Hero.location == FIRST_STAIR)
		{
			g_Hero.game_mode = SCRIPT_MODE;
			val = true;
		}
		break;
	case 22:
		g_Hero.game_mode = MENU_DISPLAY_MODE;
		if (g_Hero.location == SECOND_STAIR)
		{
			g_Hero.game_mode = SCRIPT_MODE;
			val = true;
		}
		break;
	case 23:
		g_Hero.game_mode = MENU_DISPLAY_MODE;
		if (g_Hero.location == SECOND_BOKDO1)
		{
			Scene[SECOND_CLASS].access = true;
			SwapSourceFilter();
			g_Hero.game_mode = SCRIPT_MODE;
			val = true;
		}
		break;
	case 24:
		g_Hero.game_mode = MENU_DISPLAY_MODE;
		if (g_Hero.location == SECOND_CLASS)
		{
			int o = 0, p = 0;
			g_Global_State.Text_Out++;
			g_Global_State.Key_Access = false;
			g_Global_State.Date_Display = true;
			for (i = 0; i < g_Hero.item_num; i++)
			{
				if (g_Hero.item[i] == ITEM_유리거울)
					o = 1;
				if (g_Hero.item[i] == ITEM_KNIFE)
					p = 1;
			}
			if (o == 1)
			{
				if (p == 0)
				{
					g_Global_State.end_mode = 1; //배드엔딩 2
					g_Hero.game_mode = END_MODE;
				}
				else
				{
					g_point += 20;
					g_Hero.game_mode = SCRIPT_MODE;
				}
			}
			else
			{
				g_Global_State.end_mode = 0; //배드엔딩 1
				g_Hero.game_mode = END_MODE;
			}
			val = true;
		}
		break;
	case 25:
		g_Hero.game_mode = MENU_DISPLAY_MODE;
		if (g_Hero.location == SECOND_BOKDO1)
		{
			SwapSourceFilter();
			g_Hero.game_mode = SCRIPT_MODE;
			val = true;
		}
		break;
	case 26:
		g_Hero.game_mode = MENU_DISPLAY_MODE;
		if (g_Hero.location == FORTH_BOKDO2)
		{
			int o = 0;
			g_Global_State.Text_Out++;
			g_Global_State.Key_Access = false;
			g_Global_State.Date_Display = true;
			for (i = 0; i < g_Hero.item_num; i++)
			{
				if (g_Hero.item[i] == ITEM_CAMERA)
					o = 1;
			}
			if (o == 1)
			{
				g_point += 20;
				Scene[OK_SANG].access = true;
				g_Hero.game_mode = SCRIPT_MODE;
			}
			else
			{
				g_Global_State.end_mode = 2; //배드엔딩 3
				g_Hero.game_mode = END_MODE;
			}
			val = true;
		}
		break;
	case 27:
		g_Hero.game_mode = MENU_DISPLAY_MODE;
		if (g_Hero.location == EXIT || g_Hero.location == OK_SANG)
		{

			g_Global_State.Text_Out++;
			g_Global_State.Key_Access = false;
			g_Global_State.Date_Display = true;
			if (g_Hero.location == EXIT)
			{
				g_Global_State.end_mode = 3; //중간엔딩
				g_Hero.game_mode = END_MODE;

			}
			else
			{
				g_point += 20;
				g_Hero.game_mode = SCRIPT_MODE;	//리얼 엔딩	
			}
			val = true;
		}
		break;
	case 28:
		if (g_Hero.location == GAME_END)
		{
			SwapSourceFilter();
			g_Global_State.Text_Out++;
			g_Global_State.Key_Access = false;
			g_Global_State.Date_Display = true;
			g_Hero.game_mode = SCRIPT_MODE;
			val = true;
		}
		break;

	default:
		g_Hero.game_mode = MENU_DISPLAY_MODE;
	}
	return val;
}

void Item_Set()
{
	Scene[MAN_TOILET_IN1].item[Scene[MAN_TOILET_IN1].item_num] = ITEM_담배;
	Scene[MAN_TOILET_IN1].item_num++;

	Scene[MAN_TOILET_IN1].item[Scene[MAN_TOILET_IN1].item_num] = ITEM_CD;
	Scene[MAN_TOILET_IN1].item_num++;

	Scene[WOMAN_TOILET_OUT3].item[Scene[WOMAN_TOILET_OUT3].item_num] = ITEM_KEY;
	Scene[WOMAN_TOILET_OUT3].item_num++;

	Scene[OK_SANG].item[Scene[OK_SANG].item_num] = ITEM_KEY2;
	Scene[OK_SANG].item_num++;

	Scene[OK_SANG].item[Scene[OK_SANG].item_num] = ITEM_GLASS;
	Scene[OK_SANG].item_num++;

	Scene[EXIT].item[Scene[EXIT].item_num] = ITEM_PHYSICS;
	Scene[EXIT].item_num++;

	Scene[KYOMU].item[Scene[KYOMU].item_num] = ITEM_OAKBO;
	Scene[KYOMU].item_num++;

	Scene[EXER].item[Scene[EXER].item_num] = ITEM_KNIFE;
	Scene[EXER].item_num++;

	Scene[YANG_HO].item[Scene[YANG_HO].item_num] = ITEM_DISC;
	Scene[YANG_HO].item_num++;

	Scene[MAE_JUM].item[Scene[MAE_JUM].item_num] = ITEM_DOLL;
	Scene[MAE_JUM].item_num++;

	Scene[ART_CLASS].item[Scene[ART_CLASS].item_num] = ITEM_MUNSEO;
	Scene[ART_CLASS].item_num++;

	Scene[MAN_TOILET_IN3].item[Scene[MAN_TOILET_IN3].item_num] = ITEM_KEY3;		//실험실 열쇠			
	Scene[MAN_TOILET_IN3].item_num++;


	Scene[EXPERIMENT].item[Scene[EXPERIMENT].item_num] = ITEM_KEY4;
	Scene[EXPERIMENT].item_num++;


	Scene[THIRD_CLASS].item[Scene[THIRD_CLASS].item_num] = ITEM_CAMERA;
	Scene[THIRD_CLASS].item_num++;


	Scene[THIRD_CLASS2].item[Scene[THIRD_CLASS2].item_num] = ITEM_라이터;
	Scene[THIRD_CLASS2].item_num++;


	Scene[THIRD_CLASS2].item[Scene[THIRD_CLASS2].item_num] = ITEM_PENCIL;
	Scene[THIRD_CLASS2].item_num++;


	Scene[MUSIC_CLASS].item[Scene[MUSIC_CLASS].item_num] = ITEM_유리거울;
	Scene[MUSIC_CLASS].item_num++;

}

void Item_Set2()
{
	Item[ITEM_담배].rect.left = 156 * (g_width / 640);
	Item[ITEM_담배].rect.top = 182 * (g_height / 480);
	Item[ITEM_담배].rect.right = 176 * (g_width / 640);
	Item[ITEM_담배].rect.bottom = 202 * (g_height / 480);

	Item[ITEM_CD].rect.left = 475 * (g_width / 640);
	Item[ITEM_CD].rect.top = 346 * (g_height / 480);
	Item[ITEM_CD].rect.right = 495 * (g_width / 640);
	Item[ITEM_CD].rect.bottom = 366 * (g_height / 480);

	Item[ITEM_KEY].rect.left = 430 * (g_width / 640);
	Item[ITEM_KEY].rect.top = 420 * (g_height / 480);
	Item[ITEM_KEY].rect.right = 450 * (g_width / 640);
	Item[ITEM_KEY].rect.bottom = 440 * (g_height / 480);

	Item[ITEM_KEY2].rect.left = 193 * (g_width / 640);
	Item[ITEM_KEY2].rect.top = 350 * (g_height / 480);
	Item[ITEM_KEY2].rect.right = 213 * (g_width / 640);
	Item[ITEM_KEY2].rect.bottom = 370 * (g_height / 480);

	Item[ITEM_GLASS].rect.left = 404 * (g_width / 640);
	Item[ITEM_GLASS].rect.top = 299 * (g_height / 480);
	Item[ITEM_GLASS].rect.right = 424 * (g_width / 640);
	Item[ITEM_GLASS].rect.bottom = 319 * (g_height / 480);

	Item[ITEM_PHYSICS].rect.left = 267 * (g_width / 640);
	Item[ITEM_PHYSICS].rect.top = 170 * (g_height / 480);
	Item[ITEM_PHYSICS].rect.right = 287 * (g_width / 640);
	Item[ITEM_PHYSICS].rect.bottom = 190 * (g_height / 480);

	Item[ITEM_OAKBO].rect.left = 196 * (g_width / 640);
	Item[ITEM_OAKBO].rect.top = 306 * (g_height / 480);
	Item[ITEM_OAKBO].rect.right = 216 * (g_width / 640);
	Item[ITEM_OAKBO].rect.bottom = 326 * (g_height / 480);

	Item[ITEM_KNIFE].rect.left = 596 * (g_width / 640);
	Item[ITEM_KNIFE].rect.top = 251 * (g_height / 480);
	Item[ITEM_KNIFE].rect.right = 616 * (g_width / 640);
	Item[ITEM_KNIFE].rect.bottom = 271 * (g_height / 480);

	Item[ITEM_DISC].rect.left = 116 * (g_width / 640);
	Item[ITEM_DISC].rect.top = 123 * (g_height / 480);
	Item[ITEM_DISC].rect.right = 136 * (g_width / 640);
	Item[ITEM_DISC].rect.bottom = 143 * (g_height / 480);

	Item[ITEM_DOLL].rect.left = 247 * (g_width / 640);
	Item[ITEM_DOLL].rect.top = 236 * (g_height / 480);
	Item[ITEM_DOLL].rect.right = 267 * (g_width / 640);
	Item[ITEM_DOLL].rect.bottom = 256 * (g_height / 480);

	Item[ITEM_MUNSEO].rect.left = 5 * (g_width / 640);
	Item[ITEM_MUNSEO].rect.top = 150 * (g_height / 480);
	Item[ITEM_MUNSEO].rect.right = 25 * (g_width / 640);
	Item[ITEM_MUNSEO].rect.bottom = 170 * (g_height / 480);

	Item[ITEM_KEY3].rect.left = 475 * (g_width / 640);
	Item[ITEM_KEY3].rect.top = 346 * (g_height / 480);
	Item[ITEM_KEY3].rect.right = 495 * (g_width / 640);
	Item[ITEM_KEY3].rect.bottom = 366 * (g_height / 480);

	Item[ITEM_KEY4].rect.left = 126 * (g_width / 640);
	Item[ITEM_KEY4].rect.top = 194 * (g_height / 480);
	Item[ITEM_KEY4].rect.right = 146 * (g_width / 640);
	Item[ITEM_KEY4].rect.bottom = 214 * (g_height / 480);

	Item[ITEM_CAMERA].rect.left = 308 * (g_width / 640);
	Item[ITEM_CAMERA].rect.top = 214 * (g_height / 480);
	Item[ITEM_CAMERA].rect.right = 328 * (g_width / 640);
	Item[ITEM_CAMERA].rect.bottom = 234 * (g_height / 480);

	Item[ITEM_라이터].rect.left = 308 * (g_width / 640);
	Item[ITEM_라이터].rect.top = 214 * (g_height / 480);
	Item[ITEM_라이터].rect.right = 328 * (g_width / 640);
	Item[ITEM_라이터].rect.bottom = 234 * (g_height / 480);

	Item[ITEM_PENCIL].rect.left = 338 * (g_width / 640);
	Item[ITEM_PENCIL].rect.top = 216 * (g_height / 480);
	Item[ITEM_PENCIL].rect.right = 358 * (g_width / 640);
	Item[ITEM_PENCIL].rect.bottom = 236 * (g_height / 480);

	Item[ITEM_KNIFE].Item_ID = ITEM_KNIFE;
	Item[ITEM_KEY].Item_ID = ITEM_KEY;
	Item[ITEM_DOLL].Item_ID = ITEM_DOLL;
	Item[ITEM_OAKBO].Item_ID = ITEM_OAKBO;
	Item[ITEM_CAMERA].Item_ID = ITEM_CAMERA;
	Item[ITEM_PENCIL].Item_ID = ITEM_PENCIL;
	Item[ITEM_CD].Item_ID = ITEM_CD;

	Item[ITEM_KEY2].Item_ID = ITEM_KEY2;
	Item[ITEM_BREAD].Item_ID = ITEM_BREAD;
	Item[ITEM_MUNSEO].Item_ID = ITEM_MUNSEO;
	Item[ITEM_라이터].Item_ID = ITEM_라이터;
	Item[ITEM_담배].Item_ID = ITEM_담배;
	Item[ITEM_KEY4].Item_ID = ITEM_KEY4;
	Item[ITEM_KEY3].Item_ID = ITEM_KEY3;
	Item[ITEM_PHYSICS].Item_ID = ITEM_PHYSICS;
	Item[ITEM_GLASS].Item_ID = ITEM_GLASS;
	if (jklp == 0)
		Item[ITEM_유리거울].rect.left = 82 * (g_width / 640);
	Item[ITEM_유리거울].rect.top = 394 * (g_height / 480);
	Item[ITEM_유리거울].rect.right = 102 * (g_width / 640);
	Item[ITEM_유리거울].rect.bottom = 414 * (g_height / 480);
}



bool LoadTexture(LPDIRECT3DTEXTURE9* ppTexture, LPDIRECT3DVERTEXBUFFER9& Test_Buffer, LPCTSTR strFileName, bool bUseTransparency, D3DCOLOR TransparencyColor)

{

	D3DCOLOR transparencycolor;

	if (bUseTransparency)  transparencycolor = TransparencyColor | 0xff000000;

	else                    transparencycolor = 0;



	// Create the texture using D3DX

	// 투명색에 해당하는 색상은 알파값을 0으로 만들어 준다.

	if (FAILED(D3DXCreateTextureFromFileEx(
		g_pd3dDevice,
		strFileName,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
		D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
		transparencycolor,
		NULL, NULL,
		ppTexture)))
	{
		return false;
	}

	//else


	if (Test_Buffer == NULL)
	{
		g_pd3dDevice->CreateVertexBuffer(
			6 * sizeof(BITMAPVERTEX),
			D3DUSAGE_WRITEONLY,
			D3DFVF_BITMAPVERTEX,
			D3DPOOL_MANAGED,
			&Test_Buffer,
			0);
	}



	Test_Buffer->Unlock();

	{ return true; }

}
bool Create_3DSprite(IDirect3DDevice9* pd3dDevice, LPDIRECT3DTEXTURE9& Test_Texture, LPDIRECT3DVERTEXBUFFER9& Test_Buffer, LPCTSTR lpstr, float x, float y, float width, float height, bool bUseTransparency, D3DCOLOR TransparencyColor)
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

	/*	D3DXCreateTextureFromFile(
			pd3dDevice,
		wFileName,
			&Test_Texture);  	*/

	D3DCOLOR transparencycolor;
	if (bUseTransparency)  transparencycolor = TransparencyColor | 0xff000000;
	else                    transparencycolor = 0;
	// Create the texture using D3DX

	// 투명색에 해당하는 색상은 알파값을 0으로 만들어 준다.

	if (FAILED(D3DXCreateTextureFromFileEx(
		g_pd3dDevice,
		lpstr,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
		D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
		transparencycolor,
		NULL, NULL,
		&Test_Texture)))
	{
		return false;
	}
	return true;

}