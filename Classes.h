#pragma once
#include "olcConsoleGameEngine.h"
#include "tools.h"
#include "data.h"
#include <string>
#include <vector>

using std::wstring;
using std::vector;

class Screen;

//           888                                    
//           888                                    
//           888                                    
//  88888b.  888  8888b.  888  888  .d88b.  888d888 
//  888 "88b 888     "88b 888  888 d8P  Y8b 888P"   
//  888  888 888 .d888888 888  888 88888888 888     
//  888 d88P 888 888  888 Y88b 888 Y8b.     888     
//  88888P"  888 "Y888888  "Y88888  "Y8888  888     
//  888                        888                  
//  888                   Y8b d88P                  
//  888 

class Player
{//Представляет родительский класс игрока
public:

	Player()
	{
		name = L"Undefined";
		score_total = 0;
		score_turn = 0;
		score_pick = 0;
		isLeftPlayer = false;
		table_dices = vector<int>();
	}

	wstring name;
	int score_total;
	int score_turn;
	int score_pick;
	bool isLeftPlayer;
	IntVector table_dices;

	void set_TableDices(IntVector& dices_vec);

	void add_ScoreTotal(Screen& scr);

	void add_ScoreTurn(Screen& scr);

	void add_ScorePick(Screen& scr, int score);

	void clear_ScoreTurn(Screen& scr);

	void clear_ScorePick(Screen& scr);
};

//  888                                               
//  888                                               
//  888                                               
//  88888b.  888  888 88888b.d88b.   8888b.  88888b.  
//  888 "88b 888  888 888 "888 "88b     "88b 888 "88b 
//  888  888 888  888 888  888  888 .d888888 888  888 
//  888  888 Y88b 888 888  888  888 888  888 888  888 
//  888  888  "Y88888 888  888  888 "Y888888 888  888 

class Human : public Player
{//Представляет игрока по ту сторону экрана
public:
	Human(wstring name_in, bool isLeftPlayer_in);

	IntVector get_DiceChoose(Screen& scr);
	//Возвращает вектор с выбранными игроком костями

	ActionChoice get_ActChoice(Screen& scr);
	//Возрващает ввод от игрока с информацией, хочет ли он
	//продолжить\закончить ход или выбрать другие кости
};

//                  888               888    
//                  888               888    
//                  888               888    
//  888d888 .d88b.  88888b.   .d88b.  888888 
//  888P"  d88""88b 888 "88b d88""88b 888    
//  888    888  888 888  888 888  888 888    
//  888    Y88..88P 888 d88P Y88..88P Y88b.  
//  888     "Y88P"  88888P"   "Y88P"   "Y888

class Robot : public Player
{//Представляет игрока-бота
public:

	Robot(wstring name_in, bool isLeftPlayer_in, int high_bar_in);

protected:

	IntVector claw;	//клешня робота, в которой он хранит выбранные кости
	IntVector dices_for_pick;	//копия списка с выпавшими костями. Ее использование
								//освобождает от передачи списка dices как аргумента в
								//функции take_*
	int high_bar;

public:

	void take_RangeCombo();
	//Забирает в claw наибольший диапазон костей

	void take_RowCombo();
	//Забирает в claw ряд(ы) костей

	void take_SingleCombo(int amount = 4);
	//Забирает в claw единичные кости в кол-ве amount

	int get_ChanceToContinue(int points);
	//Возвращает в процентах шанс продолжить ход в зависимости от набранного кол-ва очков

	IntVector get_CombRowDice();
	//Возвращает значение костей, образующих комбо 'row'

	IntVector get_DiceChoose();
	//Определяет и возвращает вектор с выбранными роботом костями

	ActionChoice get_ActChoice();
	//Определяет и возвращает выбор робота по завершению его действия
};

//  .d8888b   .d8888b 888d888 .d88b.   .d88b.  88888b.  
//  88K      d88P"    888P"  d8P  Y8b d8P  Y8b 888 "88b 
//  "Y8888b. 888      888    88888888 88888888 888  888 
//       X88 Y88b.    888    Y8b.     Y8b.     888  888 
//   88888P'  "Y8888P 888     "Y8888   "Y8888  888  888 

