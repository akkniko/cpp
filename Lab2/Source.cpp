#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <map>
#include <string>
#include <cmath>
#include <random>
#include <iomanip>

/// Структура для хранения коэффициентов квадратного уравнения
struct Equation {
    double a, b, c;
};

/// Типы студентов
enum class StudentType {
    Excellent,  /// Отличник
    Average,    /// Средний
    Poor        /// Плохой
};

/// Структура для представления студента
struct Student {
    std::string name;
    StudentType type;
};

/// Структура для письма в очереди
struct Letter {
    Equation equation;                         /// Уравнение
    std::pair<double, double> studentAnswer;  /// Ответ студента
    std::string studentName;                 /// Имя студента
};

/// Чтение уравнений из файла
/// в файле поданы коэффициенты квадратных уравнений, в этой функции считываю их
std::vector<Equation> readEquations(const std::string& filename) {
    std::vector<Equation> equations;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл: " << filename << std::endl;
        return equations;
    }
    double a, b, c;
    while (file >> a >> b >> c) {
        equations.push_back({ a, b, c });
    }
    file.close();
    return equations;
}

            /// Решение квадратного уравнения
std::pair<double, double> solveQuadratic(double a, double b, double c) {
    if (a == 0) {  /// Линейное уравнение
        
        if (b == 0)  return { NAN, NAN };  /// если нет корней
        
        else {
            double root = -c / b;
            return { root, root };  /// Один корень
        }
    }
    double discriminant = b * b - 4 * a * c;
    if (discriminant > 0) {  /// 2 корня
        double root1 = (-b + sqrt(discriminant)) / (2 * a);
        double root2 = (-b - sqrt(discriminant)) / (2 * a);
        return { root1, root2 };
    }
    else if (discriminant == 0) {  /// 1 корень
        double root = -b / (2 * a);
        return { root, root };
    }
    else return { NAN, NAN };
    ///Для комплексных корней, но в моих уравнениях корни - целые числа.
}

/// Генерация ответа студента
std::pair<double, double> generateStudentAnswer(const Student& student, const Equation& eq) {
    std::random_device rd;
    std::mt19937 gen(rd()); ///генератор случайных чисел
    std::uniform_real_distribution<> dis(0.0, 1.0); ///здесь распределяются случайные числа с 
                                                   ///равномерной плотностью вероятности

    switch (student.type) {
    case StudentType::Excellent:  /// Отличник: всегда правильный ответ
        return solveQuadratic(eq.a, eq.b, eq.c);
    case StudentType::Average:    /// Средний: 60% вероятность правильного ответа
        if (dis(gen) < 0.6) {
            return solveQuadratic(eq.a, eq.b, eq.c);
        }
        else {
            return { dis(gen) * 10, dis(gen) * 10 };
        }
    case StudentType::Poor:      
        return { 0.0, 0.0 };
    default:
        return { NAN, NAN };
    }
}

/// Проверка ответа студента
bool checkAnswer(const Equation& eq, const std::pair<double, double>& studentAnswer) {
    auto correctAnswer = solveQuadratic(eq.a, eq.b, eq.c);
    const double epsilon = 1e-6;  

    if (std::isnan(correctAnswer.first) && std::isnan(studentAnswer.first)) return true;

    if (std::abs(correctAnswer.first - studentAnswer.first) < epsilon &&
        std::abs(correctAnswer.second - studentAnswer.second) < epsilon) {
        return true;
    }

    if (std::abs(correctAnswer.first - studentAnswer.second) < epsilon &&
        std::abs(correctAnswer.second - studentAnswer.first) < epsilon) {
        return true;
    }

    return false;
}

///вывод таблицы
void printResults(const std::map<std::string, std::pair<int, int>>& results) {
    /// максимальная длина имени для выравнивания столбцов
    size_t maxNameLength = 0;
    for (const auto& pair : results) {
        if (pair.first.length() > maxNameLength) {
            maxNameLength = pair.first.length();
        }
    }

    std::cout << std::left << std::setw(maxNameLength + 2) << "ФИО студента"
        << std::setw(20) << "Правильно решено"
        << "Всего задач" << "\n";
    std::cout << std::string(maxNameLength + 40, '-') << "\n";

    for (const auto& pair : results) {
        std::cout << std::left << std::setw(maxNameLength + 2) << pair.first
            << std::setw(20) << pair.second.first
            << pair.second.second << "\n";
    }
}


int main() {
    setlocale(LC_CTYPE, "RU");

    std::vector<Equation> equations = readEquations("equations.txt");
    if (equations.empty()) {
        std::cout << "Не удалось загрузить уравнения.\n";
        return 1;
    }

    /// Список студентов
    std::vector<Student> students = {
        {"Иванов Иван", StudentType::Excellent},
        {"Петров Петр", StudentType::Average},
        {"Андреев Николай", StudentType::Excellent},
        {"Абаков Виктор", StudentType::Poor},
        {"Крылов Григорий", StudentType::Average},
        {"Жукарев Федор", StudentType::Average},
        {"Ситникова Арина", StudentType::Excellent},
        {"Золотухин Максим", StudentType::Average},
        {"Егоров Александр", StudentType::Average},
        {"Алексеев Алексей", StudentType::Poor},
        {"Юров Евгений", StudentType::Poor}
    };

    /// Очередь писем
    std::queue<Letter> letterQueue;
    for (const auto& student : students) {
        for (const auto& eq : equations) {
            auto answer = generateStudentAnswer(student, eq);
            letterQueue.push({ eq, answer, student.name });
        }
    }

    /// Таблица результатов: имя -> (правильные ответы, общее количество)
    std::map<std::string, std::pair<int, int>> results;
    while (!letterQueue.empty()) {
        Letter letter = letterQueue.front();
        letterQueue.pop();
        bool isCorrect = checkAnswer(letter.equation, letter.studentAnswer);
        auto& result = results[letter.studentName];
        result.second++;  
        if (isCorrect) {
            result.first++;  
        }
    }

    printResults(results);

    return 0;
}