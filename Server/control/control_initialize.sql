CREATE DATABASE IF NOT EXISTS edison;
USE edison;
DROP TABLE IF EXISTS edison.control;
CREATE TABLE IF NOT EXISTS edison.control (
    `id` int(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `mac_address` varchar(30) DEFAULT 0,
    `control_type` int(11) DEFAULT 0,
    `auto_control_command` int(11) DEFAULT 0,
    `manual_control_command` int(11) DEFAULT 0,
    `suspend_pwm1` double(5,2) DEFAULT 0.0,
    `suspend_pwm2` double(5,2) DEFAULT 0.0,
    `suspend_pwm3` double(5,2) DEFAULT 0.0,
    `suspend_pwm4` double(5,2) DEFAULT 0.0,
    `update_time` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00'
    ) ENGINE = InnoDB;

INSERT INTO `control` VALUES (1, 'mac_address DEFAULT', 0, 0, 0, '0000-00-00 00:00:00');
