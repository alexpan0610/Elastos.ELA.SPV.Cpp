// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <fstream>

#include "MasterWalletStore.h"
#include "ParamChecker.h"
#include "Utils.h"
#include "SDK/Account/StandardAccount.h"

namespace Elastos {
	namespace ElaWallet {

		MasterWalletStore::MasterWalletStore(const std::string &rootPath) {
			_account = AccountPtr(new StandardAccount(rootPath));
		}

		MasterWalletStore::~MasterWalletStore() {

		}

		void MasterWalletStore::Load(const boost::filesystem::path &path) {
			ParamChecker::checkPathExists(path);

			std::ifstream i(path.string());
			nlohmann::json j;
			i >> j;
			j >> *this;
		}

		void MasterWalletStore::Save(const boost::filesystem::path &path) {

			nlohmann::json j;
			j << *this;
			std::ofstream o(path.string());
			j >> o;
		}

		const std::vector<CoinInfo> &MasterWalletStore::GetSubWalletInfoList() const {
			return _subWalletsInfoList;
		}

		void MasterWalletStore::SetSubWalletInfoList(const std::vector<CoinInfo> &infoList) {
			_subWalletsInfoList = infoList;
		}

		const IdAgentInfo &MasterWalletStore::GetIdAgentInfo() const {
			return _idAgentInfo;
		}

		void MasterWalletStore::SetIdAgentInfo(const IdAgentInfo &info) {
			_idAgentInfo = info;
		}

		nlohmann::json &operator<<(nlohmann::json &j, const MasterWalletStore &p) {
			to_json(j, p);

			return j;
		}

		const nlohmann::json &operator>>(const nlohmann::json &j, MasterWalletStore &p) {
			from_json(j, p);

			return j;
		}

		void to_json(nlohmann::json &j, const MasterWalletStore &p) {
			j["Account"] = p.Account()->ToJson();
			j["IdAgent"] = p.GetIdAgentInfo();
			std::vector<nlohmann::json> subWallets;
			for (size_t i = 0; i < p.GetSubWalletInfoList().size(); i++) {
				subWallets.push_back(p.GetSubWalletInfoList()[i]);
			}
			j["SubWallets"] = subWallets;
		}

		void from_json(const nlohmann::json &j, MasterWalletStore &p) {
			p.Account()->FromJson(j["Account"]);
			p.SetIdAgentInfo(j["IdAgent"]);

			std::vector<CoinInfo> coinInfoList;
			std::vector<nlohmann::json> subWallets = j["SubWallets"];
			for (size_t i = 0; i < subWallets.size(); i++) {
				coinInfoList.push_back(subWallets[i]);
			}
			p.SetSubWalletInfoList(coinInfoList);
		}

		IAccount *MasterWalletStore::Account() const {
			return _account.get();
		}

	}
}