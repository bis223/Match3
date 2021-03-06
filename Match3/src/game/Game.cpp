#include "OrthoCamera.h"
#include "System.h"
#include <time.h>
#include <iostream>
#include <unordered_set>
#include <string>
#include <sstream>

#include "bis/vector.h"

#include "bis/list.h"

#include "Game.h"
#include "Cell.h"
#include "Piece.h"
#include "GameEntity.h"
#include "ICommand.h"
#include "DropPieceCommand.h"
#include "SpawnPieceCommand.h"
#include "SwapCommand.h"
#include "DestroyPiecesCommand.h"
#include "BoardInput.h"
#include "MatchChecker.h"
#include "MatchFactory.h"
#include "CascadeService.h"

#include "LoadingState.h"
#include "GamePlayState.h"
//#include "GameOverState.h"

#include "glm/glm.hpp"

Game::Game():m_Camera(0), m_BoardInput(0), m_MatchFactory(0), m_MatchChecker(0)
{
	m_isInitialised = false;
	srand((unsigned int)time(0));

	const System system = System::GetSystemInstance();
	AssetLoader* assetLoader = system.GetAssetLoader();
	Graphics* graphics = system.GetGraphics();

	system.GetStateMachine()->AddState(Constants::LOADING_STATE, new LoadingState());
	system.GetStateMachine()->AddState(Constants::GAMEPLAY_STATE, new GamePlayState());

	system.GetStateMachine()->SetState(Constants::LOADING_STATE);

	//TODO move to asset loader class
	//m_BGtexture = new Texture("res/textures/BackGround.jpg");
	//m_Celltexture = new Texture("res/textures/bis.jpg");

	assetLoader->Load<Texture>(Constants::BLUE_GEM_PATH, Constants::BLUE_GEM);
	assetLoader->Load<Texture>(Constants::RED_GEM_PATH, Constants::RED_GEM);
	assetLoader->Load<Texture>(Constants::GREEN_GEM_PATH, Constants::GREEN_GEM);
	assetLoader->Load<Texture>(Constants::YELLOW_GEM_PATH, Constants::YELLOW_GEM);
	assetLoader->Load<Texture>(Constants::PURPLE_GEM_PATH, Constants::PURPLE_GEM);

	m_Camera = new OrthoCamera(SCREEN_WIDTH, SCREEN_HEIGHT, -1.0f, 1.0f);
	m_Camera->SetProjection(*graphics);

	m_MatchFactory		= new MatchFactory(MatchFactory::MatchTemplateType::FULL_MATCHES_TEMPLATE);
	m_MatchChecker		= new MatchChecker(m_MatchFactory);
	m_CascadeService	= new CascadeService();
}


Game::~Game()
{
	for (Pieces::const_iterator cellsIt = m_BluePieces.begin(),
		end = m_BluePieces.end();
		cellsIt != end;
		++cellsIt)
	{
		delete *cellsIt;
	}
	m_BluePieces.clear();

	for (Pieces::const_iterator cellsIt = m_GreenPieces.begin(),
		end = m_GreenPieces.end();
		cellsIt != end;
		++cellsIt)
	{
		delete *cellsIt;
	}
	m_GreenPieces.clear();

	for (Pieces::const_iterator cellsIt = m_YellowPieces.begin(),
		end = m_YellowPieces.end();
		cellsIt != end;
		++cellsIt)
	{
		delete *cellsIt;
	}
	m_YellowPieces.clear();

	for (Pieces::const_iterator cellsIt = m_RedPieces.begin(),
		end = m_RedPieces.end();
		cellsIt != end;
		++cellsIt)
	{
		delete *cellsIt;
	}
	m_RedPieces.clear();

	for (Pieces::const_iterator cellsIt = m_PurplePieces.begin(),
		end = m_PurplePieces.end();
		cellsIt != end;
		++cellsIt)
	{
		delete *cellsIt;
	}
	m_PurplePieces.clear();

	for (Cells::const_iterator cellsIt = m_Cells.begin(),
		end = m_Cells.end();
		cellsIt != end;
		++cellsIt)
	{
		delete *cellsIt;
	}
	m_Cells.clear();

	for (Cells::const_iterator cellsIt = m_SpawnerCells.begin(),
		end = m_SpawnerCells.end();
		cellsIt != end;
		++cellsIt)
	{
		delete *cellsIt;
	}
	m_SpawnerCells.clear();

	delete m_Camera;
	//delete	m_BGtexture;
	//delete	m_Celltexture;
	delete	m_MatchFactory;
	delete	m_MatchChecker;
	delete	m_CascadeService;
}

