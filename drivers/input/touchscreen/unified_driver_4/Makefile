

obj-y	+=  lgtp_common_driver.o
obj-y	+=  lgtp_model_config_i2c.o
obj-y	+=  lgtp_model_config_misc.o
obj-y	+=  lgtp_platform_api_i2c.o
obj-y	+=  lgtp_platform_api_misc.o
obj-y	+=  lgtp_platform_api_power.o

ifeq ($(CONFIG_JDI_INCELL), y)
obj-y 	+= S3320/
endif

ifeq ($(CONFIG_TOUCHSCREEN_UNIFIED_MELFAS_MIT300_PH2), y)
obj-y   += MIT300_PH2/
endif
