set(srcs
    "RadarTrigger.c")

idf_component_register(SRCS "${srcs}" 
                    INCLUDE_DIRS "include" "../../main/include"
					PRIV_REQUIRES driver esp_timer)