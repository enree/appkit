project(core_test)

gtest(${PROJECT_NAME}
    GLOBBING "*"
    RECURSIVE true
    QT Core
    LIBS appkit_core)
