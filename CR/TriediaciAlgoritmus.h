#pragma once
#include <libds/adt/sorts.h>
#include <libds/amt/implicit_sequence.h>
#include <vector>
class TriediaciAlgoritmus
{
public:
	template <typename T>
	ds::amt::ImplicitSequence<T> zorad(std::vector<T> is, const std::function<bool(const T&, const T&)> compare)
	{
		ds::adt::QuickSort<T> qs;
		qs.sort(is, compare);
		return is;
	}
};