class Screen : public olcConsoleGameEngine
{//Представляет игровой экран, отображающий всю графику
 //использует движок olcConsoleGameEngine для ее отображения.

protected:

	IntVector scr_dices = vector<int>(6, 0);
	//кости, находящийся на экране. Индекс означает позицию по
	//DICES_POSITIONS.Значение "0" означает пустоту, т.е.при
	//отображении "нулевой" кости, ее позиция очистится.
	

	virtual bool OnUserCreate()
	{
		m_sAppName = L"ZONK";
		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime)
	{
		return true;
	}

public:

	//input_ - функции, работающие со вводом

	wstring input_Str()
	{//Возвращает введенную пользователем строку
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

		wstring inp = L"";
		int curs_x = ZONE_INPUT[0];
		int curs_y = ZONE_INPUT[1];

		move_Cursor(curs_x, curs_y);
		set_CursorVis(true);

		UpdateScr();

		for (int k = 0; k <= 1000; k++)
		{
			wchar_t key = _getwch();

			if ((isalpha(key) or isdigit(key)) &&
				int(inp.length()) < ZONE_INPUT[2] - ZONE_INPUT[0])	//Буква или цифра
			{
				inp += key;
				Draw(curs_x, curs_y, key);
				curs_x++;
			}
			else if (key == 13)	//Enter
			{
				break;
			}
			else if (key == 8 && inp != L"")	//BackSpace
			{
				wstring tmp = L"";
				for (unsigned int i = 0; i < inp.length() - 1; i++)
				{
					tmp += inp[i];
				}
				inp = tmp;

				curs_x--;
				Draw(curs_x, curs_y, L' ');
			}
			else if (key == 27)	//Esc
			{
				inp = L"";
				curs_x = ZONE_INPUT[0];
				curs_y = ZONE_INPUT[1];
				clear_Zone(ZONE_INPUT);
			}
			move_Cursor(curs_x, curs_y);
			UpdateScr();

			if (k == 1000)
			{
				inp = L"";
				DrawString(ZONE_INPUT[0], ZONE_INPUT[1], L"ERR: Превышен лимит ввода");
				UpdateScr();
				delay(2);
			}
		}
		set_CursorVis(false);
		clear_Zone(ZONE_INPUT);
		UpdateScr();

		return inp;
	}

	bool input_DelayInterrupt(float delay_seconds)
	{//Создает задержку, прерываемую нажатием кнопки
		bool isInterrupted = false;
		int delay_ms = int(delay_seconds * 1000);
		Sleep(100);
		for (int dt = 0; dt < delay_ms - 100; dt += 10)
		{
			if ((GetKeyState(VK_SPACE) & 0x8000) ||
				(GetKeyState(VK_RETURN) & 0x8000) ||
				(GetKeyState(VK_ESCAPE) & 0x8000))
			{
				isInterrupted = true;
				break;
			}
			UpdateScr();
			Sleep(10);
		}
		return isInterrupted;
	}

	//display_ - функции, отображающе переменную информацию на экране

	void display_Msg(std::string msg_id, wstring insert = L"", bool wait = true, float speedup = 1.0f)
	{//Выводит на экран сообщение из реестра по его кейкоду.
	 //Если в сообщении есть ключ. слово %s, то заменяет его на строку insert
	 //Если в сообщении есть ключ. слово %p, то делает паузу в выводе, определенную в data.h
	 //wait определяет, ждать ли от игрока нажатия клавиши после вывода сообщения (т.е. показывать ли мигающую стрелочку)
	 //speedup - множитель скорости вывода сообщения. Значения > 1 ускоряют вывод, < 1 - замедляют
		clear_Zone(ZONE_MSG);

		int x_start = ZONE_MSG[0] + 1;
		int y = ZONE_MSG[1];
		unsigned int x = x_start;

		WstrVector words = splitStrVec(MSG_REGISTRY[msg_id], L' ');
		for (unsigned int i = 0; i < words.size(); i++)
		{
			wstring word = words[i];
			if (word.length() > 1 && word[0] == L'%')
			{
				if (word[1] == L's')
				{
					word = replaceInsert(word, insert);
				}
				else if (word[1] == L'p')
				{
					delay(PRINT_PAUSE);
					continue;
				}
				else if (word[1] == L'n')
				{
					y++;
					x = x_start;
					move_Cursor(x_start, y);
					continue;
				}
			}
			if (int(x - x_start + word.length() + 1) >= ZONE_MSG[2] - ZONE_MSG[0])
			{
				y++;
				x = x_start;
				move_Cursor(x_start, y);
			}
			anim_PerCharWord(x, y, word, PRINT_SPEED / speedup);
			x += word.length() + 1;
		}
		if (wait)
		{
			if (!input_DelayInterrupt(1))
			{
				anim_ArrowFlick(x, y);
			}
		}
		
	}

