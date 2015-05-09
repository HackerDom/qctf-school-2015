CREATE DATABASE IF NOT EXISTS botnet;
USE botnet;
DROP TABLE IF EXISTS users;
CREATE TABLE users ( id INT NOT NULL, name VARCHAR(32) NOT NULL, pass VARCHAR(32) NOT NULL, PRIMARY KEY (id), UNIQUE (name) );

INSERT users (name, pass) VALUES ('admin', 'botnetP@ssw0rd') ON DUPLICATE KEY UPDATE pass='botnetP@ssw0rd';