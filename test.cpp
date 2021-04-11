#define _CRT_SECURE_NO_WARNINGS
#include "DxLib.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "Header.h"
#include "rolls.h"
#include "action.h"

/* グローバル変数及び関数群 */

void rollpies_preset(int pies[]);
int match(int parent);

int get_epie(int pies[], int* thpie, int* putpie, int turn, int x, int y, int* flg);
int get_epong(int pies[], int* putpie, int turn, int x, int y, int flg);
int get_ppie(int pies[], int* thpie, int* putpie, int turn, int x, int y, int* flg);
int get_ppong(int pies[], int keypie, int* putpie, int turn, int x, int y, int flg);

int pimgs[11];
int lostpies[81];
int m2pies[4];
int e2pies[4];

int p_score;
int e_score;

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
    ChangeWindowMode(TRUE); // ウィンドウモードに設定
	SetDoubleStartValidFlag(0);
	SetWindowUserCloseEnableFlag(0);
	SetWindowStyleMode(5);
	SetGraphMode(SCX, SCY, 16);
	SetMainWindowText("札雀　card-jong");

	DxLib_Init();   // DXライブラリ初期化処理

	srand((unsigned int)time(0));
	int parent;
	int key;
	bool quit = false;
	p_score = 0;
	e_score = 0;
	Load_Prof();


	do {
		DrawBox(0,0, SCX, SCY, kuro, true);
		

		p_score = 0;
		e_score = 0;
		key = 0;
		parent = GetRand( 1);

		DrawGraph(160,40, pimgs[10],true);
		DrawFormatString(160, 300, siro, "リターンキー：プレイ　ESCキー：終了");
		DrawFormatString(130, 340, siro, "(c)grurqApps 2021 SOUNDS BY PANICPUMPKIN");
		DrawFormatString(60, 360, siro, "POWERED BY DX Library Copyright (C) 2001-2021 Takumi Yamada.");
		while (key == 0) {

			ProcessMessage();
			WaitTimer(200);

			if (CheckHitKey(KEY_INPUT_RETURN) == 1)key = 1;
			if (CheckHitKey(KEY_INPUT_ESCAPE) == 1|| GetWindowUserCloseFlag(1) == 1)key = 2;
		}
		
		switch (key) {
		case 1:
			SetBackgroundColor(0, 90, 0);

			for (int i = 1; i <= 8; i++) {
				cls();
				if (i==1) {
					DrawFormatString(200, DCY, siro, "ゲームスタート　親:");
					(parent == 1) ? DrawFormatString(400, DCY, siro, "YOU") : DrawFormatString(400, DCY, siro, "COM");
					PlayMusic("dat\\スタート１.wav", 1);
					WaitTimer(4000);
					DrawBox(0, DCY, SCX, DCY + 20, midori, TRUE);

				}

				DrawFormatString(COMX+400, COMY-20, siro, "COM");
				DrawFormatString(COMX+400, COMY, siro, "SCORE:%7d", e_score);
				DrawFormatString(PLX+400, PLY-20, siro, "YOU");
				DrawFormatString(PLX+400, PLY, siro, "SCORE:%7d", p_score);
				DrawFormatString(20, 20, siro, "Round:%d/8", i);
				(parent == 1) ? DrawFormatString(PLX + 450, PLY-20, siro, "親") : DrawFormatString(COMX + 450, COMY-20, siro, "親");
				


				switch (match(parent)) {
				case 0:// 流局
					if (parent == 0) { parent = 1; }
					else { parent = 0; }
					break;
				case 1:// 人間が勝ち
					parent = 1;
					WaitKey();
					break;
				case 2:// 敵が勝ち
					parent = 0;
					WaitKey();
					break;
				case 3:// 中断
					quit = true;
					break;
				}


				if (quit == true)break;
			}
			if (!quit) {
				cls();
				
				DrawBox(0, COMY + 30, SCX, PLY - 30, daidai, true);
				DrawBox(2, COMY + 32, SCX - 2, PLY - 32, kuro, true);
				DrawGraph(10, 105, pimgs[10], true);
				DrawFormatString(400, COMY+60, siro, "結果\n\nYOU:%7d\n\nCOM:%7d", p_score, e_score);
				if (p_score > e_score) { DrawFormatString(400, PLY - 120, siro, "あなたの勝ち！"); 
				PlayMusic("dat\\クリア１３.wav", 1);
				}
				if (p_score < e_score) { DrawFormatString(400, PLY - 120, siro, "あなたの負け…");
				PlayMusic("dat\\ゲームオーバー１.wav", 1);
				}
				if (p_score == e_score) { DrawFormatString(400, PLY - 120, siro, "　ひきわけ");
				PlayMusic("dat\\スリープ３.wav", 1);
				}
				WaitTimer(4000);
				WaitKey();
			}
			break;
		case 2:
			quit = true;
			break;

		}
		
		cls();
	}while(!quit);

    DxLib_End();    // DXライブラリ終了処理
    return 0;
}



