/** @file
 * @brief      Service plugin for testing.
 *
 * @ingroup RIO_CORIOLIS
 *
 * @copyright  (C) 2015 PKB RIO Design Department
 *
 * $Id: $
 */

#pragma once

#include "coriolis/plugins/ServicePluginInterface.h"

#include "../DummyPluginInterface.h"

#include <QObject>
#include <QtPlugin>

namespace rio
{

namespace plugins
{

/**
 * Dummy widget for testing
 */

class ServicePlugin
    : public QObject
    , public ServicePluginInterface
    , public DummyPluginInterface
{
    Q_OBJECT

#ifdef QT5_BUILD
    Q_PLUGIN_METADATA(IID "com.pkb-rio.dev.ServicePluginInterface")
#endif
    Q_INTERFACES(rio::plugins::ServicePluginInterface rio::plugins::DummyPluginInterface)

public:
    ServicePlugin()
    {
        m_text = "service";
    }

    virtual QString text() const
    {
        return m_text;
    }
};

} // rio::newton

} // rio
