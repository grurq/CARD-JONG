#pragma once
#include "Header.h"
int winning(int pies[], int sc, int lsc, int flg);
void rldocs(char* doc, int sc, int* x, int* y);

int keychk(int pies[], bool changed);
int winchk(int pies[]);
int keychk2(int pies[]);
int rl_0(int pies[],int *x,int *y);
int rl_1(int pies[],int *x,int *y);
int rl_2(int pies[],int *x,int *y);
int rl_3(int pies[],int *x,int *y);
int rl_4(int pies[],int *x,int *y);
int rl_5(int pies[],int *x,int *y);
int pr_bonus(bool pr,int *x,int *y);

//役表記
void rldocs(char* doc,int sc, int* x, int* y)
{
	DrawFormatString(*x,*y, siro, doc);
	DrawFormatString(*x+120, *y, siro,"%3d", sc);
	*y += 20;
}
//適用法
int winning(int pies[], int sc,int lsc, int flg)
{
	int x = 0;
	int y = 0;
	int h;
	int ty;
	int piy = PLY;
	int pix=PLX;
	bool tsumo = (flg & 1) == 1 ? true : false;
	bool parent = (flg & 2) == 2 ? true : false;
	bool pwin = (flg & 4) == 4 ? true : false;
	// window縦幅 300

	if (!pwin) {
		y = COMY + 30;
		pix = COMX;
		piy = COMY;
		
	}
	ty = y;

	DrawBox(0, y, SCX, y + 300, daidai, true);
	DrawBox(2, y + 2, SCX - 2, y + 298, kuro, true);
	x += 40;
	y += 40;

	int (*roll[6])(int pies[], int* x, int* y);
	roll[0] = rl_0;
	roll[1] = rl_1;
	roll[2] = rl_2;
	roll[3] = rl_3;
	roll[4] = rl_4;
	roll[5] = rl_5;

	if (tsumo) {
		DrawFormatString(260, y, siro, "ツ　モ");
		//DrawFormatString(x, y, siro, "ツモ");
	}
	else {
		DrawFormatString(260, y, siro, "ロ　ン");
		//DrawFormatString(x, y, siro, "ロン");
	}
	
	y += 40;

	int gscore = 0;


	for (h = 0; h < 6; h++)
	{
		gscore += roll[h](pies, &x, &y);
	}
	pr_bonus(parent, &x, &y);
	if (parent)gscore += 10;
	for (h = 0; h < 9; h++)
	{
		pies[h] *= 9;
	}
	prt_gottapies(pies, pix, piy);
	prt_pies(pix+320, piy, pies[8], 0.5);
	y += 20;
	rldocs((char*)"合計", gscore,&x, &y);

	if (pwin) {
		DrawFormatString(300, y+40, siro, "YOU");
	}
	else { DrawFormatString(300, y+40, siro, "COM"); }
	DrawFormatString(350, y + 40, siro, "総得点%7d", sc + gscore);
	if (!pwin) {
		DrawFormatString(300, y+80, siro, "YOU");
	}
	else { DrawFormatString(300, y+80, siro, "COM"); }
	DrawFormatString(350, y + 80, siro, "総得点%7d", lsc);

	PlayMusic("dat\\クリア１.wav", 1);
	WaitTimer(4000);

	return gscore;
}
int winchk(int pies[])
{
	int cpies[9];
	int cpies2[9];
	int i;
	int sets;
	int ps;
	int keypie = -1;
	for (i = 0; i < 9; i++)
	{
		cpies[i] = pies[i];
		if (cpies[i] < 0)cpies[i] = -(1 + pies[i]);
		cpies2[i] = 80 - cpies[i];
	}
	for (i = 0; i < 2; i++) {
		pie_sort2(cpies);

		sets = 0;
		for (int j = 0; j <= 6; j += 3)
		{
			ps = 0;
			keypie = (cpies[j] / 9);
			if ((keypie == (cpies[j + 1] / 9)) && (keypie == (cpies[j + 2] / 9)))sets++;
		}

		if (sets == 3)return 3;
		memcpy(cpies, cpies2, sizeof(cpies));
	}
	return sets;
}
int keychk(int pies[], bool changed)
{
	int key = 0;
	swap(&key, &pies[8]);
	bool inkey = false;
	int keypie = -100;
	int pieflag = 0;
	for (int i = 0; i < 9; i++) {
		pies[8] = i * 9;
		pie_sort(pies);
		pieflag = winchk(pies);
		if (pieflag == 3) { keypie = i; break; }
	}
	if (!changed)swap(&key, &pies[8]);
	return keypie;
}
int keychk2(int pies[])
{
	int g, h, i;
	

	int ponged;
	int kpies[9]; memcpy(kpies, pies, sizeof(kpies));
// 全ての刻子を便宜的にポン 一度に三つまでしか判定できないので、3回繰り返す。
	for (g = 0; g < 3; g++) {
		pie_sort2(kpies);

		for (h = 0; h < 9; h++)
		{
			ponged = 0;
			for (i = 0; i < 9; i++)
			{
				if ((h == (kpies[i] / 9)) || ((80 - h * 9) / 9 == (80 - kpies[i]) / 9)) {
					if (kpies[i] >= 0) ponged++;
				}
			}
			if (ponged > 2) {
				//kpies[8] = h * 9;
				ponged = 0;
				for (int j = 0; j < 9; j++) {
					if ((h == (kpies[j] / 9)) || ((80 - h * 9) / 9 == (80 - kpies[j]) / 9))
					{
						if (kpies[j] >= 0) {
							kpies[j] = -(kpies[j] + 1);
							ponged++;
						}
					}
					if (ponged == 3)break;
				}
			}
			pie_sort2(kpies);
		}
	}
	// 疑似ポン処理ここまで

	ponged = 0;
	for (g = 0; g < 9; g++) {
		if (kpies[g] < 0)ponged++;
}
	if (ponged < 6)return -100;
	if (ponged == 9)return -200;
	if ((kpies[6] / 9) == (kpies[7] / 9))return kpies[8] / 9;
	if ((kpies[7] / 9) == (kpies[8] / 9))return kpies[6] / 9;
	return -100;
}
//暗刻チェック。必ず最初に行う
//戻り値は得点。
int rl_0(int pies[],int *x,int *y)
{
	char doc[1024];
	int sc=0;
	int i;
	int ponged = 0;
	int key;
	for (i = 0; i < 9; i++)
	{
		if (pies[i] >= 0) {
			ponged++;
		}else {
			pies[i] = -(1 + pies[i]);
		}
		key=pies[i];
		pies[i] = key / 9;
	}
	ponged /= 3;
	pie_sort2(pies);
	switch (ponged) {
	case 2:
		sprintf_s(doc,30,"%s","二暗刻");
		sc=10;
		break;
	case 3:
		sprintf_s(doc,30, "%s", "三暗刻");
		sc= 15;
		break;
	default:
		sprintf_s(doc,30, "%s", "アガり");
		sc= 5;
		break;
	}
	if (sc > 0)rldocs(doc, sc, x, y);
	return sc;
}
//1種または2種
int rl_1(int pies[], int* x, int* y)
{
	int count;
	char doc[1024]; int sc = 0;
	int key;
	for (int h = 0; h < 2; h++) {
		count = 0;
		key = pies[h * 8];
		for (int i = 0; i < 9; i++){
			if (key == pies[i])count++;
		}

	if (count == 9) {
		sprintf_s(doc,30, "%s", "1種");
		sc=60;
	}
	else if (count == 6)
	{
		sprintf_s(doc,30, "%s", "2種");
		sc=30;
	}
	}
	if (sc > 0)rldocs(doc, sc, x, y);
	return sc;
}
//1色
int rl_2(int pies[],int *x,int *y)
{
	int key[3];
	char doc[1024];int sc=0;
	for (int i = 0; i < 3; i++)
	{
		key[i] = pies[i * 3] / 3;
	}
	if (key[0] == key[1] && key[0] == key[2]) {
		sprintf_s(doc,30, "%s", "1色");
		sc= 20;
	}
	if (sc > 0)rldocs(doc, sc, x, y);
	return sc;
}
//1柄
int rl_3(int pies[],int *x,int *y)
{
	int key[3];
	char doc[1024];int sc=0;
	for (int i = 0; i < 3; i++)
	{
		key[i] = pies[i * 3] % 3;
	}
	if (key[0] == key[1] && key[0] == key[2]) {
		sprintf_s(doc,30, "%s", "1柄");
		sc+=20;
	}
	if (sc > 0)rldocs(doc, sc, x, y);
	return sc;
}
int rl_4(int pies[],int *x,int *y)
//3色
{
	int key[3];
	char doc[1024];int sc=0;
	for (int i = 0; i < 3; i++)
	{
		key[i] = pies[i * 3] / 3;
		key[i] -= i;
	}
	if (key[0] == key[1] && key[0] == key[2]) {
		sprintf_s(doc,30, "%s", "3色");
		sc= 10;
	}
	if (sc > 0)rldocs(doc, sc, x, y);
	return sc;
}
int rl_5(int pies[],int *x,int *y)
//3柄
{
	int key[3];
	char doc[1024]; int sc = 0;
	for (int i = 0; i < 3; i++)
	{
		key[i] = pies[i * 3] % 3;
	}
	if (key[0] != key[1] && key[0] != key[2] && key[1] != key[2]) {
		sprintf_s(doc,30, "%s", "3柄");
		sc= 10;
	}
	if (sc > 0)rldocs(doc, sc, x, y);
	return sc;
}
//親ボーナス
int pr_bonus(bool pr, int* x, int* y)
{
	if (pr == true)
	{
		DrawFormatString(*x, *y, siro,"親ボーナス 　　10");
		*y += 10;

		return 2;
	}
	return 1;
}