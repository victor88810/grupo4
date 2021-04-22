DROP DATABASE IF EXISTS Bingo;
CREATE DATABASE Bingo;
USE Bingo;

CREATE TABLE Jugador (
  Identificador INTEGER PRIMARY KEY NOT NULL,
  Nombre TEXT NOT NULL,
  Contraseña TEXT NOT NULL,
  Fondos INT NOT NULL,
  Conectado INT
)ENGINE = InnoDB;

CREATE TABLE Partida (
 Identificador INTEGER PRIMARY KEY NOT NULL,
 Servidor INT NOT NULL
)ENGINE = InnoDB;

CREATE TABLE Game (
  Jugador INTEGER NOT NULL,
  Partida INTEGER NOT NULL,
  FOREIGN KEY (Jugador) REFERENCES Jugador(Identificador),
  FOREIGN KEY (Partida) REFERENCES Partida(Identificador)
)ENGINE = InnoDB;

INSERT INTO Jugador VALUES(4500,'david','hola',1000,0);





