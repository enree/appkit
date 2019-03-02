/*
 * @file
 * @brief Test for PluginsLoader
 *
 *
 * $Id: $
 */

#include "DummyPluginInterface.h"

#include "exception/InvalidPath.h"
#include "plugins/PluginsHolder.h"
#include "plugins/PluginsLoader.h"
#include "plugins/ServicePluginInterface.h"

#include "gqtest"

#include <QSignalSpy>

#include <boost/bind.hpp>

#include <algorithm>

namespace appkit
{

namespace plugins
{

TEST(PluginsLoaderTest, loadAllInNonserviceMode)
{
    PluginsLoader pluginLoader;

    QSignalSpy probing(&pluginLoader, SIGNAL(probing(QString)));
    QSignalSpy loaded(&pluginLoader, SIGNAL(loaded(QString)));
    QSignalSpy failed(&pluginLoader, SIGNAL(failed(QString, QString)));
    QSignalSpy skipped(&pluginLoader, SIGNAL(skipped(QString)));

    pluginLoader.load(false, PLUGIN_PATH, "");
    pluginLoader.load(false, "", PLUGIN_PATH);

    EXPECT_EQ(1UL, pluginLoader.size());

    auto plugins = pluginLoader.plugins<DummyPluginInterface>();
    ASSERT_EQ(1UL, plugins.size());
    EXPECT_EQ(
        "non service",
        plugins.front()->instance<DummyPluginInterface>()->text());

    EXPECT_EQ(6, probing.size());
    EXPECT_EQ(1, loaded.size());
    EXPECT_EQ(0, failed.size());
    EXPECT_EQ(5, skipped.size());
}

TEST(PluginsLoaderTest, loadAllInServiceMode)
{
    PluginsLoader pluginLoader;

    QSignalSpy probing(&pluginLoader, SIGNAL(probing(QString)));
    QSignalSpy loaded(&pluginLoader, SIGNAL(loaded(QString)));
    QSignalSpy failed(&pluginLoader, SIGNAL(failed(QString, QString)));
    QSignalSpy skipped(&pluginLoader, SIGNAL(skipped(QString)));

    pluginLoader.load(true, PLUGIN_PATH);
    pluginLoader.load(true, "", PLUGIN_PATH);

    auto plugins = pluginLoader.plugins<DummyPluginInterface>();
    EXPECT_EQ(2UL, plugins.size());

    EXPECT_EQ(6, probing.size());
    EXPECT_EQ(2, loaded.size());
    EXPECT_EQ(0, failed.size());
    EXPECT_EQ(4, skipped.size());
}

TEST(PluginsLoaderTest, testPluginsLoaderIterator)
{
    PluginsLoader pluginLoader;
    pluginLoader.load(true, PLUGIN_PATH);
    EXPECT_EQ(2, std::distance(pluginLoader.begin(), pluginLoader.end()));
}

TEST(PluginsLoaderTest, testPluginsLoaderRange)
{
    PluginsLoader pluginLoader;
    pluginLoader.load(true, PLUGIN_PATH);

    bool nonServiceFound = std::any_of(
        pluginLoader.begin(),
        pluginLoader.end(),
        boost::bind(&PluginWrapper::className, _1)
            == "appkit::plugins::NonServicePlugin");

    EXPECT_TRUE(nonServiceFound);

    bool nonExistNotFound = std::any_of(
        pluginLoader.begin(),
        pluginLoader.end(),
        boost::bind(&PluginWrapper::className, _1) == "bla-bla-bla");
    EXPECT_FALSE(nonExistNotFound);
}

TEST(PluginsLoaderTest, loadFromNonexistingPath)
{
    PluginsLoader pluginLoader;
    EXPECT_THROW(
        pluginLoader.load(true, "The path never exists"),
        exception::InvalidPath);
}

TEST(PluginsLoaderTest, manualLoadAndSkipByClassName)
{
    PluginsLoader pluginLoader;

    QSignalSpy probing(&pluginLoader, SIGNAL(probing(QString)));
    QSignalSpy loaded(&pluginLoader, SIGNAL(loaded(QString)));
    QSignalSpy failed(&pluginLoader, SIGNAL(failed(QString, QString)));
    QSignalSpy skipped(&pluginLoader, SIGNAL(skipped(QString)));

    QObject* nonServicePlugin(new QObject);
    pluginLoader.add("appkit::plugins::NonServicePlugin", nonServicePlugin);

    pluginLoader.load(false, PLUGIN_PATH, "");

    auto plugins = pluginLoader.plugins<DummyPluginInterface>();
    EXPECT_EQ(0UL, plugins.size());

    EXPECT_EQ(3, probing.size());
    EXPECT_EQ(0, loaded.size());
    EXPECT_EQ(0, failed.size());
    EXPECT_EQ(3, skipped.size());
}

TEST(PluginsLoaderTest, testBasicPluginsHolderUsage)
{
    PluginsLoader pluginLoader;
    pluginLoader.load(true, PLUGIN_PATH);
    EXPECT_EQ(2UL, pluginLoader.size());

    PluginsHolder<ServicePluginInterface> servicePluginsHolder(pluginLoader);
    EXPECT_EQ(1UL, pluginLoader.size());

    EXPECT_EQ(
        1,
        std::
            distance(servicePluginsHolder.begin(), servicePluginsHolder.end()));

    auto it = servicePluginsHolder.begin();

    EXPECT_EQ("appkit::plugins::ServicePlugin", it->wrapper->className());

    ++it;
    EXPECT_EQ(it, servicePluginsHolder.end());
}

TEST(PluginsLoaderTest, testPluginsHolderAsRange)
{
    PluginsLoader pluginLoader;
    pluginLoader.load(true, PLUGIN_PATH);
    EXPECT_EQ(2UL, pluginLoader.size());

    typedef PluginsHolder<DummyPluginInterface> DummyInterfaceHolder;
    DummyInterfaceHolder servicePluginsHolder(pluginLoader);
    EXPECT_EQ(0UL, pluginLoader.size());

    // Just check if it compiles
    for (const auto& iface: servicePluginsHolder)
    {
        iface()->setText("dummy");
    }

    for (const auto& iface: servicePluginsHolder)
    {
        EXPECT_EQ("dummy", iface()->text());
    }
}

} // namespace plugins

} // namespace appkit
