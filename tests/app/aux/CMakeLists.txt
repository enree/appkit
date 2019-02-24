project(app_test_aux)

executable(
    ${PROJECT_NAME}
    GLOBBING "*" 
    RECURSIVE false
    LIBS coriolis_app pthread
    BOOST thread system
    NOINSTALL true)
