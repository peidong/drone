CREATE DATABASE IF NOT EXISTS edison;
USE edison;
CREATE TABLE IF NOT EXISTS edison.led (
    `id` int(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `led_status` int(11) DEFAULT 0,
    `led_on_ms` int(11) DEFAULT 0,
    `led_off_ms` int(11) DEFAULT 0
    ) ENGINE = InnoDB;
