

include_directories(".")
include_directories("./src/")



#file(GLOB CXX_SOURCES "./src/*.cpp" "./src/response/*.cpp")
file(GLOB CXX_SOURCES "./src/*.cpp" "./src/response/*.cpp" "./src/requests/*.cpp")



HHVM_EXTENSION("yaf" ${CXX_SOURCES})
HHVM_SYSTEMLIB("yaf" "./src/ext_yaf.php")
#HHVM_SYSTEMLIB("yaf" ${PHP_SOURCES})

SET_TARGET_PROPERTIES("yaf" PROPERTIES COMPILE_FLAGS "-g")
