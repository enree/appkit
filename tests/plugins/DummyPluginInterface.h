/** @ingroup RIO_CORIOLIS
 *  @file
 *  @brief Dummy plugin interface
 *
 *  @copyright  (C) 2014 PKB RIO Software Development Department
 *
 *  $Id: $
 */

#pragma once

#include <QtPlugin>

namespace rio
{

namespace plugins
{

/**
 * Dummy plugin interface for testing.
 */
class DummyPluginInterface
{
public:
    virtual QString text() const = 0;
    virtual void setText(const QString& text) {m_text = text;}
    virtual ~DummyPluginInterface() {}

protected:
    QString m_text;
};

} // plugins

} // rio

Q_DECLARE_INTERFACE(rio::plugins::DummyPluginInterface,
    "com.pkb-rio.dev.DummyPluginInterface")
