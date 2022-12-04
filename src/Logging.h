#pragma once
#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <mutex>
#include <unordered_map>
#include <memory>
#include <chrono>
#include <ctime>
#include <cstdlib>

#if defined( _MSC_VER)
#pragma warning(disable : 4996)
#endif

#if defined(ERROR)
#undef ERROR        //  Unfortunately ERROR is defined as a macro in windgi.h...
#endif

namespace GEngine
{
	namespace Logging
	{
		//TODO: use macros (again) so __FILE__ __LINE__ could be automatically added to certain error levels?

		//the log levels we support
		enum class LogLevel : uint8_t { TRACE = 0, DEBUG = 1, INFO = 2, WARN = 3, ERROR = 4 };
		struct enum_hasher
		{
			template <typename T> std::size_t operator() (T t) const
			{
				return static_cast<std::size_t> (t);
			}
		};
		const std::unordered_map<LogLevel, std::string, enum_hasher> uncolored
		{
			{LogLevel::ERROR, " [ERROR] "}, {LogLevel::WARN, " [WARN] "}, {LogLevel::INFO, " [INFO] "},
			{LogLevel::DEBUG, " [DEBUG] "}, {LogLevel::TRACE, " [TRACE] "}
		};

#if !defined( _WIN32 )
		const std::unordered_map<log_level, std::string, enum_hasher> colored
		{
			{log_level::ERROR, " \x1b[31;1m[ERROR]\x1b[0m "},
			{log_level::WARN, " \x1b[33;1m[WARN]\x1b[0m "},
			{log_level::INFO, " \x1b[32;1m[INFO]\x1b[0m "},
			{log_level::DEBUG, " \x1b[34;1m[DEBUG]\x1b[0m "},
			{log_level::TRACE, " \x1b[37;1m[TRACE]\x1b[0m "}
		};
#else
		const std::unordered_map<LogLevel, std::string, enum_hasher> colored
		{
			{LogLevel::ERROR, " [ERROR] "},
			{LogLevel::WARN, " [WARN] "},
			{LogLevel::INFO, " [INFO] "},
			{LogLevel::DEBUG, " [DEBUG] "},
			{LogLevel::TRACE, " [TRACE] "}
		};
#endif

		//all, something in between, none or default to info
#if defined(LOGGING_LEVEL_ALL) || defined(LOGGING_LEVEL_TRACE)
		constexpr log_level LOG_LEVEL_CUTOFF = LogLevel::TRACE;
#elif defined(LOGGING_LEVEL_DEBUG)
		constexpr log_level LOG_LEVEL_CUTOFF = LogLevel::DEBUG;
#elif defined(LOGGING_LEVEL_WARN)
		constexpr log_level LOG_LEVEL_CUTOFF = LogLevel::WARN;
#elif defined(LOGGING_LEVEL_ERROR)
		constexpr log_level LOG_LEVEL_CUTOFF = LogLevel::ERROR;
#elif defined(LOGGING_LEVEL_NONE)
		constexpr log_level LOG_LEVEL_CUTOFF = LogLevel::ERROR + 1;
#else
		constexpr LogLevel LOG_LEVEL_CUTOFF = LogLevel::INFO;
#endif

#if defined(_WIN32)
		struct tm* gmtime_r(const time_t* timer, struct tm* buf);

#endif

		//returns formated to: 'year/mo/dy hr:mn:sc.xxxxxx'
		std::string TimeStampBig();
		std::string LittleTimeStamp();

		//logger base class, not pure virtual so you can use as a null logger if you want
		using logging_config_t = std::unordered_map<std::string, std::string>;
		class Logger
		{
		public:
			Logger() = delete;
			Logger(const logging_config_t&) {};
			virtual ~Logger() {};
			virtual void Log(const std::string&, const LogLevel) {};
			virtual void Log(const std::string&) {};

		protected:
			std::mutex lock;
		};

		//logger that writes to standard out
		class ConsoleLogger : public Logger
		{
		public:
			ConsoleLogger() = delete;
			ConsoleLogger(const logging_config_t& config) : Logger(config), levels(config.find("color") != config.end() ? colored : uncolored) {}

		#if !defined(_WIN32)
			virtual void Log(const std::string& message, const LogLevel level) override;
		#else
			virtual void Log(const std::string& message, const LogLevel level) override;
		#endif
			virtual void Log(const std::string& message) override;
		protected:
			const std::unordered_map<LogLevel, std::string, enum_hasher> levels;
		};

		//TODO: add log rolling
		//logger that writes to file
		class FileLogger : public Logger
		{
		public:
			FileLogger() = delete;
			FileLogger(const logging_config_t& config);

			virtual void Log(const std::string& message, const LogLevel level) override;
			virtual void Log(const std::string& message) override;

		protected:
			void Reopen();
			std::string file_name;
			std::ofstream file;
			std::chrono::seconds reopen_interval;
			std::chrono::system_clock::time_point last_reopen;
		};

		//a factory that can create loggers (that derive from 'logger') via function pointers
		//this way you could make your own logger that sends log messages to who knows where
		using logger_creator = Logger * (*) (const logging_config_t&);
		class LoggerFactory
		{
		public:
			LoggerFactory()
			{
				creators.emplace("", [](const logging_config_t& config)->Logger* {return new Logger(config); });
				creators.emplace("std_out", [](const logging_config_t& config)->Logger* {return new ConsoleLogger(config); });
				creators.emplace("file", [](const logging_config_t& config)->Logger* {return new FileLogger(config); });
			}
			Logger* Produce(const logging_config_t& config) const
			{
				//grab the type
				auto type = config.find("type");
				if (type == config.end())
				{
					throw std::runtime_error("Logging factory configuration requires a type of logger");
				}
				//grab the logger
				auto found = creators.find(type->second);
				if (found != creators.end())
				{
					return found->second(config);
				}
				//couldn't get a logger
				throw std::runtime_error("Couldn't produce logger for type: " + type->second);
			}
		protected:
			std::unordered_map<std::string, logger_creator> creators;
		};

		//statically get a factory
		inline LoggerFactory& Instance()
		{
			static LoggerFactory factory_singleton{};
			return factory_singleton;
		}

		//get at the singleton
		inline Logger& GetLogger(const logging_config_t& config = { {"type", "std_out"}, {"color", ""} })
		{
			static std::unique_ptr<Logger> singleton(Instance().Produce(config));
			return *singleton;
		}

		//configure the singleton (once only)
		inline void Configure(const logging_config_t& config)
		{
			GetLogger(config);
		}

		//statically log manually without the macros below
		inline void Log(const std::string& message, const LogLevel level)
		{
			GetLogger().Log(message, level);
		}

		//statically log manually without a level or maybe with a custom one
		inline void Log(const std::string& message)
		{
			GetLogger().Log(message);
		}

		//these standout when reading code
		inline void TRACE(const std::string& message)
		{
			GetLogger().Log(message, LogLevel::TRACE);
		}
		inline void DEBUG(const std::string& message)
		{
			GetLogger().Log(message, LogLevel::DEBUG);
		}
		inline void INFO(const std::string& message)
		{
			GetLogger().Log(message, LogLevel::INFO);
		}
		inline void WARN(const std::string& message)
		{
			GetLogger().Log(message, LogLevel::WARN);
		}
		inline void ERROR(const std::string& message)
		{
			GetLogger().Log(message, LogLevel::ERROR);
		}
	}
}
#endif