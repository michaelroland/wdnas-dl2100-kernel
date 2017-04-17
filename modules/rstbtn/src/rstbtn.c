/*
**
** Reset Button Kernel Module for WD DL2100 NAS
** 
** Copyright (c) 2017 Michael Roland <mi.roland@gmail.com>
** 
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
** 
*/

#include <linux/acpi.h>
#include <linux/bitops.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/stat.h>
#include <linux/stddef.h>
#include <linux/workqueue.h>
#include <asm/bitops.h>
#include <asm/io.h>
//#include <acpi/acpi.h>


#define THIS_MODULE_DESCRIPTION "Reset Button Kernel Module for WD DL2100 NAS"
#define THIS_MODULE_VERSION     "1.0"
#define THIS_MODULE_AUTHOR      "Michael Roland <mi.roland@gmail.com>"
#define THIS_MODULE_LICENSE     "GPL"
#ifdef KBUILD_MODNAME
    #define THIS_MODULE_NAME    KBUILD_MODNAME
#else
    #include <libgen.h>
    #define THIS_MODULE_NAME    basename(__FILE__)
#endif


#define RSTBTN_BTNPWRUP1_REG 0x500UL
#define RSTBTN_BTNPWRUP2_REG 0x504UL
#define RSTBTN_BTNPRESS_REG  0x588UL
#define RSTBTN_BTN_RST_BIT 2


#define RSTBTN_INPUT_TYPE EV_KEY
#define RSTBTN_INPUT_CODE BTN_9

#define RSTBTN_GPE_NUMBER 0x00000012
#define RSTBTN_GPE_TYPE   0x00000000


static bool rstbtn_power_on_status;
static struct input_dev * rstbtn_input_dev;
static struct work_struct rstbtn_irq_work;

