#include "OrthoCamera.h"
#include "System.h"
#include <time.h>
#include <iostream>
#include <unordered_set>
#include <string>
#include <sstream>

#include "Game.h"
#include "Cell.h"
#include "Piece.h"
#include "GameEntity.h"
#include "ICommand.h"
#include "DropPieceCommand.h"
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
	m_Camera = new OrthoCamera(SCREEN_WIDTH, SCREEN_HEIGHT, -1.0f, 1.0f);

	System system = System::GetSystemInstance();


	system.GetStateMachine()->AddState(Constants::LOADING_STATE, new LoadingState());
	system.GetStateMachine()->AddState(Constants::GAMEPLAY_STATE, new GamePlayState());


	//TODO move to asset loader class
	//m_BGtexture = new Texture("res/textures/BackGround.jpg");
	//m_Celltexture = new Texture("res/textures/bis.jpg");
	System::GetSystemInstance().GetAssetLoader()->Load<Texture>("res/textures/Blue.png", "Blue");
	System::GetSystemInstance().GetAssetLoader()->Load<Texture>("res/textures/Red.png", "Red");
	System::GetSystemInstance().GetAssetLoader()->Load<Texture>("res/textures/Green.png", "Green");
	System::GetSystemInstance().GetAssetLoader()->Load<Texture>("res/textures/Yellow.png", "Yellow");
	System::GetSystemInstance().GetAssetLoader()->Load<Texture>("res/textures/Purple.png", "Purple");

	system.GetStateMachine()->SetState(Constants::LOADING_STATE);

	//m_BlueGemtexture = new Texture("res/textures/Blue.png"); //System::GetSystemInstance().GetAssetLoader()->GetAsset<Texture>("Blue"); //new Texture("res/textures/Blue.png");
	//m_RedGemtexture = new Texture("res/textures/Red.png");
	//m_YellowGemtexture = new Texture("res/textures/Yellow.png");
	//m_GreenGemtexture = new Texture("res/textures/Green.png");
	//m_PurpleGemtexture = new Texture("res/textures/Purple.png");

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
	/*delete	m_BlueGemtexture;
	delete	m_RedGemtexture;
	delete	m_YellowGemtexture;
	delete	m_GreenGemtexture;
	delete	m_PurpleGemtexture;*/
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

void Game::Initialise(Graphics* graphics)
{
	m_Camera->SetProjection(*graphics);

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
		std::cout << "colour id " << static_cast<int>(piece->GetMatchColour()) << std::endl;
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
			std::cout << " new colour id " << static_cast<int>(piece->GetMatchColour()) << std::endl;
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

		ICommand* dropPieceCommand = new DropPieceCommand(piece, *cellsIt, spawnerCell, 0.1);
		m_PendingCommands.push(dropPieceCommand);
		
		m_CascadeService->SetDirty((*cellsIt));

	}

	m_BoardInput = new BoardInput(m_Cells);
	m_isInitialised = true;
}

