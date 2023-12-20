# Дельцов Даниил Витальевич, БПИ226

Вариант 26. Задача об инвентаризации по книгам. После нового года в библиотеке университета обнаружилась пропажа каталога. После поиска и наказания, виноватых ректор дал указание восстановить каталог силами студентов. Фонд библиотека представляет собой прямоугольное помещение, в котором находится M рядов по N шкафов по K книг в каждом шкафу. Требуется создать многопоточное приложение, составляющее каталог. При решении использовать метод «портфель задач», причем в качестве отдельного потока задается внесение в каталог записи об отдельной книге.
Примечание. Каталог — это список книг, упорядоченный (отсортированный) по названию книги (в данном случае в качестве названия можно взять и целое число — не обязательно использовать ASCII строку символов). Каждая строка каталога содержит идентифицирующее значение (номер или строку), местоположение книги, включающее номер ряда, номер шкафа, номер книги в шкафу. Перед запуском потоков по составлению каталогов необходимо случайным образом расположить книги в шкафах, используя генератор случайных чисел. Также нужно преварительно вывести список замещения книг по шкафам, чтобы в конце сопоставить его с тем, как они оказались описанными в каталоге.

Работа выполнена на оценку 7

[Код на C++ с комментарями](https://github.com/danikd1/IndividualHomework_4_ABC)

Пройдёмся по реализации: 
1) **std::vector <Book> catalog** - Этот вектор предназначен для хранения всех обработанных книг. Каждый элемент в векторе catalog представляет отдельную книгу и содержит следующую информацию, как определено в структуре Book:
**id**: идентификатор книги, для уникальной идентификации каждой книги в каталоге.
**row**: Номер ряда, в котором находится книга
**cabinet**: Номер шкафа, в котором находится книга
**shelf**: Номер книги, которая находится внутри шкафа
**title**: Название книги. В вашем коде, представлено целочисленным значением (int) для упрощения.

2) **std::mutex catalogMutex** -  Мьютекс, который обеспечивает синхронизацию доступа к этому каталогу. Мьютекс используется для предотвращения одновременного доступа к каталогу из разных потоков

3) **void processBook()** - Функция является точкой входа для потоков. Каждый поток обрабатывает один объект **Book**. В функции используется **std::lock_guard**, который автоматически блокирует мьютекс при создании и разблокирует при уничтожении, обеспечивая безопасный доступ к каталогу.

4) **Инициализация и Запуск Потоков**
   В функции main, после инициализации и перемешивания книг с помощью **std::random_device rd**, создаются потоки с помощью **pthread_create**. Каждый поток запускается с функцией processBook, и ему передается указатель на объект Book.
   ```C++
   std::vector<pthread_t> threads;
    threads.reserve(allBooks.size());
    for (auto &book : allBooks) {
    pthread_t thread;
    pthread_create(&thread, nullptr, processBook, (void*)&book);
    threads.push_back(thread);
    }
   ```
   После создания всех потоков основной поток должен дождаться их завершения.Это гарантирует, что основной поток не завершится до тех пор, пока не завершатся все дочерние потоки. Это делается с помощью функции pthread_join:
   ```C++
   for (auto &thread : threads) {
    pthread_join(thread, nullptr);
    }
    ```
5) **Сортировка каталога**
   После того как все потоки завершат работу и каталог будет полностью заполнен, происходит сортировка каталога и вывод его содержимого.
   ```C++
   std::sort(catalog.begin(), catalog.end(), [](const Book &a, const Book &b) {
        return a.title < b.title;
    });
   ```
   - Реализован ввод исходных данных из командной строки при запуске программы,если ввод данных не произошёл, пользователь вводит параметры с консоли.
   - Комментари написаны к каждой функции и сложным фрагментам кода, поясняющие выполняемые действия.

Ниже приведен скриншот работающей программы на малых данных:
<img width="1792" alt="Снимок экрана 2023-12-20 в 03 56 51" src="https://github.com/danikd1/IndividualHomework_4_ABC/assets/36849026/37393cce-ffaf-407d-9b66-4dee6ab49272">

Cкриншоты на больших данных записаны в output_1.txt, output_2.txt, output_3.txt, output_4.txt




   
