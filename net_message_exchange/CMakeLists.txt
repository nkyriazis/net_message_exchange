option(BUILD_SHARED_LIBS "Use shared user libraries" ON)
set(DEFINITIONS -DBOOST_ALL_NO_LIB)

if(BUILD_SHARED_LIBS)
	set(LIB_TYPE SHARED)
	set(DEFINITIONS ${DEFINITIONS} -DDLL)
else(BUILD_SHARED_LIBS)
	set(LIB_TYPE STATIC)
endif(BUILD_SHARED_LIBS)


include_directories("$ENV{CS529_BOOST}/include")
link_directories("$ENV{CS529_BOOST}/lib")
add_definitions(${DEFINITIONS})
set(net_message_exchange_sources net_message_exchange.h net_message_exchange.cpp)
add_library(net_message_exchange ${LIB_TYPE} ${net_message_exchange_sources})
target_link_libraries(net_message_exchange optimized boost_serialization-vc90-mt.lib boost_system-vc90-mt.lib)
target_link_libraries(net_message_exchange debug boost_serialization-vc90-mt-gd.lib boost_system-vc90-mt-gd.lib)