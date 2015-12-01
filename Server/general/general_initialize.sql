CREATE DATABASE IF NOT EXISTS edison;
USE edison;
DROP TABLE IF EXISTS edison.general;
CREATE TABLE IF NOT EXISTS edison.general (
    `id` int(11) NOT NULL DEFAULT 0,
    `status` varchar(30) DEFAULT NULL,
    `is_pwm_change` varchar(30) DEFAULT NULL,
    `update_time` timestamp NOT NULL DEFAULT '1970-01-01 00:00:01'
    ) ENGINE = InnoDB;

INSERT INTO `general` VALUES (1, 'on', 'no', '1970-01-01 00:00:01');
