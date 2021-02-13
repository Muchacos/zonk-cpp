#pragma once
#include <random>
#include <algorithm>
#include <ctime>
#include <vector>

using std::vector;
using std::wstring;

typedef vector<int> IntVector;	//Вектор с целочисленными элементами
typedef vector<wstring> WstrVector;	//Вектор с элементами-строками

unsigned int seed = (unsigned int)std::time(0);
std::default_random_engine rng(seed);

enum ScoreType
{//Тип очков
	e_ScoreTotal,
	e_ScoreTurn,
	e_ScorePick,
};

enum class ActionChoice
{//Действие игрока (продолжить ход, закончить ход, выбрать другие кости)
	e_TurnCancel,
	e_TurnContinue,
	e_TurnEnd,
	e_Undefined,
};

struct DicesInfo
{//Информация по костям
	int score;	//Кол-во очков, которое можно получить за кости
	IntVector bad_dices;	//Кости, не приносящие очки
};

void init_random()
{//Устанавливает сид
	srand(seed);
}

int randInt(int roof)
{//Возвращает случайное число от 0 до roof
	return rand() % roof;
}

int randIntRange(int floor, int roof)
{//Возвращает случайное число от floor до roof
	return rand() % (roof - floor + 1) + floor;
}

void shuffleVec(IntVector &vec)
{//Перемешивает вектор
	std::shuffle(vec.begin(), vec.end(), rng);
}

void nullVec(IntVector &vec)
{//Обнуляет вектор
	for (unsigned int i = 0; i < vec.size(); i++)
	{
		vec[i] = 0;
	}
}

wstring replaceInsert(wstring word_to_replace, wstring word_to_insert)
{//Возвращает строку, заменяя в ней подстроку
	wstring out = word_to_insert;
	for (unsigned int i = 2; i < word_to_replace.size(); i++)
	{
		out += word_to_replace[i];
	}
	return out;
}

int idxIntVec(IntVector& vec, int elem)
{//Возвращает id первого вхождения элемента в целочисленный вектор
	for (unsigned int i = 0; i < vec.size(); i++)
	{
		if (elem == vec[i])
		{
			return i;
		}
	}
	return -1;
}

int countIntVec(IntVector& vec, int elem)
{//Подсчитывает кол-во вхождений элемента в целочисленный вектор
	int k = 0;
	for (unsigned int i = 0; i < vec.size(); i++)
	{
		if (vec[i] == elem)
		{
			k++;
		}
	}
	return k;
}

int countWstrVec(WstrVector& vec, wstring elem)
{//Подсчитывает кол-во вхождений строки в вектор строк
	int k = 0;
	for (unsigned int i = 0; i < vec.size(); i++)
	{
		if (vec[i] == elem)
		{
			k++;
		}
	}
	return k;
}

void delay(float seconds)
{//Выходит из функции после задержки в секундах
	Sleep(int(seconds * 1000));
}

WstrVector splitStrVec(wstring str, wchar_t sep = L' ')
{//Разделяет строку по разделителю sep на вектор строк
	WstrVector out;
	wstring tmp = L"";
	for (unsigned int i = 0; i <= str.length(); i++)
	{
		if (str[i] == sep || i == str.length())
		{
			out.push_back(tmp);
			tmp = L"";
		}
		else
		{
			tmp += str[i];
		}
	}
	return out;
}

void extendIntVec(IntVector& target, IntVector& data)
{//Вставляет в конец целочисленного вектора другой целочисленный вектор
	target.insert(target.end(), data.begin(), data.end());
}

void excludeIntVec(IntVector& target, IntVector& values)
{//Удаляет из целочисленного вектора все элементы из другого целочисленного вектора
	for (unsigned int i = 0; i < values.size(); i++)
	{
		target.erase(target.begin() + idxIntVec(target, values[i]));
	}
}

