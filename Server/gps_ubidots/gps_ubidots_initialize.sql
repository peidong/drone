CREATE DATABASE IF NOT EXISTS edison;
USE edison;
DROP TABLE IF EXISTS edison.gps_ubidots;
CREATE TABLE IF NOT EXISTS edison.gps_ubidots (
    `id` int(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `face_direction` int(11) DEFAULT 0.0,
    `latitude` double(9,6) DEFAULT 0.0,
    `longitude` double(9,6) DEFAULT 0.0,
    `update_time` timestamp NOT NULL DEFAULT '1970-01-01 00:00:01'
    ) ENGINE = InnoDB;

INSERT INTO `gps_ubidots` VALUES (1, 0, -1, 34.063946, -118.450714, 0, '1970-01-01 00:00:01');
