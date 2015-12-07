CREATE DATABASE IF NOT EXISTS edison;
USE edison;
DROP TABLE IF EXISTS edison.camera_search;
CREATE TABLE IF NOT EXISTS edison.camera_search (
    `id` int(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `detected` int(11) DEFAULT 0,
    `latitude` double(9,6) DEFAULT 0.0,
    `longitude` double(9,6) DEFAULT 0.0,
    `update_time` timestamp NOT NULL DEFAULT '1970-01-01 00:00:01'
    ) ENGINE = InnoDB;

INSERT INTO `camera_search` VALUES (1, 0, 34.063946, -118.450714, '1970-01-01 00:00:01');