	void display_Dices(IntVector& dices)
	{//Выводит кости на экран (в случайные позиции)
		nullVec(scr_dices);

		IntVector positions = vector<int>{ 0, 1, 2, 3, 4, 5 };
		shuffleVec(positions);

		for (unsigned int i = 0; i < dices.size(); i++)
		{
			int pos = positions[i];
			scr_dices[pos] = dices[i];
		}
		for (unsigned int i = 0; i < scr_dices.size(); i++)
		{
			display_Dice(i, scr_dices[i]);
		}
	}

	void display_Dice(int position, int value, short col = COL_WHITE)
	{//Выводит на экран кость в указанную позицию
		int x = DICES_POSITIONS[position][0];
		int y = DICES_POSITIONS[position][1];

		if (value != 0)
		{
			DrawString(x, y, ASCII_DICES[value][0], col + ATR_UNDERLINE);
			DrawString(x, y + 1, ASCII_DICES[value][1], col);
			DrawString(x, y + 2, ASCII_DICES[value][2], col);
			DrawString(x, y + 3, ASCII_DICES[value][3], col + ATR_UNDERLINE);
			DrawString(x, y, L" ", col);
			DrawString(x + 6, y, L" ", col);
			DrawString(x, y + 3, L"│", col);
			DrawString(x + 6, y + 3, L"│", col);
		}
		else
		{
			DrawString(x, y, ASCII_DICES[value][0], col);
			DrawString(x, y + 1, ASCII_DICES[value][1], col);
			DrawString(x, y + 2, ASCII_DICES[value][2], col);
			DrawString(x, y + 3, ASCII_DICES[value][3], col);
		}
		UpdateScr();
	}

	void display_Score(int score, ScoreType score_type, bool isLeftPlayer)
	{//Выводит или убирает (при score = 0) с экрана очки игрока
		int x, y;

		switch (score_type)
		{
		case e_ScoreTotal:
			y = ZONE_SCORE[1] + 3;
			break;
		case e_ScoreTurn:
			y = ZONE_SCORE[1] + 5;
			break;
		case e_ScorePick:
			y = ZONE_SCORE[1] + 6;
			break;
		default:
			y = 0;
			break;
		}

		if (isLeftPlayer) x = ZONE_SCORE[0] + 5;
		else x = ZONE_SCORE[0] + 14;

		if (score != 0)
		{
			if (score_type == e_ScorePick)
			{
				wstring str_score = L"+" + std::to_wstring(score);
				DrawString(x, y, str_score, COL_BLUE);
			}
			else
			{
				DrawString(x, y, std::to_wstring(score), COL_WHITE + ATR_UNDERLINE);
			}
		}
		else
		{
			if (score_type == e_ScorePick)
			{
				DrawString(x, y, L"+     ");
			}
			else
			{
				DrawString(x, y, L"     ", COL_WHITE + ATR_UNDERLINE);
			}
		}
		UpdateScr();
	}

	//anim_ - функции, проигрывающие анимации

