/* 출력부분은 모두 이곳 main.cpp에서 처리하되
인호.cpp나 성섭.cpp에서 함수 정의하면 함수 선언문은 main.h에다가 해놓으면 됨
예시는 아래 WM_PAINT에보면 main.h에서 선언하고
inho.cpp에서 정의한 함수 Test(int a, int b)를 직접 사용하는 부분이 있음
이런식으로 다른 파일에 함수 따로 만들면 main.cpp에 코드가 좀 줄어들듯*/

#pragma once
#include "main.h"
#include <atlimage.h>//Cimage에 사용
#include "resource.h"

#define W_WIDTH 480
#define W_HEIGHT 640
#define STAGENUMBER 11
#define TILE_BLOCK 2
#define TILE_ARROW_LEFT 3
#define TILE_ARROW_UP 4
#define TILE_ARROW_RIGHT 5
#define TILE_ARROW_DOWN 6
#define TILE_BLACKHOLE 7

HINSTANCE g_hInst;
LPCTSTR lpszClass = "미스터 스퀘어";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;

	srand((unsigned)time(NULL));////////

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;

	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 0, 0, W_WIDTH + 16, W_HEIGHT + 38, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}//end of WinMain

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	static HDC hDC, memdc, mem1dc, mem2dc, mem3dc, mem4dc, mem5dc, mem6dc;
	static HDC chardc[5], defaulttile, tiledc[5], specialtiledc[6], storebuttondc, checkdc, reversedc;
	static HBITMAP hBitmap, oldBitmap;
	static HBITMAP upMenu, startMenu, stageMenu, storeMenu, endMenu, adviceMenu;
	static HBITMAP CharImage[5], TileImage, PaintedTile[5], SpecialtileImage[6], StoreButton, CheckSign, ReverseSign;
	static HINSTANCE hInstance;

	char TestStr[100];

	int Temp = 0;
	static int PlayRectXSize = 1, PlayRectYSize = 1;

	//플레이어 상태
	static int PlayerX, PlayerY, PlayerDirection, PlayerSpeed, PlayerMoney;
	static int PlayerBoardX, PlayerBoardY;

	static int Scene, GameStage;
	static int SelectedStage, SelectedCharacter;
	static int StageBoard[10][10];
	
	static RECT S0_Play, S0_Store, S0_Advice;
	static RECT S1_Stage[STAGENUMBER], S1_Start;
	static RECT S2_character[5], S2_buybutton[5], S2_Return;
	static RECT S3_Return, S3_Reset;
	static RECT S4_Return, S4_Reset, S4_Store, S4_Next;
	static RECT S5_Return;

	static bool bPlayerMove = false, bPlayerInBlackhole = false, bPlayerInArrow = false, bReverseMode = false;
	static bool StageClear[11], CharacterAvailable[5];

	static STAGE StageInfo[STAGENUMBER];

	switch (iMessage)
	{
	case WM_CREATE:
		//비트맵 불러오기
		//--//배경
		upMenu = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP36));
		startMenu = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP21));
		stageMenu = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP20));
		storeMenu = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP23));
		endMenu = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP18));
		adviceMenu= (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP11));

		//--//캐릭터
		CharImage[0] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_CHARACTER1));
		CharImage[1] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP13));
		CharImage[2] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP14));
		CharImage[3] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP15));
		CharImage[4] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP16));//아직 추가 안함

		//--//타일
		TileImage = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP24));

		PaintedTile[0] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP31));
		PaintedTile[1] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP34));
		PaintedTile[2] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP35));
		PaintedTile[3] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP32));
		PaintedTile[4] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP33));

		SpecialtileImage[0] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP30)); // 장애물 타일
		SpecialtileImage[1] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP26));
		SpecialtileImage[2] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP28));
		SpecialtileImage[3] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP27));
		SpecialtileImage[4] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP25));
		SpecialtileImage[5] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP29));

		//--//버튼
		StoreButton = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP22));
		CheckSign = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP17));
		ReverseSign = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP19));

		//기타정보 설정
		Scene = 0;//시작화면
		for (int loop = 0; loop < 5; ++loop)
			CharacterAvailable[loop] = false;

		CharacterAvailable[0] = true;

		for (int loop = 0; loop < 11; ++loop)
			StageClear[loop] = false;

		//플레이어 정보 초기화
		PlayerX = 0, PlayerY = 0;
		PlayerBoardX = 0, PlayerBoardY = 0;
		PlayerSpeed = 5;
		PlayerMoney = 500;
		SelectedCharacter = 0;

		//버튼좌표설정
		SetButton(S0_Play, S0_Store,S0_Advice, S1_Start, S1_Stage, S2_character, S2_buybutton, S2_Return, S3_Return, S3_Reset, S4_Reset, S4_Return, S4_Store, S4_Next, S5_Return);

		//스테이지설정(장애물배치 등등)
		SetStage(StageInfo, SelectedStage);

		SetTimer(hWnd, 1, 1, NULL);
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			if (bPlayerMove)
			{
				switch (PlayerDirection) // 이동 방향 조작
				{
				case VK_LEFT:
					PlayerX -= PlayerSpeed;
					break;
				case VK_UP:
					PlayerY -= PlayerSpeed;
					break;
				case VK_RIGHT:
					PlayerX += PlayerSpeed;
					break;
				case VK_DOWN:
					PlayerY += PlayerSpeed;
					break;
				}//end of switch (PlayerDirection)

				switch (PlayerDirection)
				{//방향에따라 충돌체크해주는 포인트를 변경해주는 부분
				case VK_LEFT:	case VK_UP:
					if (PlayRectXSize < PlayRectYSize)
					{
						PlayerBoardX = (PlayerX - 40 + PlayRectXSize - 1) / PlayRectXSize;
						PlayerBoardY = (PlayerY - 90 + PlayRectXSize - 1) / PlayRectXSize;
					}
					else
					{
						PlayerBoardX = (PlayerX - 40 + PlayRectYSize - 1) / PlayRectYSize;
						PlayerBoardY = (PlayerY - 90 + PlayRectYSize - 1) / PlayRectYSize;
					}
					break;
				case VK_RIGHT:	case VK_DOWN:
					if (PlayRectXSize < PlayRectYSize)
					{
						PlayerBoardX = (PlayerX - 40) / PlayRectXSize;
						PlayerBoardY = (PlayerY - 90) / PlayRectXSize;
					}
					else
					{
						PlayerBoardX = (PlayerX - 40) / PlayRectYSize;
						PlayerBoardY = (PlayerY - 90) / PlayRectYSize;
					}
					break;
				}

				if ((StageBoard[PlayerBoardY][PlayerBoardX] >= TILE_ARROW_LEFT) && (StageBoard[PlayerBoardY][PlayerBoardX] <= TILE_ARROW_DOWN))
				{
					//화살표 타일 밟았을 때  ( 화살표 방향으로 강제 이동 )
					bPlayerInBlackhole = false;
					switch (StageBoard[PlayerBoardY][PlayerBoardX])
					{
					case TILE_ARROW_LEFT:
						if (PlayerDirection != VK_LEFT)
						{
							if (((StageBoard[PlayerBoardY][PlayerBoardX - 1] >= 1) && (StageBoard[PlayerBoardY][PlayerBoardX - 1] <= 2)) || ((PlayerBoardX - 1) < 0))
							{
								//SetPlayerPosition(PlayRectXSize, PlayRectYSize, PlayerX, PlayerY, PlayerBoardX, PlayerBoardY);
								//bPlayerMove = false;
							}
							else
							{
								SetPlayerPosition(PlayRectXSize, PlayRectYSize, PlayerX, PlayerY, PlayerBoardX, PlayerBoardY);
								PlayerDirection = VK_LEFT;
							}
						}
						break;
					case TILE_ARROW_UP:
						if (PlayerDirection != VK_UP)
						{
							if (((StageBoard[PlayerBoardY - 1][PlayerBoardX] >= 1) && (StageBoard[PlayerBoardY - 1][PlayerBoardX] <= 2)) || ((PlayerBoardY - 1) < 0))
							{
								//SetPlayerPosition(PlayRectXSize, PlayRectYSize, PlayerX, PlayerY, PlayerBoardX, PlayerBoardY);
								//bPlayerMove = false;
							}
							else
							{
								SetPlayerPosition(PlayRectXSize, PlayRectYSize, PlayerX, PlayerY, PlayerBoardX, PlayerBoardY);
								PlayerDirection = VK_UP;
							}
						}
						break;
					case TILE_ARROW_RIGHT:
						if (PlayerDirection != VK_RIGHT)
						{
							if (((StageBoard[PlayerBoardY][PlayerBoardX + 1] >= 1) && (StageBoard[PlayerBoardY][PlayerBoardX + 1] <= 2)) || ((PlayerBoardX + 1) >= StageInfo[SelectedStage].x))
							{
								//SetPlayerPosition(PlayRectXSize, PlayRectYSize, PlayerX, PlayerY, PlayerBoardX, PlayerBoardY);
								//bPlayerMove = false;
							}
							else
							{
								SetPlayerPosition(PlayRectXSize, PlayRectYSize, PlayerX, PlayerY, PlayerBoardX, PlayerBoardY);
								PlayerDirection = VK_RIGHT;
							}
						}
						break;
					case TILE_ARROW_DOWN:
						if (PlayerDirection != VK_DOWN)
						{
							if (((StageBoard[PlayerBoardY + 1][PlayerBoardX] >= 1) && (StageBoard[PlayerBoardY + 1][PlayerBoardX] <= 2)) || ((PlayerBoardY + 1) >= StageInfo[SelectedStage].y))
							{
								//SetPlayerPosition(PlayRectXSize, PlayRectYSize, PlayerX, PlayerY, PlayerBoardX, PlayerBoardY);
								//bPlayerMove = false;
							}
							else
							{
								SetPlayerPosition(PlayRectXSize, PlayRectYSize, PlayerX, PlayerY, PlayerBoardX, PlayerBoardY);
								PlayerDirection = VK_DOWN;
							}
						}
						break;
					}
				}//end of if 화살표타일 밟았을 때

				switch (PlayerDirection)
				{//방향에따라 충돌체크해주는 포인트를 변경해주는 부분
				case VK_LEFT:	case VK_UP:
					if (PlayRectXSize < PlayRectYSize)
					{
						PlayerBoardX = (PlayerX - 40 + PlayRectXSize - 1) / PlayRectXSize;
						PlayerBoardY = (PlayerY - 90 + PlayRectXSize - 1) / PlayRectXSize;
					}
					else
					{
						PlayerBoardX = (PlayerX - 40 + PlayRectYSize - 1) / PlayRectYSize;
						PlayerBoardY = (PlayerY - 90 + PlayRectYSize - 1) / PlayRectYSize;
					}
					break;
				case VK_RIGHT:	case VK_DOWN:
					if (PlayRectXSize < PlayRectYSize)
					{
						PlayerBoardX = (PlayerX - 40) / PlayRectXSize;
						PlayerBoardY = (PlayerY - 90) / PlayRectXSize;
					}
					else
					{
						PlayerBoardX = (PlayerX - 40) / PlayRectYSize;
						PlayerBoardY = (PlayerY - 90) / PlayRectYSize;
					}
					break;
				}

				//벽과의 충돌체크
				switch (PlayerDirection)
				{
				case VK_LEFT:
					//PlayerX -= PlayerSpeed;
					if (((StageBoard[PlayerBoardY][PlayerBoardX - 1] == 1) || (StageBoard[PlayerBoardY][PlayerBoardX - 1] == TILE_BLOCK)) || ((PlayerX) < 40 && (StageBoard[PlayerBoardY][PlayerBoardX] == 1)))
					{
						bPlayerMove = false;
						SetPlayerPosition(PlayRectXSize, PlayRectYSize, PlayerX, PlayerY, PlayerBoardX, PlayerBoardY);
						if (StageBoard[PlayerBoardY][PlayerBoardX] != 7)
							bPlayerInBlackhole = false;
					}
					break;
				case VK_UP:
					//PlayerY -= PlayerSpeed;
					if (((StageBoard[PlayerBoardY - 1][PlayerBoardX] == 1) || (StageBoard[PlayerBoardY - 1][PlayerBoardX] == TILE_BLOCK)) || ((PlayerY) < 90 && (StageBoard[PlayerBoardY][PlayerBoardX] == 1)))
					{
						bPlayerMove = false;
						SetPlayerPosition(PlayRectXSize, PlayRectYSize, PlayerX, PlayerY, PlayerBoardX, PlayerBoardY);
						if (StageBoard[PlayerBoardY][PlayerBoardX] != 7)
							bPlayerInBlackhole = false;
					}
					break;
				case VK_RIGHT:
					//PlayerX += PlayerSpeed;
					if (((StageBoard[PlayerBoardY][PlayerBoardX + 1] == 1) || (StageBoard[PlayerBoardY][PlayerBoardX + 1] == TILE_BLOCK)) || ((PlayerBoardX + 1) >= StageInfo[SelectedStage].x))
					{
						bPlayerMove = false;
						SetPlayerPosition(PlayRectXSize, PlayRectYSize, PlayerX, PlayerY, PlayerBoardX, PlayerBoardY);
						if (StageBoard[PlayerBoardY][PlayerBoardX] != 7)
							bPlayerInBlackhole = false;
					}
					break;
				case VK_DOWN:
					//PlayerY += PlayerSpeed;
					if (((StageBoard[PlayerBoardY + 1][PlayerBoardX] == 1) || (StageBoard[PlayerBoardY + 1][PlayerBoardX] == TILE_BLOCK)) || ((PlayerBoardY + 1) >= StageInfo[SelectedStage].y))
					{
						bPlayerMove = false;
						SetPlayerPosition(PlayRectXSize, PlayRectYSize, PlayerX, PlayerY, PlayerBoardX, PlayerBoardY);
						if (StageBoard[PlayerBoardY][PlayerBoardX] != 7)
							bPlayerInBlackhole = false;
					}
					break;
				}//end of switch (PlayerDirection)

				if (StageBoard[PlayerBoardY][PlayerBoardX] == 0)
				{//빈칸을 밟은 후 타일 색칠해주는 부분
					StageBoard[PlayerBoardY][PlayerBoardX] = 1;
					bPlayerInBlackhole = false;
				}

				switch (PlayerDirection)
				{//방향에따라 충돌체크해주는 포인트를 변경해주는 부분
				case VK_LEFT:	case VK_UP:
					if (PlayRectXSize < PlayRectYSize)
					{
						PlayerBoardX = (PlayerX - 40 + PlayRectXSize - 1) / PlayRectXSize;
						PlayerBoardY = (PlayerY - 90 + PlayRectXSize - 1) / PlayRectXSize;
					}
					else
					{
						PlayerBoardX = (PlayerX - 40 + PlayRectYSize - 1) / PlayRectYSize;
						PlayerBoardY = (PlayerY - 90 + PlayRectYSize - 1) / PlayRectYSize;
					}
					break;
				case VK_RIGHT:	case VK_DOWN:
					if (PlayRectXSize < PlayRectYSize)
					{
						PlayerBoardX = (PlayerX - 40) / PlayRectXSize;
						PlayerBoardY = (PlayerY - 90) / PlayRectXSize;
					}
					else
					{
						PlayerBoardX = (PlayerX - 40) / PlayRectYSize;
						PlayerBoardY = (PlayerY - 90) / PlayRectYSize;
					}
					break;
				}

				if (StageBoard[PlayerBoardY][PlayerBoardX] == 7)
				{//블랙홀 타일 밟았을 때
					if (bPlayerInBlackhole == false)
					{
						if (((StageInfo[SelectedStage].bx[0] - PlayerSpeed <= PlayerX) && (StageInfo[SelectedStage].bx[0] + PlayerSpeed >= PlayerX))
							&&
							((StageInfo[SelectedStage].by[0] - PlayerSpeed <= PlayerY) && (StageInfo[SelectedStage].by[0] + PlayerSpeed >= PlayerY)))
						{
							PlayerX = StageInfo[SelectedStage].bx[1];
							PlayerY = StageInfo[SelectedStage].by[1];
						}
						else if (((StageInfo[SelectedStage].bx[1] - PlayerSpeed <= PlayerX) && (StageInfo[SelectedStage].bx[1] + PlayerSpeed >= PlayerX))
							&&
							((StageInfo[SelectedStage].by[1] - PlayerSpeed <= PlayerY) && (StageInfo[SelectedStage].by[1] + PlayerSpeed >= PlayerY)))
						{
							PlayerX = StageInfo[SelectedStage].bx[0];
							PlayerY = StageInfo[SelectedStage].by[0];
						}
						bPlayerInBlackhole = true;
					}
					break;
				}//end of if 블랙홀 타일 밟았을 때

				switch (PlayerDirection)
				{//방향에따라 충돌체크해주는 포인트를 변경해주는 부분
				case VK_LEFT:	case VK_UP:
					if (PlayRectXSize < PlayRectYSize)
					{
						PlayerBoardX = (PlayerX - 40 + PlayRectXSize - 1) / PlayRectXSize;
						PlayerBoardY = (PlayerY - 90 + PlayRectXSize - 1) / PlayRectXSize;
					}
					else
					{
						PlayerBoardX = (PlayerX - 40 + PlayRectYSize - 1) / PlayRectYSize;
						PlayerBoardY = (PlayerY - 90 + PlayRectYSize - 1) / PlayRectYSize;
					}
					break;
				case VK_RIGHT:	case VK_DOWN:
					if (PlayRectXSize < PlayRectYSize)
					{
						PlayerBoardX = (PlayerX - 40) / PlayRectXSize;
						PlayerBoardY = (PlayerY - 90) / PlayRectXSize;
					}
					else
					{
						PlayerBoardX = (PlayerX - 40) / PlayRectYSize;
						PlayerBoardY = (PlayerY - 90) / PlayRectYSize;
					}
					break;
				}
			}
			break;
		}//end of if 플레이어가 움직이면
		if (Scene == 3)
		{
			if (CheckClear(StageBoard, StageClear, SelectedStage, PlayerMoney))
			{
				Scene = 4;
			}
		}
		InvalidateRect(hWnd, NULL, false);
		break;


	case WM_LBUTTONDOWN:
		switch (Scene)
		{
		case 0://시작화면일 때
			//플레이버튼 : 스테이지화면으로 넘어감
			if (PtInRect(&S0_Play, { LOWORD(lParam), HIWORD(lParam) })) { Scene = 1; }

			//상점버튼 : 상점화면으로 넘어감
			if (PtInRect(&S0_Store, { LOWORD(lParam), HIWORD(lParam) })) { Scene = 2; }

			//도움말버튼 : 도움말화면으로 넘어감
			if (PtInRect(&S0_Advice, { LOWORD(lParam), HIWORD(lParam) })) { Scene = 5; }
			break;

		case 1://스테이지화면일 때

			//뒤로가기버튼 : 시작화면으로 넘어감
			if (PtInRect(&S3_Return, { LOWORD(lParam), HIWORD(lParam) })) { Scene = 0; }

			for (int loop = 0; loop < STAGENUMBER; ++loop)
			{
				//스테이지 숫자 버튼 : 플레이 화면으로 넘어감
				if (PtInRect(&S1_Stage[loop], { LOWORD(lParam), HIWORD(lParam) }))
				{
					Scene = 3;
					GameStage = loop + 1;
					SelectedStage = loop;
					SetBoard(StageBoard, loop, StageInfo, bReverseMode);

					//초기화부분
					PlayRectXSize = (400 / StageInfo[SelectedStage].x);
					PlayRectYSize = (510 / StageInfo[SelectedStage].y);
					if (PlayRectXSize < PlayRectYSize)
					{
						PlayerX = 40 + StageInfo[SelectedStage].px * PlayRectXSize;
						PlayerY = 90 + StageInfo[SelectedStage].py * PlayRectXSize;
					}
					else
					{
						PlayerX = 40 + StageInfo[SelectedStage].px * PlayRectYSize;
						PlayerY = 90 + StageInfo[SelectedStage].py * PlayRectYSize;
					}
					bPlayerMove = false, bPlayerInBlackhole = false;

				}
			}
			break;

		case 2://상점화면일 때
			if (PtInRect(&S2_Return, { LOWORD(lParam), HIWORD(lParam) })) { Scene = 0; } //되돌아가기 버튼

			for (int loop = 0; loop < 5; loop++)//캐릭터 선택
			{
				if (PtInRect(&S2_character[loop], { LOWORD(lParam), HIWORD(lParam) }))
				{
					if (CharacterAvailable[loop] == true)
						SelectedCharacter = loop;
				}
				if (PtInRect(&S2_buybutton[loop], { LOWORD(lParam), HIWORD(lParam) }))
				{
					if (CharacterAvailable[loop] == false)
					{
						if (PlayerMoney >= 250)
						{
							PlayerMoney -= 250;
							CharacterAvailable[loop] = true;
						}
					}
				}
			}
			break;

		case 3://플레이화면일 때
			if (PtInRect(&S3_Return, { LOWORD(lParam), HIWORD(lParam) })) { Scene = 1; }//되돌아가기 버튼
			else if (PtInRect(&S3_Reset, { LOWORD(lParam), HIWORD(lParam) }))	
			{ 
				SetBoard(StageBoard, SelectedStage, StageInfo, bReverseMode);

				//초기화 부분
				PlayRectXSize = (400 / StageInfo[SelectedStage].x);
				PlayRectYSize = (510 / StageInfo[SelectedStage].y);
				if (PlayRectXSize < PlayRectYSize)
				{
					PlayerX = 40 + StageInfo[SelectedStage].px * PlayRectXSize;
					PlayerY = 90 + StageInfo[SelectedStage].py * PlayRectXSize;
				}
				else
				{
					PlayerX = 40 + StageInfo[SelectedStage].px * PlayRectYSize;
					PlayerY = 90 + StageInfo[SelectedStage].py * PlayRectYSize;
				}
				bPlayerMove = false, bPlayerInBlackhole = false;
			}//리셋 버튼
			else
				//Scene = 4;
			break;

		case 4://클리어화면일 때
			if (PtInRect(&S4_Return, { LOWORD(lParam), HIWORD(lParam) }))	{ Scene = 1; }//되돌아가기 버튼
			else if (PtInRect(&S4_Reset, { LOWORD(lParam), HIWORD(lParam) }))
			{
				Scene = 3;
				SetBoard(StageBoard, SelectedStage, StageInfo, bReverseMode);

				//초기화 부분
				PlayRectXSize = (400 / StageInfo[SelectedStage].x);
				PlayRectYSize = (510 / StageInfo[SelectedStage].y);
				if (PlayRectXSize < PlayRectYSize)
				{
					PlayerX = 40 + StageInfo[SelectedStage].px * PlayRectXSize;
					PlayerY = 90 + StageInfo[SelectedStage].py * PlayRectXSize;
				}
				else
				{
					PlayerX = 40 + StageInfo[SelectedStage].px * PlayRectYSize;
					PlayerY = 90 + StageInfo[SelectedStage].py * PlayRectYSize;
				}
				bPlayerMove = false, bPlayerInBlackhole = false;
			}//리셋 버튼
			else if (PtInRect(&S4_Store, { LOWORD(lParam), HIWORD(lParam) }))	{ Scene = 2; }
			else if (PtInRect(&S4_Next, { LOWORD(lParam), HIWORD(lParam) }))
			{
				GameStage++;
				SelectedStage++;
				if (GameStage > STAGENUMBER)
				{
					GameStage = STAGENUMBER;
					Scene = 0;
				}
				else
				{
					Scene = 3;
					SetBoard(StageBoard, SelectedStage, StageInfo, bReverseMode);

					//초기화 부분
					PlayRectXSize = (400 / StageInfo[SelectedStage].x);
					PlayRectYSize = (510 / StageInfo[SelectedStage].y);
					if (PlayRectXSize < PlayRectYSize)
					{
						PlayerX = 40 + StageInfo[SelectedStage].px * PlayRectXSize;
						PlayerY = 90 + StageInfo[SelectedStage].py * PlayRectXSize;
					}
					else
					{
						PlayerX = 40 + StageInfo[SelectedStage].px * PlayRectYSize;
						PlayerY = 90 + StageInfo[SelectedStage].py * PlayRectYSize;
					}
					bPlayerMove = false, bPlayerInBlackhole = false;
				}
			}
		case 5: // 도움말 화면
			if (PtInRect(&S5_Return, { LOWORD(lParam), HIWORD(lParam) })) { Scene = 0; }//되돌아가기 버튼
			break;
		}
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_KEYUP:
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case '0':
			SelectedCharacter = 0;
			break;
		case '1':
			SelectedCharacter = 1;
			break;
		case '2':
			SelectedCharacter = 2;
			break;
		case '3':
			SelectedCharacter = 3;
			break;
		case '4':
			SelectedCharacter = 4;
			break;
		}
		if ((Scene == 3) && (bPlayerMove == false))
		{
			switch (wParam)
			{
			case VK_LEFT:
			case VK_UP:
			case VK_RIGHT:
			case VK_DOWN:
				bPlayerMove = true;
				if(bReverseMode == true)
				{
					if(wParam == VK_LEFT)
						PlayerDirection = VK_RIGHT;
					else if (wParam == VK_UP)
						PlayerDirection = VK_DOWN;
					else if (wParam == VK_RIGHT)
						PlayerDirection = VK_LEFT;
					else if (wParam == VK_DOWN)
						PlayerDirection = VK_UP;
				}
				else
					PlayerDirection = wParam;
				break;
			}
		}
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		//Setdc : 기본 도화지 만들기
		hBitmap = CreateCompatibleBitmap(hDC, W_WIDTH, W_HEIGHT);
		memdc = CreateCompatibleDC(hDC);
		mem1dc = CreateCompatibleDC(hDC);
		mem2dc = CreateCompatibleDC(hDC);
		mem3dc = CreateCompatibleDC(hDC);
		mem4dc = CreateCompatibleDC(hDC);
		mem5dc = CreateCompatibleDC(hDC);
		mem6dc = CreateCompatibleDC(hDC);
		defaulttile = CreateCompatibleDC(hDC);
		for (int loop = 0; loop < 5; ++loop)
		{
			chardc[loop] = CreateCompatibleDC(hDC);
			tiledc[loop] = CreateCompatibleDC(hDC);
		}
		for (int loop = 0; loop < 6; ++loop)
			specialtiledc[loop] = CreateCompatibleDC(hDC);
		storebuttondc = CreateCompatibleDC(hDC);
		checkdc = CreateCompatibleDC(hDC);
		reversedc = CreateCompatibleDC(hDC);

		//SelectObject
		oldBitmap = (HBITMAP)SelectObject(memdc, hBitmap);
		for (int loop = 0; loop < 5; ++loop)
			SelectObject(chardc[loop], CharImage[loop]);
		SelectObject(storebuttondc, StoreButton);
		SelectObject(checkdc, CheckSign);
		SelectObject(reversedc, ReverseSign);

		switch (Scene)
		{
		case 0://시작화면 : 황성섭
			SelectObject(mem1dc, startMenu);
			BitBlt(memdc, 0, 0, 480, 640, mem1dc, 0, 0, SRCCOPY);
			break;
		case 1://메뉴화면(스테이지선택) : 황성섭
			SelectObject(mem2dc, stageMenu);
			BitBlt(memdc, 0, 0, 480, 640, mem2dc, 0, 0, SRCCOPY);
			for (int loop = 0; loop < 11; loop++)
			{
				if (StageClear[loop])
				{
					TransparentBlt(memdc, 130 + ((loop % 3) * 123), 142 + (loop / 3) * 85,
						36, 36, checkdc, 0, 0, 36, 36, RGB(255, 255, 255));
				}
			}
			break;
		case 2://상점화면 : 이인호
			SelectObject(mem4dc, storeMenu);
			BitBlt(memdc, 0, 0, 480, 640, mem4dc, 0, 0, SRCCOPY);

			//캐릭터 이미지 출력
			for (int loop = 0; loop < 5; loop++)
			{
				TransparentBlt(memdc, S2_character[loop].left, S2_character[loop].top,
					72, 72, chardc[loop], 0, 0, 72, 72, RGB(255, 255, 255));

				if (loop == SelectedCharacter)
				{
					TransparentBlt(memdc, S2_character[loop].left+28, S2_character[loop].top-33,
						36, 36, checkdc, 0, 0, 36, 36, RGB(255, 255, 255));
				}
			}
			
			//캐릭터 구입 버튼 출력
			for (int loop = 1; loop < 5; loop++)//0번버튼은 사용 안함
			{
				if (CharacterAvailable[loop] == false)
				{
					TransparentBlt(memdc, S2_buybutton[loop].left, S2_buybutton[loop].top,
						140, 40, storebuttondc, 0, 0, 140, 40, RGB(255, 255, 255));
				}
			}

			//소지금액 출력
			wsprintf(TestStr, "%d", PlayerMoney);
			SetBkMode(memdc, TRANSPARENT);
			SetTextColor(memdc, RGB(255, 255, 255));
			TextOut(memdc, 240 - (strlen(TestStr)*4.5), 25, TestStr, strlen(TestStr));
			break;
		case 3://플레이화면
			SelectObject(mem3dc, upMenu);
			BitBlt(memdc, 0, 0, 480, 640, mem3dc, 0, 0, SRCCOPY);

			//몇번째 스테이지인지 표시
			wsprintf(TestStr, "스테이지 : %d", GameStage);
			SetBkMode(memdc, TRANSPARENT);
			SetTextColor(memdc, RGB(255, 255, 255));
			TextOut(memdc, 240 - (strlen(TestStr)*4.5), 25, TestStr, strlen(TestStr));

			//플레이 스테이지 출력
			SelectObject(defaulttile, TileImage);
			for (int loop = 0; loop < 5; ++loop)
				SelectObject(tiledc[loop], PaintedTile[loop]);
			for (int loop = 0; loop < 6; ++loop)
				SelectObject(specialtiledc[loop], SpecialtileImage[loop]);

			for (int o = 0; o < StageInfo[SelectedStage].y; ++o)
			{
				for (int i = 0; i < StageInfo[SelectedStage].x; ++i)
				{
					if (PlayRectXSize < PlayRectYSize)
					{
						if (StageBoard[o][i] == 0)//이동가능한 바닥일때
						{
							StretchBlt(memdc, 40 + i * PlayRectXSize, 90 + o * PlayRectXSize,
								PlayRectXSize, PlayRectXSize, defaulttile, 0, 0, 72, 72, SRCCOPY);
						}
						else
						{
							switch (StageBoard[o][i])
							{
							case 1://장애물이 있을 때
								StretchBlt(memdc, 40 + i * PlayRectXSize, 90 + o * PlayRectXSize,
									PlayRectXSize, PlayRectXSize, tiledc[SelectedCharacter], 0, 0, 72, 72, SRCCOPY);
								break;
							case TILE_BLOCK://벽돌
							case TILE_ARROW_LEFT://화살표 좌
							case TILE_ARROW_UP://화살표 상
							case TILE_ARROW_RIGHT://화살표 우
							case TILE_ARROW_DOWN://화살표 하
							case TILE_BLACKHOLE://블랙홀
								StretchBlt(memdc, 40 + i * PlayRectXSize, 90 + o * PlayRectXSize,
									PlayRectXSize, PlayRectXSize, specialtiledc[StageBoard[o][i]-2], 0, 0, 72, 72, SRCCOPY);
								break;
							}
						}
					}
					else
					{
						if (StageBoard[o][i] == 0)//이동가능한 바닥일때
						{
							StretchBlt(memdc, 40 + i * PlayRectYSize, 90 + o * PlayRectYSize,
								PlayRectYSize, PlayRectYSize, defaulttile, 0, 0, 72, 72, SRCCOPY);
						}
						else//장애물이 있을 때
						{
							StretchBlt(memdc, 40 + i * PlayRectYSize, 90 + o * PlayRectYSize,
								PlayRectYSize, PlayRectYSize, tiledc[SelectedCharacter], 0, 0, 72, 72, SRCCOPY);
						}
					}
				}
			}

			//플레이어 캐릭터
			if (PlayRectXSize < PlayRectYSize)
			{
				TransparentBlt(memdc, PlayerX, PlayerY, PlayRectXSize, PlayRectXSize, chardc[SelectedCharacter], 0, 0, 72, 72, RGB(255,255,255));
				if (bReverseMode)
					TransparentBlt(memdc, PlayerX, PlayerY - PlayRectXSize / 2, PlayRectXSize, PlayRectXSize/2, reversedc, 0, 0, 72, 36, RGB(255, 255, 255));
			}
			else
			{
				TransparentBlt(memdc, PlayerX, PlayerY, PlayRectYSize, PlayRectYSize, chardc[SelectedCharacter], 0, 0, 72, 72, RGB(255, 255, 255));
				if (bReverseMode)
					TransparentBlt(memdc, PlayerX, PlayerY - PlayRectYSize / 2, PlayRectYSize, PlayRectYSize/2, reversedc, 0, 0, 72, 36, RGB(255, 255, 255));
			}

			

			break;
		case 4://클리어화면
			SelectObject(mem5dc, endMenu);
			BitBlt(memdc, 0, 0, 480, 640, mem5dc, 0, 0, SRCCOPY);
			  
			break;

		case 5: // 도움말화면
			SelectObject(mem6dc, adviceMenu);
			BitBlt(memdc, 0, 0, 480, 640, mem6dc, 0, 0, SRCCOPY);

			break;
		}

		//최종출력 memdc에서 hDC(화면)으로
		BitBlt(hDC, 0, 0, W_WIDTH, W_HEIGHT, memdc, 0, 0, SRCCOPY);

		//생성한 것들 해제 및 삭제
		SelectObject(memdc, oldBitmap);
		DeleteObject(hBitmap);
		DeleteDC(reversedc);
		DeleteDC(checkdc);
		DeleteDC(storebuttondc);
		DeleteDC(defaulttile);

		for (int loop = 0; loop < 5; ++loop)
		{
			DeleteDC(chardc[loop]);
			DeleteDC(tiledc[loop]);
		}

		for (int loop = 0; loop < 6; ++loop)
			DeleteDC(specialtiledc[loop]);

		DeleteDC(mem6dc);
		DeleteDC(mem5dc);
		DeleteDC(mem4dc);
		DeleteDC(mem3dc);
		DeleteDC(mem2dc);
		DeleteDC(mem1dc);
		DeleteDC(memdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}//end of switch(iMessage)
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}//end of CALLBACK WndProc

