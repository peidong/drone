CREATE DATABASE IF NOT EXISTS edison;
USE edison;
DROP TABLE IF EXISTS edison.pwm;
CREATE TABLE IF NOT EXISTS edison.pwm (
    `id` int(11) NOT NULL DEFAULT 0,
    `pwm1` double(8,7) DEFAULT NULL,
    `pwm2` double(8,7) DEFAULT NULL,
    `pwm3` double(8,7) DEFAULT NULL,
    `pwm4` double(8,7) DEFAULT NULL,
    `update_time` timestamp NOT NULL DEFAULT '1970-01-01 00:00:01'
    ) ENGINE = InnoDB;

INSERT INTO `pwm` VALUES (1, 0.1, 0.2, 0.3, 0.4, '1970-01-01 00:00:01');
