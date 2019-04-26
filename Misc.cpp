#include "Misc.h"

void CMisc::ChangeName()
{
	U::SetName(config.misc.customName.c_str());
}

void CMisc::ChangeClantag()
{
	U::SetClantag(config.misc.customClantag.c_str());
}