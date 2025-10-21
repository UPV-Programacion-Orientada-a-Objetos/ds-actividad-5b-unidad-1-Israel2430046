#include <iostream>

template <typename T>
class MatrizBase {
protected:
    int _filas;
    int _columnas;

public:
    MatrizBase(int f, int c) : _filas(f), _columnas(c) {}
    virtual ~MatrizBase() {}
    
    int filas() const { return _filas; }
    int columnas() const { return _columnas; }
    
    virtual void cargarValores() = 0;
    virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const = 0;
    virtual void imprimir() const = 0;
    virtual void asignar(int i, int j, const T& valor) = 0;
    virtual T at(int i, int j) const = 0;
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
        if (_datos) {
            for (int i = 0; i < this->_filas; ++i)
                delete[] _datos[i];
            delete[] _datos;
            _datos = nullptr;
        }
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

    MatrizDinamica(MatrizDinamica&& otra) noexcept : MatrizBase<T>(otra._filas, otra._columnas) {
        _datos = otra._datos;
        otra._datos = nullptr;
        otra._filas = 0;
        otra._columnas = 0;
    }

    MatrizDinamica& operator=(MatrizDinamica&& otra) noexcept {
        if (this != &otra) {
            liberar();
            _datos = otra._datos;
            this->_filas = otra._filas;
            this->_columnas = otra._columnas;
            otra._datos = nullptr;
        }
        return *this;
    }

    void cargarValores() override {
        std::cout << "Ingrese los valores correspondientes a la matriz ("<<this->_filas << "x" << this->_columnas << "):\n";
        for (int i = 0; i < this->_filas; i++) {
            for (int j = 0; j < this->_columnas; j++) {
                std::cout << "[" << i << "," << j << "]: ";
                std::cin >> _datos[i][j];
            }
        }
    }

    void asignar(int i, int j, const T& valor) override {
        _datos[i][j] = valor;
    }

    T at(int i, int j) const override {
        return _datos[i][j];
    }

    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
        if (this->_filas != otra.filas() || this->_columnas != otra.columnas()) {
            std::cout << "Error: dimensiones incompatibles.\n";
            return nullptr;
        }
        MatrizDinamica<T>* res = new MatrizDinamica<T>(this->_filas, this->_columnas);
        for (int i = 0; i < this->_filas; ++i)
            for (int j = 0; j < this->_columnas; ++j)
                res->asignar(i, j, _datos[i][j] + otra.at(i, j));
        return res;
    }

    void imprimir() const override {
        for (int i = 0; i < this->_filas; ++i) {
            std::cout << "| ";
            for (int j = 0; j < this->_columnas; ++j)
                std::cout << _datos[i][j] << " ";
            std::cout << "|\n";
        }
    }
};



template <typename T, int M, int N>
class MatrizEstatica : public MatrizBase<T> {
private:
    T _datos[M][N];

public:
    MatrizEstatica() : MatrizBase<T>(M, N) {
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < N; ++j)
                _datos[i][j] = 0;
    }

    void cargarValores() override {
        std::cout << "Ingrese los valores de la matriz (" << M << "x" << N << "):\n";
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < N; ++j) {
                std::cout << "[" << i << "," << j << "]: ";
                std::cin >> _datos[i][j];
            }
    }

    void asignar(int i, int j, const T& valor) override {
        _datos[i][j] = valor;
    }

    T at(int i, int j) const override {
        return _datos[i][j];
    }

    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
        if (M != otra.filas() || N != otra.columnas()) {
            std::cout << "Error: dimensiones incompatibles.\n";
            return nullptr;
        }
        MatrizEstatica<T, M, N>* res = new MatrizEstatica<T, M, N>();
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < N; ++j)
                res->_datos[i][j] = _datos[i][j] + otra.at(i, j);
        return res;
    }

    void imprimir() const override {
        for (int i = 0; i < M; ++i) {
            std::cout << "| ";
            for (int j = 0; j < N; ++j)
                std::cout << _datos[i][j] << " ";
            std::cout << "|\n";
        }
    }
};

template <typename T>
MatrizBase<T>* operator+(const MatrizBase<T>& A, const MatrizBase<T>& B) {
    return A.sumar(B);
}

int main(){

    std::cout << "--- Sistema Generico de Algebra Lineal ---\n\n";
    MatrizBase<float>* A = new MatrizDinamica<float>(3, 2);
    MatrizBase<float>* B = new MatrizEstatica<float, 3, 2>();

    A->asignar(0,0,1.5f); A->asignar(0,1,2.0f);
    A->asignar(1,0,0.0f); A->asignar(1,1,1.0f);
    A->asignar(2,0,4.5f); A->asignar(2,1,3.0f);

    B->asignar(0,0,0.5f); B->asignar(0,1,1.0f);
    B->asignar(1,0,2.0f); B->asignar(1,1,3.0f);
    B->asignar(2,0,1.0f); B->asignar(2,1,1.0f);

    std::cout << "Matriz A:\n"; A->imprimir();
    std::cout << "Matriz B:\n"; B->imprimir();

    MatrizBase<float>* C = *A + *B;
    if (C != nullptr) {
        std::cout << "Resultado C = A + B:\n";
        C->imprimir();
        delete C;
    }

    delete A;
    delete B;
    std::cout << "\nSistema finalizado.\n";
    return 0;
}