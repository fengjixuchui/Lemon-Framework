#pragma once
#include "Cheat.h"

class CMisc : public Singleton<CMisc>
{
public:
	void ChangeName();
	void ChangeClantag();

	void Clantag();
};