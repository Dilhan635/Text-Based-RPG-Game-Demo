#pragma once
#include <string>

enum class TimeOfDay { Morning, Afternoon, Evening, Night };

struct GameTime {
    TimeOfDay timeOfDay = TimeOfDay::Morning;

    void AdvanceTime();
    std::string GetTimeString() const;
};