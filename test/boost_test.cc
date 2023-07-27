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
    return boost::json::value_from((c1.*pmf)(boost::json::value_to<boost::remove_cv_ref_t<A>>(args[I])...));
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

#define JSONRPC4C_REGISTER(C, Members) \
    BOOST_DESCRIBE_STRUCT(C, (), Members)

JSONRPC4C_REGISTER(Object, (Greet, Add))

TEST(BoostTestSuite, FoundObjectMethodTest) {
    Object obj;
    ASSERT_TRUE(Found<Object>(obj, "Add"));
    ASSERT_FALSE(Found<Object>(obj, "Sub"));
}

TEST(BoostTestSuite, CallObjectMethodTest) {
    Object obj;
    ASSERT_EQ(Call(obj, "Greet", {"World"}), "Hello, World!");
    ASSERT_EQ(Call(obj, "Add", {1, 2}), 3);
}

TEST(BoostTestSuite, JsonTest) {
    std::string body = "{\"id\":\"1604283212\", \"jsonrpc\":\"2.0\", \"method\":\"IntRpc/Add\", \"params\": [1, 6]}";
    using namespace boost::json;
    object object = parse(body).as_object();
    ASSERT_EQ(object["jsonrpc"], "2.0");
    ASSERT_EQ(object["id"], "1604283212");

    // Test call the method using the array params
    Object obj;
    ASSERT_EQ(Call(obj, "Add", object["params"].as_array()), 7);
}

struct F {
    std::map<std::string, std::string>
    Join(std::map<std::string, std::string> b, std::map<std::string, std::string> c) {
        return std::map < std::string, std::string > {{"name", b.at("name") + " " + c.at("name")}};
    }
};

JSONRPC4C_REGISTER(F, (Join))

TEST(BoostTestSuite, ComplexJsonTest) {
    std::string body = "{\"id\":\"1604283212\", \"jsonrpc\":\"2.0\", \"method\":\"IntRpc/Add\", \"params\": [{\"name\":\"Hello\"}, {\"name\":\"World\"}]}";
    using namespace boost::json;
    F f;
    object object = parse(body).as_object();
    ASSERT_EQ(Call(f, "Join", object["params"].as_array()).at("name"), "Hello World");
}
