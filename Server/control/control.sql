CREATE DATABASE IF NOT EXISTS edison;
USE edison;
DROP TABLE IF EXISTS edison.control;
CREATE TABLE IF NOT EXISTS edison.control (
    `id` int(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `mac_address` varchar(30) DEFAULT 0,
    `ip_address` varchar(30) DEFAULT NULL,
    `control_command` varchar(30) DEFAULT NULL,
    `update_time` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00'
    ) ENGINE = InnoDB;

INSERT INTO `control` VALUES ('1', 'mac_address DEFAULT', 'ip_address DEFAULT', 'stop', '0000-00-00 00:00:00');