void Game::CreateCells(int cellsCount, int spawnerCellsCount)
{
	//m_Cells.reserve(cellsCount);
	//m_SpawnerCells.reserve(spawnerCellsCount);

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
				pos.y += 50.0f;
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

	m_BlueGemtexture = System::GetSystemInstance().GetAssetLoader()->GetAsset<Texture>("Blue");
	m_RedGemtexture = System::GetSystemInstance().GetAssetLoader()->GetAsset<Texture>("Red");
	m_YellowGemtexture = System::GetSystemInstance().GetAssetLoader()->GetAsset<Texture>("Yellow");
	m_GreenGemtexture = System::GetSystemInstance().GetAssetLoader()->GetAsset<Texture>("Green");
	m_PurpleGemtexture = System::GetSystemInstance().GetAssetLoader()->GetAsset<Texture>("Purple");

	std::vector<glm::mat4> bluePieceTransforms;
	std::vector<glm::mat4> redPieceTransforms;
	std::vector<glm::mat4> greenPieceTransforms;
	std::vector<glm::mat4> yellowPieceTransforms;
	std::vector<glm::mat4> purplePieceTransforms;

	Game::ApplyModelTransformation(m_BluePieces.begin(), m_BluePieces.end(), bluePieceTransforms); // new glm::mat4[m_BluePieces.size()];

	graphics->DrawInstancedTexture(vertices, sizeof(vertices), &bluePieceTransforms[0], m_BluePieces.size() * sizeof(glm::mat4), System::GetSystemInstance().GetAssetLoader()->GetAsset<Texture>("Blue"), m_BluePieces.size());

	Game::ApplyModelTransformation(m_RedPieces.begin(), m_RedPieces.end(), redPieceTransforms);

	graphics->DrawInstancedTexture(vertices, sizeof(vertices), &redPieceTransforms[0], m_RedPieces.size() * sizeof(glm::mat4), m_RedGemtexture, m_RedPieces.size());

	Game::ApplyModelTransformation(m_GreenPieces.begin(), m_GreenPieces.end(), greenPieceTransforms);

	graphics->DrawInstancedTexture(vertices, sizeof(vertices), &greenPieceTransforms[0], m_GreenPieces.size() * sizeof(glm::mat4), m_GreenGemtexture, m_GreenPieces.size());

	Game::ApplyModelTransformation(m_YellowPieces.begin(), m_YellowPieces.end(), yellowPieceTransforms);

	graphics->DrawInstancedTexture(vertices, sizeof(vertices), &yellowPieceTransforms[0], m_YellowPieces.size() * sizeof(glm::mat4), m_YellowGemtexture, m_YellowPieces.size());

	Game::ApplyModelTransformation(m_PurplePieces.begin(), m_PurplePieces.end(), purplePieceTransforms);

	graphics->DrawInstancedTexture(vertices, sizeof(vertices), &purplePieceTransforms[0], m_PurplePieces.size() * sizeof(glm::mat4), m_PurpleGemtexture, m_PurplePieces.size());

	//bluePieceTransforms.clear();
	//redPieceTransforms.clear();
	//greenPieceTransforms.clear();
	//yellowPieceTransforms.clear();
	//purplePieceTransforms.clear();

	//glm::mat4 model = std::make_unique<glm::mat4>(1.0f).get;
	////GameEntity* gameEntitiy = static_cast<GameEntity*>(*it);
	//Piece* p;
	//model = glm::translate(model, p->GetTransform()->position);


	//int count = static_cast<int>(MatchFactory::MatchColor::Count);
	//int first = static_cast<int>(MatchFactory::MatchColor::BLUE);
	//int last = count;
	////for (size_t i = 0; i < count; i++)
	//for (int index = first; index < last; index++)
	//{
	//	std::vector<std::unique_ptr<glm::mat4>> piecesMatrices;
	//	//Texture* pieceTexture = nullptr;
	//	//Pieces pieces;
	//	//int pieceColour = index;
	//	switch (static_cast<MatchFactory::MatchColor>(index))
	//	{
	//	case MatchFactory::MatchColor::BLUE:
	//		//pieceTexture = m_BlueGemtexture;
	//		//pieces = m_BluePieces;
	//		Game::ApplyModelTransformation(m_BluePieces.begin(), m_BluePieces.end(), piecesMatrices);
	//		graphics->DrawInstancedTexture(vertices, sizeof(vertices), &piecesMatrices[0], m_BluePieces.size() * sizeof(glm::mat4), m_BlueGemtexture, m_BluePieces.size());
	//	case MatchFactory::MatchColor::GREEN:
	//		Game::ApplyModelTransformation(m_GreenPieces.begin(), m_GreenPieces.end(), piecesMatrices);
	//		graphics->DrawInstancedTexture(vertices, sizeof(vertices), &piecesMatrices[0], m_GreenPieces.size() * sizeof(glm::mat4), m_GreenGemtexture, m_GreenPieces.size());
	//	case MatchFactory::MatchColor::YELLOW:
	//		Game::ApplyModelTransformation(m_YellowPieces.begin(), m_YellowPieces.end(), piecesMatrices);
	//		graphics->DrawInstancedTexture(vertices, sizeof(vertices), &piecesMatrices[0], m_YellowPieces.size() * sizeof(glm::mat4), m_YellowGemtexture, m_YellowPieces.size());
	//	case MatchFactory::MatchColor::RED:
	//		Game::ApplyModelTransformation(m_RedPieces.begin(), m_RedPieces.end(), piecesMatrices);
	//		graphics->DrawInstancedTexture(vertices, sizeof(vertices), &piecesMatrices[0], m_RedPieces.size() * sizeof(glm::mat4), m_RedGemtexture, m_RedPieces.size());
	//	case MatchFactory::MatchColor::PURPLE:
	//		Game::ApplyModelTransformation(m_PurplePieces.begin(), m_PurplePieces.end(), piecesMatrices);
	//		graphics->DrawInstancedTexture(vertices, sizeof(vertices), &piecesMatrices[0], m_PurplePieces.size() * sizeof(glm::mat4), m_PurpleGemtexture, m_PurplePieces.size());
	//	default:
	//		break;
	//	}


	//}
}

