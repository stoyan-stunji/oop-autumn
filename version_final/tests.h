#ifndef TESTS_H
#define TESTS_H

#include "default_data_source.h"
#include "file_data_source.h"
#include "array_data_source.h"
#include "alternate_data_source.h"
#include "generator_data_source.h"

#include "constants.h"

struct person
{
    std::string name;
    int age;

    person()
    {
        this->name = "NULL";
        this->age = -1;
    }

    person(const std::string name, const int age)
    {
        this->name = name;
        this->age = age;
    }

    friend std::ostream& operator<<(std::ostream& o, const person& p)
    {
        o << p.name << ' ' << p.age << ' ';
        return o;
    }

    friend std::ifstream& operator>>(std::ifstream& i, person& p)
    {
        if(i.is_open())
        {
            std::string name = "";
            i >> name;
            p.name = name;

            int age = 0;
            i >> age;
            p.age = age;
        }

        return i;
    }

    const bool operator==(const person& o)
    {
        return this->age == o.age && this->name == o.name;
    }
};

//______________________________________________________________________________

void test_0_s_vector()
{
    int array[] = {1, 2, 3, 4, 5};
    s_vector<int> vector = {array, 5, 8};

    vector.push_back(6);
    vector.push_back(7);
    vector.push_back(8);

    vector.print_io(); // 1 2 3 4 5 6 7 8;
    std::cout << '\n';

    vector.pop_index(0);
    vector.pop_index(2);
    vector.pop_index(4);

    size_t size_2 = vector.get_size(); // 2 3 5 6 8;
    vector.print_io();
    std::cout << '\n';

    s_vector<int> copy = vector;

    size_t size_3 = copy.get_size(); // 2 3 5 6 8;
    copy.print_io();
    std::cout << '\n';

    copy.set_size(5);
    copy.set_capacity(8);
    copy.set_elements(array);

    size_t size_4 = copy.get_size(); // 1 2 3 4 5;
    copy.print_io();
    std::cout << '\n';

    vector = copy;

    size_t size_5 = vector.get_size(); // 1 2 3 4 5;
    vector.print_io();
    std::cout << '\n';
}

void test_1_s_vector()
{
    try
    {
        //int array[] = {1, 2, 3, 4, 5};
        //s_vector<int> vector = {array, 5, 5}; // ctor_capacity;
        //s_vector<int> vector = {array, 5, 8};
        //std::cout << vector[6]; // operator[];
        //std::cout << std::boolalpha << vector.pop_index(6) << '\n'; // false;
        //vector.set_capacity(5); // setter_capacity;
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << '\n';
    }   
}

void test_2_s_vector()
{
    person array[] = {person("Stoyan", 20), person("Jessica", 21), person("Kris", 19)};
    s_vector<person> vector = {array, 3, 8};

    vector.push_back(person("Philip", 20));

    vector.print_io(); // Stoyan 20  Jessica 21  Kris 19  Philip 20;
    std::cout << '\n';

    vector.pop_index(2);

    vector.print_io(); // Stoyan 20  Jessica 21  Philip 20;
    std::cout << '\n';

    s_vector<person> copy = vector;

    copy.print_io(); // Stoyan 20  Jessica 21  Philip 20;
    std::cout << '\n';

    copy.set_size(3);
    copy.set_capacity(8);
    copy.set_elements(array);

    copy.print_io(); // Stoyan 20  Jessica 21  Kris 19;
    std::cout << '\n';

    vector = copy;

    vector.print_io(); // Stoyan 20  Jessica 21  Kris 19;
    std::cout << '\n';
}

//______________________________________________________________________________

