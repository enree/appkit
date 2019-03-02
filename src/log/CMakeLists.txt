item(appkit_log)

# This definition will be auto included with addModule function
add_definitions(-DBOOST_LOG_DYN_LINK)
set(Boost_USE_MULTITHREADED ON)

sharedLib(${PROJECT_NAME}
    GLOBBING "*"
    RECURSIVE true
    BOOST log system thread log_setup
    COMPONENT ${PROJECT_NAME})
threads(${PROJECT_NAME})

lint(${PROJECT_NAME})
