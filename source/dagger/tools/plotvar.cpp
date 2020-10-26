
#include "imgui.h"
#include <vector>
#include <map>
#include <algorithm>

namespace ImGui
{
	struct PlotVarData
	{
		ImGuiID        m_ID;
		std::vector<float>  m_Data;
		int            m_InsertIndex;
		int            m_LastFrame;

		PlotVarData() : m_ID(0), m_InsertIndex(0), m_LastFrame(-1) {}
	};

	typedef std::map<ImGuiID, PlotVarData> PlotVarsMap;
	inline static PlotVarsMap	g_PlotVarsMap;

	void PlotVar(const char* label_, float value_, float scaleMin_, float scaleMax_, size_t bufferSize_)
	{
		assert(label_);

		if (bufferSize_ == 0)
			bufferSize_ = 120;

		ImGui::PushID(label_);
		ImGuiID id = ImGui::GetID("");

		// Lookup O(log N)
		PlotVarData& pvd = g_PlotVarsMap[id];

		// Setup
		if (pvd.m_Data.capacity() != bufferSize_)
		{
			pvd.m_Data.resize(bufferSize_);
			memset(&pvd.m_Data[0], 0, sizeof(float) * bufferSize_);
			pvd.m_InsertIndex = 0;
			pvd.m_LastFrame = -1;
		}

		// Insert (avoid unnecessary modulo operator)
		if (pvd.m_InsertIndex == bufferSize_)
			pvd.m_InsertIndex = 0;
		int display_idx = pvd.m_InsertIndex;
		if (value_ != FLT_MAX)
			pvd.m_Data[pvd.m_InsertIndex++] = value_;

		// Draw
		int current_frame = ImGui::GetFrameCount();
		if (pvd.m_LastFrame != current_frame)
		{
			ImGui::PlotLines("##plot", &pvd.m_Data[0], bufferSize_, pvd.m_InsertIndex, NULL, scaleMin_, scaleMax_, ImVec2(0, 40));
			ImGui::SameLine();
			ImGui::Text("%s\n%-3.4f", label_, pvd.m_Data[display_idx]);	// Display last value in buffer
			pvd.m_LastFrame = current_frame;
		}

		ImGui::PopID();
	}

	void PlotVarFlushOldEntries()
	{
		int current_frame = ImGui::GetFrameCount();
		for (PlotVarsMap::iterator it = g_PlotVarsMap.begin(); it != g_PlotVarsMap.end(); )
		{
			PlotVarData& pvd = it->second;
			if (pvd.m_LastFrame < current_frame - std::max(400, (int)pvd.m_Data.size()))
				it = g_PlotVarsMap.erase(it);
			else
				++it;
		}
	}
}