

include_directories(".")
include_directories("./src/")
include_directories("./src/util/")



#file(GLOB CXX_SOURCES "./src/*.cpp" "./src/response/*.cpp")
file(GLOB CXX_SOURCES "./src/*.cpp" "./src/response/*.cpp" "./src/requests/*.cpp" "./src/util/*.cpp" "./src/views/*.cpp" "./src/configs/*.cpp" "./src/routes/*.cpp")



HHVM_EXTENSION("yaf" ${CXX_SOURCES})
HHVM_SYSTEMLIB("yaf" "./src/ext_yaf.php")
#HHVM_SYSTEMLIB("yaf" ${PHP_SOURCES})

SET_TARGET_PROPERTIES("yaf" PROPERTIES COMPILE_FLAGS "-g")
