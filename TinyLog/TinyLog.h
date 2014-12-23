/*
 * TinyLog.h
 *
 *  Created on: 2014-6-16
 *      Author: hanmz
 */

#ifndef TINYLOG_H_
#define TINYLOG_H_
#include <cstring>
#include <sys/syscall.h>
#include <sys/time.h>
#include <pthread.h>
#include <string>
#include <functional>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

typedef enum {
	FATAL = 0,
	ERROR,
    WARN,
    INFO,
    DEBUG,
    TRACE_DEC,
    TRACE
}LOG_LEVEL;

int GetLevelFromString(string& str);

string CurrentTimeToString();

void TrimComments(std::string& str);

void TrimSpaces(std::string& str);

void TrimChar(std::string& str, char trim_char);

void DeleteStringBetweenTwoStrings(string& origin_str, const string& start_str, const string& end_str);

string GetConfig(const string &conf_path, const string &conf_name);

class Logger {
    public:
        static Logger* getInstance() {
            return m_instance;
        };

        Logger();

        void output (std::string str, LOG_LEVEL level);

        ~Logger() {
            pthread_mutex_destroy(&mutex_);
        };
    protected:
        bool RollOut();
        Logger(const Logger&);
        Logger& operator=(const Logger&);

        string CurrentTimeToString();
        void TrimComments(std::string& str);
        int GetLevelFromString(string& str);
        void TrimSpaces(std::string& str);
        string GetConfig(const string &conf_path, const string &conf_name);

    private:
        pthread_mutex_t mutex_;

        // config item
        string log_file_name_;
        bool log_enable_;
        bool to_file_;
        bool to_stdout_;
        int roll_out_size_;
        int current_log_level_;
        int max_log_file_count_;
        string level_string[8];

        static Logger* m_instance;
};

#define LOG_BASE(params, level) {    \
        ostringstream info;    \
        info <<" ["<< __PRETTY_FUNCTION__<<"] ["<< __FILE__<<": "<< __LINE__ <<"] "<< params; \
        Logger::getInstance()->output(info.str(), level);    \
    }

#define LOG_BASE_4VECTOR(vec, level) {    \
    ostringstream info;    \
    info <<" ["<< __PRETTY_FUNCTION__<<"] ["<< __FILE__<<": "<< __LINE__ <<"] ["; \
    int begin_ = 0;\
    int end_ = vec.size();    \
    for (; begin_ != end_; ++begin_) {    \
        info << vec.at(begin_);    \
        info << ((begin_ < end_ - 1) ? ", " : "");    \
    }    \
    info << "]";    \
    Logger::getInstance()->output(info.str(), level);    \
}

#define LOG_TRACE_DEC(args) { \
    LOG_BASE(args, TRACE_DEC)    \
}

#define LOG_TRACE(args) { \
    LOG_BASE(args, TRACE)    \
}

#define LOG_DEBUG(args) { \
    LOG_BASE(args, DEBUG)    \
}

#define LOG_DEBUG_4VECTOR(args) { \
    LOG_BASE_4VECTOR(args, DEBUG)    \
}

#define LOG_DEBUG_4Vo(vo) { \
    LOG_BASE(vo, DEBUG)    \
}

#define LOG_INFO(args) { \
    LOG_BASE(args, INFO)    \
}

#define LOG_WARN(args) { \
    LOG_BASE(args,  WARN)    \
}

#define LOG_ERROR(args) { \
    LOG_BASE(args, ERROR)    \
}


// below require C99 support
#define LOG_TRACE_FMT(...) { \
    char x[1024];    \
    sprintf(x, __VA_ARGS__);    \
    LOG_BASE(x, TRACE)    \
}

#define LOG_DEBUG_FMT(...) { \
    char x[1024];    \
    sprintf(x, __VA_ARGS__);    \
    LOG_BASE(x, DEBUG)    \
}

#define LOG_INFO_FMT(...) { \
    char x[1024];    \
    sprintf(x, __VA_ARGS__);    \
    LOG_BASE(x, INFO)    \
}

#define LOG_WARN_FMT(...) { \
    char x[1024];    \
    sprintf(x, __VA_ARGS__);    \
    LOG_BASE(x, WARN)    \
}

#define LOG_ERROR_FMT(...) { \
    char x[1024];    \
    sprintf(x, __VA_ARGS__);    \
    LOG_BASE(x, ERROR)    \
}

#endif /* TINYLOG_H_ */
