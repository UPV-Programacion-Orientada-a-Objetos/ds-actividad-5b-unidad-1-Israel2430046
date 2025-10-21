#include <iostream>

template <typename T>
class Matrizbase {
protected:
    int _filas;
    int _columnas;

public:
    Matrizbase(int f, int c) : _filas(f), _columnas(c) {}
    virtual ~Matrizbase() {}
    
    int filas() const { return _filas; }
    int columnas() const { return _columnas; }
    
    virtual void cargarValores() = 0;
    virtual Matrizbase<T>* sumar(const Matrizbase<T>& otra) const = 0;
    virtual void imprimir() const = 0;
    virtual T obtener(int i, int j) const = 0;
};

template <typename T>
class Matrizdinamica : public Matrizbase<T> {
private:
    T** _datos;

    void reservar() {
        _datos = new T*[this->_filas];
        for (int i = 0; i < this->_filas; ++i) {
            _datos[i] = new T[this->_columnas];
            for (int j = 0; j < this->_columnas; ++j)
                _datos[i][j] = T(0);
        }
    }

    void liberar() {
        if (_datos) {
            for (int i = 0; i < this->_filas; ++i)
                delete[] _datos[i];
            delete[] _datos;
            _datos = nullptr;
        }
    }

public:
    Matrizdinamica(int f, int c) : Matrizbase<T>(f, c), _datos(nullptr) {
        reservar();
    }

    ~Matrizdinamica() {
        std::cout << "Liberando memoria de la matriz\n";
        liberar();
    }

    Matrizdinamica(const Matrizdinamica& otra) : Matrizbase<T>(otra._filas, otra._columnas), _datos(nullptr) {
        reservar();
        for (int i = 0; i < this->_filas; ++i)
            for (int j = 0; j < this->_columnas; ++j)
                _datos[i][j] = otra._datos[i][j];
    }

    Matrizdinamica& operator=(const Matrizdinamica& otra) {
        if (this != &otra) {
            liberar();
            this->_filas = otra._filas;
            this->_columnas = otra._columnas;
            reservar();
            for (int i = 0; i < this->_filas; ++i)
                for (int j = 0; j < this->_columnas; ++j)
                    _datos[i][j] = otra._datos[i][j];
        }
        return *this;
    }

    Matrizdinamica(Matrizdinamica&& otra) noexcept : Matrizbase<T>(otra._filas, otra._columnas) {
        _datos = otra._datos;
        otra._datos = nullptr;
        otra._filas = 0;
        otra._columnas = 0;
    }

    Matrizdinamica& operator=(Matrizdinamica&& otra) noexcept {
        if (this != &otra) {
            liberar();
            _datos = otra._datos;
            this->_filas = otra._filas;
            this->_columnas = otra._columnas;
            otra._datos = nullptr;
            otra._filas = 0;
            otra._columnas = 0;
        }
        return *this;
    }

    void cargarValores() override {
        std::cout << "Ingrese los valores de la matriz (" << this->_filas << "x" << this->_columnas << "):\n";
        for (int i = 0; i < this->_filas; ++i)
            for (int j = 0; j < this->_columnas; ++j) {
                std::cout << "[" << i << "," << j << "]: ";
                std::cin >> _datos[i][j];
            }
    }

    void establecer(int i, int j, T valor) {
        _datos[i][j] = valor;
    }

    T obtener(int i, int j) const override {
        return _datos[i][j];
    }

    Matrizbase<T>* sumar(const Matrizbase<T>& otra) const override {
        if (this->_filas != otra.filas() || this->_columnas != otra.columnas()) {
            std::cout << "Error: dimensiones incompatibles.\n";
            return nullptr;
        }
        Matrizdinamica<T>* resultado = new Matrizdinamica<T>(this->_filas, this->_columnas);
        for (int i = 0; i < this->_filas; ++i)
            for (int j = 0; j < this->_columnas; ++j)
                resultado->_datos[i][j] = _datos[i][j] + otra.obtener(i, j);
        return resultado;
    }

    void imprimir() const override {
        for (int i = 0; i < this->_columnas; ++i)
            std::cout <<"========";;
        std::cout << "\n";
        for (int i = 0; i < this->_filas; ++i) {
            for (int j = 0; j < this->_columnas; ++j) {
                std::cout << "|| " << _datos[i][j] << " ";
            }
            std::cout << "||\n";
            for (int k = 0; k < this->_columnas; ++k)
                std::cout << "=======";;
            std::cout << "\n";
        }
    }
};

template <typename T, int M, int N>
class Matrizestatica : public Matrizbase<T> {
private:
    T _datos[M][N];

public:
    Matrizestatica() : Matrizbase<T>(M, N) {
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < N; ++j)
                _datos[i][j] = T(0);
    }

    void cargarValores() override {
        std::cout << "Ingrese los valores de la matriz (" << M << "x" << N << "):\n";
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < N; ++j) {
                std::cout << "[" << i << "," << j << "]: ";
                std::cin >> _datos[i][j];
            }
    }

    void establecer(int i, int j, T valor) {
        _datos[i][j] = valor;
    }

    T obtener(int i, int j) const override {
        return _datos[i][j];
    }

    Matrizbase<T>* sumar(const Matrizbase<T>& otra) const override {
        if (M != otra.filas() || N != otra.columnas()) {
            std::cout << "Error: dimensiones incompatibles.\n";
            return nullptr;
        }
        Matrizestatica<T, M, N>* resultado = new Matrizestatica<T, M, N>();
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < N; ++j)
                resultado->_datos[i][j] = _datos[i][j] + otra.obtener(i, j);
        return resultado;
    }

    void imprimir() const override {
        for (int i = 0; i < N; ++i)
            std::cout <<"========";;
        std::cout << "\n";
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                std::cout << "|| " << _datos[i][j] << " ";
            }
            std::cout << "||\n";
            for (int k = 0; k < N; ++k)
                std::cout << "========";
            std::cout << "\n";
        }
    }
};

