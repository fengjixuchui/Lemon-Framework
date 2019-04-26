#pragma once
#define NOMINMAX

/* Standard Window Stuff */
#include <Windows.h>
#include <iostream>
#include <d3d9.h>

/* Project Files */
#include "Singleton.hpp"
#include "Valve_SDK/sdk.hpp"
#include "Valve_SDK/csgostructs.hpp"
#include "Helpers/input.hpp"
#include "Helpers/math.hpp"
#include "Helpers/utils.hpp"
#include "Helpers/vfunc_hook.hpp"
#include "Hooks.h"
#include "Render.h"
#include "Font.h"
#include "Features.h"
#include "Settings.h"
#include "GUI.h"

#include "Valve_SDK/SDK.h"

template<class T, class U>
static T clamp(T in, U low, U high) {
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}