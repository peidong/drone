CREATE DATABASE IF NOT EXISTS edison;
USE edison;
DROP TABLE IF EXISTS edison.gps;
CREATE TABLE IF NOT EXISTS edison.gps (
    `id` int(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `location_type` int(11) DEFAULT 0,
    `face_direction` int(11) DEFAULT 0,
    `latitude` double(9,6) DEFAULT 0.0,
    `longitude` double(9,6) DEFAULT 0.0,
    `stop_sign` int(11) DEFAULT 0,
    `update_time` timestamp NOT NULL DEFAULT '1970-01-01 00:00:01'
    ) ENGINE = InnoDB;

INSERT INTO `gps` VALUES (1, 0, -1, 34.063946, -118.450714, 0, '1970-01-01 00:00:01');
INSERT INTO `gps` VALUES (2, 1, 0, 0.0, 0.0, 0, '1970-01-01 00:00:01');
