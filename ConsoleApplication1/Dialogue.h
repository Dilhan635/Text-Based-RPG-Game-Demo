#pragma once
#include <string>
#include "Enemy.h"

enum class TalkOutcome {
	NONE,
	FLEE,
	ATTACK,
	TEACH_ABILITY,
	GIVE_ITEM,
	REVEAL_LORE
};

struct TalkResult {
	std::string message;
	TalkOutcome outcome;

	TalkResult(const std::string& msg, TalkOutcome out)
		: message(msg), outcome(out){ }
};

//declare the function
TalkResult TalkToEnemy(const Enemy& enemy, int playerIntelligence);