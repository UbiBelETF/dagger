#pragma once

#include <limits>

namespace ImGui
{
	void PlotVar(
		const char* label_, 
		float value_, 
		float scaleMin_ = std::numeric_limits<float>::min(), 
		float scaleMax_ = std::numeric_limits<float>::max(), 
		size_t bufferSize_ = 120);

	void PlotVarFlushOldEntries();
}