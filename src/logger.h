#pragma once

#include <iostream>
#include <sstream>
#include <fstream>

/*!
 * \brief Класс логирования
 * Может использоваться для ведения лога в процессе разработки.
 * Логирование может проводится с записью в консоль и/или файл.
 */

class Logger
{
public:
    static Logger & Instance()
    {
        static Logger inst;
        return inst;
    }

    template<typename T, template<typename, typename...> class C, typename... Args>
    Logger & operator << (C<T, Args...> const & objs)
    {
        if (m_logfile.is_open() && m_fileMode)
        {
            m_logfile << "**Container**\n";
            for (auto const & obj : objs)
            {
                m_logfile << " " << obj;
            }
        }
        if (m_terminalMode)
        {
            m_os << "**Container**\n";
            for (auto const & obj : objs)
            {
                m_os << " " << obj;
            }
        }
        return *this;
    }

    template <typename T>
    Logger & operator << (T const & obj)
    {
        if (m_terminalMode)
            m_os << obj;
        if (m_logfile.is_open() && m_fileMode)
            m_logfile << obj;
        return *this;
    }

    void TerminalModeOn();
    void TerminalModeOff();
    void FileModeOn();
    void FileModeOff();

private:
    bool m_terminalMode = true;
    bool m_fileMode = false;
    std::ostream & m_os = std::cout;
    std::ofstream m_logfile;
    Logger();
    Logger(std::ostream & os);
    virtual ~Logger();
    Logger(Logger const &) = delete;
    Logger & operator = (Logger const &) = delete;
    Logger(Logger &&) = delete;
    Logger & operator = (Logger &&) = delete;
};
