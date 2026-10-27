C_SOURCES += $(FATFS)/diskio.c\
			$(FATFS)/ff.c\
			$(FATFS)/port/fatfs_usbd.c\

C_INCLUDES += -I$(FATFS)\
				-I$(FATFS)/port


