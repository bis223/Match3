#pragma once

class Constants
{
	public:
		Constants() = delete;

		enum Direction
		{
			LEFT = 0,
			RIGHT = 1,
			UP = 2,
			DOWN = 3
		};

		static const int ROW_LIMIT		= 8;
		static const int COLUMN_LIMIT	= 8;
		static const int DROP_GRAVITY	= 2500;
		static const int CELL_SIZE		= 5;
		static const int SWAP_THRESHOLD = 15;
		//typedef std::vector<Cell*> Cells;
};

