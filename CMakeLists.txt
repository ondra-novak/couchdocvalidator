cmake_minimum_required(VERSION 2.8)
add_compile_options(-std=c++11)
add_executable (couch_jschema jSchema.cpp) 
target_link_libraries (couch_jschema LINK_PUBLIC imtjson)

install(TARGETS couch_jschema
        DESTINATION "bin"
        ) 
INSTALL(FILES couchdb_jschema.ini
        DESTINATION "/etc/couchdb/default.d"
        ) 
        
install (SCRIPT "${CMAKE_SOURCE_DIR}/PostInstall.cmake")
