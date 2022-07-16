#pragma once
#include "QL.hpp"
#include <vector>
#include <map>
#include <tuple>

namespace atlas{
	template<class A, class B, class C>
	using DoubleMap = std::unordered_map<A, std::unordered_map<B, C>>;
	/*
	* dado que el indice indica la tasa ya calculada, solo se necesita guardar cada una en el vector
	*/
	struct MarketData {
		Date refDate;
		std::vector<double> dfs;
		std::vector<double> fwds;
	};

	struct HistoricalData {
		DoubleMap<std::string, Date, double> fwds;
	};

	struct MarketRequest {
		/*
		* Fecha de referencia del request
		*/
		Date refDate;
		/*
		* los factores de descuento solo necesitan la curva y la fecha a descontar
		*/
		std::vector<std::tuple<std::string, Date>> dfs;

		/*
		* las tasas forward requieren fecha de inicio y termino
		*/
		std::vector<std::tuple<std::string, Date, Date>> fwds;
	};
}