#pragma once

#include <type_traits>
#include <vector>
#include <queue>
#include <list>

#include "Constants.h"
#include "Graphics.h"

#include "glm/gtc/matrix_transform.hpp"

class BoardInput;
class System;
class ICommand;
class Cell;
class Piece;
struct Transform;
class GameEntity;
class Texture;
class OrthoCamera;
class MatchChecker;
class MatchFactory;
class Match;
class CascadeService;

class Game
{
public:
	Game();
	~Game();
	Game(const Game &) = delete;
	void operator=(const Game &) = delete;

	void Update(float deltaTime);
	void Render(Graphics* graphics);

	void Initialise();
	Cell* FindNeighbour(const Cell&, Constants::Direction dir);
	void SwapPieces(Cell* cellA, Cell* cellB);
	//void UpdateSpawnerCells();
	typedef std::list<Cell*> Cells;
	typedef std::list<Piece*> Pieces;
	void DeletePieces(Pieces pieces);
	bool IsBoardIdle();
	bool IsColumnIdle(const int column, const int row);
private:
	//typedef std::vector<GameEntity*> GameEntities;
	typedef std::vector<Match*> Matches;
	typedef std::list<ICommand*> CommandList;
	typedef std::queue<ICommand*> CommandQueue;
	Cell* GetCellAt(const int row, const int column);

	void CreateCells(int cellCount, int spawnerCellsCount);
	Piece* GetRandomPiece(Cell* spawner);
	void InitPiece(Piece* piece, Transform* transform);
	void UpdateCells();
	void RenderBackground(Graphics* graphics);
	void RenderCells(Graphics* graphics);
	void RenderPieces(Graphics* graphics);

	void DoMatchDetection();
	void DoCascade();
	Piece* GetNeighbourPieceInDirection(Cell* recieverCell, Cell** donerCell, const Constants::Direction dir);
	void FillableList(std::list<Cell*>& fillableList);

	template <typename Iter>
	void ApplyModelTransformation(Iter it, Iter end, std::vector<glm::mat4>& modelMatrices);

	Cells m_Cells;
	Cells m_SpawnerCells;
	Pieces m_BluePieces;
	Pieces m_RedPieces;
	Pieces m_YellowPieces;
	Pieces m_GreenPieces;
	Pieces m_PurplePieces;

	OrthoCamera* m_Camera;
	BoardInput* m_BoardInput;
	MatchFactory* m_MatchFactory;
	MatchChecker* m_MatchChecker;
	CascadeService* m_CascadeService;

	//Texture* m_BGtexture;
	//Texture* m_Celltexture;

	Texture* m_BlueGemtexture;
	Texture* m_RedGemtexture;
	Texture* m_YellowGemtexture;
	Texture* m_GreenGemtexture;
	Texture* m_PurpleGemtexture;

	CommandQueue m_PendingCommands;
	CommandQueue m_OverFlowCommands;
	CommandList m_ActiveCommands;

	bool m_isInitialised;
};

template <typename Iter>
void Game::ApplyModelTransformation(Iter it, Iter end, std::vector<glm::mat4>& modelMatrices)
{
//	bool isGameEntity = std::is_base_of<GameEntity, *it>::value;
	//ASSERT(isGameEntity);
	//bool isExpectedContainer = std::equal<std::vector, C>::value && std::equal<std::list, C>::value;
	//ASSERT(isExpectedContainer);
	int size = std::distance(it, end);
	//modelMatrices.reserve(size);
	for (; it != end; ++it) 
	{ 
		if ((*it)->GetTransform()->position.y > 580)
		{
			continue;
		}

		glm::mat4 model = glm::mat4(1.0f);
		//GameEntity* gameEntitiy = static_cast<GameEntity*>(*it);

		model = glm::translate(model, (*it)->GetTransform()->position);

		model = glm::scale(model, (*it)->GetTransform()->scale);


		model = glm::rotate(model, (*it)->GetTransform()->rotationAngle, (*it)->GetTransform()->rotationAxis);

		modelMatrices.push_back(model);
	}
	
}