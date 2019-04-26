#pragma once

#include "Cheat.h"

class CChams : public Singleton<CChams>
{
	friend class Singleton<CChams>;

	CChams();
	~CChams();
public:
	void DME(IMatRenderContext* ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t* pCustomBoneToWorld);
private:
	void OverrideMaterial(bool IgnoreZ, bool flat, bool wireframe, bool glass, bool pulse, const Color& rgba);

	IMaterial* materialRegular = nullptr;
	IMaterial* materialRegularIgnoreZ = nullptr;
	IMaterial* materialFlatIgnoreZ = nullptr;
	IMaterial* materialFlat = nullptr;
	IMaterial* materialPulse = nullptr;
	IMaterial* materialPulseIgnoreZ = nullptr;
};