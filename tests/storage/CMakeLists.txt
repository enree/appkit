project(storage_test)

gtest(${PROJECT_NAME}
    GLOBBING "*" 
    QT Core
    LIBS appkit_storage appkit_serialization)
