set(srcs
    "Uart.c"
	"UartHandler.c"
	"UartHandlerSimplified.c")

idf_component_register(SRCS "${srcs}" 
                    INCLUDE_DIRS "include" "../../main/include"
					REQUIRES pulse_counter)