CREATE DATABASE IF NOT EXISTS edison;
USE edison;
DROP TABLE IF EXISTS edison.camera;
CREATE TABLE IF NOT EXISTS edison.camera (
    `id` int(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `image` int(11) DEFAULT 0,
    `video` int(11) DEFAULT 0,
    `update_time` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00'
    ) ENGINE = InnoDB;

INSERT INTO `camera` VALUES (1, 0, 0, '0000-00-00 00:00:00');