void SetButton(RECT& S0_Play, RECT& S0_Store,RECT& S0_Advice, RECT& S1_Start, RECT S1_Stage[], 
	RECT S2_character[], RECT S2_buybutton[], RECT& S2_Return, RECT& S3_Return, RECT& S3_Reset, 
	RECT& S4_Reset, RECT& S4_Return, RECT& S4_Store, RECT& S4_Next, RECT& S5_Return)
{
	//시작화면 버튼 설정
	S0_Play.left = 116;
	S0_Play.top = 253;
	S0_Play.right = 363;
	S0_Play.bottom = 338;

	S0_Store.left = 116;
	S0_Store.top = 363;
	S0_Store.right = 363;
	S0_Store.bottom = 421;

	S0_Advice.left = 116;
	S0_Advice.top = 440;
	S0_Advice.right = 363;
	S0_Advice.bottom = 512;

	//메뉴화면 플레이 버튼 좌표설정
	S1_Start.left = 200;
	S1_Start.top = 300;
	S1_Start.right = 280;
	S1_Start.bottom = 340;

	S1_Stage[0].left = 63;
	S1_Stage[0].top = 142;
	S1_Stage[0].right = 169;
	S1_Stage[0].bottom = 212;

	S1_Stage[1].left = 186;
	S1_Stage[1].top = 142;
	S1_Stage[1].right = 292;
	S1_Stage[1].bottom = 212;

	S1_Stage[2].left = 310;
	S1_Stage[2].top = 142;
	S1_Stage[2].right = 415;
	S1_Stage[2].bottom = 212;

	S1_Stage[3].left = 63;
	S1_Stage[3].top = 228;
	S1_Stage[3].right = 169;
	S1_Stage[3].bottom = 298;

	S1_Stage[4].left = 186;
	S1_Stage[4].top = 228;
	S1_Stage[4].right = 292;
	S1_Stage[4].bottom = 298;

	S1_Stage[5].left = 310;
	S1_Stage[5].top = 228;
	S1_Stage[5].right = 415;
	S1_Stage[5].bottom = 298;

	S1_Stage[6].left = 63;
	S1_Stage[6].top = 313;
	S1_Stage[6].right = 169;
	S1_Stage[6].bottom = 383;

	S1_Stage[7].left = 186;
	S1_Stage[7].top = 313;
	S1_Stage[7].right = 292;
	S1_Stage[7].bottom = 383;

	S1_Stage[8].left = 310;
	S1_Stage[8].top = 313;
	S1_Stage[8].right = 415;
	S1_Stage[8].bottom = 383;

	S1_Stage[9].left = 63;
	S1_Stage[9].top = 398;
	S1_Stage[9].right = 169;
	S1_Stage[9].bottom = 468;

	S1_Stage[10].left = 186;
	S1_Stage[10].top = 398;
	S1_Stage[10].right = 292;
	S1_Stage[10].bottom = 468;


	
	//상점화면 버튼 설정( 캐릭터 구매 및 적용, 되돌아가기 )
	S2_character[0].left = 204;
	S2_character[0].top = 450;
	S2_character[0].right = 276;
	S2_character[0].bottom = 522;

	S2_character[1].left = 84;
	S2_character[1].top = 120;
	S2_character[1].right = 156;
	S2_character[1].bottom = 192;

	S2_character[2].left = 324;
	S2_character[2].top = 120;
	S2_character[2].right = 396;
	S2_character[2].bottom = 192;

	S2_character[3].left = 84;
	S2_character[3].top = 280;
	S2_character[3].right = 156;
	S2_character[3].bottom = 352;

	S2_character[4].left = 324;
	S2_character[4].top = 280;
	S2_character[4].right = 396;
	S2_character[4].bottom = 352;

	S2_buybutton[0].left = 0;//사용안하는 버튼
	S2_buybutton[0].top = 0;
	S2_buybutton[0].right = 0;
	S2_buybutton[0].bottom = 0;

	S2_buybutton[1].left = 50;
	S2_buybutton[1].top = 200;
	S2_buybutton[1].right = 190;
	S2_buybutton[1].bottom = 240;

	S2_buybutton[2].left = 290;
	S2_buybutton[2].top = 200;
	S2_buybutton[2].right = 430;
	S2_buybutton[2].bottom = 240;

	S2_buybutton[3].left = 50;
	S2_buybutton[3].top = 360;
	S2_buybutton[3].right = 190;
	S2_buybutton[3].bottom = 400;

	S2_buybutton[4].left = 290;
	S2_buybutton[4].top = 360;
	S2_buybutton[4].right = 430;
	S2_buybutton[4].bottom = 400;

	S2_Return.left = 13;
	S2_Return.top = 13;
	S2_Return.right = 90;
	S2_Return.bottom = 52;

	//플레이화면 되돌아가기 버튼 좌표설정
	S3_Return.left = 12;
	S3_Return.top = 8;
	S3_Return.right = 93;
	S3_Return.bottom = 58;

	S3_Reset.left = 386;
	S3_Reset.top = 8;
	S3_Reset.right = 467;
	S3_Reset.bottom = 58;

	//클리어화면 버튼 좌표설정
	S4_Return.left = 116;
	S4_Return.top = 332;
	S4_Return.right = 168;
	S4_Return.bottom = 385;

	S4_Reset.left = 180;
	S4_Reset.top = 332;
	S4_Reset.right = 233;
	S4_Reset.bottom = 385;

	S4_Store.left = 245;
	S4_Store.top = 332;
	S4_Store.right = 297;
	S4_Store.bottom = 385;

	S4_Next.left = 309;
	S4_Next.top = 325;
	S4_Next.right = 378;
	S4_Next.bottom = 389;


	//도움말화면 버튼 ( 되돌아가기)
	S5_Return.left = 14;
	S5_Return.top = 14;
	S5_Return.right = 89;
	S5_Return.bottom = 53;

}

