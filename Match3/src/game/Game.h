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

	void Initialise(Graphics* graphics);
	Cell* FindNeighbour(const Cell&, Constants::Direction dir);
	void SwapPieces(Cell* cellA, Cell* cellB);
	//void UpdateSpawnerCells();
	typedef std::list<Cell*> Cells;
	typedef std::list<Piece*> Pieces;
	void DeletePieces(Pieces pieces);

private:
	//typedef std::vector<GameEntity*> GameEntities;
	typedef std::vector<Match*> Matches;
	typedef std::list<ICommand*> CommandList;
	typedef std::queue<ICommand*> CommandQueue;
	//Cell* cells[Constants::ROW_LIMIT * Constants::COLUMN_LIMIT];
	//Constants::Direction r = Constants::Direction::LEFT;

	//Cell* GetCellAt(const int index);
	//Cell* GetSpawnerAt(const int index);
	Cell* GetCellAt(const int row, const int column);

	void CreateCells(int cellCount, int spawnerCellsCount);
	Piece* GetRandomPiece(Cell* spawner);
	void InitPiece(Piece* piece, Transform* transform);
	void UpdateCells();
	void RenderBackground(Graphics* graphics);
	void RenderCells(Graphics* graphics);
	void RenderPieces(Graphics* graphics);
	//void ApplyModelTransformation(GameEntity* gameEntity);

	void DoMatchDetection();
	void DoCascade();
	Piece* GetNeighbourPieceInDirection(Cell* recieverCell, Cell** donerCell, const Constants::Direction dir);
	void FillableList(std::list<Cell*>& fillableList);

	//template <typename T, template <typename...> class Cont>
	//template <typename C, typename T>
	//glm::mat4* GetModelMatrices(C& container, std::vector<T*>& gameEntities);
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
	Texture* m_Celltexture;

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
	int i = 0;
	//glm::mat4* modelMatrices2 = new glm::mat4[size];
	
	//float scaleD = static_cast<float>(rand() % 3);
	for (; it != end; ++it) 
	{ 
		glm::mat4 model = glm::mat4(1.0f);
		//GameEntity* gameEntitiy = static_cast<GameEntity*>(*it);

		model = glm::translate(model, (*it)->GetTransform()->position);

		model = glm::scale(model, (*it)->GetTransform()->scale);

		//if(i == 2)
		///	model = glm::scale(model, glm::vec3(1.0f));
		//else
			//model = glm::scale(model, glm::vec3(5.0f));

		model = glm::rotate(model, (*it)->GetTransform()->rotationAngle, (*it)->GetTransform()->rotationAxis);

		//modelMatrices2[i] = model;
		modelMatrices.push_back(model);
	//	i++;
	}
	/*for (unsigned int i = 0; i < gameEntities.size(); i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		GameEntity* gameEntitiy = static_cast<GameEntity*>(gameEntities[i]);

		model = glm::translate(model, gameEntitiy->GetTransform()->position);

		model = glm::scale(model, gameEntitiy->GetTransform()->scale);

		model = glm::rotate(model, gameEntitiy->GetTransform()->rotationAngle, gameEntitiy->GetTransform()->rotationAxis);

		modelMatrices[i] = model;
	}*/
//	return &modelMatrices2[0];
}