#pragma comment(lib, "winmm.lib")
#include <iostream>
#include <thread>
#include <chrono>
#include <Windows.h>
#include <list>
using namespace std;


//constants
const int screenWidth = 120;
const int screenHeight = 30;
static int score = 0;
bool bdead = false;
int nfoodx = 10;
int nfoody = 10;

struct snake
{
	int x;
	int y;
};


bool getkey(int virtkey)
{
	return GetAsyncKeyState(virtkey) & 0x8000;
}

int main()
{

	wchar_t* screen = new wchar_t[120 * 30];
	
	HANDLE hconsole = CreateConsoleScreenBuffer(GENERIC_WRITE,0,NULL,CONSOLE_TEXTMODE_BUFFER,NULL);
	SetConsoleActiveScreenBuffer(hconsole);
	DWORD bytes = 0;

	

	
	
	while (1) {

		

		list<snake> sn = { {60,15},{61,15},{62,15},{63,15},{64,15},{65,15},{66,15},{67,15},{68,15},{69,15} };
		while (!bdead)
		{
			
			this_thread::sleep_for(50ms);
			for (int i = 0; i < 120 * 30; i++)
				screen[i] = ' ';

			for (int i = 0; i < screenWidth; i++)
			{
				for (int j = 0; j < screenHeight; j++)
				{
					screen[j * screenWidth + i] = (i == 0 || i == screenWidth - 1 || j == 0 || j == screenHeight - 1) ? L'*' : L' ';
					screen[2 * screenWidth + i] = L'*';
				}

			}
			PlaySound(TEXT("play_sound.wav"), NULL, SND_NOSTOP | SND_ASYNC);

			if (getkey(VK_UP))
			{
				sn.push_front({ sn.front().x , sn.front().y - 1 });
				sn.pop_back();
			}

			else if (getkey(VK_RIGHT))
			{
				sn.push_front({ sn.front().x + 1 , sn.front().y });
				sn.pop_back();
			}
			else if (getkey(VK_DOWN))
			{
				sn.push_front({ sn.front().x , sn.front().y + 1 });
				sn.pop_back();
			}

			else if (getkey(VK_LEFT))
			{
				sn.push_front({ sn.front().x - 1 , sn.front().y });
				sn.pop_back();
			}


			wsprintf(&screen[screenWidth + 50], L"SNAKE GAME SCORE:%d", score);

			for (auto s : sn)
				screen[s.y * screenWidth + s.x] = bdead ? L'+' : L'O';


			screen[sn.front().y * screenWidth + sn.front().x] = bdead ? L'X' : L'@';

			screen[nfoody * screenWidth + nfoodx] = L'$';


			//collision detection

			if (sn.front().x < 0 || sn.front().x >= screenWidth)
			
				bdead = true;
			
			if (sn.front().y < 3 || sn.front().y >= screenHeight)
				bdead = true;

			if (sn.front().x == nfoodx && sn.front().y == nfoody)
			{
				score++;

				while (screen[nfoody * screenWidth + nfoodx] != L' ')
				{
					nfoodx = rand() % screenWidth;
					nfoody = (rand() % (screenHeight - 3)) + 3;
				}
				sn.push_back({ sn.back().x,sn.back().y });
			}

			for (list<snake>::iterator it = sn.begin(); it != sn.end(); it++)
			{
				if (it != sn.begin() && it->x == sn.front().x && it->y == sn.front().y)
					bdead = true;
			}

			if (bdead)
			{
				wsprintf(&screen[screenWidth + 30], L"GAME OVER press SPACE TO restart and Esc to quit");
				PlaySound(NULL, 0, 0);
				PlaySound(TEXT("GameOver.wav"), NULL, SND_NOSTOP | SND_ASYNC);
			}
			for (auto s : sn)
				screen[s.y * screenWidth + s.x] = bdead ? L'+' : L'O';


			screen[sn.front().y * screenWidth + sn.front().x] = bdead ? L'X' : L'@';

			WriteConsoleOutputCharacter(hconsole, screen, 120 * 30, { 0,0 }, &bytes);
		}
		if (getkey(VK_SPACE))
		{
			bdead = false;
			PlaySound(NULL, 0, 0);
			
		}
		else if (getkey(VK_ESCAPE))
		{
			break;
		}
		
	}
	
	return 0;
}