int match(int parent)
{


	SetBackgroundColor(0, 90, 0);
	DxLib_Init();   // DXライブラリ初期化処理

	int turn = 0;
	int wkey = -1;// 役チェックのためのフラグ
	int result;

	int p_pies[9];
	int e_pies[9];

	int p_thpie;
	int e_thpie;
	int p_flg=0;
	int e_flg=0;

	int i;


	lp_preset();

	if (parent > 0) { p_flg = 1; }
	else { e_flg = 1; }

		rollpies_preset(e_pies);

	
	playpies_preset(p_pies);
	

	for (turn = 0; turn < 12; turn++) {

		prt_egottapies(e_pies, COMX, COMY);
		prt_gottapies(p_pies, PLX, PLY);
		DrawBox(240, 0, 480, 40, midori, true);
		DrawFormatString(240, 20, siro, "turn:%02d", turn+1);
		
		switch (parent) {
		case 0:
				result = get_epie(e_pies, &e_thpie, &p_thpie, turn, COMX, COMY, &e_flg);
				if (result > 0) {wkey = 2 + (result % 2);break;}
				WaitTimer(250);
				

				result = get_ppie(p_pies, &p_thpie, &e_thpie, turn, PLX, PLY, &p_flg);
				if (result > 0) { wkey = 4 + (result % 2); break; }
				if (result < 0)return 3;
				WaitTimer(250);
				break;
		case 1:
			result = get_ppie(p_pies, &p_thpie, &e_thpie, turn, PLX, PLY, &p_flg);
			if (result > 0) { wkey = 6 + (result % 2); break; }
			if (result < 0)return 3;
			WaitTimer(250);
			
			result = get_epie(e_pies, &e_thpie, &p_thpie, turn, COMX, COMY, &e_flg);
			if (result > 0) { wkey = result % 2; break; }
			WaitTimer(250);
			break;}
			if (wkey >= 0) {
				if (4 > wkey) {
					e_score += winning(e_pies, e_score,p_score, wkey);
					return 2;
				}
				if (wkey >= 4) {
					p_score += winning(p_pies, p_score, e_score,wkey);
					return 1;
				}
			}
			/*
				bool tsumo = (flg & 1) == 1 ? true : false;
				bool parent = (flg & 2) == 2 ? true : false;
				bool pwin = (flg & 4) == 4 ? true : false;
			*/

		}
	for (i = 0; i < 8; i++) {
		if ((e_flg & 4) != 4 && (e_pies[i] >= 0|| -81>e_pies[i])) {
				if (e_pies[i] < 81)lostpies[e_pies[i]] = e_pies[i];
				e_pies[i] = pickpie();
			}
		if (e_pies[i] < 0) {
			e_pies[i] = abs(e_pies[i]) - 1;
		}

		}

		prt_gottapies(e_pies, COMX, COMY);
		DrawFormatString(220, 220, siro, "流　局");
		PlayMusic("dat\\スリープ３.wav", 1);
		WaitTimer(4000);

		WaitKey();      // キー入力待ち
		return 0;
}


