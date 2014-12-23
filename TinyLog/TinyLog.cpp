#include "TinyLog.h"

void TrimComments(std::string& str) {
    std::size_t pos = std::string::npos;
    if ((pos = str.find("//")) != std::string::npos) {
        str = str.substr(0, pos);
    }
}

void TrimSpaces(std::string& str) {
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());
}

void TrimChar(std::string& str,  char trim_char) {
    str.erase(std::remove(str.begin(), str.end(), trim_char), str.end());
}

// "abc123456789xyz" delete between "123" and "789"
// => ab789xyz
void DeleteStringBetweenTwoStrings(string& origin_str, const string& start_str, const string& end_str) {
    string::size_type start = origin_str.find(start_str, 0);
    string::size_type end = origin_str.find(end_str, 0);
    if(start != string::npos && end != string::npos){
        origin_str = origin_str.replace(start, end - start, "");
    }
}

Logger* Logger::m_instance = new Logger();

Logger::Logger() : log_file_name_("stateslibcpp.log"),
            log_enable_(true),
            to_file_(true),
            to_stdout_(true),
            roll_out_size_(5242880),
            current_log_level_(LOG_LEVEL::INFO),
            max_log_file_count_(50) {
    pthread_mutex_init(&mutex_, NULL);

    level_string[0] = "FATAL";
    level_string[1] = "ERROR";
    level_string[2] = "WARN";
    level_string[3] = "INFO";
    level_string[4] = "DEBUG";
    level_string[5] = "TRACE-";
    level_string[6] = "TRACE";
    level_string[7] = "STRING_END";

    string value("");

    if (!(value = GetConfig("statesliblog.conf", "FILENAME")).empty()) {
        int str_size = value.size();
        if(("\"" == value.substr(0,1)) && ("\"" == value.substr(str_size - 1, 1)))
        {
            log_file_name_ = value.substr(1,str_size - 2);
        }
        else
        {
            log_file_name_ = value;
        }
    }

    if (!(value = GetConfig("TinyLog.conf", "ENABLED")).empty()) {
        log_enable_ = !value.compare("true");
    }

    if (!(value = GetConfig("TinyLog.conf", "TO_FILE")).empty()) {
        to_file_ =  !value.compare("true");
    }

    if (!(value = GetConfig("TinyLog.conf", "TO_STANDARD_OUTPUT")).empty()) {
        to_stdout_ = !value.compare("true");;
    }

    if (!(value = GetConfig("TinyLog.conf", "ROLL_OUT_SIZE")).empty()) {
        roll_out_size_ = atoi(value.c_str());
    }

    if (!(value = GetConfig("TinyLog.conf", "MAX_LOG_FILE_COUNT")).empty()) {
        max_log_file_count_ = atoi(value.c_str());
    }

    if (!(value = GetConfig("TinyLog.conf", "LOG_LEVEL")).empty()) {
        current_log_level_ = GetLevelFromString(value);
    }
}

void Logger::output(std::string str, LOG_LEVEL level) {
        if (log_enable_ && level >= current_log_level_) {
            ostringstream log_str;
            log_str<< CurrentTimeToString()<<" ["<< syscall(SYS_gettid)<<"] "<< level_string[level] << str <<endl;
            if (to_stdout_) {
                cout<< log_str.str();
            }

            pthread_mutex_lock(&mutex_);
            std::ofstream file_stream(log_file_name_, std::ofstream::app);
            if (file_stream.is_open()) {
                file_stream << log_str.str();
                streampos pos = file_stream.tellp();
                file_stream.close();
                if (pos >= roll_out_size_) {
                    RollOut();
                }
            }
            pthread_mutex_unlock(&mutex_);
        }
}

bool Logger::RollOut() {
    int oldest = 0;

    // find oldest rollout log file
    for (oldest = 1; oldest <= max_log_file_count_ + 1; oldest++) {
        ostringstream ss("");
        ss << log_file_name_ << oldest;

        // not found
        if (0 != access(ss.str().c_str(), F_OK)) {
            oldest--;
            break;
        }
    }

    // oldest file is max_log_file_count_, delete it
    if (oldest == max_log_file_count_) {
        ostringstream delete_file_max;
        delete_file_max << log_file_name_ << max_log_file_count_;
        if (0 !=remove(delete_file_max.str().c_str())) {
            return false;
        }
        oldest--;
    }

    // rename [1, lastest - 1] to [2, lastest]
    for (int from = oldest; from >= 1 ; from--) {
        ostringstream from_file, to_file;
        from_file << log_file_name_ << from;
        to_file << log_file_name_ << from + 1;
        if (0 != rename(from_file.str().c_str(), to_file.str().c_str())) {
            return false;
        }
    }

    // rename current log file to log1
    ostringstream to_file;
    to_file << log_file_name_ << 1;
    if (0 != rename(log_file_name_.c_str(), to_file.str().c_str())) {
        return false;
    }

    return true;
}

string Logger::CurrentTimeToString() {
    time_t tm;
    struct tm *p_tm;
    time(&tm);
    p_tm = localtime(&tm);

    struct timeval tv;
    struct timezone tz;
    gettimeofday (&tv , &tz);

    char str[64];
    memset(str, 0 ,64);
    sprintf(str, "%4d-%02d-%02d %02d:%02d:%02d.%03ld",
            p_tm->tm_year + 1900,
            p_tm->tm_mon + 1,
            p_tm->tm_mday,
            p_tm->tm_hour,
            p_tm->tm_min,
            p_tm->tm_sec,
            tv.tv_usec);

    return string(str);
}
void Logger::TrimComments(std::string& str) {
    ::TrimComments(str);
}

int Logger::GetLevelFromString(string& str){
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    int idx = 0;
    while (level_string[idx] != "STRING_END" && level_string[idx] != str) idx++;
    return idx;
}

void Logger::TrimSpaces(std::string& str) {
    ::TrimSpaces(str);
}

string Logger::GetConfig(const string &conf_path, const string &conf_name)
{
    std::ifstream config_file(conf_path);
    std::string buf_line("");
    string conf_value("");
    int split_pos = std::string::npos;

    if (!config_file.is_open()) {
        return "";
    }

    while (config_file.good()) {
        std::getline(config_file, buf_line);
        TrimComments(buf_line);
        TrimSpaces(buf_line);

        if ((split_pos = buf_line.find_first_of('=')) != std::string::npos) {
            if (buf_line.substr(0, split_pos) == conf_name) {
                conf_value =  buf_line.substr(split_pos + 1, buf_line.length() - 1);
                break;
            }
        }
    }

    config_file.close();
    return conf_value;
}
