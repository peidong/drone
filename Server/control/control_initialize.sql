CREATE DATABASE IF NOT EXISTS edison;
USE edison;
DROP TABLE IF EXISTS edison.control;
CREATE TABLE IF NOT EXISTS edison.control (
    `id` int(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `mac_address` varchar(30) DEFAULT 0,
    `control_type` int(11) DEFAULT 0,
    `auto_control_command` int(11) DEFAULT 0,
    `manual_control_command` int(11) DEFAULT 0,
    `suspend_pwm1` double(8,7) DEFAULT 0.0,
    `suspend_pwm2` double(8,7) DEFAULT 0.0,
    `suspend_pwm3` double(8,7) DEFAULT 0.0,
    `suspend_pwm4` double(8,7) DEFAULT 0.0,
    `update_time` timestamp NOT NULL DEFAULT '1970-01-01 00:00:01'
    ) ENGINE = InnoDB;

INSERT INTO `control` VALUES (1, 'fc:c2:de:3d:7f:af', 0, 0, 0, 0.0, 0.0, 0.0, 0.0, '1970-01-01 00:00:01');
