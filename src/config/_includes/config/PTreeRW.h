/**
 * @file
 * @brief PTree read/write routines. Implementation.
 *
 *
 * $Id: $
 *
**/

#pragma once

#include <boost/property_tree/ptree.hpp>

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/info_parser.hpp>

#include <iostream>
#include <set>
#include <string>

namespace appkit
{

namespace config
{

namespace format
{
    struct INI {};
    struct XML {};
    struct JSON {};
    struct INFO {};
} // format

template <typename InputType, typename Format>
struct Reader;

template <typename InputType>
struct Reader <InputType, format::INI>
{
    static boost::property_tree::ptree read(InputType input)
    {
        boost::property_tree::ptree ptree;
        boost::property_tree::ini_parser::read_ini(input, ptree);
        return ptree;
    }
};

template <typename InputType>
struct Reader <InputType, format::XML>
{
    static boost::property_tree::ptree read(InputType input)
    {
        boost::property_tree::ptree ptree;
        boost::property_tree::xml_parser::read_xml(input, ptree);
        return ptree;
    }
};

template <typename InputType>
struct Reader <InputType, format::JSON>
{
    static boost::property_tree::ptree read(InputType input)
    {
        boost::property_tree::ptree ptree;
        boost::property_tree::json_parser::read_json(input, ptree);
        return ptree;
    }
};

template <typename InputType>
struct Reader <InputType, format::INFO>
{
    static boost::property_tree::ptree read(InputType input)
    {
        boost::property_tree::ptree ptree;
        boost::property_tree::info_parser::read_info(input, ptree);
        return ptree;
    }
};

template <typename InputType, typename Format>
struct Writer;

template <typename InputType>
struct Writer <InputType, format::INI>
{
    static void write(const boost::property_tree::ptree& ptree, InputType input)
    {
        boost::property_tree::ini_parser::write_ini(input, ptree);
    }
};

template <typename InputType>
struct Writer <InputType, format::XML>
{
    static void write(const boost::property_tree::ptree& ptree, InputType input)
    {
        boost::property_tree::xml_parser::write_xml(input, ptree);
    }
};

template <typename InputType>
struct Writer <InputType, format::JSON>
{
    static void write(const boost::property_tree::ptree& ptree, InputType input)
    {
        boost::property_tree::json_parser::write_json(input, ptree);
    }
};

template <typename InputType>
struct Writer <InputType, format::INFO>
{
    static void write(const boost::property_tree::ptree& ptree, InputType input)
    {
        boost::property_tree::info_parser::write_info(input, ptree);
    }
};


/**
 * Read property tree from @a stream
 */
template <typename Format>
boost::property_tree::ptree fromStream(std::istream& stream)
{
    return Reader<std::istream&, Format>::read(stream);
}

/**
 * Read property tree from @a file
 */
template <typename Format>
boost::property_tree::ptree fromFile(const std::string& fileName)
{
    return Reader<const std::string&, Format>::read(fileName);
}

/**
 * Read property tree from @a string
 */
template <typename Format>
boost::property_tree::ptree fromString(const std::string& data)
{
    std::istringstream stream(data);
    return fromStream<Format>(stream);
}

/**
 * Read propert tree from command line.
 * @a argc, @a argv - command line args.
 * @a keys - unique keys collection.
 * @a strict - if flag is set, unrecognized options cause exception
 * @return ptree object.
 */
boost::property_tree::ptree fromCommandLine(int argc, char **argv,
                                            const std::set<std::string>& keys,
                                            bool strict);

/**
 * Write property tree to @a stream
 */
template <typename Format>
void toStream(const boost::property_tree::ptree& ptree, std::ostream& stream)
{
    Writer<std::ostream&, Format>::write(ptree, stream);
}

/**
 * Write property tree to @a file
 */
template <typename Format>
void toFile(const boost::property_tree::ptree& ptree, const std::string& fileName)
{
    Writer<const std::string&, Format>::write(ptree, fileName);
}

/**
 * Write property tree to string
 */
template <typename Format>
std::string toString(const boost::property_tree::ptree& ptree)
{
    std::ostringstream stream;
    toStream<Format>(ptree, stream);
    return stream.str();
}

} //config

} //appkit
