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
#include <linux/delay.h>

#include "mpu_6000.h"

static int write_reg(struct spi_device *spi, uint8_t reg, uint8_t val);
static ssize_t read_reg(struct spi_device *spi, uint8_t reg);

struct {
	unsigned		_sample_rate;
} MPU6000_data;


MODULE_LICENSE("Dual BSD/GPL");

static struct of_device_id imu_of_match[] = {
  { .compatible = "ti,imu", },
  {}
};

MODULE_DEVICE_TABLE(of, imu_of_match);

static int imu_remove(struct spi_device *spi) {
	printk("<1> Goodbye world probe!\n");
	return 0;
}

static int imu_probe(struct spi_device *spi) {
	uint16_t command;
	int status = 0;
	uint8_t id = 0;

	//reset
	status = write_reg(spi,MPUREG_PWR_MGMT_1,BIT_H_RESET);
	mdelay(100);

	//sleep mode off
	status = write_reg(spi, MPUREG_PWR_MGMT_1, 0x00); //clear SLEEP bit
	mdelay(100);

	//disable I2C
	status = write_reg(spi, MPUREG_USER_CTRL, BIT_I2C_IF_DIS); //set bit I2C_IF_DIS to 1
	mdelay(100);

	return 0;
}

int write_reg(struct spi_device *spi, uint8_t reg, uint8_t val) {

	uint8_t tx[2];

	tx[0] = reg;
	tx[1] = val;

	return spi_write(spi,tx,sizeof(tx));
}

ssize_t read_reg(struct spi_device *spi, uint8_t reg) {
    return spi_w8r8(spi,reg|0x80);
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

set_sample_rate(struct spi_device *spi, uint16_t desired_sample_rate_hz)
{
  uint8_t div = 1000 / desired_sample_rate_hz;
  if(div>200) div=200;
  if(div<1) div=1;
  write_reg(spi, MPUREG_SMPLRT_DIV, div-1);
  MPU6000_data._sample_rate = 1000 / div;
}

module_spi_driver(imu_driver);

MODULE_DESCRIPTION("Driver for IMU");
MODULE_AUTHOR("Jimmy Johnson");
MODULE_LICENSE("GPL");
MODULE_ALIAS("imu");

//module_init(hello_init);
//module_exit(hello_exit);