void test_0_default_data_source()
{
    default_data_source<int> dds;

    for(size_t i = 0; i < 16; ++i) // 16 x 0;
    {
        std::cout << dds.next() << ' ';
    }
    std::cout << '\n';

    std::cout << std::boolalpha << dds.end() << '\n'; // false (always);

    std::cout << std::boolalpha << dds.reset() << '\n'; // true;
 
    std::cout << std::boolalpha << dds.end() << '\n'; // false (always);

    s_vector<int> vec = dds.next_n(10); // 10 x 0;
    vec.print_io();
    std::cout << '\n';

    std::cout << dds() << '\n'; // 0;

    int a = 0;
    dds >> a;
    std::cout << a << '\n'; // 0;

    std::cout << std::boolalpha << (bool)dds << '\n'; // false (always);
} 

void test_1_default_data_source()
{
    default_data_source<person> dds;

    for(size_t i = 0; i < 16; ++i) // 16 x NUll -1
    {
        std::cout << dds.next() << ' ';
    }
    std::cout << '\n';

    std::cout << std::boolalpha << dds.end() << '\n'; // false (always);

    std::cout << std::boolalpha << dds.reset() << '\n'; // true;

    std::cout << std::boolalpha << dds.end() << '\n'; // false (always);

    s_vector<person> vec = dds.next_n(3); // 3 x NULL -1;
    vec.print_io();
    std::cout << '\n';

    std::cout << dds() << '\n'; // NULL -1;

    person a;
    dds >> a;
    std::cout << a << '\n'; // NULL -1

    std::cout << std::boolalpha << (bool)dds << '\n'; // false (always);
}

//______________________________________________________________________________

void test_0_file_data_source()
{
    file_data_source<int> fds("test_file_1.txt");

    while(!fds.end()) // 1-16;
    {
        std::cout << fds.next() << ' ';
    }
    std::cout << '\n';

    std::cout << std::boolalpha << fds.end() << '\n'; // true;

    std::cout << std::boolalpha << fds.reset() << '\n'; // true;

    std::cout << std::boolalpha << fds.end() << '\n'; // false;

    for (size_t i = 0; i < 5; i++)
    {
        fds.next();
    }  

    s_vector<int> vec = fds.next_n(25); // 6-16;
    vec.print_io();
    std::cout << '\n';

    std::cout << std::boolalpha << (bool)fds << '\n'; // true;

    std::cout << std::boolalpha << fds.reset() << '\n'; // true;

    std::cout << fds() << '\n'; // 1;

    int a = 0;
    fds >> a;
    int b = 0;
    fds >> b;
    std::cout << b << '\n'; // 3;

    std::cout << std::boolalpha << (bool)fds << '\n'; // false;
}

void test_1_file_data_source()
{
    file_data_source<person> fds("test_file_2.txt");

    while(!fds.end()) // Stoyan 20  Jessica 21  Kris 20  Philip 19  Apostol 15 Velizar 30;
    {
        std::cout << fds.next() << ' ';
    }
    std::cout << '\n';

    std::cout << std::boolalpha << fds.end() << '\n'; // true;

    std::cout << std::boolalpha << fds.reset() << '\n'; // true;

    std::cout << std::boolalpha << fds.end() << '\n'; // false;

    s_vector<person> vec = fds.next_n(3); // Stoyan 20 Jessica 21 Kris 20;
    vec.print_io();
    std::cout << '\n';

    std::cout << fds() << '\n'; // Philip 19;

    person a;
    fds >> a;
    std::cout << a << '\n'; // Apostol 15;

    std::cout << std::boolalpha << (bool)fds << '\n'; // false;

    while(!fds.end()) // Velizar 30;
    {
        std::cout << fds.next() << ' ';
    }
    std::cout << '\n';

    std::cout << std::boolalpha << (bool)fds << '\n'; // true;

    std::cout << std::boolalpha << fds.reset() << '\n'; // true;
}

//______________________________________________________________________________

