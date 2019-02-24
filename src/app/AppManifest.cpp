/** @file
 * @brief
 *
 * @ingroup
 *
 * @copyright  (C) 2016 PKB RIO Design Department
 *
 * $Id: $
 */

#include "AppManifest.h"
#include "Defs.h"

#include "coriolis/Translate.h"
#include "coriolis/qt/StringUtils.h"

#include <QCoreApplication>

#include <boost/format.hpp>

namespace rio
{

namespace app
{

std::string AppManifest::organization() const
{
    static std::string value = CORIOLIS_TR("CJSC PKB RIO");
    return value;
}

std::string AppManifest::version() const
{
    static std::string value
        = (boost::format("%1$s (%2$s)") % VERSION % REVISION).str();
    return value;
}

std::string AppManifest::build() const
{
    static std::string value
        = (boost::format("%1$s (%2$s)") % BUILD_NUMBER % BUILD_DATE).str();
    return value;
}

std::string AppManifest::copyright() const
{
    static std::string value
        = CORIOLIS_TR("Copyright (C) 2013-{1}, {2}", BUILD_YEAR, organization());
    return value;
}

std::string AppManifest::configBase() const
{
    return id();
}

std::string AppManifest::metapackage() const
{
    const std::string package(METAPACKAGE_NAME);
    return package.empty() ? id() : package;
}

std::ostream& operator<<(std::ostream& stream, const AppManifest& manifest)
{
    stream << manifest.name() << std::endl
           << "\t" << manifest.description() << std::endl
           << "\t" << CORIOLIS_TR("Version: {1}", manifest.version()) << std::endl
           << "\t" << CORIOLIS_TR("Build: {1}", manifest.build()) << std::endl
           << "\t" << manifest.copyright() << std::endl;

    return stream;
}

QVariantMap toVariantMap(const AppManifest &manifest)
{
    QVariantMap context;
    context["name"] = strings::fromUtf8(manifest.name());
    context["package"] = strings::fromUtf8(manifest.metapackage());
    context["version"] = strings::fromUtf8(manifest.version());
    context["build"] = QCoreApplication::translate("Manifest", "build %1")
                           .arg(strings::fromUtf8(manifest.build()));
    context["qtversion"] = QString("Qt: %1").arg(QT_VERSION_STR);
    context["compiler"]
        = QString("%1: %2").arg(COMPILER_ID, COMPILER_VERSION);
    context["copyright"] = strings::fromUtf8(manifest.copyright());
    return context;
}

} // namespace app

} // namespace rio
