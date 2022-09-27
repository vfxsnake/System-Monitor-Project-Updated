#include <string>
#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) 
{
    std::string current_seconds = std::to_string(seconds % 60);
    std::string current_minutes = std::to_string((seconds/60) % 60);
    std::string curret_hours = std::to_string(seconds/3600);
    
    string string_formatted = PadZeros(curret_hours) + ":" + PadZeros(current_minutes) + ":" + PadZeros(current_seconds);
    return string_formatted; 
}

string Format::PadZeros(std::string &source, int padding_size)
{
    return source.insert(0, padding_size - source.length(), '0');
}