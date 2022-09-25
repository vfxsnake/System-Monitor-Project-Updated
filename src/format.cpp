#include <string>
#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) 
{
    int current_seconds = int(seconds % 60);
    int current_minutes = (seconds/60) % 60;
    int curret_hours = seconds/3600;
    string string_formatted = std::to_string(curret_hours) + ":" + std::to_string(current_minutes) + ":" + std::to_string(current_seconds);
    return string_formatted; 
}