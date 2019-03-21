#include "MatchFactory.h"

MatchFactory::MatchFactory(MatchTemplateType matchTemplateType)
{
	switch (matchTemplateType)
	{
	case MatchFactory::MatchTemplateType::FULL_MATCHES_TEMPLATE:
		CreateFullMatchesTemplate();
		break;
	default:
		break;
	}
}

MatchFactory::~MatchFactory()
{
	for (MatchCheckers::const_iterator checkerIt = m_MatchCheckers.begin(),
		end = m_MatchCheckers.end();
		checkerIt != end;
		++checkerIt)
	{
		delete *checkerIt;
	}
	m_MatchCheckers.clear();
}

void MatchFactory::CreateFullMatchesTemplate()
{
	IMatchChecker* match3Checker = new Match3Checker(10, 30);
	IMatchChecker* match4Checker = new Match4Checker(10, 40);

	m_MatchCheckers.push_back(match4Checker);
	m_MatchCheckers.push_back(match3Checker);
}
