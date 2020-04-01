#idf_component_register(SRCS "Tsensor.c" INCLUDE_DIRS include)

set(COMPONENT_ADD_INCLUDEDIRS include)
set(COMPONENT_SRCS "Tsensor.c")
set(COMPONENT_PRIV_REQUIRES "GPIO_Extender")

register_component()