#include <iostream>

template <typename T>
class MatrizBase {
protected:
    int _filas;
    int _columnas;

public:
    virtual void cargarValores() = 0;
    virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const = 0;
    virtual void imprimir() const = 0;
};


template <typename T>
class MatrizDinamica : public MatrizBase<T> {
private:
    T** _datos;

    void reservar() {
        _datos = new T*[this->_filas];
        for (int i = 0; i < this->_filas; ++i)
            _datos[i] = new T[this->_columnas];
    }

    void liberar() {
        for (int i = 0; i < this->_filas; ++i)
            delete[] _datos[i];
        delete[] _datos;
        _datos = 0;
    }

    void copiar(const MatrizDinamica& otra) {
        this->_filas = otra._filas;
        this->_columnas = otra._columnas;
        reservar();
        for (int i = 0; i < this->_filas; ++i)
            for (int j = 0; j < this->_columnas; ++j)
                _datos[i][j] = otra._datos[i][j];
    }

public:
    MatrizDinamica(int f, int c) : MatrizBase<T>(f, c) {
        if (f <= 0 || c <= 0) {
            std::cout << "Error: dimensiones invalidas.\n";
            this->_filas = this->_columnas = 1;
        }
        reservar();
        for (int i = 0; i < this->_filas; ++i)
            for (int j = 0; j < this->_columnas; ++j)
                _datos[i][j] = 0;
    }

    ~MatrizDinamica() {
        std::cout << "Liberando memoria de Matriz Dinamica...\n";
        liberar();
    }

    MatrizDinamica(const MatrizDinamica& otra) : MatrizBase<T>(otra._filas, otra._columnas) {
        copiar(otra);
    }

    MatrizDinamica& operator=(const MatrizDinamica& otra) {
        if (this != &otra) {
            liberar();
            copiar(otra);
        }
        return *this;
    }

    MatrizDinamica(MatrizDinamica&& otra) : MatrizBase<T>(otra._filas, otra._columnas) {
        _datos = otra._datos;
        otra._datos = 0;
        otra._filas = 0;
        otra._columnas = 0;
    }

    MatrizDinamica& operator=(MatrizDinamica&& otra) {
        if (this != &otra) {
            liberar();
            _datos = otra._datos;
            this->_filas = otra._filas;
            this->_columnas = otra._columnas;
            otra._datos = 0;
        }
        return *this;
    }


};

int main(){

    return 0;
}