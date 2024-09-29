#include <iostream>
#include <Eigen/Dense>
#include <random>
#include <vector>
#include <iomanip>

using namespace std;
using Eigen::MatrixXd;

// Function to generate two random matrices
pair<MatrixXd, MatrixXd> Generate_Matrix() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(-100, 100);

    MatrixXd A(10, 10);
    MatrixXd B(10, 10);
    
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            A(i, j) = dis(gen);
            B(i, j) = dis(gen);
        }
    }
    return make_pair(A, B);
}

// Function to find Nash equilibria
vector<pair<int, int>> Nash(const pair<MatrixXd, MatrixXd>& matrix) {
    int N = matrix.first.rows();
    vector<pair<int, int>> Nashes;

    vector<pair<int, int>> A_Player(N);
    for (int j = 0; j < N; ++j) {
        double cur_Max = -101;
        pair<int, int> cur_Index;
        for (int i = 0; i < N; ++i) {
            if (matrix.first(i, j) > cur_Max) {
                cur_Max = matrix.first(i, j);
                cur_Index = {i, j};
            }
        }
        A_Player[j] = cur_Index;
    }

    vector<pair<int, int>> B_Player(N);
    for (int i = 0; i < N; ++i) {
        double cur_Max = -101;
        pair<int, int> cur_Index;
        for (int j = 0; j < N; ++j) {
            if (matrix.second(i, j) > cur_Max) {
                cur_Max = matrix.second(i, j);
                cur_Index = {i, j};
            }
        }
        B_Player[i] = cur_Index;
    }

    for (const auto& element : A_Player) {
        if (find(B_Player.begin(), B_Player.end(), element) != B_Player.end()) {
            Nashes.push_back(element);
        }
    }
    
    return Nashes;
}

// Function to find Pareto optimal points
vector<pair<int, int>> Pareto(const pair<MatrixXd, MatrixXd>& matrix) {
    int N = matrix.first.rows();
    vector<pair<int, int>> Paretos;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            bool flag = true;
            for (int k = 0; k < N; ++k) {
                for (int z = 0; z < N; ++z) {
                    if (k == i && z == j) continue;
                    if (matrix.first(k, z) > matrix.first(i, j) &&
                        matrix.second(k, z) > matrix.second(i, j)) {
                        flag = false;
                    }
                }
            }
            if (flag) {
                Paretos.push_back({i, j});
            }
        }
    }
    
    return Paretos;
}

// Function to print the matrices and results
void Print_Matrix(const pair<MatrixXd, MatrixXd>& matrix, const vector<pair<int, int>>& Nashes, const vector<pair<int, int>>& Paretos) {
    int N = matrix.first.rows();

    cout << "\n" << string(50, '-') << endl;
    cout << setw(10) << " " << setw(15) << "Игрок 1" << setw(15) << "Игрок 2" << endl;
    cout << setw(10) << " " << string(15, '-') << string(15, '-') << endl;

    for (int i = 0; i < N; ++i) {
        cout << setw(10) << "Стратегия " << i + 1 << ": ";
        for (int j = 0; j < N; ++j) {
            string prefix = "  ";
            if (find(Nashes.begin(), Nashes.end(), make_pair(i, j)) != Nashes.end() &&
                find(Paretos.begin(), Paretos.end(), make_pair(i, j)) != Paretos.end()) {
                prefix = "НП";  // Nash-Pareto
            } else if (find(Nashes.begin(), Nashes.end(), make_pair(i, j)) != Nashes.end()) {
                prefix = "Н ";  // Nash
            } else if (find(Paretos.begin(), Paretos.end(), make_pair(i, j)) != Paretos.end()) {
                prefix = "П";  // Pareto
            }

            cout << prefix << setw(5) << matrix.first(i, j) << " / " << setw(5) << matrix.second(i, j) << "  ";
        }
        cout << endl;
    }

    // Print Nash equilibria
    if (!Nashes.empty()) {
        cout << "\nРавновесие по Нэшу:\n";
        for (const auto& elem : Nashes) {
            cout << "(" << elem.first + 1 << ", " << elem.second + 1 << ") "
                 << matrix.first(elem.first, elem.second) << " / "
                 << matrix.second(elem.first, elem.second) << endl;
        }
    }

    // Print Pareto optimal points
    if (!Paretos.empty()) {
        cout << "\nОптимальность по Парето:\n";
        for (const auto& elem : Paretos) {
            cout << "(" << elem.first + 1 << ", " << elem.second + 1 << ") "
                 << matrix.first(elem.first, elem.second) << " / "
                 << matrix.second(elem.first, elem.second) << endl;
        }
    }

    // Print intersections
    vector<pair<int, int>> Intersections;
    for (const auto& elem : Nashes) {
        if (find(Paretos.begin(), Paretos.end(), elem) != Paretos.end()) {
            Intersections.push_back(elem);
        }
    }

    if (!Intersections.empty()) {
        cout << "\nПересечение множеств Нэша и Парето:\n";
        for (const auto& elem : Intersections) {
            cout << "(" << elem.first + 1 << ", " << elem.second + 1 << ") "
                 << matrix.first(elem.first, elem.second) << " / "
                 << matrix.second(elem.first, elem.second) << endl;
        }
    }
}

// Function to implement the Prisoner's Dilemma
void Prisoners_Dilemma() {
    cout << "\n\n\n----------------------------------------------------------------\n\n\n";
    cout << "Дилемма заключенного:\n";
    
    // Payoff matrix for the Prisoner's Dilemma
    MatrixXd A(2, 2);
    MatrixXd B(2, 2);
    
    // Fill the payoff matrices (A: Player 1, B: Player 2)
    A << -1, -3,
         0, -2; // Payoffs for Player 1
         
    B << -1, 0,
         -3, -2; // Payoffs for Player 2

    cout << "Матрица Игрока 1:\n" << A << endl;
    cout << "Матрица Игрока 2:\n" << B << endl;

    auto Nashes = Nash(make_pair(A, B));
    auto Paretos = Pareto(make_pair(A, B));
    Print_Matrix(make_pair(A, B), Nashes, Paretos);
}

int main() {
    cout << "\n\n\n----------------------------------------------------------------\n\n\n";
    cout << "Случайная матрица 10x10:\n";
    
    auto matrix = Generate_Matrix();
    cout << "Матрица Игрока 1:\n" << matrix.first << endl;
    cout << "Матрица Игрока 2:\n" << matrix.second << endl;

    auto Nashes = Nash(matrix);
    auto Paretos = Pareto(matrix);
    Print_Matrix(matrix, Nashes, Paretos);

    Prisoners_Dilemma();

    cout << "\n\n\n----------------------------------------------------------------\n\n\n";
    cout << "Перекресток\n";
    matrix = { (MatrixXd(2, 2) << 2, 1, 3, 0).finished(), (MatrixXd(2, 2) << 2, 3, 1, 0).finished() };
    cout << "Матрица Игрока 1:\n" << matrix.first << endl;
    cout << "Матрица Игрока 2:\n" << matrix.second << endl;

    Nashes = Nash(matrix);
    Paretos = Pareto(matrix);
    Print_Matrix(matrix, Nashes, Paretos);

    cout << "\n\n\n----------------------------------------------------------------\n\n\n";

    return 0;
}
