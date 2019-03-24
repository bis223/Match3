#pragma once

#include "GameEntity.h"
#include "MatchFactory.h"

class Cell;


class Piece : public GameEntity
{
	public:
		Piece(MatchFactory::MatchColor matchColur);
		~Piece();
		inline MatchFactory::MatchColor GetMatchColour()	const { return m_MatchColour; }
		inline bool IsMatching(Piece* piece)			const { return (int)m_MatchColour == (int)piece->GetMatchColour(); }
		inline Cell* GetOwner()							const { return m_OwnerCell; }
		inline void SetOwner(Cell* cell)			          {  m_OwnerCell = cell; }
	private:
		Cell* m_OwnerCell;
		MatchFactory::MatchColor m_MatchColour;
};


