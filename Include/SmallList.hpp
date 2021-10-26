#pragma once

namespace hh
{
	template<typename SmallList, bool CONST> class SmallListIterator
	{
		friend SmallList;
		template<typename, bool> friend class SmallListIterator;
		template<typename> friend struct std::pointer_traits;

	public:
		using iterator_concept = std::contiguous_iterator_tag;
		using pointer		   = std::conditional_t<CONST, typename SmallList::const_pointer, typename SmallList::pointer>;
		using reference		   = std::conditional_t<CONST, typename SmallList::const_reference, typename SmallList::reference>;
		using value_type	   = typename SmallList::value_type;
		using difference_type  = typename SmallList::difference_type;

		SmallListIterator() = default;

		operator SmallListIterator<SmallList, true>() const noexcept
		{
#if DEBUG
			return {ptr, list};
#else
			return {ptr};
#endif
		}

		reference operator*() const noexcept
		{
			return *operator->();
		}

		pointer operator->() const noexcept
		{
			HH_ASSERT(list->data() <= ptr && ptr < list->data() + list->size(),
					  "Tried to dereference value-initialized or end iterator.");
			return ptr;
		}

		template<bool CONST2> bool operator==(SmallListIterator<SmallList, CONST2> that) const noexcept
		{
			return ptr == that.ptr;
		}

		template<bool CONST2> bool operator!=(SmallListIterator<SmallList, CONST2> that) const noexcept
		{
			return ptr != that.ptr;
		}

		template<bool CONST2> bool operator<(SmallListIterator<SmallList, CONST2> that) const noexcept
		{
			return ptr < that.ptr;
		}

		template<bool CONST2> bool operator<=(SmallListIterator<SmallList, CONST2> that) const noexcept
		{
			return ptr <= that.ptr;
		}

		template<bool CONST2> bool operator>(SmallListIterator<SmallList, CONST2> that) const noexcept
		{
			return ptr > that.ptr;
		}

		template<bool CONST2> bool operator>=(SmallListIterator<SmallList, CONST2> that) const noexcept
		{
			return ptr >= that.ptr;
		}

		template<bool CONST2> std::strong_ordering operator<=>(SmallListIterator<SmallList, CONST2> that) const noexcept
		{
			return ptr <=> that.ptr;
		}

		SmallListIterator& operator++() noexcept
		{
			return *this += 1;
		}

		SmallListIterator operator++(int) noexcept
		{
			auto old = *this;
			*this += 1;
			return old;
		}

		SmallListIterator& operator--() noexcept
		{
			return *this -= 1;
		}

		SmallListIterator operator--(int) noexcept
		{
			auto old = *this;
			*this -= 1;
			return old;
		}

		SmallListIterator& operator+=(difference_type offset) noexcept
		{
			HH_ASSERT(offset == 0 || ptr, "Cannot offset value-initialized iterator.");
			HH_ASSERT(offset >= 0 || offset >= list->data() - ptr, "Cannot offset list iterator before begin.");
			HH_ASSERT(offset <= 0 || offset <= list->data() + list->size() - ptr, "Cannot offset list iterator past end.");

			ptr += offset;
			return *this;
		}

		SmallListIterator operator+(difference_type offset) const noexcept
		{
			auto old = *this;
			return old += offset;
		}

		friend SmallListIterator operator+(difference_type offset, SmallListIterator iterator) noexcept
		{
			return iterator + offset;
		}

		SmallListIterator& operator-=(difference_type offset) noexcept
		{
			return *this += -offset;
		}

		SmallListIterator operator-(difference_type offset) const noexcept
		{
			auto old = *this;
			return old -= offset;
		}

		template<bool CONST2> difference_type operator-(SmallListIterator<SmallList, CONST2> that) const noexcept
		{
			return ptr - that.ptr;
		}

		reference operator[](difference_type offset) const noexcept
		{
			return *(*this + offset);
		}

	private:
		pointer ptr = nullptr;
#if DEBUG
		SmallList const* list = nullptr;

		SmallListIterator(pointer ptr, SmallList const* list) noexcept : ptr(ptr), list(list)
		{}
#else
		SmallListIterator(pointer ptr) noexcept : ptr(ptr)
		{}
#endif
	};

	template<typename T, size_t SIZE = 64, typename Allocator = std::allocator<T>> class SmallList
	{
		using AllocTraits = std::allocator_traits<Allocator>;

	public:
		using value_type			 = typename AllocTraits::value_type;
		using reference				 = value_type&;
		using const_reference		 = value_type const&;
		using pointer				 = typename AllocTraits::pointer;
		using const_pointer			 = typename AllocTraits::const_pointer;
		using size_type				 = typename AllocTraits::size_type;
		using difference_type		 = typename AllocTraits::difference_type;
		using iterator				 = SmallListIterator<SmallList, false>;
		using const_iterator		 = SmallListIterator<SmallList, true>;
		using reverse_iterator		 = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	private:
	};
}

template<typename SmallList> struct std::pointer_traits<hh::SmallListIterator<SmallList, true>>
{
	using pointer		  = hh::SmallListIterator<SmallList, true>;
	using element_type	  = typename pointer::value_type const;
	using difference_type = typename pointer::difference_type;

	[[nodiscard]] static constexpr element_type* to_address(pointer it) noexcept
	{
		HH_ASSERT(it.list->data() <= it.ptr && it.ptr <= it.list->data() + it.list->size(),
				  "Iterator is not within a validly addressable range.");
		return it.ptr;
	}
};

template<typename SmallList> struct std::pointer_traits<hh::SmallListIterator<SmallList, false>>
{
	using pointer		  = hh::SmallListIterator<SmallList, false>;
	using element_type	  = typename pointer::value_type;
	using difference_type = typename pointer::difference_type;

	[[nodiscard]] static constexpr element_type* to_address(pointer it) noexcept
	{
		HH_ASSERT(it.list->data() <= it.ptr && it.ptr <= it.list->data() + it.list->size(),
				  "Iterator is not within a validly addressable range.");
		return it.ptr;
	}
};
