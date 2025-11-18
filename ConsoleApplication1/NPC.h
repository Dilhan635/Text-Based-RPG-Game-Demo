#pragma once
#include <string>
#include <vector>
#include "LoreDatabase.h"
#include "Character.h"

class NPC {
public:
	std::string name;
	std::string description;
	int x, y;

	std::string currentArea;

	std::vector<std::string> dialogueLines;
	mutable int dialogueIndex = 0; //tracks which dialogue line to show next

	LoreEntry loreEntry;

	mutable bool loreGiven = false;
	std::string fallbackLine;

	bool isProcedural = false;

	NPC(); //enables default construction

	NPC(const std::string& name, const std::string& description, int x, int y,
		const std::vector<std::string>& dialogueLines,
		const LoreEntry& loreEntry,
		const std::string& currentArea,
		const std::string& fallbackLine,
		bool isProcedural);

	void Interact(Character& player) const;
};

NPC GenerateProceduralNPC(int x, int y, const std::string& areaName);