	void anim_HlPlayer(bool isSetLeftActive, wstring name1, wstring name2)
	{//Проигрывает анимацию бегунка, выделяющего имена игроков
		int y = ZONE_SCORE[1] + 1;

		int len_n1 = name1.length();
		int len_n2 = name2.length();

		if (isSetLeftActive)
		{
			for (int dx = 0; dx < len_n2 + 9; dx++)
			{
				int x_f = ZONE_SCORE[0] + 14 - dx;
				int x_b = ZONE_SCORE[0] + 14 + len_n2 - dx;

				if (x_f >= ZONE_SCORE[0] + 5)
				{
					PaintChar(x_f, y, COL_INV);
				}
				if (x_b >= ZONE_SCORE[0] + 5 + len_n1)
				{
					PaintChar(x_b, y);
				}
				UpdateScr();
				delay(0.04f);
			}
		}
		else
		{
			for (int dx = 0; dx < len_n2 + 9; dx++)
			{
				int x_f = ZONE_SCORE[0] + 5 + len_n1 + dx;
				int x_b = ZONE_SCORE[0] + 5 + dx;

				if (x_f < ZONE_SCORE[0] + 14 + len_n2)
				{
					PaintChar(x_f, y, COL_INV);
				}
				if (x_b < ZONE_SCORE[0] + 14)
				{
					PaintChar(x_b, y);
				}
				UpdateScr();
				delay(0.04f);
			}
		}
	}

	void anim_Diceroll(int num_dices)
	{//Проигрывает анимацию броска костей
		vector<int> rand_dices(6, 0);
		for (int k = 0; k < 10; k++)
		{
			for (int i = 0; i < num_dices; i++)
			{
				rand_dices[i] = randIntRange(1, 6);
			}
			display_Dices(rand_dices);
			UpdateScr();
			delay(0.1f);
		}
	}

	void anim_ArrowFlick(int x, int y)
	{//Проигрывает анимащию мигающей стрелки (прерывается нажатием клавиши)
		wchar_t curr = L'▼';
		wchar_t last = L' ';
		wchar_t tmp;

		bool isInterrupted = false;
		while (!isInterrupted)
		{
			Draw(x, y, curr);
			tmp = curr;
			curr = last;
			last = tmp;
			UpdateScr();

			isInterrupted = input_DelayInterrupt(0.8f);
		}
		Draw(x, y, L' ');
		UpdateScr();
	}

	void anim_PerCharWord(int x, int y, wstring word, float ch_print_delay = PRINT_SPEED)
	{//Посимвольно выводит слово на экран
		for (unsigned int i = 0; i < word.length(); i++)
		{
			Draw(x, y, word[i]);
			x++;
			UpdateScr();
			delay(ch_print_delay);
		}
		UpdateScr();
	}

	void anim_Start()
	{//Постепенно прорисовывает интерфейс
		float f_delay = 0.5f;
		for (int y = 0; y < SH; y++)
		{
			DrawLine(0, y, SW, y, L'∙', 0x0021);
			UpdateScr();
			delay(0.06f);
		}
		delay(0.5f);
		add_Zone(ZONE_MSG, L"┤msg├");
		delay(f_delay);
		add_Zone(ZONE_DICES, L"┤dices├");
		delay(f_delay);
		add_Zone(ZONE_SCORE, L"┤score├");
		delay(f_delay);
		add_Zone(ZONE_INPUT, L"┤input├");
		delay(f_delay * 1.5f);
		add_ZoneScoreInterface();

		UpdateScr();
	}

	void anim_Ending()
	{//Проигрывает анимацию очищения сверху вниз
		for (int y = 0; y <= SH; y++)
		{
			DrawLine(0, y, SW, y, L' ', 0x00E0);
			UpdateScr();
			delay(0.035f);
		}
	}

	//effect_ - функции, отображающие эффекты

	void effect_HlPlayers(bool isLeftActive, wstring name1, wstring name2)
	{//Выделяет имя текущего игрока, снимая выделение с предыдущего
		if (isLeftActive)
		{
			DrawString(ZONE_SCORE[0] + 5,  ZONE_SCORE[1] + 1, name1, COL_INV);
			DrawString(ZONE_SCORE[0] + 14, ZONE_SCORE[1] + 1, name2);
		}
		else
		{
			DrawString(ZONE_SCORE[0] + 5,  ZONE_SCORE[1] + 1, name1);
			DrawString(ZONE_SCORE[0] + 14, ZONE_SCORE[1] + 1, name2, COL_INV);
		}
		UpdateScr();
	}

