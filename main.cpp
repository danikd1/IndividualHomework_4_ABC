#include <iostream>
#include <vector>
#include <algorithm>
#include <pthread.h>
#include <random>
#include <mutex>

// Структура для книги
struct Book {
    int id;
    int row;
    int cabinet;
    int shelf;
    int title; // Используем int для простоты
};

// Глобальный каталог и мьютекс для его синхронизации
std::vector<Book> catalog;
std::mutex catalogMutex;

// Функция для обработки книги в потоке
void* processBook(void* arg) {
    Book* book = (Book*)arg;

    // Синхронизированное добавление книги в каталог
    std::lock_guard<std::mutex> guard(catalogMutex);
    catalog.push_back(*book);

    return nullptr;
}

void getInput(int& M, int& N, int& K) {
    std::cout << "Введите кол-во рядов: ";
    std::cin >> M;
    std::cout << "Введите кол-во шкафов в ряду: ";
    std::cin >> N;
    std::cout << "Введите кол-во книг в шкафу: ";
    std::cin >> K;
}

int main(int argc, char* argv[]) {
    int M, N, K;

    if (argc == 4) {
        // Если аргументы предоставлены через командную строку
        M = std::stoi(argv[1]);
        N = std::stoi(argv[2]);
        K = std::stoi(argv[3]);
    } else {
        // Если аргументы не предоставлены, запросить их через консоль
        getInput(M, N, K);
    }

    std::vector<Book> allBooks;
    allBooks.reserve(M * N * K);

    // Инициализация книг с уникальными ID и названиями
    int id = 0;
    for (int i = 0; i < M * N * K; ++i) {
        allBooks.push_back({id++, -1, -1, -1, rand() % 1000});
    }

    // Перемешиваем книги
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(allBooks.begin(), allBooks.end(), g);

    // Распределяем книги по шкафам
    std::vector<std::vector<std::vector<Book>>> cabinets(M, std::vector<std::vector<Book>>(N));
    int bookIndex = 0;
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < K; ++k) {
                if (bookIndex < allBooks.size()) {
                    Book &book = allBooks[bookIndex++];
                    book.row = i;
                    book.cabinet = j;
                    book.shelf = k;
                    cabinets[i][j].push_back(book);
                }
            }
        }
    }

    // Выводим список замещения книг
    std::cout << "Исходное распределение книг:\n";
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            for (const auto &book : cabinets[i][j]) {
                std::cout << "Ряд: " << book.row << ", Шкаф: " << book.cabinet << ", Номер книги в шкафу: " << book.shelf << ", Название: " << book.title <<
                ", Уникальный ID книги: " << book.id << "\n";
            }
        }
    }

    // Создание и запуск потоков
    std::vector<pthread_t> threads;
    threads.reserve(allBooks.size());
    for (auto &book : allBooks) {
        pthread_t thread;
        pthread_create(&thread, nullptr, processBook, (void*)&book);
        std::cout << "\033[32m" << "Создан поток " << thread << " для книги с ID: " << book.id << "\033[0m" << std::endl;
        threads.push_back(thread);
    }

    // Ожидание завершения потоков
    for (auto &thread : threads) {
        pthread_join(thread, nullptr);
    }

    // Сортировка каталога
    std::sort(catalog.begin(), catalog.end(), [](const Book &a, const Book &b) {
        return a.title < b.title;
    });

    // Вывод каталога
    std::cout << "\nОтсортированный каталог:\n";
    for (const auto &book : catalog) {
        std::cout <<"Ряд: " << book.row << ", Шкаф: " << book.cabinet << ", Номер книги в шкафу: " << book.shelf << ", Название: " << book.title <<
        " ---> Уникальный ID книги: " << book.id << "\n";
    }
    return 0;
}
