ifndef RTOS
	RTOS := ../FreeRTOS
endif

C_SOURCES +=\
	$(RTOS)/croutine.c \
	$(RTOS)/event_groups.c \
	$(RTOS)/list.c \
	$(RTOS)/queue.c \
	$(RTOS)/stream_buffer.c \
	$(RTOS)/tasks.c \
	$(RTOS)/timers.c \
	$(RTOS)/portable/MemMang/heap_4.c\
	$(RTOS)/portable/GCC/ARM_CM4F/port.c

C_INCLUDES +=\
	-I$(RTOS)/include\
	-I$(RTOS)/portable/include\
	-I$(RTOS)/portable/GCC/ARM_CM4F
