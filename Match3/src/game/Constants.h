#pragma once
#include <string>

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

		static constexpr const char* BLUE_GEM	= "Blue";
		static constexpr const char* RED_GEM	= "Red";
		static constexpr const char* GREEN_GEM	= "Green";
		static constexpr const char* YELLOW_GEM = "Yellow";
		static constexpr const char* PURPLE_GEM = "Purple";
		
#pragma region AssetRegistry

		static constexpr const char* BLUE_GEM_PATH	 = "res/textures/Blue.png";
		static constexpr const char* RED_GEM_PATH	 = "res/textures/Red.png";
		static constexpr const char* GREEN_GEM_PATH  = "res/textures/Green.png";
		static constexpr const char* YELLOW_GEM_PATH = "res/textures/Yellow.png";
		static constexpr const char* PURPLE_GEM_PATH = "res/textures/Purple.png";

#pragma endregion

#pragma region GameStates

		static constexpr const char* LOADING_STATE  = "LoadingState";
		static constexpr const char* GAMEPLAY_STATE = "GamePlayState";
		static constexpr const char* GAMEOVER_STATE = "GameOverState";

#pragma endregion

};
