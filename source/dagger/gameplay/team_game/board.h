#pragma once

#include "core/core.h"

enum struct Occupacy
{
	Empty = 0,
	Friend,
	Foe,
	Neutral
};

struct Board
{
	UInt32 width;
	UInt32 height;
	Sequence<Sequence<Occupacy>> space;

	Board(UInt32 w_, UInt32 h_)
		: width{ w_ }
		, height{ h_ }
		, space{}
	{
		space.resize(w_);
		for (UInt32 i = 0; i < w_; i++)
		{
			space[i].resize(h_);
			for (UInt32 j = 0; j < h_; j++)
			{
				space[i][j] = Occupacy::Empty;
			}
		}
	}
};
