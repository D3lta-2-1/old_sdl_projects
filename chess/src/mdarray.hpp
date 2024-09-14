#pragma once
#include <memory>
#include <array>
#include <cassert>
#include <utility>

using dimensionIndex = std::size_t;

template<typename T,std::size_t... dimensionSizes >
class mdarray{
protected:
    const std::array<std::size_t, sizeof...(dimensionSizes)> _dimensionSizes{ dimensionSizes... }; //erreur "std::array requiert une liste d'argument modèle" //size of each dimension
	std::unique_ptr<T[]> _data{ std::make_unique<T[]>(linearSize()) }; //raw data array
public:
	//ctors, dtor
	mdarray() = default;
	template<typename... Args>
	mdarray(Args... args);
	mdarray(mdarray& arg);
	mdarray(mdarray&& arg) noexcept;
	~mdarray() = default;
	//operators =
	void operator=(mdarray& arg) noexcept;
	void operator=(mdarray&& arg) noexcept;

	//getters 
	const std::size_t linearSize();
	template<std::size_t index>
		std::size_t size() const noexcept;
	template<typename ... Pos>
		T& operator()(Pos... pos) noexcept;
	template<typename ... Pos>
		const T& operator()(Pos... pos) const noexcept;

	
};

//impc construcotr
template<typename T, std::size_t ...dimensionSizes>
template<typename ...Args>
inline mdarray<T, dimensionSizes...>::mdarray(Args...args)
{
	for (auto i = 0u; i < linearSize(); i++)
	{
		_data[i] = T(args...);
	}
}

template<typename T, std::size_t ...dimensionSizes>
inline mdarray<T, dimensionSizes...>::mdarray(mdarray& arg)
{
	*this = arg;
}

template<typename T, std::size_t ...dimensionSizes>
inline mdarray<T, dimensionSizes...>::mdarray(mdarray&& arg) noexcept
{
	*this = std::move(arg);
}

//imp opertor= 
template<typename T, std::size_t ...dimensionSizes>
inline void mdarray<T, dimensionSizes...>::operator=(mdarray& arg) noexcept
{
	for (auto i = 0u; i < linearSize(); i++)
	{
		_data[i] = arg._data[i];
	}
}

template<typename T, std::size_t ...dimensionSizes>
inline void mdarray<T, dimensionSizes...>::operator=(mdarray&& arg) noexcept
{
	_data = std::move(arg._data);
}

//getters

template<typename T, std::size_t ...dimensionSizes>
inline const std::size_t mdarray<T, dimensionSizes...>::linearSize()
{
	std::size_t totalSize{ 1 };
	for (auto size : _dimensionSizes)
	{
		totalSize *= size;
	}
	return totalSize;
}

template<typename T, std::size_t ...dimensionSizes>
template<std::size_t index>
inline std::size_t mdarray<T, dimensionSizes...>::size()  const noexcept
{
	assert(index < _dimensionSizes.size() && "you cannot get the size of a dimension which doesn't exist");
	return _dimensionSizes[index];
}

template<typename T, std::size_t ...dimensionSizes>
template<typename ...Pos>
inline T& mdarray<T, dimensionSizes...>::operator()(Pos ...pos) noexcept
{
	std::array<std::size_t, sizeof...(Pos)> arrayOfPos{ static_cast<std::size_t>(pos)... };
	assert(arrayOfPos.size() == _dimensionSizes.size() && "you haven't the good number of dimensensions");
	std::size_t index{0};
	std::size_t dimensionTotalSize{ 1 };
	for (auto i = 0u; i < arrayOfPos.size(); i++)
	{
		assert(arrayOfPos[i] < _dimensionSizes[i] && "out of border");
		index += arrayOfPos[i] * dimensionTotalSize;
		dimensionTotalSize *= _dimensionSizes[i];
	}
	return _data[index];
}

template<typename T, std::size_t ...dimensionSizes>
template<typename ...Pos>
inline const T& mdarray<T, dimensionSizes...>::operator()(Pos ...pos) const noexcept
{
	std::array<std::size_t, sizeof...(Pos)> arrayOfPos{ static_cast<std::size_t>(pos)... };
	assert(arrayOfPos.size() == _dimensionSizes.size() && "you haven't the good number of dimensensions");
	std::size_t index{ 0 };
	std::size_t dimensionTotalSize{ 1 };
	for (auto i = 0u; i < arrayOfPos.size(); i++)
	{
		assert(arrayOfPos[i] < _dimensionSizes[i] && "out of border");
		index += arrayOfPos[i] * dimensionTotalSize;
		dimensionTotalSize *= _dimensionSizes[i];
	}
	return _data[index];
}
