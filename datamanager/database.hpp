#pragma once

#include <pqxx/pqxx>

class DataManager {
	public:
		DataManager(const std::string ip = "10.191.199.13",
			const std::string& port = "5432",
			const std::string& user = "postgres",
			const std::string& pass = "jp123",
			const std::string& dbName = "Bolsa",
			const std::string& schema = "basesbalance") :
			ip_(ip), port_(port), user_(user), pass_(pass), dbName_(dbName), schema_(schema) {}

		void connect() {
			if (conn_ != nullptr)
			{
				if (conn_->is_open())
					conn_->close();
			}
			std::string connString = "dbname = " + dbName_ + " user = " + user_ + " password = " + pass_ + " hostaddr = " + ip_ + " port = " + port_ + " client_encoding = LATIN1;";
			conn_ = std::make_shared<pqxx::connection>(pqxx::connection(connString));
			if (conn_->is_open())
			{
				std::string setSchmea = "SET search_path TO " + schema_ + ";";
				pqxx::work w{ *conn_ };
				w.exec0(setSchmea);
			}
		}

		void disconnect() {
			if (conn_->is_open())
				conn_->close();
		}

		void setDB(const std::string& dbName) {
			dbName_ = dbName;
		}

		void setSchema(const std::string& schema) {
			schema_ = schema;
		}

		void prepareQuery(const std::string& queryName, const std::string& query) {
			conn_->prepare(queryName, query);
		}

		pqxx::result executePrepared(const std::string& queryName, auto... Args) {
			pqxx::work w{ *conn_ };
			return w.exec_prepared(queryName, Args...);
		}
		pqxx::stream_from executeQuery(const std::string& sql) {
			std::string setSchmea = "SET search_path TO " + schema_ + ";";
			pqxx::work w{ *conn_ };
			w.exec0(setSchmea);
			return pqxx::stream_from::query(w, sql);
		}
	
	private:
		std::string ip_, port_, user_, pass_, dbName_, schema_;
		std::shared_ptr<pqxx::connection> conn_;
};

DataManager createManager() {
	DataManager db;
	db.connect();

	/*cf*/
	db.prepareQuery("CF_CLP", "select anos, clp from cf.cf_historico where fecha=to_date($1, 'DDMMYYYY') and clp is not null");
	db.prepareQuery("CF_UF", "select anos, uf from cf.cf_historico where fecha=to_date($1, 'DDMMYYYY') and uf is not null");
	db.prepareQuery("CF_USD", "select anos, usd from cf.cf_historico where fecha=to_date($1, 'DDMMYYYY') and usd is not null");

	/*icp*/
	db.prepareQuery("ICP", "select yf, ICP from basesbalance.market_zeros where date=to_date($1, 'DDMMYYYY')");
	db.prepareQuery("SOFR", "select yf, \"SOFR\" from basesbalance.market_zeros where date = to_date($1, 'DDMMYYYY')");

	return db;
}


/*
void testConn() {
	std::string connString = "dbname = Bolsa user = postgres password = jp123 hostaddr = 10.191.199.13 port = 5432 client_encoding = LATIN1;";
	pqxx::connection conn_(connString);
	if (conn_.is_open())
		std::cout << "Database is open." << "\n";
	std::string setSchmea = "SET search_path TO cf;";
	pqxx::work w{ conn_ };
	w.exec0(setSchmea);
	std::string sql = "Select anos, clp from cf_historico where fecha=\'07-06-2022\' and clp is not null";
	pqxx::stream_from s = pqxx::stream_from::query(w, sql);

	std::tuple<double, double> row;

	while (s >> row)
		std::cout << std::get<0>(row) << "\n";
}

void testConn2() {
	DataManager db;
	db.connect();
	db.prepareQuery("curva_cf_clp", "select anos, clp from cf.cf_historico where fecha=to_date($1, 'DD-MM-YYYY') and clp is not null");
	pqxx::result results = db.executePrepared("curva_cf_clp", "07-06-2022");

	for (auto const& row : results) {
		double t = row[0].as<double>();
		double r = row[1].as<double>();
		std::cout << "t: " << t << ", r: " << r << "\n";
	}

}
*/