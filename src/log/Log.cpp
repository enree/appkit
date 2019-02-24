#include "Log.h"

#include "CustomSyslogFactory.h"
#include "TimeStampFormatterFactory.h"

#include <boost/filesystem.hpp>
#include <boost/log/attributes/current_process_name.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/from_stream.hpp>

#include <fstream>

#ifdef __linux
#include <pwd.h>
#include <unistd.h>
#endif

namespace rio
{

namespace logger
{

namespace
{
void init ()
{
    boost::log::register_sink_factory("CustomSyslog", boost::make_shared<CustomSyslogFactory>());

    boost::log::register_formatter_factory("TimeStamp", boost::make_shared<TimeStampFormatterFactory>());
    boost::log::register_simple_formatter_factory<rio::logger::LogParams, char>("Params");

    // We have some kind of magic here. On MSVS platform the order of this call is essential.
    // If we register formatter before filter, we have no message in output
    boost::log::register_simple_filter_factory<rio::logger::SeverityLevel, char>("Severity");
    boost::log::register_simple_formatter_factory<rio::logger::SeverityLevel, char>("Severity");
}

} // namespace

namespace logging = boost::log;

Log::Log(const std::string& configFilename)
{
    init();
    std::ifstream logSettings(configFilename.c_str());
    boost::log::init_from_stream(logSettings);
    setupAttributes();
}

Log::Log(const boost::log::settings &settings)
{
   init();
   logging::init_from_settings(settings);
   setupAttributes();
}

Log::~Log()
{
    boost::log::core::get()->remove_all_sinks();
}

void Log::setupAttributes()
{
    boost::shared_ptr<logging::core> core = logging::core::get();
    BOOST_ASSERT(core);

    boost::log::add_common_attributes();
#ifdef __linux
    uid_t uid(geteuid());

    core->add_global_attribute("pid", boost::log::attributes::constant<__pid_t>(getpid()));
    core->add_global_attribute("ppid", boost::log::attributes::constant<__pid_t>(getppid()));
    core->add_global_attribute("uid", boost::log::attributes::constant<__pid_t>(uid));
    core->add_global_attribute("gid", boost::log::attributes::constant<__pid_t>(getgid()));
    core->add_global_attribute("euid", boost::log::attributes::constant<__pid_t>(geteuid()));
    if (struct passwd *pw = getpwuid(uid))
    {
        core->add_global_attribute("user",
            boost::log::attributes::constant<std::string>(std::string(pw->pw_name)));
    }
#endif
    core->add_global_attribute("AppName", boost::log::attributes::current_process_name());
}

} // namespace logger

} // namespace rio