// thpie これから捨てるパイ、putpie　すでに捨てた（ポンできる）牌
int get_epie(int pies[], int* thpie, int* putpie, int turn, int x, int y, int* flg)
{
	pie_sort2(pies);
	int i,r;
	prt_pies(x + 320, y, 81, 1);
	int ponged = get_epong(pies, putpie, turn, x, y, *flg);

	switch (ponged) {
	case 2:
		DrawBox(PLX - 20 + ((turn - (*flg % 2)) * 40), PLY - 110, PLX + 20 + ((turn - (*flg % 2)) * 40), PLY - 50, midori, true);
		PlayMusic("dat\\物＿コルクを抜く.wav", DX_PLAYTYPE_BACK);
		WaitTimer(500);
		return 4;
		break;
	case 1:
		DrawBox(PLX - 20 + ((turn - (*flg % 2)) * 40), PLY - 110, PLX + 20 + ((turn - (*flg % 2)) * 40), PLY - 50, midori, true);
		PlayMusic("dat\\物＿コルクを抜く.wav", DX_PLAYTYPE_BACK);
		break;
	default:
		PlayMusic("dat\\カーソル（柔らかい）.wav", DX_PLAYTYPE_BACK);
		break;
	}
	WaitTimer(500);
	prt_egottapies(pies, x, y);
	if (ponged == 2) {
		
	}
	
	*thpie = pickpie();
	if (GetRand(26) <= turn){
		for (i = 0; i < 9; i++) {
			if (pies[i] > 80)break;
			if (i == 8) { 
				if ((*flg & 4) == 4) return  3;
				DrawFormatString(x + 500, y - 20, siro, "リーチ:ON");
				*flg += 4;
			
			}
		}
		
	}
	
	ponged = 0;
	for (i = 0; i < 9; i++) {
		if (pies[i] < 0)ponged++;
	}
	if (ponged == 9)return 4;
	r = GetRandom(ponged, 8);
	if ((*flg & 4) == 4)r = 8;
	DrawBox((40  * r), y - 30, 40 * (r+1), y + 30, midori, TRUE);

	prt_egottapies(pies, x, y);
	((*flg & 4) == 4) ? PlayMusic("dat\\武器＿ハンマー.wav", DX_PLAYTYPE_BACK) : PlayMusic("dat\\カーソル（柔らかい）2.wav", DX_PLAYTYPE_BACK);
	prt_pies(x + (turn * 40), y + 80, *thpie, 1);
	WaitTimer(250);
	return 0;
}

