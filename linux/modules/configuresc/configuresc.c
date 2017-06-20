#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>

#include <asm/uaccess.h>
#include <asm/kvm_para.h>
#include <asm/io.h>
//#include <configuresc.h>

#define DEVICE_NAME "configuresc"

static int major_number;

static long configuresc_ioctl(struct file *f, unsigned int ioctl_num, unsigned long ioctl_param) {

    uint64_t budget, period;
    uint64_t *temp;

    temp = (uint64_t *) ioctl_param;
    get_user(budget, temp);
    temp++;
    get_user(period, temp);

    printk(KERN_INFO "Got %llu/%llu\n", budget, period);

    kvm_hypercall4(4, (uint32_t) budget, (uint32_t) (budget >> 32llu),
                      (uint32_t) period, (uint32_t) (period >> 32llu));
    return 0;
}

struct file_operations fops = {
    .compat_ioctl = configuresc_ioctl,
    .unlocked_ioctl = configuresc_ioctl
};

static int __init configuresc_init(void) {
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    printk(KERN_INFO "%s initialized with major number %d\n", DEVICE_NAME, major_number);
    return 0;
}

static void __exit configuresc_exit(void) {
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "%s exit\n", DEVICE_NAME);
}

module_init(configuresc_init);
module_exit(configuresc_exit);