	void effect_HlDices(IntVector dices, short col = COL_BLUE)
	{//Выделяет кости цветом col
		IntVector scr_dices_copy = copyVec(scr_dices);
		for (unsigned int i = 0; i < dices.size(); i++)
		{
			int dice_val = dices[i];
			int scr_pos = idxIntVec(scr_dices_copy, dice_val);
			display_Dice(scr_pos, dice_val, col);
			scr_dices_copy[scr_pos] = 0;
		}
		UpdateScr();
	}

	void effect_UnHlDices()
	{//Снимает выделение со всех костей
		for (unsigned int i = 0; i < scr_dices.size(); i++)
		{
			display_Dice(i, scr_dices[i], COL_WHITE);
		}
		UpdateScr();
	}

	//add_ - функции. мнгновенно добавляющие что-либо на экран

	void add_Zone(const int zone[4], wstring headline)
	{//Добавляет на экран игровую зону, созданную с заданными цветами
		int x1 = zone[0] - 1, y1 = zone[1] - 1;
		int x2 = zone[2] + 1, y2 = zone[3] + 1;

		Fill(x1, y1, x2, y2, ' ');
		DrawLine(x1, y1, x2, y1, L'─', 0x004F);
		DrawLine(x1, y2, x2, y2, L'─');
		DrawLine(x1, y1, x1, y2, L'│', 0x004F);
		DrawLine(x2, y1, x2, y2, L'│');
		Draw(x1, y1, L'∙', 0x004F);
		Draw(x1, y2, L'∙', 0x004F);
		Draw(x2, y1, L'∙', 0x004F);
		Draw(x2, y2, L'∙');
		DrawLine(x1 + 1, y2 + 1, x2 + 1, y2 + 1, L'∙', 0x0065);	//Нижняя темная тень
		DrawLine(x2 + 1, y1 + 1, x2 + 1, y2, L'∙', 0x0065);		//Боковая темная тень
		DrawLine(x1 + 2, y2 + 2, x2 + 2, y2 + 2, L'∙', 0x0043);	//Нижняя светлая тень
		DrawLine(x2 + 2, y1 + 2, x2 + 2, y2 + 1, L'∙', 0x0043);	//Боковая светлая тень
		DrawString(x2 - headline.length() - 2, y1, headline);
		UpdateScr();
	}

	void add_ZoneScoreInterface()
	{//Отрисовывает зону для очков
		DrawString(ZONE_SCORE[0],      ZONE_SCORE[1] + 3, L" Tot");
		DrawString(ZONE_SCORE[0] + 5,  ZONE_SCORE[1] + 3, L"     ", COL_WHITE + ATR_UNDERLINE);
		DrawString(ZONE_SCORE[0] + 14, ZONE_SCORE[1] + 3, L"     ", COL_WHITE + ATR_UNDERLINE);
		DrawString(ZONE_SCORE[0],      ZONE_SCORE[1] + 5, L" Tur");
		DrawString(ZONE_SCORE[0] + 5,  ZONE_SCORE[1] + 5, L"     ", COL_WHITE + ATR_UNDERLINE);
		DrawString(ZONE_SCORE[0] + 14, ZONE_SCORE[1] + 5, L"     ", COL_WHITE + ATR_UNDERLINE);
		DrawString(ZONE_SCORE[0],      ZONE_SCORE[1] + 6, L"     +        +     ");
		DrawString(ZONE_SCORE[0],      ZONE_SCORE[1] + 7, L" Win");
		DrawString(ZONE_SCORE[0] + 9, ZONE_SCORE[1] + 7, L"      ", COL_WHITE + ATR_UNDERLINE);
	}

	void add_Players(wstring name1, wstring name2)
	{//Добавляет на экран имена игроков
		DrawString(ZONE_SCORE[0] + 5, ZONE_SCORE[1] + 1, name1);
		DrawString(ZONE_SCORE[0] + 14, ZONE_SCORE[1] + 1, name2);
		UpdateScr();
	}

	void add_HighBar(int hbar_value)
	{//Добавляет на экран high_bar
		DrawString(ZONE_SCORE[0] + 10, ZONE_SCORE[1] + 7,
			std::to_wstring(hbar_value), ATR_UNDERLINE + COL_WHITE);
		UpdateScr();
	}

