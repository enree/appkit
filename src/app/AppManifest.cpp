/** @file
 * @brief
 *
 * @ingroup
 *
 *
 * $Id: $
 */

#include "app/AppManifest.h"
#include "Defs.h"

#include "appkit/Translate.h"
#include "qt/Strings.h"

#include <QCoreApplication>

#include <boost/format.hpp>

namespace appkit
{

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
        = APPKIT_TR("Copyright (C) 2018-{1}, {2}", BUILD_YEAR, organization());
    return value;
}

std::string AppManifest::configBase() const
{
    return id();
}

std::ostream& operator<<(std::ostream& stream, const AppManifest& manifest)
{
    stream << manifest.name() << std::endl
           << "\t" << manifest.description() << std::endl
           << "\t" << APPKIT_TR("Version: {1}", manifest.version()) << std::endl
           << "\t" << APPKIT_TR("Build: {1}", manifest.build()) << std::endl
           << "\t" << manifest.copyright() << std::endl;

    return stream;
}

QVariantMap toVariantMap(const AppManifest& manifest)
{
    QVariantMap context;
    context["name"] = strings::fromUtf8(manifest.name());
    context["version"] = strings::fromUtf8(manifest.version());
    context["build"] = QCoreApplication::translate("Manifest", "build %1")
                           .arg(strings::fromUtf8(manifest.build()));
    context["qtversion"] = QString("Qt: %1").arg(QT_VERSION_STR);
    context["compiler"] = QString("%1: %2").arg(COMPILER_ID, COMPILER_VERSION);
    context["copyright"] = strings::fromUtf8(manifest.copyright());
    return context;
}

} // namespace appkit
