DROP DATABASE IF EXISTS M4_Bingo;
CREATE DATABASE M4_Bingo;
USE M4_Bingo;

CREATE TABLE Jugador (
  Username VARCHAR(20) PRIMARY KEY NOT NULL,
  Password VARCHAR(20) NOT NULL,
  Nombre VARCHAR(20) NOT NULL,
  Fondos INT NOT NULL
)ENGINE = InnoDB;

CREATE TABLE Partida (
  Identificador INTEGER PRIMARY KEY NOT NULL,
  Fecha DATE NOT NULL,
  Durada TIME NOT NULL,
  Servidor INT NOT NULL
)ENGINE = InnoDB;

CREATE TABLE Game (
  Username VARCHAR(20) NOT NULL,
  Identificador INTEGER NOT NULL,
  FOREIGN KEY (Username) REFERENCES Jugador(Username),
  FOREIGN KEY (Identificador) REFERENCES Partida(Identificador)
)ENGINE = InnoDB;

INSERT INTO Jugador VALUES('David00','contraseña','David',1000);
INSERT INTO Jugador VALUES('Victor00','contraseña','Victor',2000);
INSERT INTO Jugador VALUES('Mario99','contraseña','Mario',800);
INSERT INTO Jugador VALUES('Carlitos','contraseña','Carlos',500);
INSERT INTO Jugador VALUES('JuliaGP','contraseña','Julia',1800);
INSERT INTO Jugador VALUES('HelenaPJ','contraseña','Hellen',2100);
INSERT INTO Jugador VALUES('Markituss','contraseña','Marc',200);
INSERT INTO Partida VALUES(1,'2021-03-23','00:15:32',4);
INSERT INTO Partida VALUES(2,'2021-03-23','00:20:24',6);
INSERT INTO Partida VALUES(3,'2021-03-23','00:30:56',5);
INSERT INTO Game VALUES('David00',3);
INSERT INTO Game VALUES('Victor00',1);
INSERT INTO Game VALUES('Mario99',3);
INSERT INTO Game VALUES('Carlitos',2);
INSERT INTO Game VALUES('JuliaGP',2);
INSERT INTO Game VALUES('HelenaPJ',2);
INSERT INTO Game VALUES('Markituss',1);




