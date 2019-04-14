#pragma once

#include <vector>
#include <unordered_set>
#include <queue>
#include <memory>

#include "Constants.h"
#include "GameEntity.h"
#include "ICommand.h"

class Piece;
class Cell;

struct Neighbours
{
	Cell* left;
	Cell* right;
	Cell* up;
	Cell* down;
};

class Cell : public GameEntity
{
public:
	Cell(int row, int column, bool isSpawner);
	~Cell();

	void Update(float deltaTime);
	void Render() const;

	inline int GetRow()							const { return m_Row;	 }
	inline int GetColumn()						const { return m_Column; }
	inline bool IsSpawner()						const { return m_IsSpawner;}
	inline bool IsFillable()					const { return !m_Piece && !m_IsSpawner; }
	inline bool IsMatchable()					const { return m_Piece && !m_IsSpawner; }
	inline bool IsInLeftBorder()				const { return m_Column <= 0; }
	inline bool IsInRightBorder()				const { return m_Column >= Constants::COLUMN_LIMIT - 1;}
	inline bool IsInTopBorder()					const { return m_Row <= 0; }
	inline bool IsInBottomBorder()				const { return m_Row >= Constants::ROW_LIMIT - 1; }
	inline Piece* GetPiece()					const { return m_Piece; }
	inline Cell* GetLeft()						const { return m_Neighbours.left; }
	inline Cell* GetRight()						const { return m_Neighbours.right; }
	inline Cell* GetUp()						const { return m_Neighbours.up; }
	inline Cell* GetDown()						const { return m_Neighbours.down; }
	inline ICommand* GetCurrentCommand()		const { return m_CurrentCommand; }
	//inline Neighbours GetNeighbours()	const { return { m_Left , m_RLight, m_Up, m_Down }; }

	bool IsNeighour(Cell* neighbour) const;
	Cell* GetNeighbour(const Constants::Direction dir);
	void SetNeighbours(Cell* left, Cell* right, Cell* up, Cell* down);
	void AttachPiece(Piece* piece);
	void DetachPiece();

	bool CompareCells(Cell* neighbourCell, std::unordered_set<Cell*>& resolvedMatchCells) const;
	bool HasMatchingPiece(Piece* p) const;

	void PrepareForCommand(ICommand *command);
	bool IsReadyForCommand(ICommand *command);
	void BeginCommand(ICommand *command);
	bool IsBusy();
	void ReleaseCommand(ICommand *command);

private:
	int m_Row;
	int m_Column;
	bool m_IsSpawner;
	Piece* m_Piece;
	Neighbours m_Neighbours;
	std::queue<ICommand*> m_PendingCellCommands;
	ICommand* m_CurrentCommand;


};

