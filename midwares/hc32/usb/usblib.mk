C_SOURCES +=\
	$(USBLIB)/usb_device_lib/device_core/usb_dev_core.c \
	$(USBLIB)/usb_device_lib/device_core/usb_dev_ctrleptrans.c \
	$(USBLIB)/usb_device_lib/device_core/usb_dev_driver.c \
	$(USBLIB)/usb_device_lib/device_core/usb_dev_int.c \
	$(USBLIB)/usb_device_lib/device_core/usb_dev_stdreq.c

C_INCLUDES +=\
	-I$(USBLIB)/usb_device_lib/device_core\
	-I$(USBLIB)/
