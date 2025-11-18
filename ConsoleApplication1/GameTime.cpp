#include "GameTime.h"

void GameTime::AdvanceTime() {
    switch (timeOfDay) {
    case TimeOfDay::Morning: timeOfDay = TimeOfDay::Afternoon; break;
    case TimeOfDay::Afternoon: timeOfDay = TimeOfDay::Evening; break;
    case TimeOfDay::Evening: timeOfDay = TimeOfDay::Night; break;
    case TimeOfDay::Night: timeOfDay = TimeOfDay::Morning; break;
    }
}

std::string GameTime::GetTimeString() const {
    switch (timeOfDay) {
    case TimeOfDay::Morning: return "Morning";
    case TimeOfDay::Afternoon: return "Afternoon";
    case TimeOfDay::Evening: return "Evening";
    case TimeOfDay::Night: return "Night";
    }
    return "Unknown";
}