void Game::Update(float deltaTime)
{
	if (!m_isInitialised)
		return;

	ICommand* command;


	while (!m_PendingCommands.empty() && (command = m_PendingCommands.front()) != nullptr)
	{
		m_PendingCommands.pop();
		if (command->IsReady())
		{
			command->Execute();
			m_ActiveCommands.push_back(command);
		}
		else
		{
			m_OverFlowCommands.push(command); // enqueues overflowing actions
		}
	}

	if (!m_ActiveCommands.empty())
	{
		CommandList::const_iterator commandIt = m_ActiveCommands.begin();

		while (commandIt != m_ActiveCommands.end())
		{
			if ((*commandIt)->IsComplete())
			{
				delete *commandIt;
				commandIt = m_ActiveCommands.erase(commandIt);
			}
			else
			{
				if ((*commandIt)->IsActive())
				{
					(*commandIt)->Update(deltaTime);       // calling upate method in a every deltatime to the active commands
				}
				++commandIt;
			}
		}
	}
	
	std::swap(m_OverFlowCommands, m_PendingCommands); // swaps pending and overflowing actions

	m_BoardInput->Update();

	if (m_CascadeService->NeedsCascading())
	{
		DoCascade();
	}
	if (m_CascadeService->NeedsMatchDetection())
	{
		DoMatchDetection();
	}
}

void Game::Render(Graphics* graphics)
{
	m_Camera->SetView(*graphics);
	
	graphics->ClearColor();

	//RenderBackground(graphics);
	//RenderCells(graphics);
	RenderPieces(graphics);
}

void Game::Initialise()
{
	
	CreateCells(Constants::ROW_LIMIT * Constants::COLUMN_LIMIT, Constants::COLUMN_LIMIT);
	
	
	for (Cells::const_reverse_iterator cellsIt = m_Cells.rbegin(),
		end = m_Cells.rend();
		cellsIt != end;
		++cellsIt)
	{
		Cell* left = FindNeighbour(**cellsIt, Constants::Direction::LEFT);
		Cell* right = FindNeighbour(**cellsIt, Constants::Direction::RIGHT);
		Cell* up = FindNeighbour(**cellsIt, Constants::Direction::UP);
		Cell* down = FindNeighbour(**cellsIt, Constants::Direction::DOWN);
		(*cellsIt)->SetNeighbours(left, right, up, down);
	}

	for (Cells::const_reverse_iterator cellsIt = m_Cells.rbegin(),
		end = m_Cells.rend();
		cellsIt != end;
		++cellsIt)
	{
		Cell* spawnerCell = GetCellAt( -1, (*cellsIt)->GetColumn());
		Piece* piece = GetRandomPiece(spawnerCell);
		(*cellsIt)->AttachPiece(piece);

		std::unordered_set<Cell*> newCell = std::unordered_set<Cell*>{ (*cellsIt) };
		std::unordered_set<Cell*> resolvedCells;
		std::vector<Match*> matches = m_MatchChecker->GetMatches(newCell, resolvedCells);
		while (!matches.empty())
		{
			for (std::vector<Match*>::const_iterator iterator = matches.begin(),
				end = matches.end();
				iterator != end;
				++iterator)
			{
				delete *iterator;
			}
			matches.clear();

			resolvedCells.clear();
			(*cellsIt)->DetachPiece();
			delete piece;

			piece = GetRandomPiece(spawnerCell);
			(*cellsIt)->AttachPiece(piece);
			matches = m_MatchChecker->GetMatches(newCell, resolvedCells);
		}

		for (std::vector<Match*>::const_iterator iterator = matches.begin(),
			end = matches.end();
			iterator != end;
			++iterator)
		{
			delete *iterator;
		}
		matches.clear();


		InitPiece(piece, spawnerCell->GetTransform());

//	std::unique_ptr<ICommand> dropCommand = std::make_unique<DropPieceCommand>(piece, *cellsIt, 2.0);

		m_CascadeService->SetFilled((*cellsIt));

		ICommand* spawnPieceCommand = new SpawnPieceCommand(piece, *cellsIt, spawnerCell, 0.1f);
		m_PendingCommands.push(spawnPieceCommand);
		
		m_CascadeService->SetDirty((*cellsIt));

	}

	m_BlueGemtexture = System::GetSystemInstance().GetAssetLoader()->GetAsset<Texture>(Constants::BLUE_GEM);
	m_RedGemtexture = System::GetSystemInstance().GetAssetLoader()->GetAsset<Texture>(Constants::RED_GEM);
	m_YellowGemtexture = System::GetSystemInstance().GetAssetLoader()->GetAsset<Texture>(Constants::YELLOW_GEM);
	m_GreenGemtexture = System::GetSystemInstance().GetAssetLoader()->GetAsset<Texture>(Constants::GREEN_GEM);
	m_PurpleGemtexture = System::GetSystemInstance().GetAssetLoader()->GetAsset<Texture>(Constants::PURPLE_GEM);

	m_BoardInput = new BoardInput(m_Cells);

	m_isInitialised = true;
}

