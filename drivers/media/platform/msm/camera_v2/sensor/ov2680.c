/* Copyright (c) 2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include "msm_sensor.h"

#define OV2680_SENSOR_NAME "ov2680"
DEFINE_MSM_MUTEX(ov2680_mut);

static struct msm_sensor_ctrl_t ov2680_s_ctrl;

static struct msm_sensor_power_setting ov2680_power_setting[] = {
	{
		.seq_type = SENSOR_VREG,
		.seq_val = CAM_VANA,
		.config_val = 0,
		.delay = 0,
	},

	{
		.seq_type = SENSOR_VREG,
		.seq_val = CAM_VIO,
		.config_val = 0,
		.delay =1,
	},

	{
		.seq_type = SENSOR_VREG,
		.seq_val = CAM_VDIG,
		.config_val = 0,
		.delay = 5,
	},

	{
		.seq_type = SENSOR_GPIO,
		.seq_val = SENSOR_GPIO_STANDBY,
		.config_val = GPIO_OUT_LOW,
		.delay = 5,
	},
	{
		.seq_type = SENSOR_GPIO,
		.seq_val = SENSOR_GPIO_STANDBY,
		.config_val = GPIO_OUT_HIGH,
		.delay = 10,
	},
	{
		.seq_type = SENSOR_GPIO,
		.seq_val = SENSOR_GPIO_RESET,
		.config_val = GPIO_OUT_LOW,
		.delay = 5,
	},
	{
		.seq_type = SENSOR_GPIO,
		.seq_val = SENSOR_GPIO_RESET,
		.config_val = GPIO_OUT_HIGH,
		.delay = 10,
	},
	{
		.seq_type = SENSOR_CLK,
		.seq_val = SENSOR_CAM_MCLK,
		.config_val = 24000000,
		.delay =10,
	},
	{
		.seq_type = SENSOR_I2C_MUX,
		.seq_val = 0,
		.config_val = 0,
		.delay = 0,
	},
};

static struct v4l2_subdev_info ov2680_subdev_info[] = {
	{
		.code   = V4L2_MBUS_FMT_SBGGR10_1X10,
		.colorspace = V4L2_COLORSPACE_JPEG,
		.fmt    = 1,
		.order    = 0,
	},
};     

static int32_t msm_ov2680_i2c_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	return msm_sensor_i2c_probe(client, id, &ov2680_s_ctrl);
}

static const struct i2c_device_id ov2680_i2c_id[] = {
	{OV2680_SENSOR_NAME, (kernel_ulong_t)&ov2680_s_ctrl},
	{ }
};

static struct i2c_driver ov2680_i2c_driver = {
	.id_table = ov2680_i2c_id,
	.probe  = msm_ov2680_i2c_probe,
	.driver = {
		.name = OV2680_SENSOR_NAME,
	},
};

static struct msm_camera_i2c_client ov2680_sensor_i2c_client = {
	.addr_type = MSM_CAMERA_I2C_WORD_ADDR,
};

static struct msm_sensor_ctrl_t ov2680_s_ctrl = {
	.sensor_i2c_client = &ov2680_sensor_i2c_client,
	.power_setting_array.power_setting = ov2680_power_setting,
	.power_setting_array.size = ARRAY_SIZE(ov2680_power_setting),
	.msm_sensor_mutex = &ov2680_mut,
	.sensor_v4l2_subdev_info = ov2680_subdev_info,
	.sensor_v4l2_subdev_info_size = ARRAY_SIZE(ov2680_subdev_info),
};

static const struct of_device_id ov2680_dt_match[] = {
	{.compatible = "qcom,ov2680", .data = &ov2680_s_ctrl},
	{}
};

MODULE_DEVICE_TABLE(of, ov2680_dt_match);

static struct platform_driver ov2680_platform_driver = {
	.driver = {
		.name = "qcom,ov2680",
		.owner = THIS_MODULE,
		.of_match_table = ov2680_dt_match,
	},
};

static int32_t ov2680_platform_probe(struct platform_device *pdev)
{
	int32_t rc = 0;
	const struct of_device_id *match;

	printk("Enter %s\n",__func__);

	match = of_match_device(ov2680_dt_match, &pdev->dev);
	rc = msm_sensor_platform_probe(pdev, match->data);
	return rc;
}

static int __init ov2680_init_module(void)
{
	int32_t rc = 0;
	printk("Enter %s\n",__func__);
	rc = platform_driver_probe(&ov2680_platform_driver,
		ov2680_platform_probe);
	if (!rc)
		return rc;
	return i2c_add_driver(&ov2680_i2c_driver);
}

static void __exit ov2680_exit_module(void)
{
	if (ov2680_s_ctrl.pdev) {
		msm_sensor_free_sensor_data(&ov2680_s_ctrl);
		platform_driver_unregister(&ov2680_platform_driver);
	} else
		i2c_del_driver(&ov2680_i2c_driver);
	return;
}

module_init(ov2680_init_module);
module_exit(ov2680_exit_module);
MODULE_DESCRIPTION("ov2680");
MODULE_LICENSE("GPL v2");
