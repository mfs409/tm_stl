/**
 * Test the many modifier methods of std::list
 */
void modifier_test()
{
    // create a new list with 9 elements, so we can be sure the elements get
    // deleted
    my_list = new std::list<int>(9);

    // create another list, that we can use for assigning into the first list
    std::list<int> l = {1, 2, 3, 4, 5};

    // test range assignment (1)
    auto i = l.begin();
    i++;i++;i++;
    my_list->assign(l.begin(), i);
    check("Expect 1, 2, 3");

    // test fill assignment (2)
    my_list->assign(8, 32);
    check("Expect 32, 32, 32, 32, 32, 32, 32, 32");

    // test initializer list assignment (3)
    my_list->assign({8, 7, 6, 5, 4, 3, 2, 1});
    check("Expect 8, 7, 6, 5, 4, 3, 2, 1");

    // test emplace_front
    my_list->emplace_front(7);
    check("Expect 7, 8, 7, 6, 5, 4, 3, 2, 1");

}
