/**
 *  Test the flavors of front() and back()
 */
void element_test()
{
    std::cout << "Testing element access methods:" << std::endl;

    my_list = new std::list<int>({1, 2, 3, 4, 5, 6});
    // start with a basic test
    int f = my_list->front();
    int b = my_list->back();
    std::cout << "Expect 1, 6: " << f << ", " << b << std::endl;

    std::list<int>::const_reference cb = my_list->back();
    std::list<int>::const_reference cf = my_list->front();
    // Ensure these don't compile:
    // cf += 11;
    // cb += 11;
    std::list<int>::reference rb = my_list->back();
    std::list<int>::reference rf = my_list->front();
    rb += 11;
    rf -= 11;

    std::cout << "Expect -10, 17, -10, 17: " << cf << ", " << cb << ", "
              << rf << ", " << rb << std::endl;
    delete my_list;
}
