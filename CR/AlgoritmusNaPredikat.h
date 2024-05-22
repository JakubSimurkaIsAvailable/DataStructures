#pragma once
#include <string>
#include <vector>
#include <libds/heap_monitor.h>
#include "UzemnaJednotka.h"
#include <algorithm>
#include <iostream>
#include <functional>
class AlgoritmusNaPredikat
{
public:
	/*
	template<typename T>
	std::vector<T*> najdiAk(std::vector<T*>& povVec, std::string str, const std::function<bool(std::string, std::string)>& funkc)
	{
		std::vector<T*> temp;
		for (auto var : povVec)
		{
			if (funkc(str, var->getNazovUzemnejJednotky())) {
				//std::cout << &var;
				temp.push_back(var);
			}
			//base_++;
		}
		this->vypis(temp);
		return temp;
	}

	*/
	
	template<typename T, typename I>
	std::vector<T*> najdiAk(I base_, I end_, std::string str, const std::function<bool(T*, std::string)> funkc)
	{
		std::vector<T*> temp;
		while (base_ != end_)
		{
			if (funkc(*base_, str)) {
				//std::cout << base_->getNazovUzemnejJednotky() << std::endl;
				temp.push_back(*base_);
			}
			++base_;
		}
		//this->vypis(temp.begin(), temp.end());
		return temp;
	}


	template<typename T>
	void vypis(T base_, T end_)
	{
		while (base_ != end_)
		{
			if((**base_).getNazovUzemnejJednotky() != "")
			{
				std::cout << (**base_).getKodUzemnejJednotky() << " " << (**base_).getNazovUzemnejJednotky() << std::endl;
			}
			
			++base_;
		}
	}
};

