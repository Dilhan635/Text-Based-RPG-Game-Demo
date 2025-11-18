#include "dialogue.h"

TalkResult TalkToEnemy(const Enemy& enemy, int playerIntelligence) {
    switch (enemy.personality) {
    case PersonalityType::TIMID:
        if (playerIntelligence >= 5) return TalkResult(enemy.name + " panics and flees!", TalkOutcome::FLEE);
        else return TalkResult(enemy.name + " is too scared to speak.", TalkOutcome::NONE);

    case PersonalityType::AGGRESSIVE:
        if (playerIntelligence >= 7) return TalkResult(enemy.name + " growls but backs off, impressed.", TalkOutcome::FLEE);
        else return TalkResult(enemy.name + " attacks immediately!", TalkOutcome::ATTACK);

    case PersonalityType::STALWART:
        if (playerIntelligence >= 8) return TalkResult(enemy.name + " respects your wisdom and teaches you " + enemy.ability + ".", TalkOutcome::TEACH_ABILITY);
        else return TalkResult(enemy.name + " refuses to talk and prepares to fight.", TalkOutcome::ATTACK);

    case PersonalityType::CUNNING:
        if (playerIntelligence >= 6) return TalkResult(enemy.name + " offers you a deal and gives you a rare item.", TalkOutcome::GIVE_ITEM);
        else return TalkResult(enemy.name + " tries to trick you and escapes.", TalkOutcome::FLEE);

    case PersonalityType::FANATICAL:
        if (playerIntelligence >= 9) return TalkResult(enemy.name + " shares cryptic lore about their beliefs.", TalkOutcome::REVEAL_LORE);
        else return TalkResult(enemy.name + " declares you unworthy and attacks.", TalkOutcome::ATTACK);

    default:
        return TalkResult(enemy.name + " is not prepared to listen...", TalkOutcome::NONE);
    }
}
