#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/usb.h>
#include <linux/reboot.h>
#include "config.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("parazyd, based on kaepora's silk-guardian fork.");
MODULE_DESCRIPTION
    ("Shreds files and shuts down computer when a defined USB device is removed.");

static void panic_time(struct usb_device *usb)
{
	int i;
	struct device *dev;

	pr_info("Shredding...\n");
	for (i = 0; remove_files[i] != NULL; i++) {
		char *shred_argv[] =
		    { "/usr/bin/shred", "-f", "-u", "-n", shrediters,
			remove_files[i], NULL
		};
		call_usermodehelper(shred_argv[0], shred_argv, NULL,
				    UMH_WAIT_EXEC);
	}
	printk("...done\n");

	for (dev = &usb->dev; dev; dev = dev->parent)
		mutex_unlock(&dev->mutex);

	printk("Powering off...\n");
	kernel_power_off();
};

static int usb_match_device(struct usb_device *dev, const struct usb_device_id *id)
{
	if ((id->match_flags & USB_DEVICE_ID_MATCH_VENDOR) &&
	    (id->idVendor != le16_to_cpu(dev->descriptor.idVendor)))
		return 0;

	if ((id->match_flags & USB_DEVICE_ID_MATCH_PRODUCT) &&
	    (id->idProduct != le16_to_cpu(dev->descriptor.idProduct)))
		return 0;

	if ((id->match_flags & USB_DEVICE_ID_MATCH_DEV_LO) &&
	    (id->bcdDevice_lo > le16_to_cpu(dev->descriptor.bcdDevice)))
		return 0;

	if ((id->match_flags & USB_DEVICE_ID_MATCH_DEV_HI) &&
	    (id->bcdDevice_hi < le16_to_cpu(dev->descriptor.bcdDevice)))
		return 0;

	if ((id->match_flags & USB_DEVICE_ID_MATCH_DEV_CLASS) &&
	    (id->bDeviceClass != dev->descriptor.bDeviceClass))
		return 0;

	if ((id->match_flags & USB_DEVICE_ID_MATCH_DEV_SUBCLASS) &&
	    (id->bDeviceSubClass != dev->descriptor.bDeviceSubClass))
		return 0;

	if ((id->match_flags & USB_DEVICE_ID_MATCH_DEV_PROTOCOL) &&
	    (id->bDeviceProtocol != dev->descriptor.bDeviceProtocol))
		return 0;

	return 1;
};

static void usb_dev_removed(struct usb_device *dev)
{
	if (usb_match_device(dev, &my_device)) {
		pr_info("DEVICE REMOVED!\n");
		panic_time(dev);
	}
};

static int notify(struct notifier_block *self, unsigned long action, void *dev)
{
	switch (action) {
	case USB_DEVICE_REMOVE:
		usb_dev_removed(dev);
		break;
	default:
		break;
	}
	return 0;
};

static struct notifier_block usb_notify = {
	.notifier_call = notify,
};

static int __init protecc_init(void)
{
	usb_register_notify(&usb_notify);
	pr_info("Now watching USB devices.\n");
	return 0;
};

module_init(protecc_init);

static void __exit protecc_exit(void)
{
	usb_unregister_notify(&usb_notify);
	pr_info("No longer watching USB devices.\n");
};

module_exit(protecc_exit);