void Game::CreateCells(int cellsCount, int spawnerCellsCount)
{

	float xOffset = 60.0f;
	float yOffset = -60.0f;

	float x = 0, y = 660;
	for (int row = -1; row < Constants::ROW_LIMIT; ++row)
	{
		y += yOffset;
		for (int column = 0; column < Constants::COLUMN_LIMIT; ++column)
		{
			x += xOffset;
			glm::vec3 pos, rotAxis, scale;
			float rotAngle;
			pos.x = x;
			pos.y = y;
			pos.z = 0;
			rotAxis = glm::vec3(0.4f, 0.6f, 0.8f);
			rotAngle = 0.0f;
			scale = glm::vec3(Constants::CELL_SIZE);

			Cell* cell = nullptr;

			std::ostringstream oss;
			if (row == -1)
			{
				//pos.y += 30.0f;
				cell = new Cell(row, column, true);
				oss << "Spawner " << row << "," << column;
				m_SpawnerCells.push_back(cell);
			}
			else
			{
				cell = new Cell(row, column, false);
				oss << "Cell " << row << "," << column;
				m_Cells.push_back(cell);
			}

			if (cell)
			{
				cell->SetName(oss.str());
				cell->SetTransform(pos, scale, rotAxis, rotAngle);
			}
		}
		x = 0;
	}
}

Piece* Game::GetRandomPiece(Cell * spawner)
{
	int count = static_cast<int>(MatchFactory::MatchColor::Count);
	MatchFactory::MatchColor colour = static_cast<MatchFactory::MatchColor>(rand() % count);
	Piece* piece = new Piece(colour);
	
	return piece;
}

void Game::InitPiece(Piece* piece, Transform* transform)
{
	piece->SetTransform(*transform);

	switch (piece->GetMatchColour())
	{
	case MatchFactory::MatchColor::BLUE:
		m_BluePieces.push_back(piece);
		break;
	case MatchFactory::MatchColor::RED:
		m_RedPieces.push_back(piece);
		break;
	case MatchFactory::MatchColor::GREEN:
		m_GreenPieces.push_back(piece);
		break;
	case MatchFactory::MatchColor::YELLOW:
		m_YellowPieces.push_back(piece);
		break;
	case MatchFactory::MatchColor::PURPLE:
		m_PurplePieces.push_back(piece);
		break;
	default:
		break;
	}

}

void Game::UpdateCells()
{	

}

void Game::RenderBackground(Graphics * graphics) 
{
	float vertices[] = {
	-5.0f, -5.0f, 0.0f, 0.0f,	//0
	 5.0f, -5.0f, 1.0f, 0.0f,	//1
	 5.0f, 5.0f,  1.0f, 1.0f,	//2
	-5.0f, 5.0f,  0.0f, 1.0f,	//3
	};

	//graphics->DrawStaticTexture(vertices, sizeof(vertices), m_BGtexture);
}

