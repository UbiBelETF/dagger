#pragma once

#include "core/core.h"

enum struct Occupacy
{
	Empty = 0,
	Neutral = 1,
	Friend = 2,
	Foe = 3,
};

enum struct PeepState
{
	Dead,
	Active,
	Tired,
};

struct Peep
{
	Entity entity;
	Occupacy side;

	void SetState(PeepState state_);
};

struct PeepGroup
{
	String name;
	Occupacy side;
	Sequence<Peep> peeps;
};

struct PeepArmy
{
	String name;
	Occupacy side;
	Sequence<PeepGroup> groups;
};

struct Board
{
	UInt32 width;
	UInt32 height;

	Sequence<Sequence<UInt32>> space;
	Sequence<PeepArmy> armies;

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
				space[i][j] = 0;
			}
		}
	}
};
