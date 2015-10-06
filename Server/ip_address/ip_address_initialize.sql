CREATE DATABASE IF NOT EXISTS edison;
USE edison;
DROP TABLE IF EXISTS edison.ip_address;
CREATE TABLE IF NOT EXISTS edison.ip_address (
    `id` int(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `ip_address` varchar(30) DEFAULT 0,
    `update_time` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00'
    ) ENGINE = InnoDB;

INSERT INTO `ip_address` VALUES ('1', '192.168.0.1', '0000-00-00 00:00:00');
