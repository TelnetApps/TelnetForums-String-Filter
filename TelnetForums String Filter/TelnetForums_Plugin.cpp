// TelnetForums_Plugin.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "TelnetForums_Plugin.h"
#include <string> // Include the <string> header
#include <algorithm> 
#include <cctype>

//    
//     ___       _     _              ___    __
//    / __|  ___| |_  | |_ ___   __ _( _ )  / /
//    \__ \ / -_)  _| |  _/ _ \  \ \ / _ \ / _ \
//    |___/ \___|\__|  \__\___/  /_\_\___/ \___/
//    (set to x86)
//   

// Declare the functions with extern "C" to prevent name mangling
extern "C" {


    bool loadFilters(std::vector<std::string>& filters) {
        std::ifstream filterFile("filters.txt");
        if (filterFile.is_open()) {
            std::string filter;
            while (std::getline(filterFile, filter)) {
                filters.push_back(filter);
            }
            filterFile.close();
            return true;
        }
        else {
            return false;
        }
    }

    // Helper function
    std::string censorString(const std::string& input, const std::vector<std::string>& filters) {
        
        std::string result = input;

        for (const std::string& filter : filters) {
            std::string filterLower = filter;
            std::string inputLower = input;    // Copy the input string

            std::transform(filterLower.begin(), filterLower.end(), filterLower.begin(), ::tolower);
            std::transform(inputLower.begin(), inputLower.end(), inputLower.begin(), ::tolower);

            size_t pos = 0;
            while ((pos = inputLower.find(filterLower, pos)) != std::string::npos) {
                result.replace(pos, filter.length(), std::string(filter.length(), '*'));
                pos += filter.length(); 
            }
        }

        return result;
    }

    __declspec(dllexport) const char* StringProcessFunction(const char* input) {
        std::string inputStr = input;
        std::vector<std::string> filters;

        if (loadFilters(filters)) {
            std::string result = censorString(inputStr, filters);

            char* returnString = new char[result.size() + 1];
            strcpy_s(returnString, result.size() + 1, result.c_str());
            returnString[result.size()] = '\0'; // Null-terminate the string
            return returnString;


        }
        else {
            // Handle the case where "filters.txt" is not found
            char* returnString = new char[inputStr.size() + 1];
            strcpy_s(returnString, inputStr.size() + 1, inputStr.c_str());
            returnString[inputStr.size()] = '\0'; // Null-terminate the string
            return returnString;
        }
    }

        // TelnetForums requires a plugin to have a plugin description
        extern const char* PLUGIN_DESCRIPTION = "This plugin filters words from filters.txt.";

        __declspec(dllexport) const char* PluginDescription() {
            return PLUGIN_DESCRIPTION;
        }
    
}