#include "Piece.h"
#include "Cell.h"
#include <iostream>

Piece::Piece(MatchFactory::MatchColor matchColour): m_MatchColour(matchColour)
{
}

Piece::~Piece()
{
	//m_OwnerCell->DetachPiece();
	//std::cout << "Piece destroyed" << std::endl;
}