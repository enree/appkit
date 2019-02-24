item(appkit_log)

# This definition will be auto included with addModule function
add_definitions(-DBOOST_LOG_DYN_LINK)
set(Boost_USE_MULTITHREADED ON)

sources(SOURCES logcore)
sources(SOURCES printers GLOBBING "printers/*")

sharedLib(${PROJECT_NAME}
    SOURCES ${logcore} ${printers}
    BOOST log system thread log_setup
    COMPONENT ${PROJECT_NAME})
useThreads()


lint(${PROJECT_NAME})
