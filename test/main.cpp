#include <utility>
#include "fb/tag_invoke.hpp"
#include "test.hpp"

constexpr struct test_cpo_t {
	template <typename T>
	friend int tag_invoke(test_cpo_t, const T&) noexcept {
		return 1;
	}

	template <typename T>
	constexpr auto operator()(T&& t) const noexcept(fb::is_nothrow_tag_invocable_v<test_cpo_t, T&&>)
						     -> fb::tag_invoke_result_t<test_cpo_t, T&&>
	{
		return fb::tag_invoke(*this, std::forward<T&&>(t));
	}
} test_cpo = {};

struct foo {
	friend int tag_invoke(fb::tag_t<test_cpo>, foo const&) {
		return 2;
	}
};

struct bar {
	friend int tag_invoke(fb::tag_t<test_cpo>, bar const&) {
		return 3;
	}
};

constinit auto tests = std::tuple{
	[](){
		test::assert(test_cpo(nullptr) == 1);
		test::assert(test_cpo(foo{}) == 2);
		test::assert(test_cpo(bar{}) == 3);
	}
};

int main(int argc, char const* args[]) {
	return test::run(tests);
}
