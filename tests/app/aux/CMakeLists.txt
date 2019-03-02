project(app_test_aux)

executable(
    ${PROJECT_NAME}
    GLOBBING "*" 
    RECURSIVE false
    LIBS appkit_app pthread
    BOOST thread system log
    NOINSTALL true)
