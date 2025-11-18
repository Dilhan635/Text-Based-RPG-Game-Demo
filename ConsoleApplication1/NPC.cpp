#include "NPC.h"
#include <iostream>

NPC::NPC()
    : name(""), description(""), x(0), y(0),
    dialogueLines({}), loreEntry(LoreEntry("")), currentArea(""), fallbackLine("") {
}


NPC::NPC(const std::string& name, const std::string& description, int x, int y,
    const std::vector<std::string>& dialogueLines,
    const LoreEntry& loreEntry,
    const std::string& currentArea,
    const std::string& fallbackLine,
    bool isProcedural)
    : name(name), description(description), x(x), y(y),
    dialogueLines(dialogueLines), loreEntry(loreEntry), currentArea(currentArea), fallbackLine(fallbackLine), isProcedural(isProcedural) {
}


void NPC::Interact(Character& player) const {

    if (dialogueIndex < dialogueLines.size()) {
        std::cout << dialogueLines[dialogueIndex] << std::endl;
        dialogueIndex++;

        if (dialogueIndex == dialogueLines.size() && !loreGiven) {
            player.DiscoverLore(loreEntry);
            loreGiven = true;
        }
    }
    else {
        std::cout << fallbackLine << "\n"; // After all lines have been shown
    }

    }

NPC GenerateProceduralNPC(int x, int y, const std::string& areaName) {
    std::vector<std::string> names = { "Lira", "Thorn", "Eldon", "Mira", "Bram", "Nyra", "Joss", "Fen" };
    std::vector<std::string> roles = { "Merchant", "Hermit", "Bandit", "Healer", "Wanderer" };
    std::vector<std::string> personalities = { "Cheerful", "Grumpy", "Mysterious", "Wise", "Suspicious" };
    std::vector<std::string> dialogues = {
        "Greetings, traveler.",
        "What do you want?",
        "I've seen things you wouldn't believe...",
        "Need supplies?",
        "Stay away from the mountains."
    };

    std::string name = names[rand() % names.size()];
    std::string role = roles[rand() % roles.size()];
    std::string personality = personalities[rand() % personalities.size()];
    std::string description = role + " (" + personality + ")";
    std::vector<std::string> lines = { dialogues[rand() % dialogues.size()] };

// You can generate lore later if desired
    LoreEntry loreEntry("");

    return NPC(name, description, x, y, lines, loreEntry, areaName, "They say nothing more.", true);
}
