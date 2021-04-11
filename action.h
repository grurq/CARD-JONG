#pragma once
#include "Header.h"
int get_epong(int pies[], int* putpie, int turn, int x, int y, int flg)
{
	if (*putpie < 0)return 0;
	if ((flg&1)==1 && turn == 0) return 0;
	int ponged = 0;
	int pongpie = (*putpie) / 9;
	int i;

	for (i = 0; i < 9; i++)
	{
		if (pongpie == (pies[i] / 9) && pies[i] >= 0)ponged++;
	}
	if (ponged > 2) {
		if (GetRand(1) == 0) {
			
			ponged = 0;
				for (int j = 0; j < 9; j++) {
					if (pongpie == (pies[j] / 9))
					{
						pies[j] = -(pies[j] + 1);
						ponged++;
						if (ponged == 3)break;
					}
				}
				if ((flg & 4) == 4) return 2; 

			*putpie = -100;
			pie_sort2(pies);
			return 1;
		}
	}
	return 0;
}
int get_ppong(int pies[], int keypie, int* putpie, int turn, int x, int y, int flg)
{
	int ponged = 1;

	int pongpie = (*putpie) / 9;
	int i;
	if (*putpie < 0 || pongpie < 0)return 0;
	if ((flg&1)==1 && turn == 0) return 0;
	for (i = 0; i < 8; i++)
	{
		if (pongpie == pies[i]/9 &&pies[i]>=0)ponged++;
	}
	if (ponged > 2) {
		if (keypie == pongpie &&(flg&4)==4/*|| (8 - keypie) == (8 - pongpie)*/) {
			if (docsYN((char*)"上がりますか?（Y/N）") == true) {

				pies[8] = -(pies[8] + 1);
				*putpie = -100;
				DrawFormatString(140, 300, siro, "あなたの勝ち！"); return 2;
			}
			DrawBox(0, 240, 480, 260, midori, TRUE);
		}
		if (keypie!=pongpie&& (flg&4)!=4) {
			if (docsYN((char*)"ポンしますか？(Y/N)") == true) {
				pies[8] = *putpie;
				ponged = 0;
				for (int j = 0; j < 9; j++) {
					if ((*putpie / 9) == (pies[j] / 9) || ((80 - *putpie) / 9) == ((80 - pies[j]) / 9))
					{
						pies[j] = -(pies[j] + 1);
						ponged++;
					}
					if (ponged == 3)break;

				}
				*putpie = -100;
				pie_sort2(pies);
				DrawBox(COMX - 20 + ((turn - (flg % 2)) * 40), COMY + 50, COMX + 20 + ((turn - (flg % 2)) * 40), COMY + 110, midori, true);
				DrawBox(0, 240, 480, 260, midori, TRUE);
				//DrawBox(x - 20 + ((turn - (flg%2)) * 40), y - 110, x + 20 + ((turn - (flg%2)) * 40), y - 50, GetColor(0, 90, 0), true);
				PlayMusic("dat\\物＿コルクを抜く.wav", DX_PLAYTYPE_BACK);
				WaitTimer(100);
				return 1;
			}

		}
	
		DrawBox(0, 240, 480, 260, midori, TRUE);

	}

	
	return 0;
}