void test_0_array_data_source()
{
    int array[] = {1, 2, 3, 4, 5};
    s_vector<int> vector = {array, 5, 8};
    array_data_source<int> ads(vector);

    while(!ads.end()) // 1-5;
    {
        std::cout << ads.next() << ' ';
    }
    std::cout << '\n';

    std::cout << std::boolalpha << ads.end() << '\n'; // true;

    std::cout << std::boolalpha << ads.reset() << '\n'; // true;

    std::cout << std::boolalpha << ads.end() << '\n'; // false;

    for (size_t i = 0; i < 2; i++)
    {
        ads.next();
    }  

    s_vector<int> vec = ads.next_n(2); // 3 4;
    vec.print_io();
    std::cout << '\n';

    std::cout << std::boolalpha << (bool)ads << '\n'; // false;

    std::cout << std::boolalpha << ads.reset() << '\n'; // true;

    std::cout << ads() << '\n'; // 1;

    int a = 0;
    ads >> a;
    int b = 0;
    ads >> b;
    std::cout << b << '\n'; // 3;

    std::cout << std::boolalpha << (bool)ads << '\n'; // false;

    std::cout << ads() << '\n'; // 4;
    std::cout << ads() << '\n'; // 5;

    std::cout << std::boolalpha << (bool)ads << '\n'; // true;
}

void test_1_array_data_source()
{
    person array[] = {person("Stoyan", 20), person("Jessica", 21), person("Kris", 19)};
    s_vector<person> vector = {array, 3, 8};
    array_data_source<person> ads(vector);

    while(!ads.end()) // Stoyan 20  Jessica 21  Kris 19;
    {
        std::cout << ads.next() << ' ';
    }
    std::cout << '\n';

    std::cout << std::boolalpha << ads.end() << '\n'; // true;

    std::cout << std::boolalpha << ads.reset() << '\n'; // true;

    std::cout << std::boolalpha << ads.end() << '\n'; // false;

    s_vector<person> vec = ads.next_n(2); // Stoyan 20  Jessica 21;
    vec.print_io();
    std::cout << '\n';

    std::cout << std::boolalpha << (bool)ads << '\n'; // false;

    std::cout << std::boolalpha << ads.reset() << '\n'; // true;

    std::cout << ads() << '\n'; // Stoyan 20;

    person a;
    ads >> a;
    std::cout << a << '\n'; // Jessica 21;

    std::cout << std::boolalpha << (bool)ads << '\n'; // false;

    std::cout << ads() << '\n'; // Kris 19;

    std::cout << std::boolalpha << (bool)ads << '\n'; // true;

    std::cout << std::boolalpha << ads.reset() << '\n'; // true;

    ads += person("Ally", 22);

    while(!(bool)ads) // Stoyan 20  Jessica 21  Kris 19  Ally 22
    {
        std::cout << ads() << ' ';
    }
    std::cout << '\n';

    std::cout << std::boolalpha << ads.reset() << '\n'; // true;

    std::cout << ads() << '\n'; // Stoyan 20;

    ads--;

    std::cout << ads() << '\n'; // Stoyan 20;

    while(!(bool)ads) // Jessica 21  Kris 19  Ally 22
    {
        std::cout << ads() << ' ';
    }
    std::cout << '\n';

    ads = ads + person("Dimitar", 20);

    std::cout << ads() << '\n'; // Dimitar 20

    std::cout << std::boolalpha << ads.end() << '\n'; // true

    std::cout << std::boolalpha << ads.reset() << '\n'; // true

    while(!(bool)ads) // Stoyan 20 Jessica 21  Kris 19  Ally 22 Dimitar 20
    {
        std::cout << ads() << ' ';
    }
    std::cout << '\n';

    std::cout << std::boolalpha << ads.end() << '\n'; // true
}

//______________________________________________________________________________