void Game::DoMatchDetection()
{
	std::unordered_set<Cell*> dirtyCells = m_CascadeService->GetDirtyCells();
	//std::unordered_set<Cell*> emptyCells = m_CascadeService->GetEmptyCells();
	std::unordered_set<Cell*> resolvedCells;
	if (!dirtyCells.empty())
	{
		int count = 0;
		std::vector<Match*> matches = m_MatchChecker->GetMatches(dirtyCells, resolvedCells);
		std::unordered_set<Cell*> emptyCellsserg = m_CascadeService->GetEmptyCells();
		for (Matches::const_iterator matchIt = matches.begin(),
			end = matches.end();
			matchIt != end;
			++matchIt)
		{
			std::list<Cell*> matchedCells = (*matchIt)->GetMatchedCells();
			std::list<Piece*> destroyingPieces;
			for (const auto& cell : matchedCells)
			{
				count++;
			}
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

			ICommand* dropCommand = new DropPieceCommand(newPiece, receiverCell, donorCell, delay);
			m_PendingCommands.push(dropCommand);

			m_CascadeService->SetDirty(receiverCell);
			cellIt = emptyCells.erase(cellIt);

		}
	}
}



Piece* Game::GetNeighbourPieceInDirection(Cell * receiverCell, Cell** donorCell, const Constants::Direction dir)
{
	Piece* piece = nullptr;
	//donorCell = recieverCell->GetNeighbour(dir);
	
	//Cell* don = *donorCell;
	while (!piece)
	{
		if ((*donorCell)->IsSpawner())
		{
			Cell* spawnerCell = GetCellAt(-1, (*donorCell)->GetColumn());
			piece = GetRandomPiece(spawnerCell);
			InitPiece(piece, spawnerCell->GetTransform());
			//std::cout << "new puece" << std::endl;
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
			else
			{
				//std::cout << "stolen puece" << std::endl;
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
	//m_CascadeService->SetEmpty(cells);
	pieces.clear();
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
			//cellIndex = cell.GetRow() * Constants::COLUMN_LIMIT + (cell.GetColumn() - 1);
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
			//cellIndex = cell.GetRow() * Constants::COLUMN_LIMIT + (cell.GetColumn() + 1);
		}
		else
		{
			return nullptr;
		}
		break;
	case Constants::Direction::UP:
		if (cell.IsInTopBorder())
		{
			//cellIndex = cell.GetColumn();
			row = -1;
			column = cell.GetColumn();
			//return GetSpawnerAt(cellIndex);
		}
		else
		{
			row = cell.GetRow() - 1;
			column = cell.GetColumn();
			//cellIndex = (cell.GetRow() - 1) * Constants::COLUMN_LIMIT + cell.GetColumn();
		}
		break;
	case Constants::Direction::DOWN:
		if (!cell.IsInBottomBorder())
		{
			row = cell.GetRow() + 1;
			column = cell.GetColumn();
			//cellIndex = (cell.GetRow() + 1) * Constants::COLUMN_LIMIT + cell.GetColumn();
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

	//m_Pending.Enqueue(new SwapAction(cellA, cellB, pieceA, pieceB));
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
			//m_CascadeService->SetDirty(matchedCells);
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
		//GameController.Instance.audioManager.PlaySFX(Constants.SFX.SWAP_BACK);
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