	void add_Interface()
	{//Добавляет интерфейс на экран
		Fill(0, 0, SW, SH, L'∙', 0x0021);
		add_Zone(ZONE_MSG, L"┤msg├");
		add_Zone(ZONE_DICES, L"┤dices├");
		add_Zone(ZONE_SCORE, L"┤score├");
		add_Zone(ZONE_INPUT, L"┤input├");
		add_ZoneScoreInterface();

		UpdateScr();
	}

	//else - дополнительные функции без типа

	void clear_Zone(const int zone[4])
	{//Отчищает переданную игровую зону
		for (int y = zone[1]; y <= zone[3]; y++)
		{
			DrawLine(zone[0], y, zone[2], y, L' ');
		}
		UpdateScr();
	}

	void set_Colors()
	{//Устанавливает цвета, заменяя стандартные цвета консоли
		CONSOLE_SCREEN_BUFFER_INFOEX csbi_ex;
		csbi_ex.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
		GetConsoleScreenBufferInfoEx(m_hConsole, &csbi_ex);
		csbi_ex.ColorTable[0] = RGB(0, 6, 21);			//Черно-синий - основной фон
		csbi_ex.ColorTable[1] = RGB(165, 202, 246);	//Фон f
		csbi_ex.ColorTable[2] = RGB(8, 28, 69);		//Фон b
		csbi_ex.ColorTable[3] = RGB(0, 70, 140);		//Светлая тень f
		csbi_ex.ColorTable[4] = RGB(1, 10, 35);		//Светлая тень b
		csbi_ex.ColorTable[5] = RGB(0, 39, 78);		//Темная тень f
		csbi_ex.ColorTable[6] = RGB(0, 6, 21);			//Темная тень b

		csbi_ex.ColorTable[12] = RGB(51, 204, 255);	//Голубое выделение
		csbi_ex.ColorTable[13] = RGB(255, 70, 70);		//Красное выделение
		csbi_ex.ColorTable[14] = RGB(12, 12, 12);		//Консольно-черный
		csbi_ex.ColorTable[15] = RGB(255, 255, 255);	//Белый
		SetConsoleScreenBufferInfoEx(m_hConsole, &csbi_ex);
	}

	void reset_Colors()
	{//Возвращает основные стандартные цвета консоли
		CONSOLE_SCREEN_BUFFER_INFOEX csbi_ex;
		csbi_ex.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
		GetConsoleScreenBufferInfoEx(m_hConsole, &csbi_ex);
		csbi_ex.ColorTable[0] = RGB(12, 12, 12);
		csbi_ex.ColorTable[15] = RGB(255, 255, 255);
		SetConsoleScreenBufferInfoEx(m_hConsole, &csbi_ex);
	}

	void move_Cursor(int x, int y)
	{//Перемещает курсор на позицию x, y
		COORD pos;
		pos.X = x;
		pos.Y = y;
		SetConsoleCursorPosition(m_hConsole, pos);
	}

	void set_CursorVis(bool isEnable)
	{//Включает\отключает видимость курсора
		CONSOLE_CURSOR_INFO cci;
		GetConsoleCursorInfo(m_hConsole, &cci);
		if (isEnable)
		{
			cci.bVisible = TRUE;
		}
		else
		{
			cci.bVisible = FALSE;
		}
		SetConsoleCursorInfo(m_hConsole, &cci);
	}

	friend class Player;
	friend class Human;
	friend class Robot;
};

//                         888    888                    888          
//                         888    888                    888          
//                         888    888                    888          
//  88888b.d88b.   .d88b.  888888 88888b.   .d88b.   .d88888 .d8888b  
//  888 "888 "88b d8P  Y8b 888    888 "88b d88""88b d88" 888 88K      
//  888  888  888 88888888 888    888  888 888  888 888  888 "Y8888b. 
//  888  888  888 Y8b.     Y88b.  888  888 Y88..88P Y88b 888      X88 
//  888  888  888  "Y8888   "Y888 888  888  "Y88P"   "Y88888  88888P' 

//           888                                    
//           888                                    
//           888                                    
//  88888b.  888  8888b.  888  888  .d88b.  888d888 
//  888 "88b 888     "88b 888  888 d8P  Y8b 888P"   
//  888  888 888 .d888888 888  888 88888888 888     
//  888 d88P 888 888  888 Y88b 888 Y8b.     888     
//  88888P"  888 "Y888888  "Y88888  "Y8888  888     
//  888                        888                  
//  888                   Y8b d88P                  
//  888                    "Y88P"                  

