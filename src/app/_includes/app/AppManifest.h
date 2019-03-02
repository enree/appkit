/** @file
 * @brief Application description class
 *
 * @ingroup
 *
 * $Id: $
 */

#pragma once

#include <QVariantMap>

#include <boost/noncopyable.hpp>

#include <iostream>
#include <string>

namespace appkit
{

/**
 * This class contains application descriptions
 * One should inherit this class to provide additional agent information
 */
class AppManifest : private boost::noncopyable
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
    virtual std::string organization() const = 0;
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
};

/**
 * Put AppManifest info to stream
 */
std::ostream& operator<<(std::ostream& stream, const AppManifest& manifest);

/**
 * Convert manifest to variant hash
 */
QVariantMap toVariantMap(const AppManifest& manifest);

/**
 * Return Application manifest. One should provide function implementation
 * in his own project
 */
extern const AppManifest& manifest();

} // namespace appkit
