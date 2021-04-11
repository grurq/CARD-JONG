#ifndef _INCLUDE_HEADER_
#define _INCLUDE_HEADER_
#define _CRT_SECURE_NO_WARNINGS
#include "DxLib.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define PI	3.141592654
#define COMX 20
#define COMY 80
#define PLX	20
#define PLY 400
#define SCX 640
#define SCY 480
#define DCY 220

#define siro	0xffffff
#define kuro	0x000000
#define midori	0x005a00
#define daidai	0xff9900


extern int pimgs[11];

extern int lostpies[81];

int swap(int *a, int *b);
int GetRandom(int min, int max);
void initiarize(int arr[], int var);

void playpies_preset(int pies[]);
void lp_preset(void);
void rollpies_preset(int pies[]);

void prt_gottapies(int pies[], int x, int y);
void pie_sort(int pies[]);
void pie_sort2(int pies[]);

void prt_pies(int x, int y, int piedata,double ang);
void cls(void);


int pickpie(void);

bool docsYN(char *doc);
void Load_Prof(void);


void prt_egottapies(int pies[], int x, int y);


void Load_Prof(void) 
{
	char buf[256];
	// 画像
	for (int n = 0; n < 11; n++) {
		sprintf_s(buf, 256, "dat\\%d.png", n);
		pimgs[n] = LoadGraph(buf);
	}
}
void cls(void)
{
	DrawBox(0, 0, SCX, SCY, midori, TRUE);

}
void prt_pies(int x, int y, int piedata, double ang)
{
	//拡張可能性を考慮しDrawRotaGraphとする。中心座標。
	DrawRotaGraph(x, y, 1, PI / ang, pimgs[piedata / 9], FALSE);

}
void prt_gottapies(int pies[],int x,int y){

	//ClearDrawScreen();

	pie_sort(pies);
	DrawBox(x, y - 30, x + 340, y + 30, midori, TRUE);

	int i;
	for(i = 0; i <8;i++){
		if(pies[i]>=0 &&pies[i]<81)prt_pies(x + (i * 40), y,pies[i],0.5);
		if(pies[i]<0&& pies[i] >= -81)prt_pies(x + (i * 40), y, -(pies[i]+1), 1);
	}
	
}
void prt_egottapies(int pies[], int x, int y) 
{

	//ClearDrawScreen();
	pie_sort(pies);
	DrawBox(x, y - 30, x + 340, y + 30, midori, TRUE);

	int i;
	for (i = 0; i <8; i++) {
		if (pies[i] < 0&& pies[i]>-82) 
		{
			prt_pies(x + (i * 40), y,abs(pies[i])-1, 1);
		}
		else 
		{
			prt_pies(x + (i * 40), y,81, 1);
		}

	}

}

int pickpie(void)
{
	int r;
	do {
		r = GetRand( 80);
	} while (lostpies[r] < 0);
	lostpies[r] = -(1 + r);
		return r;
}


bool docsYN(char *doc)
{
	bool YN;
	bool inkey = false;
		DrawFormatString(120, DCY, siro, doc);
		while (inkey == false) {
			ProcessMessage();
			if (CheckHitKey(KEY_INPUT_Y) == 1 && inkey == false) { YN = true; inkey = true; }
			if (CheckHitKey(KEY_INPUT_N) == 1 && inkey == false) { YN = false; inkey = true; }
		}
		DrawBox(0, DCY, 480, DCY+20, midori, TRUE);
		return YN;
}

void lp_preset(void){
	for (int h = 0; h < 81; h++){
		lostpies[h] = h;
	}

}
//上がり役を代入　敵向け
//値を現に代入してしまうほう
void rollpies_preset(int pies[])
{
	int h, i, j, r, keypie;
	int ntpie = 117;//不能

	for (h = 0; h < 3; h++) {
		keypie = GetRand(8);
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 9; j++) {
				if (keypie > 8) { r = ntpie; ntpie += 9; break; }
				r = keypie * 9 + j;
				if (lostpies[r] >= 0)break;
				if (j == 8) { r = ntpie; ntpie += 9; }
			}
			pies[h * 3 + i] = r;
			if (r < 81)lostpies[r] = -(lostpies[r] + 1);
		}
	}
	for (h = 0; h < 3; h++) {
		r = GetRand(62);
		if (r < 9 && pies[r] < 81) {
			lostpies[pies[r]] = pies[r];
			pies[r] = ntpie;
			ntpie += 9;
		}
	}

	pie_sort2(pies);
	/*
	for (j = 0; j < 9; j++) {
		printfDx("%3d", pies[j]);
	}
	printfDx("\n");
	*/

}
void playpies_preset(int pies[])
{
	int r;
	for (int i = 0; i < 8; i++){
		do {
			r = GetRand( 80);
			if (lostpies[r] >= 0) { pies[i] = lostpies[r]; }

		} while (lostpies[r] < 0);
		lostpies[r] = -1;
	}
}
void pie_sort(int pies[])
{
	// 手持ち牌の最高添え字は8。
	for (int n = 8; n >= 0; n--)
	{
		for (int i = 1; i < n; i++) {
			if (pies[i] < pies[i - 1])swap(&pies[i], &pies[i - 1]);
		}
	}
}
void pie_sort2(int pies[])
{
	/*
	チェック用のpies[8]まで含めた配列整理。
	id:012345678
	------------
	bf:223477885
	af:223457788
	*/
	for (int n = 8; n >= 0; n--)
	{
		for (int i = 0; i < n; i++) {
			if (pies[i] > pies[i + 1])swap(&pies[i], &pies[i + 1]);
		}
	}
}
int swap(int *a, int *b) {
	int tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;

	return 0;

}
int GetRandom(int min, int max)
{
	return min + (int)(rand()*(max - min + 1.0) / (1.0 + RAND_MAX));
}

void initiarize(int arr[], int var)
{
	for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++) {
		arr[i] = var;
	}
}
#endif
