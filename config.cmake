

include_directories(".")



file(GLOB CXX_SOURCES "./src/*.cpp")
#file(GLOB PHP_SOURCES "./src/*.php")


HHVM_EXTENSION("yaf" ${CXX_SOURCES})
HHVM_SYSTEMLIB("yaf" "./src/ext_yaf.php")
#HHVM_SYSTEMLIB("yaf" ${PHP_SOURCES})

SET_TARGET_PROPERTIES("yaf" PROPERTIES COMPILE_FLAGS "-g")
