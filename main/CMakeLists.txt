set(srcs
    "Main.c"
    "RadarTrigger.c"
	"PulseCounter.c"
	"LedControl.c")

idf_component_register(SRCS "${srcs}" 
                    INCLUDE_DIRS "../include")