//#define RSTBTN_DEVICE_PERM  (S_IWUGO | S_IRUGO)
#define RSTBTN_DEVICE_PERM  (S_IWUSR | S_IRUGO)
#define RSTBTN_DEVICE_ATTR(_modifier, _name) \
        _modifier ssize_t rstbtn_##_name##_show(struct device * dev, struct device_attribute * attr, char * buf); \
        _modifier ssize_t rstbtn_##_name##_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t count); \
        _modifier DEVICE_ATTR(rstbtn_##_name, RSTBTN_DEVICE_PERM, rstbtn_##_name##_show, rstbtn_##_name##_store)
RSTBTN_DEVICE_ATTR(static, simulate_button);
RSTBTN_DEVICE_ATTR(static, poweron_status);
RSTBTN_DEVICE_ATTR(static, current_status);


static bool rstbtn_was_pressed_at_boot(void) {
    u8 buttonPowerupReg;
  
    buttonPowerupReg = inb(RSTBTN_BTNPWRUP1_REG);
    if ((buttonPowerupReg & BIT(RSTBTN_BTN_RST_BIT)) != 0) {
        buttonPowerupReg = inb(RSTBTN_BTNPWRUP2_REG);
        return (buttonPowerupReg & BIT(RSTBTN_BTN_RST_BIT)) != 0;
    }
    
    return false;
}

static bool rstbtn_is_pressed(void) {
    u8 buttonPressedReg;
    
    buttonPressedReg = inb(RSTBTN_BTNPRESS_REG);
    return (buttonPressedReg & BIT(RSTBTN_BTN_RST_BIT)) == 0;
}

static void rstbtn_process_button_event(bool event) {
    printk(KERN_INFO THIS_MODULE_NAME ": reset button event: keycode=%u, pressed=%s\n",
            RSTBTN_INPUT_CODE,
            event ? "true" : "false");
    
    input_event(rstbtn_input_dev, RSTBTN_INPUT_TYPE, RSTBTN_INPUT_CODE, event != false);
    input_event(rstbtn_input_dev, EV_SYN, SYN_REPORT, 0);
}


static void rstbtn_interrupt_work(struct work_struct * work) {
    bool buttonPressed;

    buttonPressed = rstbtn_is_pressed();
    
    if (rstbtn_input_dev) {
        rstbtn_process_button_event(buttonPressed);
    }
    
    acpi_finish_gpe(NULL, RSTBTN_GPE_NUMBER);
}


static u32 rstbtn_gpe_handler(acpi_handle gpe_device, u32 gpe_number, void * context) {
    printk(KERN_INFO THIS_MODULE_NAME ": gpe %u handler\n", gpe_number);
    
    if (gpe_number == RSTBTN_GPE_NUMBER) {
        if (rstbtn_input_dev) {
            queue_work_on(2, system_wq, &rstbtn_irq_work);
        }
    }
    
    return 1;
}


static int __init rstbtn_init(void) {
    int error;
    
    printk(KERN_INFO THIS_MODULE_NAME " version " THIS_MODULE_VERSION " initializing...\n");
    
    rstbtn_power_on_status = rstbtn_was_pressed_at_boot();
    if (rstbtn_power_on_status) {
        printk(KERN_INFO THIS_MODULE_NAME ": reset button was held upon boot\n");
    }
  
    rstbtn_input_dev = input_allocate_device();
    if (rstbtn_input_dev == NULL) {
        printk(KERN_ERR THIS_MODULE_NAME ": failed to allocate input device\n");
        return -ENXIO;
    }

    rstbtn_input_dev->name = THIS_MODULE_NAME;    
    set_bit(RSTBTN_INPUT_TYPE, rstbtn_input_dev->evbit);
    set_bit(RSTBTN_INPUT_CODE, rstbtn_input_dev->keybit);
    
    error = input_register_device(rstbtn_input_dev);
    if (error) {
        printk(KERN_ERR THIS_MODULE_NAME ": failed to register input device (%d)\n", error);
    } else {
        if ((error = device_create_file(&rstbtn_input_dev->dev, &dev_attr_rstbtn_simulate_button)) == 0) {
            if ((error = device_create_file(&rstbtn_input_dev->dev, &dev_attr_rstbtn_poweron_status)) == 0) {
                if ((error = device_create_file(&rstbtn_input_dev->dev, &dev_attr_rstbtn_current_status)) == 0) {
                    INIT_WORK(&rstbtn_irq_work, rstbtn_interrupt_work);
                    
                    if ((error = acpi_install_gpe_handler(NULL, RSTBTN_GPE_NUMBER, RSTBTN_GPE_TYPE, rstbtn_gpe_handler, NULL)) != 0) {
                        printk(KERN_ERR THIS_MODULE_NAME ": acpi_install_gpe_handler failed for gpe %u: 0x%X\n",
                                RSTBTN_GPE_NUMBER,
                                error);
                    } else if ((error = acpi_clear_gpe(NULL, RSTBTN_GPE_NUMBER)) != 0) {
                        printk(KERN_ERR THIS_MODULE_NAME ": acpi_clear_gpe failed for gpe %u: 0x%X\n",
                                RSTBTN_GPE_NUMBER,
                                error);
                    } else if ((error = acpi_enable_gpe(NULL, RSTBTN_GPE_NUMBER)) != 0) {
                        printk(KERN_ERR THIS_MODULE_NAME ": acpi_enable_gpe failed for gpe %u: 0x%X\n",
                                RSTBTN_GPE_NUMBER,
                                error);
                    } else {
                        return 0;
                    }
                    
                    error = -ENXIO;
                    device_remove_file(&rstbtn_input_dev->dev, &dev_attr_rstbtn_current_status);
                }
                device_remove_file(&rstbtn_input_dev->dev, &dev_attr_rstbtn_poweron_status);
            }
            device_remove_file(&rstbtn_input_dev->dev, &dev_attr_rstbtn_simulate_button);
        }
        input_unregister_device(rstbtn_input_dev);
        rstbtn_input_dev = NULL;
    }
    if (rstbtn_input_dev) {
        input_free_device(rstbtn_input_dev);
        rstbtn_input_dev = NULL;
    }
    return error;
}


static void __exit rstbtn_exit(void) {
    int error;
    
    flush_work(&rstbtn_irq_work);
    
    if ((error = acpi_disable_gpe(NULL, RSTBTN_GPE_NUMBER)) != 0) {
        printk(KERN_ERR THIS_MODULE_NAME ": acpi_disable_gpe failed for gpe %u: 0x%X\n", RSTBTN_GPE_NUMBER, error);
    } else if ((error = acpi_clear_gpe(NULL, RSTBTN_GPE_NUMBER)) != 0) {
        printk(KERN_ERR THIS_MODULE_NAME ": acpi_clear_gpe failed for gpe %u: 0x%X\n", RSTBTN_GPE_NUMBER, error);
    } else if ((error = acpi_remove_gpe_handler(NULL, RSTBTN_GPE_NUMBER, rstbtn_gpe_handler)) != 0) {
        printk(KERN_ERR THIS_MODULE_NAME ": acpi_remove_gpe_handler failed for gpe %u: 0x%X\n", RSTBTN_GPE_NUMBER, error);
    }
    
    if (rstbtn_input_dev) {
        device_remove_file(&rstbtn_input_dev->dev, &dev_attr_rstbtn_current_status);
        device_remove_file(&rstbtn_input_dev->dev, &dev_attr_rstbtn_poweron_status);
        device_remove_file(&rstbtn_input_dev->dev, &dev_attr_rstbtn_simulate_button);
        input_unregister_device(rstbtn_input_dev);
        rstbtn_input_dev = NULL;
    }
    
    printk(KERN_INFO THIS_MODULE_NAME " version " THIS_MODULE_VERSION " exited.\n");
    return;
}


static ssize_t rstbtn_simulate_button_show(struct device * dev, struct device_attribute * attr, char * buf) {
    return 0;
}

static ssize_t rstbtn_simulate_button_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t count) {
    if (count > 0) {
        if (buf[0] == '1') {
            if (rstbtn_input_dev) {
                rstbtn_process_button_event(true);
            }
        } else if (buf[0] == '0') {
            if (rstbtn_input_dev) {
                rstbtn_process_button_event(false);
            }
        }
    }
    
    return count;
}


static ssize_t rstbtn_poweron_status_show(struct device * dev, struct device_attribute * attr, char * buf) {
    ssize_t count = 0;

    if (buf) {
        count = sprintf(buf, "%s\n", rstbtn_power_on_status ? "1" : "0");
    }
    
    return count;
}

static ssize_t rstbtn_poweron_status_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t count) {
    return count;
}


static ssize_t rstbtn_current_status_show(struct device * dev, struct device_attribute * attr, char * buf) {
    ssize_t count = 0;

    if (buf) {
        count = sprintf(buf, "%s\n", rstbtn_is_pressed() ? "1" : "0");
    }
    
    return count;
}

static ssize_t rstbtn_current_status_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t count) {
    return count;
}


module_init(rstbtn_init);
module_exit(rstbtn_exit);

MODULE_LICENSE(THIS_MODULE_LICENSE);
MODULE_VERSION(THIS_MODULE_VERSION);
MODULE_DESCRIPTION(THIS_MODULE_DESCRIPTION);
MODULE_AUTHOR(THIS_MODULE_AUTHOR);

