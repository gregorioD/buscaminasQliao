#include "structs.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <cstdio>

using namespace std;
// Crear archivo si no existe; 
bool ExisteBDD(PDB database){
	bool resultado = false;
	ifstream archivo;
	archivo.open("usuarios.bin", ios::binary);
	if (!archivo.fail()){
		if(!archivo.eof()) resultado = true;
		else{
			crearBaseDeDatos(database);
		}
		archivo.close();
	}else{
		archivo.close();
		ofstream archivo2;
		archivo2.open("usuarios.bin", ios::binary);
		if(archivo2.fail()){
			cout<<"";
		}
		archivo2.close();
	}
	return resultado;
}

void crearBaseDeDatos(PDB database){
	database -> cantidad_usuarios = 0;
	ofstream archivo;
	archivo.open("usuarios.bin", ios::binary);
	if(!archivo.fail()){
		archivo.write((char*) database, sizeof(*database));
		archivo.close();
	}else{
		cout<<"Error. crearBaseDeDatos"<<endl;
	}
}
void crearUsuario(PDB database){
	int cont = 0, QUsuarios = database -> cantidad_usuarios;
	Usuario user = database -> usuarios[database -> cantidad_usuarios];
	PUsuario Puser = &user;
	char nombre[11], n[11], contrasena[13];
	bool correcto = false, tamanoCorrecto = false, esAlNum = true, esUnic = true;
	if (QUsuarios < 100){
		cin.ignore(1000, '\n');
		cout<<"Ingrese su nombre: ";
		gets(nombre);
		while(!correcto){
			if(strlen(nombre)>7 && strlen(nombre) <11){
				tamanoCorrecto = true;
			}
			while(esAlNum && cont<strlen(nombre)){
				if(!isalnum(nombre[cont])) esAlNum = false;
				else cont++;
			}
			cont = 0;
			if(QUsuarios > 0){
				while(esUnic && cont < QUsuarios){
					strcpy(n, database -> usuarios[cont].nombre);
					if(strcmp(nombre, n)==0) esUnic = false;
					else cont++;
				}
				cont = 0;
			}
			if (tamanoCorrecto && esAlNum && esUnic){
				correcto = true;
			}
		}
		cout <<"Su nombre es: "<<nombre;
		correcto = false;
		tamanoCorrecto = false;
		esAlNum = true;
		cin.ignore(1000, '\n');
		cout<<"Ingrese su contrasena: ";
		gets(contrasena);
		while (!correcto){
			if (strlen(contrasena) > 0 && strlen(contrasena)<13){
				tamanoCorrecto = true;
			}
			while(esAlNum && cont<strlen(contrasena)){
				if(!isalnum(contrasena[cont])) esAlNum = false;
				else cont++;
			}
			if (esAlNum && tamanoCorrecto){
				correcto = true;
				cout<<"Su contrasena es: "<<contrasena<<endl;
			}
		}
		strcpy((Puser -> nombre), nombre);
		strcpy((Puser -> contrasena), contrasena);
		Puser -> perdidas = 0;
		Puser -> ganadas = 0;
		Puser -> abandonos = 0;
		database -> cantidad_usuarios++;
		}else{
			cout<<"Error. Ya se alcanzo la cantidad maxima de usuarios."<<endl;
		}		

}
void GuardarPartida (PPartida match, int dif, int score, char tipo){
	// Cambie la funcion para aprovechar que pasaste todo a punteros,
	// ahora lo que hace es generar una fecha nueva con cada nueva partida
	// y despues le asigna al puntero de fecha de la partida el valor de memoria
	// de la nueva fecha uwu
	Fecha date;
	Pfecha punterodate = &date;
	obtenerFecha(punterodate);
	match -> fecha = punterodate;
	match -> dificultad = dif;
	match -> puntaje = score;
	match -> tipo = tipo;
}
void obtenerFecha(Pfecha date){
	
	int dig0, dig1, dig2, dig3, mes, anio, dia;
	char fecha[25], nombreMes[3];
	time_t current_time;
	current_time=time(NULL);
	ctime(&current_time);
	strcpy(fecha, ctime(&current_time));
	dig0 = (int)fecha[8] - 48;
	dig1 = (int)fecha[9] -48;
	dia = dig0 * 10 + dig1;
	date -> dia = dia;
	for (int i = 0; i<3; i++){
		nombreMes[i] = fecha[i+4];
	}
	mes = QueMesEs(nombreMes);
	date -> mes = mes;
	dig0 = (int)fecha[20] - 48;
	dig1 = (int)fecha[21] -48;
	dig2 = (int)fecha[22] -48;
	dig3 = (int)fecha[23] -48;
	anio = dig0 * 1000 + dig1 * 100 + dig2 * 10 + dig3;
	date -> anyo = anio;
}
	
