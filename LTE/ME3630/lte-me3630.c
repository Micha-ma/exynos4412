#include <linux/init.h>
#include <linux/module.h>

/*驱动注册的头文件，包含驱动的结构体和注册和卸载的函数*/
#include <linux/platform_device.h>
/*注册杂项设备头文件*/
#include <linux/miscdevice.h>
/*注册设备节点的文件结构体*/
#include <linux/fs.h>

/*Linux中申请GPIO的头文件*/
#include <linux/gpio.h>
/*三星平台的GPIO配置函数头文件*/
/*三星平台EXYNOS系列平台，GPIO配置参数宏定义头文件*/
#include <plat/gpio-cfg.h>
#include <mach/gpio.h>
/*三星平台4412平台，GPIO宏定义头文件*/
#include <mach/gpio-exynos4.h>

#define DRIVER_NAME "me3630"
#define DEVICE_NAME "lte_ctl"


MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Michael");

static long lte_ioctl( struct file *files, unsigned int cmd, unsigned int arg){
	printk("cmd is %d,arg is %d\n",cmd,arg);
	
	if(cmd > 1){
		printk(KERN_EMERG "cmd is 0 or 1\n");
	}
	
	switch(arg){
		case 0:
			gpio_set_value(EXYNOS4_GPX1(3),cmd);
			break;
		case 1:
			gpio_set_value(EXYNOS4_GPF3(5),cmd);
			break;
		case 2:
			gpio_set_value(EXYNOS4_GPC0(4),cmd);
			break;
		case 3:
			gpio_set_value(EXYNOS4_GPX3(4),cmd);
			break;
		default:
			printk(KERN_EMERG "arg must be less than 4\n");
	}
		
	return 0;
}

static int lte_release(struct inode *inode, struct file *file){
	printk(KERN_EMERG "lte release\n");
	return 0;
}

static int lte_open(struct inode *inode, struct file *file){
	printk(KERN_EMERG "lte open\n");
	return 0;
}

static struct file_operations lte_ops = {
	.owner = THIS_MODULE,
	.open = lte_open,
	.release = lte_release,
	.unlocked_ioctl = lte_ioctl,
};

static  struct miscdevice lte_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &lte_ops,
};


static int lte_probe(struct platform_device *pdv){
	int ret, pow, wake_in, wake_out;
	
	printk(KERN_EMERG "\tinitialized\n");
	
	pow = gpio_request(EXYNOS4_GPX1(3),"ME3630_POWER");
	if(pow < 0){
		printk(KERN_EMERG "gpio_request EXYNOS4_GPX1(3) failed!\n");
		return pow;
	}
	
	s3c_gpio_cfgpin(EXYNOS4_GPX1(3),S3C_GPIO_OUTPUT);
	
	gpio_set_value(EXYNOS4_GPX1(3),1);
	
	ret = gpio_request(EXYNOS4_GPF3(5),"ME3630_RESET");
	if(ret < 0){
		printk(KERN_EMERG "gpio_request EXYNOS4_GPF3(5) failed!\n");
		return ret;
	}
	
	s3c_gpio_cfgpin(EXYNOS4_GPF3(5),S3C_GPIO_OUTPUT);
	
	gpio_set_value(EXYNOS4_GPF3(5),1);
	
	wake_in = gpio_request(EXYNOS4_GPC0(4),"ME3630_WAKEIN");
	if(wake_in < 0){
		printk(KERN_EMERG "gpio_request EXYNOS4_GPC0(4) failed!\n");
		return wake_in;
	}
	
	s3c_gpio_cfgpin(EXYNOS4_GPC0(4),S3C_GPIO_OUTPUT);
	
	gpio_set_value(EXYNOS4_GPC0(4),1);
	
	wake_out = gpio_request(EXYNOS4_GPX3(4),"ME3630_WAKEOUT");
	if(wake_out < 0){
		printk(KERN_EMERG "gpio_request EXYNOS4_GPX3(4) failed!\n");
		return wake_out;
	}
	
	s3c_gpio_cfgpin(EXYNOS4_GPX3(4),S3C_GPIO_INPUT);
	
	gpio_set_value(EXYNOS4_GPX3(4),1);
	
	misc_register(&lte_dev);
	
	return 0;
}

static int lte_remove(struct platform_device *pdv){
	
	printk(KERN_EMERG "\tremove\n");
	misc_deregister(&lte_dev);
	return 0;
}

static void lte_shutdown(struct platform_device *pdv){
	
	;
}

static int lte_suspend(struct platform_device *pdv,pm_message_t pmt){
	
	return 0;
}

static int lte_resume(struct platform_device *pdv){
	
	return 0;
}

struct platform_driver lte_driver = {
	.probe = lte_probe,
	.remove = lte_remove,
	.shutdown = lte_shutdown,
	.suspend = lte_suspend,
	.resume = lte_resume,
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
	}
};


static int lte_init(void)
{
	int DriverState;
	
	printk(KERN_EMERG "lte WORLD enter!\n");
	DriverState = platform_driver_register(&lte_driver);
	
	printk(KERN_EMERG "\tDriverState is %d\n",DriverState);
	return 0;
}


static void lte_exit(void)
{
	printk(KERN_EMERG "lte WORLD exit!\n");
	
	platform_driver_unregister(&lte_driver);	
}

module_init(lte_init);
module_exit(lte_exit);