void SetBoard(int StageBoard[][10], int &loop, STAGE StageInfo[], bool& bReverseMode)
{
	//기본세팅 시작
	bReverseMode = false;
	for (int o = 0; o < 10; ++o)
	{
		for (int i = 0; i < 10; ++i)
		{
			StageBoard[o][i] = 1;
		}
	}
	for (int o = 0; o < StageInfo[loop].y; ++o)
	{
		for (int i = 0; i < StageInfo[loop].x; ++i)
		{
			StageBoard[o][i] = 0;
		}
	}
	//기본세팅 종료

	//스테이지별 세팅
	switch (loop)
	{
	case 0://장애물 없음
		break;
	case 1://벽
		StageBoard[2][0] = TILE_BLOCK;//벽
		StageBoard[1][4] = TILE_BLOCK;//벽
		StageBoard[3][5] = TILE_BLOCK;//벽
		StageBoard[4][1] = TILE_BLOCK;//벽
		break;
	case 2://벽  
		StageBoard[0][1] = TILE_BLOCK;
		StageBoard[2][0] = TILE_BLOCK;
		StageBoard[2][4] = TILE_BLOCK;
		StageBoard[4][1] = TILE_BLOCK;
		break;
	case 3://벽, 화살표 
		StageBoard[2][0] = TILE_ARROW_RIGHT;
		StageBoard[2][5] = TILE_ARROW_UP;
		StageBoard[0][5] = TILE_ARROW_LEFT;
		StageBoard[0][1] = TILE_ARROW_DOWN;
		StageBoard[1][1] = TILE_ARROW_RIGHT;
		break;
	case 4://화살표 
		StageBoard[0][3] = TILE_ARROW_DOWN;
		StageBoard[1][1] = TILE_ARROW_DOWN;
		StageBoard[3][1] = TILE_ARROW_RIGHT;
		break;
	case 5://화살표, 블랙홀 
		StageBoard[2][0] = TILE_BLOCK;
		StageBoard[2][1] = TILE_BLOCK;
		StageBoard[2][2] = TILE_BLOCK;
		StageBoard[2][3] = TILE_BLOCK;
		StageBoard[2][4] = TILE_BLOCK;
		StageBoard[0][4] = TILE_BLACKHOLE;
		StageBoard[3][3] = TILE_BLACKHOLE;
		break;
	case 6://벽, 블랙홀 
		StageBoard[0][0] = TILE_BLOCK;
		StageBoard[0][4] = TILE_BLOCK;
		StageBoard[1][2] = TILE_BLOCK;
		StageBoard[3][3] = TILE_BLOCK;
		StageBoard[1][1] = TILE_BLACKHOLE;//블랙홀
		StageBoard[4][0] = TILE_BLACKHOLE;//블랙홀
		break;
	case 7://화살표, 블랙홀, 벽  
		StageBoard[1][0] = TILE_ARROW_UP;
		StageBoard[0][0] = TILE_ARROW_RIGHT;
		StageBoard[0][6] = TILE_ARROW_DOWN;
		StageBoard[2][0] = TILE_BLOCK;
		StageBoard[2][1] = TILE_BLOCK;
		StageBoard[2][5] = TILE_BLOCK;
		StageBoard[2][6] = TILE_BLOCK;
		StageBoard[2][2] = TILE_BLACKHOLE;//블랙홀
		StageBoard[1][6] = TILE_BLACKHOLE;//블랙홀
		break;
	case 8://화살표, 블랙홀, 벽 
		StageBoard[1][2] = TILE_BLOCK;
		StageBoard[1][3] = TILE_ARROW_UP;
		StageBoard[2][2] = TILE_ARROW_LEFT;
		StageBoard[3][3] = TILE_ARROW_DOWN;
		StageBoard[2][4] = TILE_ARROW_RIGHT;
		StageBoard[2][3] = TILE_BLACKHOLE;//블랙홀
		StageBoard[4][1] = TILE_BLACKHOLE;//블랙홀
		break;
	case 9://리버스 
		   //장애물 없음
		bReverseMode = true;
		break;
	case 10://리버스
		StageBoard[1][1] = TILE_BLOCK;
		StageBoard[0][3] = TILE_BLOCK;
		StageBoard[1][5] = TILE_BLOCK;
		bReverseMode = true;
		break;
	}
}
void SetStage(STAGE StageInfo[], int& SelectedStage)
{
	int PlayRectXSize, PlayRectYSize;

	//스테이지1
	StageInfo[0].x = 4;
	StageInfo[0].y = 4;
	StageInfo[0].px = 0;
	StageInfo[0].py = 0;

	// 벽  ( 기반 1-9 )
	StageInfo[1].x = 6;//맵사이즈
	StageInfo[1].y = 6;
	StageInfo[1].px = 2;//플레이어 시작 위치
	StageInfo[1].py = 3;

	// 벽  ( 기반 1-7 )
	StageInfo[2].x = 5;
	StageInfo[2].y = 6;
	StageInfo[2].px = 3;
	StageInfo[2].py = 4;

	// 화살표 ( 자작 )
	StageInfo[3].x = 6;
	StageInfo[3].y = 3;
	StageInfo[3].px = 0;
	StageInfo[3].py = 0;

	// 화살표 ( 기반 4-3 )
	StageInfo[4].x = 5;
	StageInfo[4].y = 5;
	StageInfo[4].px = 2;
	StageInfo[4].py = 2;

	// 블랙홀, 화살표 ( 자작 )
	StageInfo[5].x = 5;
	StageInfo[5].y = 5;
	StageInfo[5].px = 2;
	StageInfo[5].py = 3;
	PlayRectXSize = (400 / StageInfo[5].x);
	PlayRectYSize = (510 / StageInfo[5].y);
	StageInfo[5].bx[0] = 40 + 4 * PlayRectXSize;
	StageInfo[5].by[0] = 90 + 0 * PlayRectXSize;
	StageInfo[5].bx[1] = 40 + 3 * PlayRectXSize;
	StageInfo[5].by[1] = 90 + 3 * PlayRectXSize;

	// 블랙홀 , 벽  ( 기반 3-5 )
	StageInfo[6].x = 5;
	StageInfo[6].y = 5;
	StageInfo[6].px = 1;
	StageInfo[6].py = 2;
	PlayRectXSize = (400 / StageInfo[6].x);
	PlayRectYSize = (510 / StageInfo[6].y);
	StageInfo[6].bx[0] = 40 + 1 * PlayRectXSize;
	StageInfo[6].by[0] = 90 + 1 * PlayRectXSize;
	StageInfo[6].bx[1] = 40 + 0 * PlayRectXSize;
	StageInfo[6].by[1] = 90 + 4 * PlayRectXSize;

	// 화살표, 블랙홀, 벽 ( 자작 )
	StageInfo[7].x = 7;
	StageInfo[7].y = 3;
	StageInfo[7].px = 3;
	StageInfo[7].py = 1;
	PlayRectXSize = (400 / StageInfo[7].x);
	PlayRectYSize = (510 / StageInfo[7].y);
	StageInfo[7].bx[0] = 40 + 2 * PlayRectXSize;
	StageInfo[7].by[0] = 90 + 2 * PlayRectXSize;
	StageInfo[7].bx[1] = 40 + 6 * PlayRectXSize;
	StageInfo[7].by[1] = 90 + 1 * PlayRectXSize;

	// 화살표 , 블랙홀 , 벽 ( 기반 4-7 )
	StageInfo[8].x = 5;
	StageInfo[8].y = 5;
	StageInfo[8].px = 4;
	StageInfo[8].py = 2;
	PlayRectXSize = (400 / StageInfo[8].x);
	PlayRectYSize = (510 / StageInfo[8].y);
	StageInfo[8].bx[0] = 40 + 3 * PlayRectXSize;
	StageInfo[8].by[0] = 90 + 2 * PlayRectXSize;
	StageInfo[8].bx[1] = 40 + 1 * PlayRectXSize;
	StageInfo[8].by[1] = 90 + 4 * PlayRectXSize;

	// 리버스 (기반 5-1)
	StageInfo[9].x = 3;
	StageInfo[9].y = 3;
	StageInfo[9].px = 1;
	StageInfo[9].py = 1;

	// 리버스 (기반 5-3)
	StageInfo[10].x = 7;
	StageInfo[10].y = 3;
	StageInfo[10].px = 3;
	StageInfo[10].py = 1;
}

void SetPlayerPosition(int& PlayRectXSize, int& PlayRectYSize, int& PlayerX, int& PlayerY, int& PlayerBoardX, int& PlayerBoardY)
{
	if (PlayRectXSize < PlayRectYSize)
	{
		PlayerX = 40 + PlayerBoardX * PlayRectXSize;
		PlayerY = 90 + PlayerBoardY * PlayRectXSize;
	}
	else
	{
		PlayerX = 40 + PlayerBoardX * PlayRectYSize;
		PlayerY = 90 + PlayerBoardY * PlayRectYSize;
	}
}

bool CheckClear(int StageBoard[][10], bool StageClear[], int& SelectedStage, int& PlayerMoney)
{
	for (int o = 0; o < 10; ++o)
	{
		for (int i = 0; i < 10; ++i)
		{
			if (StageBoard[o][i] == 0)
				return false;
		}
	}
	if (StageClear[SelectedStage] == false)
	{
		StageClear[SelectedStage] = true;
		PlayerMoney += 100;
	}
	return true;
}