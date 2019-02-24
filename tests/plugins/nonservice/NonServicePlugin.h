/** @file
 * @brief      NonService plugin for testing.
 *
 * @ingroup RIO_CORIOLIS
 *
 * @copyright  (C) 2015 PKB RIO Design Department
 *
 * $Id: $
 */

#pragma once

#include "../DummyPluginInterface.h"

#include <QObject>

namespace rio
{

namespace plugins
{

/**
 * Dummy widget for testing
 */
class NonServicePlugin
    : public QObject
    , public DummyPluginInterface
{
    Q_OBJECT

#ifdef QT5_BUILD
    Q_PLUGIN_METADATA(IID "com.pkb-rio.dev.DummyPluginInterface")
#endif
    Q_INTERFACES(rio::plugins::DummyPluginInterface)

public:
    virtual QString text() const
    {
        return m_text.isEmpty() ? "non service" : m_text;;
    }
};

} // rio::newton

} // rio
