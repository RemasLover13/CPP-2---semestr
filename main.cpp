// Макрос заставит Catch самостоятельно добавить определение функции main()
// Это можно сделать только в одном файле
#define CATCH_CONFIG_MAIN
#include "include/catch2/catch.hpp"
#include <vector>


using namespace std;

vector<char> hw02_test(std::vector<int> ascii) {
    // Код домашнего задания
    // Преобразование ASCII кода в символьное представление (5 баллов)
    // На вход подается список - численное представление символов в ASCII виде
    // Требуется:
    //  a. представить ASCII код в виде символов (2 балла)
    //  b. отсортировать символы по убыванию (lambda) (1 балл)
    // Пример: input = {65, 66, 67}, output = {'C', 'B', 'A'}
    // Написать 5 тестов
    if (ascii.empty()) {
        return vector<char>();
    }

    vector<char> charVector(ascii.size());
    for (size_t i = 0; i != ascii.size(); ++i) {
        charVector[i] = static_cast<char> (ascii[i]);

    }
    sort(charVector.begin(), charVector.end(), [] (char a, char b) -> bool {
        return a > b;
    });
    return charVector;

}

TEST_CASE("Successfully conversation! ", "[full completed]") {
    REQUIRE(hw02_test(vector<int>{65,66,67}) == vector<char>{'C', 'B', 'A'});
    REQUIRE(hw02_test(vector<int>{65,66,67}) == vector<char>{'A', 'B', 'C'});
    REQUIRE(hw02_test(vector<int>{}) == vector<char>{});

}








//TEST_CASE("hw01", "[test]") {
//    REQUIRE(hw01_test(std::vector<int>{1, 2, 3, 4}) == std::vector<int>{2, 4});
//}
//
//TEST_CASE("Squares are computed", "[Square]")
//{
//    REQUIRE(Square(1) == 1);
//    REQUIRE(Square(2) == 4);
//    REQUIRE(Square(3) == 9);
//    REQUIRE(Square(7) == 49);
//    REQUIRE(Square(10) == 100);
//}