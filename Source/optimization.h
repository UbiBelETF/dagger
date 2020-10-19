#pragma once

#ifndef USE_DEDICATED_GPU
#define USE_DEDICATED_GPU
extern "C"
{
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 0x00000001;
}
#endif // USE_DEDICATED_GPU
