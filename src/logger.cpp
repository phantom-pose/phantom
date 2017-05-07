#include "logger.h"

Logger::Logger(std::ostream & os)
    :m_os(os)
{}

Logger::Logger()
    :m_logfile("logfile.txt", std::ios_base::app)
{}

Logger::~Logger()
{
    if (m_fileMode)
    {
        m_logfile << "-----End of session-----\n";
        m_logfile << "\n";
    }
    m_logfile.close();
}

void Logger::TerminalModeOn() { m_terminalMode = true; }
void Logger::TerminalModeOff() { m_terminalMode = false; }
void Logger::FileModeOn() { m_fileMode = true; }
void Logger::FileModeOff() { m_fileMode = false; }
