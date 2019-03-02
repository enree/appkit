/** @file
 * @brief  Translation utilities for std strings. Declaration
 *
 * $Id: $
 */

#include "translations/Translations.h"

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

namespace appkit
{

namespace translate
{

void installBoostTranslation(
    const std::string& path, const std::vector<std::string>& domains)
{
    boost::locale::generator gen;
    gen.add_messages_path(path);

    for (const std::string& domain: domains)
    {
        gen.add_messages_domain(domain);
    }

    // Create an object with default locale
    std::locale locale = std::locale::global(gen(""));
    UNUSED(locale);
}

bool matchTranslationFiles(const boost::filesystem::path& path)
{
    const boost::regex mo_filter(".*\\.mo");
    boost::smatch what;
    return boost::regex_match(path.filename().string(), what, mo_filter);
}

std::vector<std::string> installBoostTranslation(const std::string& path)
{
    std::vector<std::string> domains;

    boost::filesystem::recursive_directory_iterator first(path);
    boost::filesystem::recursive_directory_iterator last;

    for (; first != last; ++first)
    {
        if (boost::filesystem::is_regular_file(*first)
            && matchTranslationFiles(*first))
        {
            domains.push_back(first->path().stem().string());
        }
    }

    installBoostTranslation(path, domains);
    return domains;
}

} // namespace translate

} // namespace appkit
