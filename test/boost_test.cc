#include <gtest/gtest.h>
#include <boost/describe.hpp>
#include <boost/mp11.hpp>

template<class C>
bool FoundObjectMethod(C &c, std::string method) {
    using Fd = boost::describe::describe_members<C,
            boost::describe::mod_public | boost::describe::mod_function>;

    bool found = false;

    boost::mp11::mp_for_each<Fd>([&](auto D) {
        if( !found && method == D.name)
        {
            found = true;
        }
    });
    return found;
}

struct Object {
    int add(int x, int y) {
        return x + y;
    }
};

BOOST_DESCRIBE_STRUCT(Object, (), (add))

TEST(BoostTestSuite, TestFoundObjectMethod) {
    Object obj;
    ASSERT_TRUE(FoundObjectMethod<Object>(obj, "add"));
    ASSERT_FALSE(FoundObjectMethod<Object>(obj, "greet"));
}