bool isDigitWStr(wstring str)
{//Возвращает true, если переданная строка - число
	if (str.length() > 0)
	{
		for (unsigned int i = 0; i < str.size(); i++)
		{
			if (!iswdigit(str[i]))
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

void removeIntVec(IntVector& target, int value)
{//Удаляет элемент из целочисленного вектора
	int val_idx = idxIntVec(target, value);
	target.erase(target.begin() + val_idx);
}

IntVector copyVec(IntVector& target)
{//Возвращает копию переданного целочисленного вектора
	vector<int> new_vec;
	std::copy(target.begin(), target.end(), std::back_inserter(new_vec));
	return new_vec;
}

int wcharToInt(wchar_t ch)
{//Преобразует wchar_t в число (возвращает -1 при неудаче)
	switch (ch)
	{
	case L'0':
		return 0;
	case L'1':
		return 1;
	case L'2':
		return 2;
	case L'3':
		return 3;
	case L'4':
		return 4;
	case L'5':
		return 5;
	case L'6':
		return 6;
	case L'7':
		return 7;
	case L'8':
		return 8;
	case L'9':
		return 9;
	default:
		return -1;
	}
}

bool get_BoolByChance(int chance)
{//Возвращает true с шансом chance (от 0 до 100)
	if (chance >= 100) { return true; }
	else if (chance <= 0) { return false; }

	vector<bool> a(true, chance);
	for (int i = chance; i <= 100; i++)
	{
		a.push_back(false);
	}
	std::shuffle(a.begin(), a.end(), rng);

	return a[0];
}

bool has_RowCombo(IntVector& dices)
{//Возвращает true, если среди костей есть три или больше одинаковых кости
	for (unsigned int i = 0; i < dices.size(); i++)
	{
		if (countIntVec(dices, dices[i]) >= 3) { return true; }
	}
	return false;
}

bool has_RangeCombo(IntVector& dices)
{//Возвращает true, если среди костей есть все кости от 1 до 5
	for (int d_val = 1; d_val <= 5; d_val++)
	{
		if (countIntVec(dices, d_val) == 0) { return false; }
	}
	return true;
}

bool has_SingleCombo(IntVector& dices)
{//Возвращает true, если среди костей есть кость 1 или 5
	return countIntVec(dices, 1) != 0 || countIntVec(dices, 5) != 0;
}

bool has_AnyCombo(IntVector& dices)
{//Возвращает true, если среди костей есть хотя бы одна комбинация
	return has_SingleCombo(dices) || has_RowCombo(dices);
}

IntVector get_RandDices(int amount)
{//Возвращает целочисленный вектор со случайными значениями от 1 до 6 в кол-ве amount
	IntVector out;
	for (int i = 0; i < amount; i++)
	{
		out.push_back(randIntRange(1, 6));
	}
	return out;
}

DicesInfo get_DicesInfo(IntVector& dices)
{//Возвращает структуру с потенциальным кол-вом очков и костями, не приносящими очки
	int score = 0;
	IntVector loc_dices = copyVec(dices);

	if (has_RangeCombo(loc_dices))
	{
		if (countIntVec(loc_dices, 6) != 0)
		{
			score += 1500;
			loc_dices.clear();
		}
		else
		{
			score += 750;
			for (int d_val = 1; d_val <= 5; d_val++)
			{
				removeIntVec(loc_dices, d_val);
			}
		}
	}
	if (has_RowCombo(loc_dices))
	{
		IntVector counted_dices;
		IntVector loc_loc_dices = copyVec(loc_dices);

		for (unsigned int i = 0; i < loc_loc_dices.size(); i++)
		{
			int d_val = loc_loc_dices[i];
			if (countIntVec(counted_dices, d_val) != 0)
			{
				continue;
			}
			counted_dices.push_back(d_val);

			int loc_score = 0;
			int row_len = countIntVec(loc_dices, d_val);

			if (row_len >= 3)
			{
				if (d_val == 1)
				{
					loc_score += 1000;
				}
				else
				{
					loc_score += d_val * 100;
				}
				loc_score *= (row_len - 2);
				score += loc_score;
				for (int i = 0; i < row_len; i++)
				{
					removeIntVec(loc_dices, d_val);
				}
			}
		}
	}

	if (has_SingleCombo(loc_dices))
	{
		IntVector loc_loc_dices = copyVec(loc_dices);
		for (unsigned int i = 0; i < loc_loc_dices.size(); i++)
		{
			int d_val = loc_loc_dices[i];
			if (d_val == 1)
			{
				score += 100;
				removeIntVec(loc_dices, d_val);
			}
			else if (d_val == 5)
			{
				score += 50;
				removeIntVec(loc_dices, d_val);
			}
		}
	}
	DicesInfo out;
	out.score = score;
	out.bad_dices = loc_dices;
	return out;
}