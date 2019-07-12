#include <iomanip>
#include <windows.h>
#include <conio.h>
#include<iostream>
using namespace std;

HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);//全局句柄
class snake;
void Init_Console();//控制台初始化
void gotoPos(SHORT x = 0, SHORT y = 0);//光标坐标控制
void Init_Map();//画边框
void KeyConctrl(snake&, int);//键盘控制函数
void is_gameover();//结束提示
void Prompt_info(int, int);//提示信息
void dis_date(int, int, double, int);//得分信息

bool gameOver = false;
bool withdraw = false;

class snake
{
private:
	bool flash;
	int speed, length, print_bit, clear_bit;
	COORD in_Pos, bit_Pos;
	COORD _Pos[255];
	enum direction { go_stop = 0, go_left, go_right, go_up, go_down }dir;
public:
	snake(SHORT x = 9, SHORT y = 5)
	{
		clear_bit = print_bit = length = speed = 0;
		in_Pos.X = x; in_Pos.Y = y; bit_Pos.X = 11; bit_Pos.Y = 8;
		flash = true;
		dir = go_stop;
		_Pos[0].X = in_Pos.X;
		_Pos[0].Y = in_Pos.Y;
	}
	~snake() {}
	void up_speed()
	{
		if (this->speed < 5)(this->speed)++;


	}
	double get_speed()
	{
		return this->speed;
	}
	int get_length()
	{
		return this->length;
	}
	//
	void dir_control(char _dir)
	{
		switch (_dir)
		{
		case 's': this->dir = go_stop;
			break;
		case 'l': if (this->dir != go_right)this->dir = go_left;
			break;
		case 'r': if (this->dir != go_left)this->dir = go_right;
			break;
		case 'u': if (this->dir != go_down)this->dir = go_up;
			break;
		case 'd': if (this->dir != go_up)this->dir = go_down;
			break;

		}
	}

	//光标位置
	void setPos(COORD pos)
	{
		SetConsoleCursorPosition(hOut, pos);
	}

	//碰撞检测
	void check_bit()
	{
		if (in_Pos.X == bit_Pos.X && in_Pos.Y == bit_Pos.Y)
		{
			length++;
			if ((length % 5 == 0))
			{
				if (this->speed < 5)this->speed++;
			}
			if (length == 254)
			{
				std::cout<< "游戏通关！";
			}
			do
			{
				srand(time(NULL));
				bit_Pos.X = 3 + (rand() % 20) * 2;
				bit_Pos.Y = 2 + rand() % 20;
			} while (check_snk(bit_Pos));
		}
		else
		{
			cle();
			clear_bit++;
		}
	}
	//撞蛇 撞边检测
	bool check_snk(COORD snk_Pos)
	{
		//边界检测
		if (snk_Pos.Y <= 1 || (snk_Pos.Y >= 23) || (snk_Pos.X <= 1) || snk_Pos.X >= 45)
		{
			return true;
		}
		for (int i = clear_bit; i <= print_bit; i++)
		{
			if (_Pos[i].X == snk_Pos.X && _Pos[i].Y == snk_Pos.Y) return true;
		}
		return false;
	}

	//显示
	void dis()
	{
		if (!flash)
		{
			setPos(bit_Pos);
			SetConsoleTextAttribute(hOut, 0x0e);
			cout << " ";
			flash = true;
		}
		else
		{
			setPos(bit_Pos);
			SetConsoleTextAttribute(hOut, 0x0e);
			cout << "■";
			flash = false;
		}
		setPos(in_Pos);
		SetConsoleTextAttribute(hOut, 0x09);
		cout << "■";

	}
	void cle()
	{

		setPos(_Pos[clear_bit]);
		SetConsoleTextAttribute(hOut, 0x05);
		cout << " ";
	}

	void save_date()
	{

		if (print_bit == 254)
		{
			for (int i = 0; i <= length; i++)
			{
				_Pos[i].X = _Pos[clear_bit + i].X;
				_Pos[i].Y = _Pos[clear_bit + i].Y;
			}
			clear_bit = 0;
			print_bit = length;
		}
		print_bit++;
		_Pos[print_bit].X = in_Pos.X;
		_Pos[print_bit].Y = in_Pos.Y;

	}

	//    //移动
	bool move()
	{
		switch (this->dir)
		{
		case go_stop:
			break;
		case go_left:
			in_Pos.X -= 2;
			if (check_snk(in_Pos))
			{
				return true;
			}
			dis();
			save_date();
			check_bit();
			break;
		case go_right:
			in_Pos.X += 2;
			if (check_snk(in_Pos))
			{
				return true;
			}
			dis();
			save_date();
			check_bit();
			break;
		case go_up:
			in_Pos.Y--;
			if (check_snk(in_Pos))
			{
				return true;
			}
			dis();
			save_date();
			check_bit();
			break;
		case go_down:
			in_Pos.Y++;
			if (check_snk(in_Pos))
			{
				return true;
			}
			dis();
			save_date();
			check_bit();
			break;
		}
		return false;
	}
};

