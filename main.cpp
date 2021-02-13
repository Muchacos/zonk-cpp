#include "Classes.h"
#include "tools.h"
#include "data.h"

//Создание объектов экрана и игроков
Screen scr;
Human p1(HUMAN_DEF_NAME, true);
Robot p2(ROBOT_NAME, false, HIGH_BAR);

//Инициализация костей и флагов игры
IntVector Table_Dices;
bool Is_P1_Active = true;
bool Game_Flag = true;

void init_scr()
{//Изменяет стандартное окно консоли под игровое
	scr.ConstructConsole(SW, SH, GLYPH_SIZE_X, GLYPH_SIZE_Y);
	SetConsoleTitle(CONSOLE_TITLE);

	scr.set_CursorVis(false);
	scr.set_Colors();
}

void end_scr()
{//Сброс окна консоли
	scr.reset_Colors();
	scr.ConstructConsole(120, 30, 7, 16);
	scr.move_Cursor(0, 0);
	scr.set_CursorVis(true);
}

bool check_win()
{//Возвращает true, если кто-то из игроков набрал нужное для победы кол-во очков
	return (Is_P1_Active && p1.score_total >= HIGH_BAR ||
		    !Is_P1_Active && p2.score_total >= HIGH_BAR);
}

void restore_dices()
{//Устанавливает кол-во костей на столе в 6
	Table_Dices = vector<int>(6, 0);
}

void switch_player()
{//Делает текущего игрока неактивным, а другого - активным
	Is_P1_Active = !Is_P1_Active;
	scr.anim_HlPlayer(Is_P1_Active, p1.name, p2.name);
}

void action()
{//Отвечает за одно игровое действие
	int n_dices = Table_Dices.size();
	Table_Dices = get_RandDices(n_dices);
	p1.set_TableDices(Table_Dices);
	p2.set_TableDices(Table_Dices);
	scr.anim_Diceroll(n_dices);
	scr.display_Dices(Table_Dices);

	if (!has_AnyCombo(Table_Dices))
	{
		scr.display_Msg("a_nocombos");
		scr.clear_Zone(ZONE_DICES);
		if (Is_P1_Active) { p1.clear_ScoreTurn(scr); }
		else { p2.clear_ScoreTurn(scr); }
		restore_dices();
		switch_player();
		return;
	}

	IntVector pick_good_dices;
	int pick_score = 0;
	ActionChoice act_choice = ActionChoice::e_TurnCancel;
	while (act_choice == ActionChoice::e_TurnCancel)
	{
		IntVector pick;
		if (Is_P1_Active)
		{
			p1.clear_ScorePick(scr);
			scr.display_Msg("a_getpick", L"", false);
			pick = p1.get_DiceChoose(scr);
		}
		else
		{
			pick = p2.get_DiceChoose();
		}
		
		pick_good_dices = copyVec(pick);
		DicesInfo pick_info = get_DicesInfo(pick);
		IntVector pick_bad_dices = pick_info.bad_dices;
		excludeIntVec(pick_good_dices, pick_bad_dices);
		pick_score = pick_info.score;


		if (pick_score == 0)
		{
			scr.effect_HlDices(pick, COL_RED);
			scr.display_Msg("a_badallpick", p1.name);
			scr.effect_UnHlDices();
			continue;
		}
		
		if (Is_P1_Active) { p1.add_ScorePick(scr, pick_score); }
		else { p2.add_ScorePick(scr, pick_score); }
		scr.effect_HlDices(pick_good_dices);

		if (pick_bad_dices.size() != 0)
		{
			scr.effect_HlDices(pick_bad_dices, COL_RED);
			scr.display_Msg("a_badpick");
		}

		if (Is_P1_Active)
		{
			scr.display_Msg("a_actchoice", L"", false, 2.0f);
			act_choice = p1.get_ActChoice(scr);
		}
		else
		{
			act_choice = p2.get_ActChoice();
			if (act_choice == ActionChoice::e_TurnEnd)
			{
				scr.display_Msg("a_robturnE", L"", true, 1.3f);
			}
			else
			{
				scr.display_Msg("a_robturnC", L"", true, 1.3f);
			}
		}
		scr.effect_UnHlDices();
	}

	excludeIntVec(Table_Dices, pick_good_dices);
	scr.effect_HlDices(pick_good_dices, COL_BLACK);
	if (Is_P1_Active)
	{
		p1.add_ScoreTurn(scr);
		scr.display_Msg("a_h_scrpick", std::to_wstring(pick_score), true, 1.3f);
	}

	else
	{
		p2.add_ScoreTurn(scr);
		scr.display_Msg("a_r_scrpick", std::to_wstring(pick_score), true, 1.3f);
	}

	if (act_choice == ActionChoice::e_TurnEnd)
	{
		if (Is_P1_Active)
		{ 
			p1.add_ScoreTotal(scr);
			scr.display_Msg("a_h_scrtotl",std::to_wstring(p1.score_total),
				            true, 1.3f);
		}
		else 
		{ 
			p2.add_ScoreTotal(scr); 
			scr.display_Msg("a_r_scrtotl", std::to_wstring(p2.score_total),
				true, 1.3f);
		}

		if (check_win())
		{
			Game_Flag = false;
			restore_dices();
			return;
		}
		restore_dices();
		switch_player();
	}
	else if (Table_Dices.size() == 0)
	{
		restore_dices();
	}
}