void test_0_alternate_data_source()
{
    file_data_source<int> fs("test_file_1.txt");

    int arr[] = {100, 200, 300, 400, 500};
    s_vector<int> vec = {arr, 5, 8};
    array_data_source<int> ads(vec);

    data_source<int>* array[] = {&ads, &fs};
    s_vector<data_source<int>*> vector = {array, 2, 8};
    alternate_data_source<int> altds(vector);

    while(!altds.end()) // 100 1 200 2 300 3 400 4 500 5 6 7 8 9 10 11 12 13 14 15 16; 
    {
        std::cout << altds.next() << ' ';
    }
    std::cout << '\n';

    std::cout << std::boolalpha << (bool)altds << '\n'; // true;

    std::cout << std::boolalpha << altds.reset() << '\n'; // true;

    std::cout << altds() << '\n'; // 100;

    int b = 0;
    altds >> b;
    std::cout << b << '\n'; // 1;

    s_vector<int> v = altds.next_n(10); // 200 2 300 3 400 4 500 5 6 7;
    v.print_io();
    std::cout << '\n';

    std::cout << std::boolalpha << altds.end() << '\n'; // false;

    while(!altds.end()) // 8 9 10 11 12 13 14 15 16;
    {
        std::cout << altds.next() << ' ';
    }
    std::cout << '\n';

    std::cout << std::boolalpha << (bool)altds << '\n'; // true;
}

void test_1_alternate_data_source()
{
    file_data_source<person> fs("test_file_2.txt");

    person arr[] = {person("Georgi", 25), person("Martin", 23)};
    s_vector<person> vec = {arr, 2, 8};
    array_data_source<person> ads(vec);

    data_source<person>* array[] = {&fs, &ads};
    s_vector<data_source<person>*> vector = {array, 2, 8};
    alternate_data_source<person> altds(vector);

    while(!altds.end()) // Stoyan 20  Georgi 25  Jessica 21  Martin 23  Kris 20  Philip 19  Apostol 15  Velizar 30;
    {
        std::cout << altds.next() << ' ';
    }
    std::cout << '\n';

    std::cout << std::boolalpha << (bool)altds << '\n'; // true;

    std::cout << std::boolalpha << altds.reset() << '\n'; // true;

    std::cout << altds() << '\n'; // Stoyan 20;

    person b;
    altds >> b;
    std::cout << b << '\n'; // Georgi 25;

    s_vector<person> v = altds.next_n(10); // Jessica 21 Martin 23  Kris 20  Philip 19  Apostol 15  Velizar 30;
    v.print_io();
    std::cout << '\n';

    std::cout << std::boolalpha << altds.reset() << '\n'; // true;
}

//______________________________________________________________________________

int consecutive_natural_numbers()
{
    static int number = 0;
    return number++;
}

int consecutive_even_numbers()
{
    static int number = 0;
    return number += 2;
}

void test_0_generator_data_source()
{
    generator_data_source<int> gds(consecutive_natural_numbers);

    std::cout << gds.next() << '\n'; // 0

    s_vector<int> vector = gds.next_n(10); // 1 2 3 4 5 6 7 8 9 10;
    vector.print_io();
    std::cout << '\n';

    std::cout << std::boolalpha << (bool)gds << '\n'; // false (always);

    int a = 0;
    gds >> a;
    std::cout << a << '\n'; // 11;

    std::cout << gds() << '\n'; // 12;

    std::cout << std::boolalpha << (bool)gds << '\n'; // false (always);

//______________________________________________________________________________

    generator_data_source<int> gds_o(consecutive_even_numbers);

    std::cout << gds_o.next() << '\n'; // 2;

    s_vector<int> vector_o = gds_o.next_n(25); // 4-52;
    vector_o.print_io();
    std::cout << '\n';
}

//____________________________TASK_EXAMPLES_____________________________________

std::string generate_strings()
{
    static const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";

    std::string temp;

    for (size_t i = 0; i < STRING_LENGHT; ++i)
    {
        temp += alphabet[rand() % (sizeof(alphabet) - 1)];
    }
    
    return temp;
}

