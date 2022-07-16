#pragma once

#include <iostream>

#include "atlas.hpp"

using namespace atlas;

/* Clase de ayuda para la base de datos*/
/* mover a otro proyecto */


/* no funcionando por que la base dap no esta guardada */
std::vector<DAP> buildDapPortfolio() {

	std::string sql = "Select  to_char(fecha_apertura,'DDMMYYYY'), plazo, tasa, saldo_inicial from basedap limit 100";
	pqxx::stream_from stream = dbManager.executeQuery(sql);


	std::vector<DAP> daps;
	std::tuple<std::string, int, double, double> row;

	while (stream >> row)
	{
		Date startDate = fastDateParser(std::get<0>(row));
		Date endDate = startDate + std::get<1>(row);
		DAP dap(startDate, endDate, std::get<3>(row), std::get<2>(row) * 12 / 100);
		daps.push_back(dap);
	}
	stream.complete();
	return daps;
}

/* genera las curvas guardadas de costo de fondo */
template<class Interp>
ZeroRateCurve<Interp> queryCurve(const std::string & curveName, const std::string& date) {
	pqxx::result results = dbManager.executePrepared(curveName, date);
	std::vector<double> times, rates;

	for (auto const& r: results)
	{
		times.push_back(r[0].as<double>());
		rates.push_back(r[1].as<double>()/100);
	}
	if (times.size() == 0 || rates.size() == 0)
		throw std::runtime_error("Curva " + date + "no encontrada en la base de datos");

	/* agrega primer vertice (hoy) */
	times.insert(times.begin(), 0);
	rates.insert(rates.begin(), rates.front());

	Date refDate = fastDateParser(date);
	
	return ZeroRateCurve<Interp>(refDate, times, rates, Act360());
}

template<class Interp>
ZeroRateCurve<Interp> buildFlatCurve(const Date& refDate, double rate) {
	std::vector<Date> dates{
		refDate,
		refDate+Period(50, TimeUnit::Years)
	};
	std::vector<double> rates(2, rate);
	return ZeroRateCurve<Interp>(dates, rates);
}