wstring get_name()
{//Пытается получить от игрока его имя и возвращает либо его, либо кейкод ошибки при неудаче
	wstring name = scr.input_Str();
	int len = name.length();
	if (len == 0) { return L"#None"; }
	else if (len > 6) { return L"#Long"; }
	else if (len < 3) { return L"#Short"; }
	return name;
}

void run_intro()
{//Проигрывает интро - отрисовку интерфейса, приветствие и ввод имени
	delay(0.7f);
	scr.anim_Start();
	delay(2.0f);
	scr.display_Msg("0_welcome_seq_1");
	scr.display_Msg("0_welcome_seq_2");
	scr.display_Msg("0_welcome_seq_3");
	scr.display_Msg("0_welcome_seq_4");
	scr.display_Msg("0_welcome_seq_5");
	scr.display_Msg("0_welcome_seq_6");

	scr.display_Msg("0_playername", L"", false);
	short none_count = 0;
	short long_count = 0;
	short short_count = 0;
	for (int i = 0; i < 4; i++)
	{
		wstring name = get_name();
		if (name[0] != L'#')
		{
			p1.name = name;
			scr.add_Players(p1.name, L"");
			if (none_count + long_count + short_count == 0)
			{
				scr.display_Msg("0_nicetomeet", name);
			}
			else
			{
				scr.display_Msg("0_oktomeet", name);
			}
			break;

		}
		else if (i == 3)
		{
			continue;
		}
		else if (name == L"#None")
		{
			switch (none_count++)
			{
			case 0:
			{
				scr.display_Msg("0_nanname1_1");
				scr.display_Msg("0_nanname1_2", L"", false);
				break;
			}
			case 1:
			{
				scr.display_Msg("0_nanname2_1", L"", false);
				break;
			}
			case 2:
			{
				scr.display_Msg("0_nanname3_1");
				scr.display_Msg("0_nanname3_2", L"", false);
				break;
			}
			default:
				break;
			}
		}
		else if (name == L"#Long")
		{
			switch (long_count++)
			{
			case 0:
			{
				scr.display_Msg("0_longname1_1");
				scr.display_Msg("0_longname1_2", L"", false);
				break;
			}
			case 1:
			{
				scr.display_Msg("0_longname2_1");
				scr.display_Msg("0_longname2_2", L"", false);
				break;
			}
			case 2:
			{
				scr.display_Msg("0_longname3_1", L"", false);
				break;
			}
			default:
				break;
			}
		}
		else if (name == L"#Short")
		{
			switch (short_count++)
			{
			case 0:
			{
				scr.display_Msg("0_shortname1", L"", false);
				break;
			}
			case 1:
			{
				scr.display_Msg("0_shortname2", L"", false);
				break;
			}
			case 2:
			{
				scr.display_Msg("0_shortname3", L"", false);
				break;
			}
			default:
				break;
			}
		}
	}
	if (none_count + long_count + short_count == 3)
	{
		scr.display_Msg("0_enougth");
		scr.add_Players(p1.name, L"");
		scr.display_Msg("0_stdname");
	}

	scr.display_Msg("0_rules1");
	scr.display_Msg("0_rules2", p1.name);
}

int main()
{
	setlocale(LC_ALL, "Rus");

	//Инициализация
	init_scr();
	init_random();

	//Интро
	run_intro();
	restore_dices();

	//Приветствие в игре
	scr.add_Players(p1.name, p2.name);
	scr.display_Msg("1_enemy_seq_1");
	scr.display_Msg("1_enemy_seq_2");
	scr.display_Msg("1_enemy_seq_3");
	scr.display_Msg("1_enemy_seq_4");

	scr.add_HighBar(HIGH_BAR);
	scr.display_Msg("1_begin_seq_1", std::to_wstring(HIGH_BAR));
	scr.display_Msg("1_begin_seq_2", p1.name);
	scr.display_Msg("1_begin_seq_3");
	scr.display_Msg("1_begin_seq_4");

	scr.effect_HlPlayers(Is_P1_Active, p1.name, p2.name);
	scr.clear_Zone(ZONE_MSG);
	
	//Основной цикл игры
	while (Game_Flag)
	{
		action();
		Game_Flag = !check_win();
	}
	scr.clear_Zone(ZONE_DICES);

	//Вывод сообщения о победе/поражении
	if (Is_P1_Active)
	{
		scr.display_Msg("1_win_seq_1", p1.name);
		scr.display_Msg("1_win_seq_2");
	}
	else
	{
		scr.display_Msg("1_loose_seq_1");
		scr.display_Msg("1_loose_seq_2", p1.name);
	}
	
	//Завершение работы
	scr.anim_Ending();
	end_scr();

	return 0;
}