int get_ppie(int pies[], int* thpie, int* putpie, int turn, int x, int y, int* flg)
{
	int key = -1;
	int i;
	bool inkey = false;
	int keypie = keychk(pies, true);
	/*　デバッグ用
	DrawFormatString(500, turn * 20, siro, "%4d", keypie);
	*/
	int ponged = 0;
	ponged = get_ppong(pies, keypie, putpie, turn, x, y, *flg);
	if (ponged == 2)return 2;
	if (ponged == 0) {
		pies[8]= pickpie();
	}

	prt_gottapies(pies, PLX, PLY);
	prt_pies(x + 320, y, pies[8], 0.5);

	
	keypie = keychk2(pies);
	/*デバッグ用
	DrawFormatString(550, turn * 20, siro, "%4d", keypie);
	*/
	bool thkey[9];
	int cdoc = 0;
	if (keypie >= 0 && (*flg & 2) != 2) { cdoc = 1; *flg += 2; }
	for (i = 0; i < 9; i++) {
		if (pies[i] >= 0) {
			if((*flg & 4) != 4)DrawFormatString(x + (i * 40), y - 50, siro, "%d", i + 1);
			if ((*flg & 2) == 2 && keypie == (pies[i] / 9)) {
				if ((*flg & 4) == 4)DrawFormatString(x + (i * 40), y - 50, siro, "%d", i + 1);
				DrawFormatString(x + (i * 40), y - 50, siro, "_"); 
				thkey[i] = true;
			}
		}
		
	}

	

	if(cdoc==1){
		DrawFormatString(560, y+100, siro, "リーチ:OFF");
		*flg -= 2;
	}
	
	key = -1;
	inkey = false;
	if (ponged == 0)PlayMusic("dat\\カーソル（柔らかい）.wav", DX_PLAYTYPE_BACK);

	WaitTimer(500);
	while (key < 0 && inkey == false) {
		if ((*flg & 4) == 4 && keypie == -200) {
			DrawFormatString(140, 300, siro, "あなたの勝ち！");
			return 1;
		}else{
			ProcessMessage();
			WaitTimer(50);
			if (CheckHitKey(KEY_INPUT_1) == 1)key = 0;
			if (CheckHitKey(KEY_INPUT_2) == 1)key = 1;
			if (CheckHitKey(KEY_INPUT_3) == 1)key = 2;
			if (CheckHitKey(KEY_INPUT_4) == 1)key = 3;
			if (CheckHitKey(KEY_INPUT_5) == 1)key = 4;
			if (CheckHitKey(KEY_INPUT_6) == 1)key = 5;
			if (CheckHitKey(KEY_INPUT_7) == 1)key = 6;
			if (CheckHitKey(KEY_INPUT_8) == 1)key = 7;
			if (CheckHitKey(KEY_INPUT_9) == 1)key = 8; // キー配列の関係で9を0番と指定
			if (cdoc == 2 && !thkey[key])key = -1;

			if (CheckHitKey(KEY_INPUT_ESCAPE) == 1 || GetWindowUserCloseFlag(1) == 1)return -1;

			if (CheckHitKey(KEY_INPUT_Z) == 1 && cdoc > 0 && (*flg&4)!=4 ) {

				DrawBox(x+500, y-20, SCX, y, midori, true);
				switch (cdoc) {
				case 1:
					DrawFormatString(x+500, y-20, siro, "リーチ:ON");
					*flg += 2;
					cdoc = 2;
					break;
				case 2:
					DrawFormatString(x+500, y-20, siro, "リーチ:OFF");
					*flg -= 2;
					cdoc = 1;
					break;
				}
				WaitTimer(150);
			}
		}

		
		//if (cdoc == 1)DrawBox(300, y + 100, 480, y + 140, midori, true);
		if (key >= 0)
		{
			if (pies[key] >= 0) {
				if((*flg & 4) == 4) {
				((pies[key]/9) == keypie) ? inkey = true: key = -1;

				}else { 
					inkey = true; 
				}
			}
		}
		
	}

	*thpie = pies[key];
	if ((pies[key] / 9) == keypie && (*flg & 2) == 2) {
		if ((*flg & 4) != 4)*flg += 4;
	}
	else {
		if ((*flg & 4) != 4)DrawBox(300, y + 100, 480, y + 140, midori, true);
		if ((*flg & 2) == 2)* flg -= 2;
	}
	DrawBox(x - 20, y - 50, x + 340, y - 30, midori, TRUE);
	DrawBox(x + (key * 40) - 20, y - 30, x + (key * 40) + 20, y + 30, midori, TRUE);

	DrawBox(x + 300, y - 30, x + 340, y + 30, midori, TRUE);
	prt_pies(x + (turn * 40), y - 80, *thpie, 0.5);
	if (key != 8)swap(&pies[key], &pies[8]);

	prt_gottapies(pies, PLX, PLY);
	((*flg & 4) == 4) ? PlayMusic("dat\\武器＿ハンマー.wav", DX_PLAYTYPE_BACK):PlayMusic("dat\\カーソル（柔らかい）2.wav", DX_PLAYTYPE_BACK);
	
	WaitTimer(100);

	return 0;
}