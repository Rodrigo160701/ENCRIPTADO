#include <iostream> 
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cctype>
using namespace std;

// Estructura para almacenar una entrada
struct Entrada {
    int id;
    string textoCifrado;
};

// Funcion para cifrar usando Vigenere
string cifrarVigenere(string texto, string clave) {
    string resultado = "";
    int n = static_cast<int>(clave.length());
    int j = 0; // Variable para controlar el avance de la clave
    for (size_t i = 0; i < texto.length(); ++i) {
        char letraTexto = texto[i];
        if (isalpha(letraTexto)) {
            char letraClave = toupper(clave[j % n]);
            char base = isupper(letraTexto) ? 'A' : 'a';
            char cifrado = ((letraTexto - base) + (toupper(letraClave) - 'A')) % 26 + base;
            resultado += cifrado;
            j++; // Solo avanzar la clave si se cifró un carácter alfabético
        }
        else {
            resultado += letraTexto;
        }
    }
    return resultado;
}

// Funcion para descifrar usando Vigenere
string descifrarVigenere(string textoCifrado, string clave) {
    string resultado = "";
    int n = static_cast<int>(clave.length());
    int j = 0; // Variable para controlar el avance de la clave
    for (size_t i = 0; i < textoCifrado.length(); ++i) {
        char letraTexto = textoCifrado[i];
        if (isalpha(letraTexto)) {
            char letraClave = toupper(clave[j % n]);
            char base = isupper(letraTexto) ? 'A' : 'a';
            char descifrado = ((letraTexto - base) - (toupper(letraClave) - 'A') + 26) % 26 + base;
            resultado += descifrado;
            j++; // Solo avanzar la clave si se descifró un carácter alfabético
        }
        else {
            resultado += letraTexto;
        }
    }
    return resultado;
}

// Funcion para cargar las entradas desde un archivo
vector<Entrada> cargarEntradas(const string& nombreArchivo) {
    vector<Entrada> entradas;
    ifstream archivoEntrada(nombreArchivo);
    if (archivoEntrada.is_open()) {
        string linea;
        while (getline(archivoEntrada, linea)) {
            istringstream stream(linea);
            Entrada entrada;
            stream >> entrada.id;
            getline(stream, entrada.textoCifrado);
            entradas.push_back(entrada);
        }
        archivoEntrada.close();
    }
    return entradas;
}

// Funcion para guardar las entradas en un archivo
void guardarEntradas(const string& nombreArchivo, const vector<Entrada>& entradas) {
    ofstream archivoSalida(nombreArchivo, ios::trunc);
    if (archivoSalida.is_open()) {
        for (const auto& entrada : entradas) {
            archivoSalida << entrada.id << " " << entrada.textoCifrado << endl;
        }
        archivoSalida.close();
    }
    else {
        cerr << "Error al guardar las entradas." << endl;
    }
}

// Funcion para agregar una nueva entrada
void agregarEntrada(vector<Entrada>& entradas, const string& nombreArchivo) {
    string texto, clave;
    cout << "Introduce el texto a cifrar: ";
    cin.ignore();
    getline(cin, texto);
    cout << "Introduce la clave: ";
    getline(cin, clave);

    string textoCifrado = cifrarVigenere(texto, clave);
    int id = entradas.empty() ? 1 : entradas.back().id + 1;

    Entrada nuevaEntrada = { id, textoCifrado };
    entradas.push_back(nuevaEntrada);
    guardarEntradas(nombreArchivo, entradas);

    cout << "Entrada agregada con ID " << id << " y guardada en el archivo." << endl;
}

// Funcion para listar las entradas
void listarEntradas(const vector<Entrada>& entradas) {
    if (entradas.empty()) {
        cout << "No hay entradas disponibles." << endl;
        return;
    }
    cout << "\n--- Listado de Entradas ---" << endl;
    for (const auto& entrada : entradas) {
        cout << "ID: " << entrada.id << " | Texto cifrado: " << entrada.textoCifrado << endl;
    }
    cout << "---------------------------" << endl;
}

