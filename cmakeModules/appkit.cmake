if (DEFINED APPKIT_PATH)
    return()
endif()
    
set (APPKIT_PATH ${CMAKE_CURRENT_LIST_DIR}/..)


include(${APPKIT_PATH}/src/translations/translate.cmake)