void Game::RenderCells(Graphics* graphics) 
{
	float vertices[] = {
		-5.0f, -5.0f, 0.0f, 0.0f,	//0
		 5.0f, -5.0f, 1.0f, 0.0f,	//1
		 5.0f, 5.0f,  1.0f, 1.0f,	//2
		-5.0f, 5.0f,  0.0f, 1.0f,	//3
		};


	//std::vector<glm::mat4> modelMatrices;
	

	//glm::mat4* cells = Game::GetModelMatrices(m_Cells.begin(), m_Cells.end());
	//graphics->DrawInstancedTexture(vertices, sizeof(vertices), &cells[0], m_Cells.size() * sizeof(glm::mat4), m_Celltexture, m_Cells.size());


}

void Game::RenderPieces(Graphics* graphics) 
{

	float vertices[] = {
		-5.0f, -5.0f, 0.0f, 0.0f,	//0
		 5.0f, -5.0f, 1.0f, 0.0f,	//1
		 5.0f, 5.0f,  1.0f, 1.0f,	//2
		-5.0f, 5.0f,  0.0f, 1.0f,	//3
	};


	std::vector<glm::mat4> bluePieceTransforms;
	std::vector<glm::mat4> redPieceTransforms;
	std::vector<glm::mat4> greenPieceTransforms;
	std::vector<glm::mat4> yellowPieceTransforms;
	std::vector<glm::mat4> purplePieceTransforms;

	Game::ApplyModelTransformation(m_BluePieces.begin(), m_BluePieces.end(), bluePieceTransforms); 

	if(!bluePieceTransforms.empty())
		graphics->DrawInstancedTexture(vertices, sizeof(vertices), &bluePieceTransforms[0], bluePieceTransforms.size() * sizeof(glm::mat4), m_BlueGemtexture, bluePieceTransforms.size());

	Game::ApplyModelTransformation(m_RedPieces.begin(), m_RedPieces.end(), redPieceTransforms);

	if (!redPieceTransforms.empty())
		graphics->DrawInstancedTexture(vertices, sizeof(vertices), &redPieceTransforms[0], redPieceTransforms.size() * sizeof(glm::mat4), m_RedGemtexture, redPieceTransforms.size());

	Game::ApplyModelTransformation(m_GreenPieces.begin(), m_GreenPieces.end(), greenPieceTransforms);

	if (!greenPieceTransforms.empty())
		graphics->DrawInstancedTexture(vertices, sizeof(vertices), &greenPieceTransforms[0], greenPieceTransforms.size() * sizeof(glm::mat4), m_GreenGemtexture, greenPieceTransforms.size());

	Game::ApplyModelTransformation(m_YellowPieces.begin(), m_YellowPieces.end(), yellowPieceTransforms);

	if (!yellowPieceTransforms.empty())
		graphics->DrawInstancedTexture(vertices, sizeof(vertices), &yellowPieceTransforms[0], yellowPieceTransforms.size() * sizeof(glm::mat4), m_YellowGemtexture, yellowPieceTransforms.size());

	Game::ApplyModelTransformation(m_PurplePieces.begin(), m_PurplePieces.end(), purplePieceTransforms);

	if (!purplePieceTransforms.empty())
		graphics->DrawInstancedTexture(vertices, sizeof(vertices), &purplePieceTransforms[0], purplePieceTransforms.size() * sizeof(glm::mat4), m_PurpleGemtexture, purplePieceTransforms.size());

	
}

void Game::DoMatchDetection()
{
	std::unordered_set<Cell*> dirtyCells = m_CascadeService->GetDirtyCells();
	//std::unordered_set<Cell*> emptyCells = m_CascadeService->GetEmptyCells();
	std::unordered_set<Cell*> resolvedCells;
	if (!dirtyCells.empty())
	{

		std::vector<Match*> matches = m_MatchChecker->GetMatches(dirtyCells, resolvedCells);

		for (Matches::const_iterator matchIt = matches.begin(),
			end = matches.end();
			matchIt != end;
			++matchIt)
		{
			std::list<Cell*> matchedCells = (*matchIt)->GetMatchedCells();
			std::list<Piece*> destroyingPieces;

			for (const auto& cell : matchedCells)
			{
				Piece* p = cell->GetPiece();
				if (!p)
					ASSERT(false);
				destroyingPieces.push_back(cell->GetPiece());
				cell->DetachPiece();
			}

			m_CascadeService->SetEmpty(matchedCells);
			ICommand* destroyPiecesCommand = new DestroyPiecesCommand(matchedCells, destroyingPieces, 0.0f);
			m_PendingCommands.push(destroyPiecesCommand);

		}

		for (std::vector<Match*>::const_iterator iterator = matches.begin(),
			end = matches.end();
			iterator != end;
			++iterator)
		{
			delete *iterator;
		}
		matches.clear();

		m_CascadeService->ClearAllDirtyCells();
	}
}

