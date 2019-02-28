/**
 * @ingroup RIO_CORIOLIS
 * @file
 * @brief      Class for simplifing ptree parsing
 * @copyright  (C) 2015 PKB RIO Design Department
 *
 * $Id: $
 */

#include "ConfigParser.h"

#include "exception/DuplicateEntity.h"
#include "exception/KeyInfo.h"

#include "PTreeRW.h"
#include "PTreeUtils.h"

#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <boost/algorithm/string/trim.hpp>
#include <boost/range/algorithm/transform.hpp>

#include <algorithm>

namespace appkit
{

namespace config
{

namespace
{

const int USAGE_INDENT_SIZE = 4;
const size_t MIN_DESCRIPTION_OFFSET = 30;

class ProgramOptionsHelper
{
public:
    ProgramOptionsHelper() = default;

    ProgramOptionsHelper(
        std::string key, std::string shortKey, std::string description)
        : m_key(std::move(key))
        , m_shortKey(std::move(shortKey))
        , m_description(std::move(description))
    {
    }

    std::string printable() const
    {
        return std::string("--") + m_key + ", -" + m_shortKey;
    }

    std::string boostOption() const
    {
        return m_key + "," + m_shortKey;
    }

    std::string key() const
    {
        return m_key;
    }

    std::string description() const
    {
        return m_description;
    }

private:
    std::string m_key;
    std::string m_shortKey;
    std::string m_description;
};

const ProgramOptionsHelper&
programOptionsHelper(ConfigParser::ProgramOption option)
{
    // Return ref to map value is healthy while it is defined as static
    static std::map<ConfigParser::ProgramOption, ProgramOptionsHelper> helpers;
    if (helpers.empty())
    {
        helpers[ConfigParser::HELP]
            = ProgramOptionsHelper("help", "h", APPKIT_TR("Print help"));
        helpers[ConfigParser::VERSION]
            = ProgramOptionsHelper("version", "v", APPKIT_TR("Print version"));
        helpers[ConfigParser::CONFIG] = ProgramOptionsHelper(
            "config", "c", APPKIT_TR("Set config file path"));
    }

    return helpers[option];
}

void printOption(
    const std::string& key,
    int descriptionOffset,
    const std::string& description,
    std::ostream& stream,
    int indent)
{
    stream << std::setw(USAGE_INDENT_SIZE * indent) << "" << std::left
           << std::setw(descriptionOffset) << key
           << boost::trim_right_copy(description) << std::endl;
}

} // namespace

ConfigParser& ConfigParser::addPtree(
    const boost::property_tree::ptree& ptree, const std::string& source)
{
    std::set<std::string> leaves;
    pullLeaves(leaves, ptree);

    for (const auto& leaf: leaves)
    {
        m_keySources[leaf] = source;
    }

    if (m_config.empty())
    {
        m_config = ptree;
    }
    else
    {
        m_config = merge(m_config, ptree);
    }
    return *this;
}

void ConfigParser::readAll() const
{
    try
    {
        // Read single options
        std::for_each(
            m_options.begin(),
            m_options.end(),
            boost::bind(
                &Option::read,
                boost::bind(&Options::value_type::second, _1),
                m_config));
    }
    catch (const exception::InvalidConfigEntry& ex)
    {
        const std::vector<std::string>* keysInfo
            = boost::get_error_info<exception::KeysInfo>(ex);
        std::string keys;
        if (keysInfo != nullptr && keysInfo->size() == 2)
        {
            const std::string& key = keysInfo->at(0);
            std::string source;
            auto it = m_keySources.find(key);
            if (it == m_keySources.end())
            {
                source = APPKIT_TR("unknown");
            }
            else
            {
                source = it->second;
            }
            keys = APPKIT_TR(
                "Source: {1}, key: {2}, value: {3} ",
                source,
                key,
                keysInfo->at(1));
        }

        std::string what;
        const exception::ForeignExceptionDetails* details
            = boost::get_error_info<exception::ForeignExceptionInfo>(ex);
        if (details != nullptr)
        {
            what = details->what;
        }

        std::cout << APPKIT_TR(
            "Unable to parse settings. {1}\n{2}\nUsage:\n", keys, what);
        printUsage();
        throw;
    }
    catch (const boost::property_tree::ptree_error& ex)
    {
        std::cout
            << APPKIT_TR("Unable to parse settings. {1}\nUsage:\n", ex.what());
        printUsage();
        throw;
    }
}

std::set<std::string> ConfigParser::keys() const
{
    std::set<std::string> optionsKeys;
    for (const auto& value: m_options)
    {
        std::set<std::string> subkeys = value.second->keys();
        optionsKeys.insert(subkeys.begin(), subkeys.end());
    }
    return optionsKeys;
}

ConfigParser::ProgramOptions ConfigParser::infoOptions(int argc, char** argv)
{
    using namespace boost::program_options;

    options_description desc("Info options");
    desc.add_options()(
        programOptionsHelper(ConfigParser::HELP).boostOption().c_str(),
        "Print help message")(
        programOptionsHelper(ConfigParser::VERSION).boostOption().c_str(),
        "Print version");

    variables_map vm;
    command_line_parser parser(argc, argv);
    parser.allow_unregistered();
    store(parser.options(desc).run(), vm);

    ConfigParser::ProgramOptions options;
    if (vm.count(programOptionsHelper(ConfigParser::HELP).key()) > 0)
    {
        options.insert(ConfigParser::HELP);
    }

    if (vm.count(programOptionsHelper(ConfigParser::VERSION).key()) > 0)
    {
        options.insert(ConfigParser::VERSION);
    }

    return options;
}

void ConfigParser::printUsage() const
{
    std::cout << std::endl << APPKIT_TR("Program options") << std::endl;
    printProgramOptionsUsage(std::cout, 1);

    std::cout << std::endl << APPKIT_TR("Config options") << std::endl;
    printConfigOptionsUsage(std::cout, 1);
}

void ConfigParser::printProgramOptionsUsage(
    std::ostream& stream, int indent) const
{
    const std::string help_keys
        = programOptionsHelper(ConfigParser::HELP).printable();
    const std::string help_desc
        = programOptionsHelper(ConfigParser::HELP).description();

    const std::string version_keys
        = programOptionsHelper(ConfigParser::VERSION).printable();
    const std::string version_desc
        = programOptionsHelper(ConfigParser::VERSION).description();

    const std::string config_keys
        = programOptionsHelper(ConfigParser::CONFIG).printable();
    const std::string config_desc
        = programOptionsHelper(ConfigParser::CONFIG).description();

    size_t keyMaxSize = std::
        max(std::max(help_keys.size(), version_keys.size()),
            config_keys.size());

    const size_t descriptionOffset
        = std::max(MIN_DESCRIPTION_OFFSET, keyMaxSize + USAGE_INDENT_SIZE);

    printOption(help_keys, descriptionOffset, help_desc, stream, indent);
    printOption(version_keys, descriptionOffset, version_desc, stream, indent);
    printOption(config_keys, descriptionOffset, config_desc, stream, indent);
}

void ConfigParser::printConfigOptionsUsage(
    std::ostream& stream, int indent) const
{
    size_t keyMaxSize = 0;
    size_t descMaxSize = 0;

    for (const auto& value: m_options)
    {
        const auto& option = value.second;
        keyMaxSize = std::max(keyMaxSize, option->key().size());
        descMaxSize = std::max(descMaxSize, option->description(indent).size());
    }

    for (const auto& value: m_options)
    {
        const auto& option = value.second;
        printOption(
            option->key(),
            std::max(MIN_DESCRIPTION_OFFSET, keyMaxSize + USAGE_INDENT_SIZE),
            option->description(indent),
            stream,
            indent);
    }
}

std::string ConfigParser::toJson() const
{
    return config::toString<config::format::JSON>(m_config);
}

void ConfigParser::verifyKey(const std::string& key) const
{
    if (m_options.count(key) > 0)
    {
        BOOST_THROW_EXCEPTION(
            exception::DuplicateEntity() << exception::toKeyInfo(key));
    }
}

std::string
configFileName(int argc, char** argv, const std::string& defaultConfigFile)
{
    std::string configFileName;
    ConfigParser configParser;
    configParser
        .addOption(
            programOptionsHelper(ConfigParser::CONFIG).key(),
            APPKIT_TR("Config file path"),
            configFileName)
        .setDefaultValue(defaultConfigFile);
    configParser
        .addPtree(
            fromCommandLine(argc, argv, configParser.keys(), false),
            APPKIT_TR("command line"))
        .readAll();

    return configFileName;
}

std::string cmdLineToString(int argc, char** argv)
{
    std::ostringstream stream;
    std::ostream_iterator<char*> it(stream, " ");
    std::copy(argv, argv + argc, it);
    return stream.str();
}

} // namespace config

} // namespace appkit
