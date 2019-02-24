/** @file
 * @brief     Librarian test suite
 *
 * @ingroup   RIO_CORIOLIS
 *
 * @copyright (C) 2017 PKB RIO Design Department
 *
 * $Id: $
 */

#include "Paths.h"

#include "aux/aux.h"

#include "coriolis/app/AppSentinel.h"
#include "coriolis/qt/StringUtils.h"

#include <QProcess>

#include <gtest/gtest.h>


#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

namespace rio
{

namespace app
{

namespace tests
{

std::string valid_executable_filename()
{
    static std::string filename;
    if(filename.empty())
    {
        using namespace boost::filesystem;
        path directory(PLUGIN_PATH);
        for (directory_iterator iter(directory), end; iter != end; ++iter)
        {
            std::string name = iter->path().string();
            boost::regex pattern(".*app_test_aux.*");
            if (boost::regex_match(name, pattern))
            {
                filename = name;
                break;
            }
        }
    }

    return filename;
}

const char* name = "appSentinelTest";
const char* msg = "Give me lock, bastard!";

TEST(AppSentinelTest, duplicateRun)
{
    QProcess auxProc;
    auxProc.start(
            strings::fromUtf8(valid_executable_filename()),
            QStringList() << name << msg);

    auxProc.waitForStarted();
    EXPECT_EQ(QProcess::Running, auxProc.state());
    boost::this_thread::sleep(boost::posix_time::milliseconds(50));

    AppSentinel sentinel(name);
    EXPECT_FALSE(sentinel.lock(msg));

    auxProc.waitForFinished();
    EXPECT_EQ(QProcess::NotRunning, auxProc.state());
    EXPECT_EQ(NO_ERROR, auxProc.exitCode());
}

} // tests

} // app

} // rio