inline void Player::set_TableDices(IntVector& dices_vec)
{
	table_dices = dices_vec;
}

inline void Player::add_ScoreTotal(Screen& scr)
{
	score_total += score_turn;
	score_turn = 0;
	scr.display_Score(score_turn, e_ScoreTurn, isLeftPlayer);
	scr.display_Score(score_total, e_ScoreTotal, isLeftPlayer);
}

inline void Player::add_ScoreTurn(Screen& scr)
{
	score_turn += score_pick;
	score_pick = 0;
	scr.display_Score(score_pick, e_ScorePick, isLeftPlayer);
	scr.display_Score(score_turn, e_ScoreTurn, isLeftPlayer);
}

inline void Player::add_ScorePick(Screen& scr, int score)
{
	score_pick += score;
	scr.display_Score(score_pick, e_ScorePick, isLeftPlayer);
}

inline void Player::clear_ScoreTurn(Screen& scr)
{
	score_turn = 0;
	scr.display_Score(score_turn, e_ScoreTurn, isLeftPlayer);
}

inline void Player::clear_ScorePick(Screen& scr)
{
	score_pick = 0;
	scr.display_Score(score_pick, e_ScorePick, isLeftPlayer);
}

//  888                                               
//  888                                               
//  888                                               
//  88888b.  888  888 88888b.d88b.   8888b.  88888b.  
//  888 "88b 888  888 888 "888 "88b     "88b 888 "88b 
//  888  888 888  888 888  888  888 .d888888 888  888 
//  888  888 Y88b 888 888  888  888 888  888 888  888 
//  888  888  "Y88888 888  888  888 "Y888888 888  888 


inline Human::Human(wstring name_in, bool isLeftPlayer_in)
{
	name = name_in;
	isLeftPlayer = isLeftPlayer_in;
	score_total = 0;
	score_turn = 0;
	score_pick = 0;
}

inline IntVector Human::get_DiceChoose(Screen& scr)
{
	IntVector dices = table_dices;
	for (int k = 0; k < 1000; k++)
	{
		wstring inp = scr.input_Str();
		if (isDigitWStr(inp))
		{
			bool isBadInput = false;
			for (unsigned int i = 0; i < inp.length(); i++)
			{
				if (count(inp.begin(), inp.end(), inp[i]) >
					count(dices.begin(), dices.end(), wcharToInt(inp[i])))
				{
					isBadInput = true;
					break;
				}
			}
			if (!isBadInput)
			{
				IntVector out = vector<int>(inp.size());
				for (unsigned int i = 0; i < inp.size(); i++)
				{
					out[i] = wcharToInt(inp[i]);
				}
				return out;
			}
		}
	}
	return vector<int>();
}

inline ActionChoice Human::get_ActChoice(Screen& scr)
{
	typedef ActionChoice AC;
	for (int k = 0; k < 1000; k++)
	{
		wstring inp = scr.input_Str();
		if (isDigitWStr(inp) && inp.size() == 1)
		{
			AC key_code = AC(wcharToInt(inp[0]));
			switch (key_code)
			{
			case AC::e_TurnCancel:
				return AC::e_TurnCancel;
			case AC::e_TurnContinue:
				return AC::e_TurnContinue;
			case AC::e_TurnEnd:
				return AC::e_TurnEnd;
			default:
				break;
			}
		}
	}
	return AC::e_Undefined;
}

//                  888               888    
//                  888               888    
//                  888               888    
//  888d888 .d88b.  88888b.   .d88b.  888888 
//  888P"  d88""88b 888 "88b d88""88b 888    
//  888    888  888 888  888 888  888 888    
//  888    Y88..88P 888 d88P Y88..88P Y88b.  
//  888     "Y88P"  88888P"   "Y88P"   "Y888

inline Robot::Robot(wstring name_in, bool isLeftPlayer_in, int high_bar_in)
{
	name = name_in;
	isLeftPlayer = isLeftPlayer_in;
	high_bar = high_bar_in;
	score_total = 0;
	score_turn = 0;
	score_pick = 0;
}

