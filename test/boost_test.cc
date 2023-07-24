#include <gtest/gtest.h>
#include <boost/describe.hpp>
#include <boost/mp11.hpp>
#include <boost/json/src.hpp>
#include <boost/type_traits.hpp>
#include <boost/utility/string_view.hpp>
#include <stdexcept>
#include <string>

template<class C1, class C2, class R, class... A, std::size_t... I>
boost::json::value
CallImpl_(C1 &c1, R (C2::*pmf)(A...), boost::json::array const &args,
          std::index_sequence<I...>) {
    return boost::json::value_from((c1.*pmf)(boost::json::value_to<boost::remove_cv_ref_t<A> >(args[I])...));
}

template<class C1, class C2, class R, class... A>
boost::json::value
CallImpl(C1 &c1, R (C2::*pmf)(A...), boost::json::array const &args) {
    if (args.size() != sizeof...(A)) {
        throw std::invalid_argument("Invalid number of arguments");
    }

    return CallImpl_(c1, pmf, args, std::index_sequence_for<A...>());
}

template<class C>
boost::json::value
Call(C &c, std::string method, boost::json::value const &args) {
    using Fd = boost::describe::describe_members<C, boost::describe::mod_public | boost::describe::mod_function>;

    bool found = false;
    boost::json::value result;

    boost::mp11::mp_for_each<Fd>([&](auto D) {
        if (!found && method == D.name) {
            result = CallImpl(c, D.pointer, args.as_array());
            found = true;
        }
    });

    if (!found) {
        throw std::invalid_argument("Invalid method name");
    }

    return result;
}

template<class C>
bool Found(C &c, std::string method) {
    using Fd = boost::describe::describe_members<C,
            boost::describe::mod_public | boost::describe::mod_function>;

    bool found = false;

    boost::mp11::mp_for_each<Fd>([&](auto D) {
        if (!found && method == D.name) {
            found = true;
        }
    });
    return found;
}

struct Object {
    std::string Greet(std::string const &who) {
        return "Hello, " + who + "!";
    }

    int Add(int x, int y) {
        return x + y;
    }
};

#define JSONRPC4C_REGISTER(C, Bases, Members) \
    BOOST_DESCRIBE_STRUCT(C, Bases, Members)

JSONRPC4C_REGISTER(Object, (), (Greet, Add))

TEST(BoostTestSuite, TestFoundObjectMethod) {
    Object obj;
    ASSERT_TRUE(Found<Object>(obj, "Add"));
    ASSERT_FALSE(Found<Object>(obj, "Sub"));
}

TEST(BoostTestSuite, TestCallObjectMethod) {
    Object obj;
    ASSERT_EQ(Call(obj, "Greet", {"World"}), "Hello, World!");
    ASSERT_EQ(Call(obj, "Add", {1, 2}), 3);
}
