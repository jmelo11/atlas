#pragma once

#include <pqxx/pqxx>
#include <string>

/* basic data manager */
namespace DataBase{
	
	class DataManager {
	
		public:
		
			DataManager(const std::string& ip = "10.191.199.13",
						const std::string& port = "5432",
						const std::string& user = "postgres",
						const std::string& pass = "jp123",
						const std::string& dbName = "Banking") :
				ip_(ip), port_(port), user_(user), pass_(pass), dbName_(dbName){}

			bool connect() {
				if (conn_ != nullptr)				
					if (conn_->is_open())
						conn_->close();
				
				std::string connString = "dbname = " + dbName_ + " user = " + user_ + " password = " + pass_ + " hostaddr = " + ip_ + " port = " + port_;
				conn_ = std::make_shared<pqxx::connection>(connString);
				if (conn_->is_open())
					return true;
				return false;
			}

			void disconnect() {
				if (conn_->is_open())
					conn_->close();
			}

			void setDB(const std::string& dbName) {
				dbName_ = dbName;
			}

			void prepareQuery(const std::string& queryName, const std::string& query) {
				conn_->prepare(queryName, query);
			}

			pqxx::result executePrepared(const std::string& queryName, auto... Args) {
				pqxx::work w{ *conn_ };
				return w.exec_prepared(queryName, Args...);
			}
			pqxx::stream_from executeQuery(const std::string& sql) {
				pqxx::work w{ *conn_ };
				return pqxx::stream_from::query(w, sql);
			}
	
		private:
			std::string ip_, port_, user_, pass_, dbName_;
			std::shared_ptr<pqxx::connection> conn_;
	};

	/* prepared queries */
	static DataManager setupDB() {

		DataManager db;
		bool isConnnected = db.connect();
		if (isConnnected) {
			/* get swap price/rate */
			db.prepareQuery("prices", "select distinct px_last from curves.prices where ticker = $1 and reference_date=to_date($2, 'DDMMYYYY')");

			/* gets curve params*/
			db.prepareQuery("latest_curve_config", "select to_char(config_date, 'DDMMYYYY') from curves.curves where curve_name=$1 order by config_date desc limit 1");
			db.prepareQuery("curves", "select curve_id, day_counter from curves.curves where config_date=to_date($1, 'DDMMYYYY') and curve_name = $2");

			/* gets helpers ids */
			db.prepareQuery("helpers", "select helper_id, type from curves.helpers where curve_id = $1");

			/* gets helper params */
			db.prepareQuery("helper_configs", "select field, value from curves.helper_configs where helper_id = $1");
		}
		else {
			throw std::runtime_error("failed to connect to db");
		}
		return db;
	}
}