// Funcion para eliminar una entrada por ID
void eliminarEntrada(vector<Entrada>& entradas, const string& nombreArchivo) {
    int id;
    cout << "Introduce el ID de la entrada que deseas eliminar: ";
    cin >> id;

    auto it = remove_if(entradas.begin(), entradas.end(), [id](const Entrada& entrada) {
        return entrada.id == id;
        });

    if (it != entradas.end()) {
        entradas.erase(it, entradas.end());
        guardarEntradas(nombreArchivo, entradas);
        cout << "Entrada con ID " << id << " eliminada." << endl;
    }
    else {
        cout << "No se encontro ninguna entrada con el ID proporcionado." << endl;
    }
}

// Funcion para modificar una entrada por ID
void modificarEntrada(vector<Entrada>& entradas, const string& nombreArchivo) {
    int id;
    cout << "Introduce el ID de la entrada que deseas modificar: ";
    cin >> id;

    for (auto& entrada : entradas) {
        if (entrada.id == id) {
            cout << "Introduce el nuevo texto: ";
            cin.ignore();
            string nuevoTexto;
            getline(cin, nuevoTexto);
            cout << "Introduce la nueva clave: ";
            string nuevaClave;
            getline(cin, nuevaClave);

            entrada.textoCifrado = cifrarVigenere(nuevoTexto, nuevaClave);

            guardarEntradas(nombreArchivo, entradas);
            cout << "Entrada con ID " << id << " modificada." << endl;
            return;
        }
    }
    cout << "No se encontro ninguna entrada con el ID proporcionado." << endl;
}

// Funcion para descifrar una entrada por ID
void descifrarEntrada(const vector<Entrada>& entradas) {
    int id;
    cout << "Introduce el ID de la entrada que deseas descifrar: ";
    cin >> id;

    for (const auto& entrada : entradas) {
        if (entrada.id == id) {
            string clave;
            cout << "Introduce la clave para descifrar: ";
            cin.ignore();
            getline(cin, clave);
            string textoDescifrado = descifrarVigenere(entrada.textoCifrado, clave);
            cout << "Texto descifrado (ID " << id << "): " << textoDescifrado << endl;
            return;
        }
    }
    cout << "No se encontro ninguna entrada con el ID proporcionado." << endl;
}

// Menu de opciones secundarias dentro de Listar
void menuListar(vector<Entrada>& entradas, const string& nombreArchivo) {
    int opcion;
    do {
        listarEntradas(entradas);
        cout << "\n--- Opciones dentro de Listar ---\n";
        cout << "1. Eliminar por ID\n";
        cout << "2. Modificar por ID\n";
        cout << "3. Descifrar por ID\n";
        cout << "4. Volver al menu principal\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            eliminarEntrada(entradas, nombreArchivo);
            break;
        case 2:
            modificarEntrada(entradas, nombreArchivo);
            break;
        case 3:
            descifrarEntrada(entradas);
            break;
        case 4:
            cout << "Volviendo al menu principal." << endl;
            break;
        default:
            cout << "Opcion no valida. Intente de nuevo." << endl;
        }
    } while (opcion != 4);
}

// Menu principal
int main() {
    string nombreArchivo = "cifrado.txt";
    vector<Entrada> entradas = cargarEntradas(nombreArchivo);
    int opcion;

    do {
        cout << "\n--- Menu Principal ---\n";
        cout << "1. Agregar entrada cifrada\n";
        cout << "2. Listar entradas\n";
        cout << "3. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            agregarEntrada(entradas, nombreArchivo);
            break;
        case 2:
            menuListar(entradas, nombreArchivo);
            break;
        case 3:
            cout << "Saliendo..." << endl;
            break;
        default:
            cout << "Opcion no valida. Intente de nuevo." << endl;
        }
    } while (opcion != 3);

    return 0;
}