void Game::DoCascade()
{
	std::list<Cell*> emptyCells;
	std::unordered_set<Cell*> dirtyCells = m_CascadeService->GetDirtyCells();
	FillableList(emptyCells);// m_CascadeService->GetEmptyCells();
	Cell* donorCell = nullptr;

	if(!emptyCells.empty())
	{
		Cells::const_iterator cellIt = emptyCells.begin();

		while (cellIt != emptyCells.end())
		{

			Cell* receiverCell = *cellIt;
			donorCell = receiverCell->GetNeighbour(Constants::Direction::UP);
			Piece* newPiece = GetNeighbourPieceInDirection(receiverCell, &donorCell, Constants::Direction::UP);
			float delay = 0.125f;
			if (!donorCell->IsSpawner())
			{
				delay = 0.0f;
				m_CascadeService->SetEmpty(donorCell);
				donorCell->DetachPiece();
			}
			if (!newPiece)
				ASSERT(false);
			receiverCell->AttachPiece(newPiece);
			m_CascadeService->SetFilled(receiverCell);

			ICommand* dropCommand;
			
			if (donorCell->IsSpawner())
			{
				dropCommand = new SpawnPieceCommand(newPiece, receiverCell, donorCell, delay);				
			}
			else
			{
				dropCommand = new DropPieceCommand(newPiece, receiverCell, donorCell, delay);
			}

			
			m_PendingCommands.push(dropCommand);

			m_CascadeService->SetDirty(receiverCell);
			cellIt = emptyCells.erase(cellIt);
		}
	}
}



Piece* Game::GetNeighbourPieceInDirection(Cell * receiverCell, Cell** donorCell, const Constants::Direction dir)
{
	Piece* piece = nullptr;

	while (!piece)
	{
		if ((*donorCell)->IsSpawner())
		{
			Cell* spawnerCell = GetCellAt(-1, (*donorCell)->GetColumn());
			piece = GetRandomPiece(spawnerCell);
			InitPiece(piece, spawnerCell->GetTransform());
		}
		else
		{
			piece = (*donorCell)->GetPiece();

			const bool is_in = m_CascadeService->GetEmptyCells().find(*donorCell) != m_CascadeService->GetEmptyCells().end();
			// (!is_in)
//				ASSERT(false);
			if (!piece || is_in)
			{
				piece = nullptr;
				*donorCell = (*donorCell)->GetNeighbour(dir);
			}

		}
	}
	return piece;
}

void Game::FillableList(std::list<Cell*>& fillableList)
{

	for (Cells::const_reverse_iterator cellIt = m_Cells.rbegin();
		cellIt != m_Cells.rend(); ++cellIt) 
	{
		Cell* cell = *(cellIt);
		if (cell->IsFillable())
		{
			fillableList.push_back(cell);
		}
	}

}

void Game::DeletePieces(Pieces pieces)
{
	for (Pieces::const_iterator pieceIt = pieces.begin(),
	end = pieces.end();
	pieceIt != end;
	++pieceIt)
	{
		Piece* p = *pieceIt;
		if (!p)
			ASSERT(false);
		switch (p->GetMatchColour())
		{
			case MatchFactory::MatchColor::BLUE:
				m_BluePieces.remove(p);
			case MatchFactory::MatchColor::RED:
				m_RedPieces.remove(p);
			case MatchFactory::MatchColor::GREEN:
				m_GreenPieces.remove(p);
			case MatchFactory::MatchColor::YELLOW:
				m_YellowPieces.remove(p);
			case MatchFactory::MatchColor::PURPLE:
				m_PurplePieces.remove(p);
			default:
				break;
		}

		delete p;
	}
	pieces.clear();
}

