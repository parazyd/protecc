/* Set your chosen USB device's ID here. Use lsusb to find it. */
static const struct usb_device_id my_device = {USB_DEVICE(0x1234, 0x5678)};

/* Files protecc will remove upon chosen USB removal. */
static char *remove_files[] = {
	"/home/luther/.nyan",
	NULL, /* Must be NULL terminated. */
};

/* How many times you want to shred the files. More = longer. */
static char *shrediters = "3";
