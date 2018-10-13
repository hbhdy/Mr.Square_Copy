/*�̰����� ��� ��������� �����ұ�?
�ٸ� cpp���Ͽ��� �� main.h�� �����ϸ� �ʿ��� ��� ����� ������ �� �ְ�
�߰��ؾ��ϴ� ����� ������ ���⿡ �߰��ع����� ���������� ������

���� �߰��ؾ��� ��������� ��û���� �þ�� �뵵�� �°� ����� �� �����ϴ°� ���� ������
�츮�� �� ������Ʈ�� �׷��� ���� ��������� �ʿ��� �� ������ �ʾƺ��� ����*/

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