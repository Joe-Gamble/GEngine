#include "Logging.h"
#include "NetworkManager.h"

#if defined(_WIN32)
#include "color.h"
#endif

#if defined(ERROR)
#undef ERROR       
#endif

using namespace GEngine::Logging;
using namespace GEngine::Networking;

tm* GEngine::Logging::gmtime_r(const time_t* timer, tm* buf)
{
	{
#if defined( _MSC_VER )
		gmtime_s(buf, timer);
#elif defined(__BORLANDC__)
		gmtime_s(timer, buf);
#endif
		return buf;
	}
}

std::string GEngine::Logging::TimeStampBig()
{
	//get the time
	std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
	std::time_t tt = std::chrono::system_clock::to_time_t(tp);
	std::tm gmt{};
	gmtime_r(&tt, &gmt);
	std::chrono::duration<double> fractional_seconds =
		(tp - std::chrono::system_clock::from_time_t(tt)) + std::chrono::seconds(gmt.tm_sec);
	//format the string
	std::string buffer("year/mo/dy hr:mn:sc.xxxxxx");
	sprintf(&buffer.front(), "%04d/%02d/%02d %02d:%02d:%09.6f", gmt.tm_year + 1900, gmt.tm_mon + 1,
		gmt.tm_mday, gmt.tm_hour, gmt.tm_min, fractional_seconds.count());
	return buffer;
}

std::string GEngine::Logging::LittleTimeStamp()
{
	//get the time
	std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
	std::time_t tt = std::chrono::system_clock::to_time_t(tp);
	std::tm gmt{};
	gmtime_r(&tt, &gmt);
	std::chrono::duration<double> fractional_seconds =
		(tp - std::chrono::system_clock::from_time_t(tt)) + std::chrono::seconds(gmt.tm_sec);
	//format the string
	std::string buffer("hr:mn:sc.xx");
	sprintf(&buffer.front(), "%02d:%02d:%09.6f", gmt.tm_hour, gmt.tm_min, fractional_seconds.count());
	return buffer;
}

#if !defined(_WIN32)
void ConsoleLogger::Log(const std::string& message, const LogLevel level)
{
	if (level < LOG_LEVEL_CUTOFF)
	{
		return;
	}
	std::string output;
	output.reserve(message.length() + 64);
	output.append(timestamp());


	output.append(levels.find(level)->second);
	output.append(message);
	output.push_back('\n');
	log(output);
}
#else
void ConsoleLogger::Log(const std::string& message, const LogLevel level)
{
	if (level < LOG_LEVEL_CUTOFF)
	{
		return;
	}
	NetworkManager& nm = NetworkManager::Instance();
	std::string author = "";

	if (nm.IsClient() || nm.IsServer())
	{
		if (!nm.HasAuthority())
			author = "[CLIENT] ";
		else if (nm.HasAuthority() && nm.IsClient())
			author = "[HOST]   ";
		else if (nm.IsServer())
			author = "[SERVER] ";

		std::cout << dye::light_red(author);
	}
	else
	{
		author = "[ENGINE] ";
		std::cout << dye::light_yellow(author);
	}

	

	std::lock_guard<std::mutex> lk{ lock };

	std::cout << LittleTimeStamp();
	switch (level)
	{
	case LogLevel::ERROR:
		std::cout << dye::red(levels.find(level)->second);
		break;
	case LogLevel::WARN:
		std::cout << dye::yellow(levels.find(level)->second);
		break;
	case LogLevel::INFO:
		std::cout << dye::green(levels.find(level)->second);
		break;
	case LogLevel::DEBUG:
		std::cout << dye::blue(levels.find(level)->second);
		break;
	case LogLevel::TRACE:
		std::cout << dye::white(levels.find(level)->second);
		break;

	}

	std::cout << dye::white(message) << std::endl;
}

void ConsoleLogger::Log(const std::string& message)
{
	//cout is thread safe, to avoid multiple threads interleaving on one line
	//though, we make sure to only call the << operator once on std::cout
	//otherwise the << operators from different threads could interleave
	//obviously we dont care if flushes interleave
	//std::lock_guard<std::mutex> lk{lock};
	std::cout << message;
	std::cout.flush();
}


#endif

FileLogger::FileLogger(const logging_config_t& config) : Logger(config)
{
	//grab the file name
	auto name = config.find("file_name");
	if (name == config.end())
	{
		throw std::runtime_error("No output file provided to file logger");
	}
	file_name = name->second;

	//if we specify an interval
	reopen_interval = std::chrono::seconds(300);
	auto interval = config.find("reopen_interval");
	if (interval != config.end())
	{
		try
		{
			reopen_interval = std::chrono::seconds(std::stoul(interval->second));
		}
		catch (...)
		{
			throw std::runtime_error(interval->second + " is not a valid reopen interval");
		}
	}

	//crack the file open
	Reopen();
}

void FileLogger::Log(const std::string& message, const LogLevel level)
{
	if (level < LOG_LEVEL_CUTOFF)
	{
		return;
	}
	std::string output;
	output.reserve(message.length() + 64);
	output.append(TimeStampBig());
	output.append(uncolored.find(level)->second);
	output.append(message);
	output.push_back('\n');
	Log(output);
}

void FileLogger::Log(const std::string& message)
{
	lock.lock();
	file << message;
	file.flush();
	lock.unlock();
	Reopen();
}

void FileLogger::Reopen()
{
	//check if it should be closed and reopened
	auto now = std::chrono::system_clock::now();
	lock.lock();
	if (now - last_reopen > reopen_interval)
	{
		last_reopen = now;
		try
		{
			file.close();
		}
		catch (...) {}
		try
		{
			file.open(file_name, std::ofstream::out | std::ofstream::app);
			last_reopen = std::chrono::system_clock::now();
		}
		catch (std::exception& e)
		{
			try
			{
				file.close();
			}
			catch (...) {}
			throw e;
		}
	}
	lock.unlock();
}
