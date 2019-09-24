set(srcs
    "Main.c"
    "RadarTrigger.c"
	"UartHandler.c"
	"PulseCounter.c"
	"LedControl.c"
	"Uart.c"
	"UartHandlerSimplified.c")

idf_component_register(SRCS "${srcs}" 
                    INCLUDE_DIRS "../include")