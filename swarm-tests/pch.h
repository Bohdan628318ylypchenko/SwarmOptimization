#pragma once

#include "pch_swarm_core.h"

#include "CppUnitTest.h"

#ifdef _DEBUG
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#define _CRTDBG_MAP_ALLOC
#include "crtdbg.h"
#include "memory_leak.h"
#endif

#include <string>
#include <cstring>
#include <algorithm>
#include <array>
#include <utility>