int main()
{
	do
	{
		Init_Console();
		Init_Map();
		Prompt_info(3, 3);
		snake s(27, 11);
		clock_t t_in, t_out;
		t_in = clock();
		s.dis();
		//
		while (!gameOver)
		{
			if (_kbhit())
			{
				KeyConctrl(s, _getch());
			}

			t_out = clock();
			dis_date(3, 5, s.get_speed(), s.get_length());
			if (t_out - t_in > ((0.25 - (s.get_speed() / 25)) * CLOCKS_PER_SEC))
			{
				t_in = t_out;
				gameOver = s.move();
				if (gameOver)
				{
					is_gameover();
				}
			}
			Sleep(10);
		}
		//
		while (gameOver)
		{
			if (_kbhit())
			{
				switch (_getch())
				{
				case 'y':
				case 'Y':
					gameOver = false;
					system("cls");
					break;
				case 'n':
				case 'N':
					gameOver = false;
					withdraw = true;
					break;
				}
			}
			Sleep(10);
		}
	} while (!withdraw);
	gotoPos(15, 13);
	return 0;
}

//控制台初始化
void Init_Console()
{
	SetConsoleTitleA("Console_贪吃蛇");
	COORD dSiz = { 80, 25 };
	SetConsoleScreenBufferSize(hOut, dSiz);//设置窗口缓冲区大小
	CONSOLE_CURSOR_INFO _guan_biao = { 1, FALSE };//设置光标大小，隐藏光标
	SetConsoleCursorInfo(hOut, &_guan_biao);
	system("color 0f");//设置画布颜色

}
//光标位置
void gotoPos(SHORT x, SHORT y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(hOut, pos);
}
//画边框
void Init_Map()
{
	//SetConsoleTextAttribute(hOut, 0xF0); //设置前景色，背景色
	system("cls");
	//左边框
	for (int i = 0; i < 23; i++)
	{
		cout << endl << " ●";
	}
	//上边框
	gotoPos(3, 1);
	for (int i = 0; i < 21; i++)
	{
		cout << "●";
	}
	//下边框
	gotoPos(3, 23);
	for (int i = 0; i < 21; i++)
	{
		cout << "●";
	}
	//右边框
	for (SHORT i = 1; i <= 23; i++)
	{
		gotoPos(45, i);
		cout << "●";
	}
}
//键盘控制函数
void KeyConctrl(snake & _snk, int _key)
{
	switch (_key)
	{
	case ' ':
		_snk.dir_control('s');
		break;
	case 'w':
	case 'W':
	case 72: _snk.dir_control('u');
		break;
	case 'a':
	case 'A':
	case 75: _snk.dir_control('l');
		break;
	case 'd':
	case 'D':
	case 77: _snk.dir_control('r');
		break;
	case 's':
	case 'S':
	case 80: _snk.dir_control('d');
		break;
	case '+': _snk.up_speed();
		break;
	default: break;
	}
}
//结束提示
void is_gameover()
{

	gotoPos(17, 11);
	SetConsoleTextAttribute(hOut, 0xec);
	cout << "game over!";
	gotoPos(15, 12);
	cout << "Y重新开始/N退出";
	SetConsoleTextAttribute(hOut, 0x0f);

}
//提示信息
void Prompt_info(int _x, int _y)
{

	SetConsoleTextAttribute(hOut, 0xB);
	gotoPos(_x + 47, _y + 0);
	cout << "■游戏说明：";
	gotoPos(_x + 51, _y + 2);
	cout << "A.每得500分移速自动加1";
	gotoPos(_x + 51, _y + 3);
	cout << "B.可手动加速,最高移速5";
	gotoPos(_x + 47, _y + 5);
	cout << "■操作说明：";
	gotoPos(_x + 51, _y + 7);
	cout << "□向左移动：← A";
	gotoPos(_x + 51, _y + 8);
	cout << "□向右移动：→ D";
	gotoPos(_x + 51, _y + 9);
	cout << "□向下移动：↓ S";
	gotoPos(_x + 51, _y + 10);
	cout << "□向上移动：↑ W";
	gotoPos(_x + 51, _y + 11);
	cout << "□控制加速：+";
	gotoPos(_x + 51, _y + 12);
	cout << "□暂停游戏：空格";
	gotoPos(_x + 51, _y + 13);
	cout << "□开始游戏：任意方向键";
	gotoPos(_x + 47, 22);
	cout << "■By: Flowingwind 18.01.11";
}
//速度积分显示
void dis_date(int _x, int _y, double Sped, int Score)
{
	SetConsoleTextAttribute(hOut, 0xB);
	gotoPos(_x + 47, _y + 13);
	cout << setw(2) << "◆ 移动速度： ";
	SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << Sped;
	gotoPos(_x + 47, _y + 14);
	SetConsoleTextAttribute(hOut, 0xB);
	cout << "◆ 当前积分： ";
	SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << setw(2) << Score * 100;
}
