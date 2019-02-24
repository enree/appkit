/** @file
 * @brief Application description class
 *
 * @ingroup
 *
 * @copyright  (C) 2016 PKB RIO Design Department
 *
 * $Id: $
 */

#pragma once

#include <QVariantMap>

#include <boost/noncopyable.hpp>

#include <string>
#include <iostream>

namespace appkit
{

namespace app
{

/**
 * This class contains application descriptions
 * One should inherit this class to provide additional agent information
 */
class AppManifest:
        private boost::noncopyable
{
public:
    /**
     * Application id (binary name)
     */
    virtual std::string id() const = 0;
    /**
     * Application display name
     */
    virtual std::string name() const = 0;
    /**
     * Application brief summary
     */
    virtual std::string description() const = 0;
    /**
     * Organization display name
     */
    virtual std::string organization() const;
    /**
     * Version string
     */
    virtual std::string version() const;
    /**
     * Build string
     */
    virtual std::string build() const;
    /**
     * Copyright string
     */
    virtual std::string copyright() const;
    /**
     * Base part of config files
     */
    virtual std::string configBase() const;
    /**
     * Metapackage name
     */
    virtual std::string metapackage() const;
};

/**
 * Put AppManifest info to stream
 */
std::ostream& operator<<(std::ostream& stream, const AppManifest& manifest);

/**
 * Convert manifest to variant hash
 */
QVariantMap toVariantMap(const app::AppManifest& manifest);

} // namespace app

/**
 * Return Application manifest. One should provide function implementation
 * in his own project
 */
extern const app::AppManifest& manifest();

} // namespace appkit
