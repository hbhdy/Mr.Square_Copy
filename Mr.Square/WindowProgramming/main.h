/*이곳에서 모든 헤더파일을 선언할까?
다른 cpp파일에선 이 main.h만 선언하면 필요한 모든 헤더를 선언할 수 있고
추가해야하는 헤더가 있으면 여기에 추가해버리면 유지관리도 좋을듯

물론 추가해야할 헤더파일이 엄청나게 늘어나면 용도에 맞게 헤더를 또 분할하는게 좋긴 하지만
우리가 할 프로젝트는 그렇게 많은 헤더파일이 필요할 것 같지는 않아보임 ㅋㅋ*/

#pragma once
#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

typedef struct STAGE {
	int x, y;
	int px, py;
	int bx[2], by[2];
}STAGE;

void SetButton(RECT& S0_Play, RECT& S0_Store,RECT& S0_Advice,
	RECT& S1_Start, RECT S1_Stage[],
	RECT S2_character[], RECT S2_buybutton[], RECT& S2_Return,
	RECT& S3_Return, RECT& S3_Reset,
	RECT& S4_Reset, RECT& S4_Return, RECT& S4_Store, RECT& S4_Next,
	RECT& S5_Return
	);
void SetBoard(int StageBoard[][10], int &loop, STAGE StageInfo[], bool& bReverseMode);
void SetStage(STAGE StageInfo[], int& SelectedStage);
void SetPlayerPosition(int& PlayRectXSize, int& PlayRectYSize, int& PlayerX, int& PlayerY, int& PlayerBoardX, int& PlayerBoardY);
bool CheckClear(int StageBoard[][10], bool StageClear[], int& SelectedStage, int& PlayerMoney);