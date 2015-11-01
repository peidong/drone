CREATE DATABASE IF NOT EXISTS edison;
USE edison;
DROP TABLE IF EXISTS edison.pid_tuning;
CREATE TABLE IF NOT EXISTS edison.pid_tuning (
    `id` int(11) NOT NULL DEFAULT 0,
    `kp_pitch` double(7,6) DEFAULT NULL,
    `ki_pitch` double(7,6) DEFAULT NULL,
    `kd_pitch` double(7,6) DEFAULT NULL,
    `kp_roll` double(7,6) DEFAULT NULL,
    `ki_roll` double(7,6) DEFAULT NULL,
    `kd_roll` double(7,6) DEFAULT NULL,
    `kp_yaw` double(7,6) DEFAULT NULL,
    `ki_yaw` double(7,6) DEFAULT NULL,
    `kd_yaw` double(7,6) DEFAULT NULL,
    `update_time` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00'
    ) ENGINE = InnoDB;

INSERT INTO `pid_tuning` VALUES (1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, '0000-00-00 00:00:00');
