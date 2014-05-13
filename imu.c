#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/sched.h>
#include <linux/mod_devicetable.h>

#include <linux/spi/spi.h>
#include <linux/spi/eeprom.h>
#include <linux/of.h>

MODULE_LICENSE("Dual BSD/GPL");

static struct of_device_id imu_of_match[] = {
  { .compatible = "ti,imu", },
  {}
};

MODULE_DEVICE_TABLE(of, imu_of_match);

static int imu_remove(struct spi_device *spi)
{
	printk("<1> Goodbye world probe!\n");
	return 0;
}

static int imu_probe(struct spi_device *spi)
{
	 printk("<1> Hello world probe!\n");
	  return 0;
}

static struct spi_driver imu_driver = {
	.driver = {
		.name		= "imu",
		.owner		= THIS_MODULE,
		.of_match_table = imu_of_match,
	},
	.probe		= imu_probe,
	.remove		= imu_remove,
};

//static int hello_init(void) {
//  printk("<1> Hello world!\n");
//  return spi_register_driver(&imu_driver);
//}
//
//static void hello_exit(void) {
//  printk("<1> Bye, cruel world\n");
//  spi_unregister_driver(&imu_driver);
//}

module_spi_driver(imu_driver);

MODULE_DESCRIPTION("Driver for IMU");
MODULE_AUTHOR("Jimmy JOhnson");
MODULE_LICENSE("GPL");
MODULE_ALIAS("imu");

//module_init(hello_init);
//module_exit(hello_exit);