template <typename T>
Matrizbase<T>* operator+(const Matrizbase<T>& A, const Matrizbase<T>& B) {
    return A.sumar(B);
}

int main() {
    std::cout << "--- Sistema Generico de Algebra Lineal ---\n\n";
    
    std::cout << ">> Demostracion de Genericidad (Tipo INT) <<\n\n";

    std::cout << "Creando Matriz Dinamica A (2x2)\n";
    Matrizbase<int>* A1 = new Matrizdinamica<int>(2, 2);
    
    Matrizdinamica<int>* ptrA1 = dynamic_cast<Matrizdinamica<int>*>(A1);
    ptrA1->establecer(0, 0, 5); ptrA1->establecer(0, 1, 10);
    ptrA1->establecer(1, 0, 3); ptrA1->establecer(1, 1, 7);
    
    std::cout << "A =\n";
    std::cout << "||     A    ||\n";
    A1->imprimir();

    std::cout << "Creando Matriz Estatica B (2x2)\n";
    Matrizbase<int>* B1 = new Matrizestatica<int, 2, 2>();
    
    Matrizestatica<int, 2, 2>* ptrB1 = dynamic_cast<Matrizestatica<int, 2, 2>*>(B1);
    ptrB1->establecer(0, 0, 2); ptrB1->establecer(0, 1, 4);
    ptrB1->establecer(1, 0, 1); ptrB1->establecer(1, 1, 3);
    
    std::cout << "B=\n";
    std::cout << "||      B     ||\n";
    B1->imprimir();

    std::cout << "SUMANDO: Matriz C = A + B\n";
    std::cout << "Matriz Resultado C:\n\n";
    Matrizbase<int>* C1 = *A1 + *B1;
    
    if (C1 != nullptr) {
        std::cout << "||     C      ||\n";
        C1->imprimir();
    }

    std::cout << ">> Memoria liberada <<\n";
    if (C1 != nullptr) {
        delete C1;
        C1 = nullptr;
    }
    if (A1 != nullptr) {
        delete A1;
        A1 = nullptr;
    }
    if (B1 != nullptr) {
        delete B1;
        B1 = nullptr;
    }

    std::cout << "\n\n";
    std::cout << ">> Demostracion de Genericidad (Tipo FLOAT) <<\n\n";

    int opcion;
    std::cout << "Seleccione el modo de carga de datos:\n";
    std::cout << "1. Cargar valores manualmente\n";
    std::cout << "2. Usar valores predeterminados\n";
    std::cout << "Opcion: ";
    std::cin >> opcion;
    std::cout << "\n";

    std::cout << "Creando Matriz Dinamica A (3x2)\n";
    Matrizbase<float>* A = new Matrizdinamica<float>(3, 2);
    
    if (opcion == 1) {
        A->cargarValores();
    } else {
        Matrizdinamica<float>* ptrA = dynamic_cast<Matrizdinamica<float>*>(A);
        ptrA->establecer(0, 0, 1.5f); ptrA->establecer(0, 1, 2.0f);
        ptrA->establecer(1, 0, 0.0f); ptrA->establecer(1, 1, 1.0f);
        ptrA->establecer(2, 0, 4.5f); ptrA->establecer(2, 1, 3.0f);
    }
    std::cout << "A =\n";
    std::cout << "||     A    ||\n";
    A->imprimir();

    std::cout << "Creando Matriz Estatica B (3x2)\n";
    Matrizbase<float>* B = new Matrizestatica<float, 3, 2>();
    
    if (opcion == 1) {
        B->cargarValores();
    } else {
        Matrizestatica<float, 3, 2>* ptrB = dynamic_cast<Matrizestatica<float, 3, 2>*>(B);
        ptrB->establecer(0, 0, 0.5f); ptrB->establecer(0, 1, 1.0f);
        ptrB->establecer(1, 0, 2.0f); ptrB->establecer(1, 1, 3.0f);
        ptrB->establecer(2, 0, 1.0f); ptrB->establecer(2, 1, 1.0f);
    }
    std::cout << "B=\n";
    std::cout << "||      B     ||\n";
    B->imprimir();

    std::cout << "SUMANDO: Matriz C = A + B\n";
    std::cout << "Matriz Resultado C:\n\n";
    Matrizbase<float>* C = *A + *B;
    
    if (C != nullptr) {
        std::cout << "||     C      ||\n";
        C->imprimir();
    }

    std::cout << ">> Memoria liberada <<\n";
    if (C != nullptr) {
        delete C;
        C = nullptr;
    }
    if (A != nullptr) {
        delete A;
        A = nullptr;
    }
    if (B != nullptr) {
        delete B;
        B = nullptr;
    }
    std::cout << "Sistema cerrado.\n";

    return 0;
}