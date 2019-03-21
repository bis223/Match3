#pragma once

#include "vector"
#include "IMatchChecker.h"
#include "Match3Checker.h"
#include "Match4Checker.h"

//struct MatchTemplate {};
//struct FullMatchesTemplate : MatchTemplate
//{
//	Match3Checker* match3Checker;	
//};
//
//struct MatchPalette
//{
//	MatchTemplate fullMatchTemplate;
//};


class MatchFactory
{
public:
	enum class MatchColor
	{
		RED = 0, BLUE = 1, GREEN = 2, YELLOW = 3, PURPLE = 4, Count = 5
	};
	enum class MatchTemplateType
	{
		FULL_MATCHES_TEMPLATE = 0
	};
	MatchFactory(MatchTemplateType matchTemplateType);
	~MatchFactory();
	typedef std::vector<IMatchChecker*> MatchCheckers;
	inline const MatchCheckers& GetMatchCheckers()	const { return m_MatchCheckers; }
private:
	MatchCheckers m_MatchCheckers;
	void CreateFullMatchesTemplate();
};