inline void Robot::take_RangeCombo()
{
	vector<int> range{ 1, 2, 3, 4, 5 };
	extendIntVec(claw, range);
	if (countIntVec(dices_for_pick, 6) != 0)
	{
		claw.push_back(6);
	}
	excludeIntVec(dices_for_pick, claw);
}

inline void Robot::take_RowCombo()
{
	IntVector table_dices_copy = copyVec(dices_for_pick);
	for (unsigned int i = 0; i < table_dices_copy.size(); i++)
	{
		int dice_val = table_dices_copy[i];
		if (countIntVec(table_dices_copy, dice_val) >= 3)
		{
			claw.push_back(dice_val);
			removeIntVec(dices_for_pick, dice_val);
		}
	}

}

inline void Robot::take_SingleCombo(int amount)
{
	IntVector table_dices_copy = copyVec(dices_for_pick);
	for (int d_value = 1; d_value <= 5; d_value += 4)
	{
		for (unsigned int  i = 0; i < table_dices_copy.size(); i++)
		{
			if (amount == 0) { return; }
			if (table_dices_copy[i] == d_value)
			{
				claw.push_back(d_value);
				removeIntVec(dices_for_pick, d_value);
				amount--;
			}
		}
	}
}

inline int Robot::get_ChanceToContinue(int points)
{
	int chance;
	if (points < 200) { chance = 100; }
	else if (points > 1800) { chance = 0; }
	else { chance = int(round(-2.5 * sqrt((points - 200)) + 100)); }
	return chance;
}

inline IntVector Robot::get_CombRowDice()
{
	vector<int> out;
	for (unsigned int i = 0; i < dices_for_pick.size(); i++)
	{
		int d_val = dices_for_pick[i];
		if (countIntVec(dices_for_pick, d_val) >= 3 &&
			countIntVec(out, d_val) == 0)
		{
			out.push_back(d_val);
		}
	}
	return out;
}

inline IntVector Robot::get_DiceChoose()
{
	claw.clear();
	dices_for_pick = copyVec(table_dices);
	unsigned int singles_count = countIntVec(dices_for_pick, 1) +
		                         countIntVec(dices_for_pick, 5);

	if (has_RangeCombo(dices_for_pick))
	{
		take_RangeCombo();
		if (has_SingleCombo(dices_for_pick))
		{
			take_SingleCombo();
		}
	}
	else if (has_RowCombo(dices_for_pick))
	{
		take_RowCombo();
		if (has_SingleCombo(dices_for_pick) && get_BoolByChance(25))
		{
			take_SingleCombo();
		}
	}
	else if (singles_count == dices_for_pick.size())
	{
		take_SingleCombo();
	}
	else if (dices_for_pick.size() > 3)
	{
		if (get_BoolByChance(75) || singles_count == 1)
		{
			take_SingleCombo(1);
		}
		else
		{
			take_SingleCombo();
		}
	}
	else if (dices_for_pick.size() <= 3)
	{
		if (singles_count > 1 && get_BoolByChance(75))
		{
			take_SingleCombo();
		}
		else
		{
			take_SingleCombo(1);
		}
	}
	int possible_points = get_DicesInfo(dices_for_pick).score;
	if (has_SingleCombo(dices_for_pick) &&
		possible_points + score_turn + score_total >= high_bar)
	{
		take_SingleCombo();
	}
	return claw;
}

inline ActionChoice Robot::get_ActChoice()
{
	IntVector table_dices_except_claw = copyVec(table_dices);
	excludeIntVec(table_dices_except_claw, claw);
	int n_dices = table_dices_except_claw.size();
	int chance_to_continue = get_ChanceToContinue(score_turn);

	if (score_total + score_turn + score_pick >= high_bar) { chance_to_continue = 0; }
	else if (n_dices == 0) { chance_to_continue += 90; }
	else if (n_dices == 5) { chance_to_continue += 25; }
	else if (n_dices < 3)  { chance_to_continue -= 30; }


	ActionChoice out;
	bool choice = get_BoolByChance(chance_to_continue);
	if (choice) { out = ActionChoice::e_TurnContinue; }
	else { out =  ActionChoice::e_TurnEnd; }
	return out;
}