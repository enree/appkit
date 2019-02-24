# Generate logger for project
# name is a name of logger, for example if name is riolog, the following macros
# will be declared
# RIOLOG_DEBUG etc.
# src is a variable where generated Log.cpp will be added
# if we have fourth parameter, it used for specific log_path

macro(generateLogger name src path)
    if("${ARGN}" STREQUAL "")
        set(log_path ${CORIOLIS_PATH}/templates/logging)
    else()
        set(log_path ${ARGV3})
    endif()

    set(LOGGER_ID "${name}Logger")
    set(LOGGER_NAME ${name})
    string(TOUPPER ${name} LOGGER_NAME_UPPER)

    configure_file(${log_path}/Log.h.in ${path}/Log.h @ONLY)
    configure_file(${log_path}/Log.cpp.in ${path}/Log.cpp @ONLY)

    list(APPEND ${src} ${path}/Log.cpp)
endmacro()

function(cat IN_FILE OUT_FILE)
    file(READ ${IN_FILE} CONTENTS)
    file(APPEND ${OUT_FILE} "${CONTENTS}")
    get_filename_component(PART "${IN_FILE}" NAME)
    add_custom_target(${PROJECT_NAME}_${PART} SOURCES ${IN_FILE})
endfunction()

# Create logger config
function(createLoggerConfig TARGET)
    option(_log_console "Log into console" ON)
    option(_log_syslog "Log into syslog" ON)

    set(TEMPLATES_PATH ${CORIOLIS_PATH}/templates/logging )
    set(OUTPUT_PATH "${GENERATED_CONFIG}/${TARGET}")
    set(OUTPUT_IN "${CMAKE_CURRENT_BINARY_DIR}/tmp/log.ini.in")
    file(REMOVE ${OUTPUT_IN})
    cat("${TEMPLATES_PATH}/_core.log.ini.in" ${OUTPUT_IN})

    if(_log_console)
        cat("${TEMPLATES_PATH}/_console.log.ini.in" ${OUTPUT_IN})
    endif()

    if(_log_syslog)
        cat("${TEMPLATES_PATH}/_syslog.log.ini.in" ${OUTPUT_IN})

        if (NOT SYSLOG_IP)
            set(SYSLOG_IP "127.0.0.1")
        endif()
    endif()

    if(NOT "${ARGN}" STREQUAL "")
        cat(${ARGN} ${OUTPUT_IN})
    endif()

    configure_file(
        "${OUTPUT_IN}"
        "${OUTPUT_PATH}/${TARGET}.log.ini"
        @ONLY)
    configure_file(
        "${TEMPLATES_PATH}/logrotate.conf.in"
        "${OUTPUT_PATH}/${TARGET}.logrotate.conf"
        @ONLY)
endfunction()