void task_1()
{
    generator_data_source<std::string> gds(generate_strings);

    for (size_t i = 0; i < TASK_1_LIMIT; i++)
    {
        std::cout << gds() << ' ';
    }
}

bool is_prime(int n)
{
    if (n <= 1)
    {
        return false;
    }

    for (size_t i = 2; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            return false;
        }
    }
    return true;
}

int consecutive_prime_numbers()
{
    static int number = 1;  

    while (true)
    {
        ++number;

        if (is_prime(number)) 
        {
            return number;
        }
    }
}

int consecutive_random_real_numbers()
{
    return rand();
}

int consecutive_fibonacci_numbers()
{
    static int a = 1;
    static int b = 1;
    static int count = 0;

    if (count == 0)
    {
        ++count;
        return a;
    } 
    else if (count == 1)
    {
        ++count;
        return b;
    }

    int next = a + b;
    a = b;
    b = next;
    ++count;

    return next;
}

void bin_to_txt(const std::string& bin_file_name, const std::string& text_file_name)
{
    std::ifstream bin_file(bin_file_name, std::ios::binary);

    if (!bin_file)
    {
        std::cout << "bin_to_txt::bin_file_error\n";
        return;
    }

    std::ofstream text_file(text_file_name);
    if (!text_file)
    {
        std::cout << "bin_to_txt::txt_file_error\n";
        return;
    }

    int n = 0;
    size_t count = 0;

    while (bin_file.read(reinterpret_cast<char*>(&n), sizeof(n)))
    {
        if(++count >= TASK_2_LIMIT_FILE)
        {
            text_file << n;
        }
        else
        {
            text_file << n << '\n';
        }
    }

    if (!bin_file.eof())
    {
        std::cout << "bin_to_txt::failed_read_from_bin\n";
        bin_file.close();
        text_file.close();
        return;
    }

    bin_file.close();
    text_file.close();
}

void task_2()
{
    generator_data_source<int> gds_prime(consecutive_prime_numbers);
    generator_data_source<int> gds_real(consecutive_random_real_numbers);
    
    generator_data_source<int> gds_fibonacci(consecutive_fibonacci_numbers);
    s_vector<int> vector = gds_fibonacci.next_n(TASK_2_LIMIT_FIB);
    array_data_source<int> ads(vector);

    data_source<int>* array[] = {&gds_prime, &gds_real, &ads};
    s_vector<data_source<int>*> sources = {array, 3, 8};
    alternate_data_source<int> altds(sources);
    
    std::string file_name;
    std::cin >> file_name;
 
    std::ofstream out(file_name + ".dat", std::ios::binary);
    if(out.is_open())
    {
        for (size_t i = 0; i < TASK_2_LIMIT_FILE; ++i)
        {
            int j = 0;
            altds >> j;
            
            out.write(reinterpret_cast<const char*>(&j), sizeof(j));
        }
    }
    out.close();

    std::string old_name = file_name + ".dat";
    std::string new_name = file_name + ".txt";

    std::ifstream in_dat(old_name, std::ios::binary);
    std::ofstream out_txt(new_name);

    bin_to_txt(old_name, new_name);

    file_data_source<int> fs(new_name.c_str());
    while(!fs.end())
    {
        std::cout << fs() << ' ';
    }
}

//______________________________________________________________________________

void run()
{
    //test_0_s_vector();
    //test_1_s_vector();
    //test_2_s_vector();

    //test_0_default_data_source();
    //test_1_default_data_source();

    //test_0_file_data_source();
    //test_1_file_data_source();

    //test_0_array_data_source();
    //test_1_array_data_source();

    //test_0_alternate_data_source();
    //test_1_alternate_data_source();

    //test_0_generator_data_source();

    std::cout << "\nTASK_1: \n";
    task_1();
    std::cout << "\n\nTASK_2: \n";
    std::cout << "[enter file name]: "; // user_file
    task_2();

}

#endif