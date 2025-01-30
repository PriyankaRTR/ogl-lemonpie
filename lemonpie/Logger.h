#pragma once
// C++ program to implement a basic logging system.

#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

// Enum to represent log levels
enum LogLevel { 
    LOG_DEBUG, 
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL
};

class Logger {
public:
    // Constructor: Opens the log file in append mode
    Logger(const string& filename)
    {
        logFile.open(filename, ios::app);
        if (!logFile.is_open()) {
            cerr << "Error opening log file." << endl;
        }
    }

    // Destructor: Closes the log file
    ~Logger() { logFile.close(); }

    // Logs a message with a given log level
    void log(LogLevel level, const string& message)
    {
        // Get current timestamp
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp),
            "%Y-%m-%d %H:%M:%S", timeinfo);

        // Create log entry
        ostringstream logEntry;
        logEntry << "[" << timestamp << "] "
            << levelToString(level) << ": " << message
            << endl;

        // Output to console
        cout << logEntry.str();

        // Output to log file
        if (logFile.is_open()) {
            logFile << logEntry.str();
            logFile
                .flush(); // Ensure immediate write to file
        }
    }

private:
    ofstream logFile; // File stream for the log file

    // Converts log level to a string for output
    string levelToString(LogLevel level)
    {
        switch (level) {
        case LOG_DEBUG:
            return "DEBUG";
        case LOG_INFO:
            return "INFO";
        case LOG_WARNING:
            return "WARNING";
        case LOG_ERROR:
            return "ERROR";
        case LOG_CRITICAL:
            return "CRITICAL";
        default:
            return "UNKNOWN";
        }
    }
};