bool Game::IsBoardIdle()
{
	for (const auto& cellIt : m_Cells)
	{
		if (cellIt->IsBusy())
			return false;
	}
	return !m_CascadeService->NeedsCascading();
}

bool Game::IsColumnIdle(const int column, const int row)
{
	for (const auto& cellIt : m_Cells)
	{
		if (cellIt->GetColumn() == column && cellIt->GetRow() > row && cellIt->IsBusy())
			return false;
	}
	return true;
}


Cell* Game::FindNeighbour(const Cell& cell, Constants::Direction dir)
{
	int row = -1;
	int column = 0;
	switch (dir)
	{
	case Constants::Direction::LEFT:

		if (!cell.IsInLeftBorder())
		{
			row = cell.GetRow();
			column = cell.GetColumn() - 1;
		}
		else
		{
			return nullptr;
		}
		break;
	case Constants::Direction::RIGHT:
		if (!cell.IsInRightBorder())
		{
			row = cell.GetRow();
			column = cell.GetColumn() + 1;
		}
		else
		{
			return nullptr;
		}
		break;
	case Constants::Direction::UP:
		if (cell.IsInTopBorder())
		{
			row = -1;
			column = cell.GetColumn();
		}
		else
		{
			row = cell.GetRow() - 1;
			column = cell.GetColumn();
		}
		break;
	case Constants::Direction::DOWN:
		if (!cell.IsInBottomBorder())
		{
			row = cell.GetRow() + 1;
			column = cell.GetColumn();
		}
		else
		{
			return nullptr;
		}
		break;
	default:
		return nullptr;
		break;
	}
	return GetCellAt(row, column);
}

void Game::SwapPieces(Cell * cellA, Cell * cellB)
{
	Piece* pieceA = cellA->GetPiece();
	Piece* pieceB = cellB->GetPiece();

	ICommand* swapCommand = new SwapCommand(cellA, cellB);
	m_PendingCommands.push(swapCommand);

	if (!pieceA)
		ASSERT(false);

	if (!pieceB)
		ASSERT(false);
	
	cellA->AttachPiece(pieceB);
	cellB->AttachPiece(pieceA);
	std::unordered_set<Cell*> swappedCells = std::unordered_set<Cell*> { cellB, cellA };
	std::unordered_set<Cell*> resolvedCells;
	std::vector<Match*> matches = m_MatchChecker->GetMatches(swappedCells, resolvedCells);
	if (!matches.empty())
	{
		for (Matches::const_iterator matchIt = matches.begin(),
			end = matches.end();
			matchIt != end;
			++matchIt)
		{
			std::list<Cell*> matchedCells = (*matchIt)->GetMatchedCells();
			m_CascadeService->SetEmpty(matchedCells);
			std::list<Piece*> destroyingPieces;
			for (const auto& cell: matchedCells) 
			{
				destroyingPieces.push_back(cell->GetPiece());
				cell->DetachPiece();
			}

			ICommand* destroyPiecesCommand = new DestroyPiecesCommand(matchedCells, destroyingPieces, 0.0f);
			m_PendingCommands.push(destroyPiecesCommand);
		}	

		for (std::vector<Match*>::const_iterator iterator = matches.begin(),
			end = matches.end();
			iterator != end;
			++iterator)
		{
			delete *iterator;
		}
		matches.clear();
	}
	else
	{
		ICommand* swapBackCommand = new SwapCommand(cellB, cellA);
		m_PendingCommands.push(swapBackCommand);
		cellA->AttachPiece(pieceA);
		cellB->AttachPiece(pieceB);
	}
}


Cell * Game::GetCellAt(const int row, const int column)
{
	if (row == -1)
	{
		for (Cells::const_iterator cellIt = m_SpawnerCells.begin(),
			end = m_SpawnerCells.end();
			cellIt != end;
			++cellIt)
		{
			if ((*cellIt)->GetColumn() == column)
				return (*cellIt);
		}
	}


	for (Cells::const_iterator cellIt = m_Cells.begin(),
		end = m_Cells.end();
		cellIt != end;
		++cellIt)
	{
		if ((*cellIt)->GetRow() == row && (*cellIt)->GetColumn() == column)
			return (*cellIt);
	}
	return nullptr;
}
