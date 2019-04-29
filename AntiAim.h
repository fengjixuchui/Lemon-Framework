#pragma once
#include "Cheat.h"

class CAntiAim : public Singleton<CAntiAim>
{
public:
	void OnCreateMove(CUserCmd* cmd, bool &bSendpacket);
private:
	void DoAntiAim(CUserCmd* cmd, bool &bSendpacket);
};