#ifndef FB_TAG_INVOKE_HPP
#define FB_TAG_INVOKE_HPP

namespace fb {
	namespace tag_invoke_detail {
		template <typename T> struct remove_reference { using type = T; };
		template <typename T> struct remove_reference<T&> { using type = T; };
		template <typename T> struct remove_reference<T&&> { using type = T; };
		template <typename T> using remove_reference_t = typename remove_reference<T>::type;

		template <typename T>
		constexpr T&& forward(remove_reference_t<T>& t) noexcept {
			return static_cast<T&&>(t);
		}

		template <typename T>
		constexpr T&& forward(remove_reference_t<T>&& t) noexcept {
			return static_cast<T&&>(t);
		}

		template <typename T>
		constexpr T&& declval() noexcept;
	}

	namespace tag_invoke_cpo {
		constexpr inline struct tag_invoke_t {
			template <typename CPO, typename... Args>
			constexpr auto operator()(CPO&& cpo, Args&&... args) const noexcept(noexcept(tag_invoke(tag_invoke_detail::declval<CPO>(), tag_invoke_detail::declval<Args>()...)))
											 -> decltype(tag_invoke(tag_invoke_detail::forward<CPO&&>(cpo), tag_invoke_detail::forward<Args&&>(args)...))
			{
				return tag_invoke(tag_invoke_detail::forward<CPO&&>(cpo), tag_invoke_detail::forward<Args&&>(args)...);
			}
		} tag_invoke = {};

		template <typename CPO, typename... Args>
		using tag_invoke_result_t = decltype(tag_invoke(tag_invoke_detail::declval<CPO>(), tag_invoke_detail::declval<Args>()...));
	}

	using tag_invoke_cpo::tag_invoke;
	using tag_invoke_cpo::tag_invoke_result_t;

	namespace tag_invoke_detail {
		template <typename I, I val>
		struct integral_constant {
			static constexpr I value = val;
		};

		using true_type = integral_constant<bool, true>;
		using false_type = integral_constant<bool, false>;

		template <typename CPO, typename... Args>
		constexpr auto try_tag_invoke(int) noexcept(noexcept(tag_invoke(declval<CPO>(), declval<Args>()...)))
						-> decltype(tag_invoke(declval<CPO>(), declval<Args>()...), true_type{});

		constexpr auto try_tag_invoke(...) noexcept(false)
						-> false_type;
	}

	template <typename CPO, typename... Args>
	inline constexpr bool is_tag_invocable_v = decltype(tag_invoke_detail::try_tag_invoke<CPO, Args...>)::value;

	template <typename CPO, typename... Args>
	inline constexpr bool is_nothrow_tag_invocable_v = noexcept(tag_invoke_detail::try_tag_invoke<CPO, Args...>);

	namespace tag_invoke_detail {
		template <typename T> struct remove_cv { using type = T; };
		template <typename T> struct remove_cv<T const> { using type = T; };
		template <typename T> struct remove_cv<T volatile> { using type = T; };
		template <typename T> using remove_cv_t = typename remove_cv<T>::type;
		template <typename T> using remove_cvref_t = remove_cv_t<remove_reference_t<T>>;
	}

	template <auto& cpo>
	using tag_t = tag_invoke_detail::remove_cvref_t<decltype(cpo)>;
}

#endif