int QueMesEs(char mes[3]){
	int rdo = -1;
	if (mes[0]=='D'){
		rdo = 12;
	}else{
		if (mes[0]=='N'){
			rdo = 11;
		}else{
			if (mes[0]=='O'){
				rdo = 10;
			}else{
				if (mes[0]=='S'){
					rdo = 9;
				}else{
					if (mes[0]=='F'){
						rdo = 2;
					}else{
						if(mes[0]=='A'){
							if (mes[1]=='p') rdo = 4;
							else rdo = 8;
						}else{
							if(mes[0]=='M'){
								if(mes[2]=='r') rdo = 3;
								else rdo = 5;
							}else{
								if(mes[0]=='J'){
									if(mes[1]=='a') rdo = 1;
									else{
										if (mes[2]=='n') rdo = 6;
										else rdo = 7;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return  rdo;
}

void partidaAUsuario(PPartida match, PUsuario usr){
		if (match -> tipo == 'G') usr -> gan++;
		else if (match -> tipo == 'P') usr -> perd++;
		else usr -> ab++;
		int total = (usr -> gan) + (usr -> perd) + (usr -> ab);
		usr -> ganadas = usr -> gan * 100.0 / total;	
		usr -> perdidas = usr -> perd * 100.0 / total;
		usr -> abandonos = usr -> ab * 100.0 / total;
}



void guardarDB(PDB database){
	// tiene que crear si no lo hay un archivo binario y guardarle la base de datos
	// en caso de que el archivo ya exista se le sobreescribe la base de datos,
	// asi se puede llamar a la misma funcion cada vez que se quiera guardar algo
	ofstream archivo;
	archivo.open("usuarios.bin",ios::binary);
	if(!archivo.fail()){
		archivo.write((char*) database, sizeof(*database));
		archivo.close();
	}else{
		cout<<"Error al abrir el archivo. guardarDB"<<endl;
	}

}

Usuario AbrirUsuario (PDB database){
	Usuario user;
	int QU = database -> cantidad_usuarios, cont = 0, i = 0, longitud;
	char nombre[11], n[11], pwrd[13], p[13];
	bool encontrado = false, coincide = false;
	while(!encontrado){
		cin.ignore(1000, '\n');
		cout<<"Ingrese nombre de usuario: ";
		gets(nombre);
		while(!encontrado && cont < QU){
			strcpy(n, (database->usuarios[cont].nombre));
			if (strcmp(nombre, n)==0) encontrado = true;
			else cont++;
		}
		cont = 0;
		if(!encontrado) cout<<"Usuario inexistente."<<endl;
	}
	while(!coincide){
		strcpy(p, (database -> usuarios[cont].contrasena));
		cin.ignore(1000, '\n');
		cout<<"Ingrese su contrasena: ";
		gets(pwrd);
		if (strcmp(pwrd, p)==0) coincide = true;
		else cout<<"Contrasena incorrecta, por favor, intente de nuevo."<<endl;
	}	
	if (encontrado && coincide) user = database -> usuarios[cont];
	
	return user;
}

DB AbrirBaseDeDatos(){
	DB database;
	ifstream archivo;
	archivo.open("usuarios.bin",ios::binary);
	if(!archivo.fail()){
		archivo.read((char*) &database, sizeof(database));
		archivo.close();
	}else{
		puts("Error al abrir el archivo.(Abrir base de datos)");
	}
	return database;
}



