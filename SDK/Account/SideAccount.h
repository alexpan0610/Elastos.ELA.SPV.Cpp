// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <SDK/Plugin/Transaction/Transaction.h>
#include <SDK/WalletCore/BIPs/Address.h>
#include <SDK/WalletCore/BIPs/HDKeychain.h>
#include <SDK/WalletCore/BIPs/Key.h>
#include <SDK/WalletCore/KeyStore/BitcoreWalletClientJson.h>
#include <SDK/WalletCore/KeyStore/CoinInfo.h>
#include <SDK/WalletCore/KeyStore/KeyStore.h>

#include "IAccount.h"
#include "ISubAccount.h"

#ifndef __ELASTOS_SDK_SIDEACCOUNT_H__
#define __ELASTOS_SDK_SIDEACCOUNT_H__

namespace Elastos {
	namespace ElaWallet {
		class FakeParent : public IAccount {
		public:
			bytes_t RequestPubKey() const { return bytes_t(0); }

			HDKeychainPtr RootKey(const std::string &) const { return nullptr; }

			Key RequestPrivKey(const std::string &) const { return Key(); }

			HDKeychainPtr MasterPubKey() const { return nullptr; }

			std::string GetxPrvKeyString(const std::string &) const { return ""; }

			std::string MasterPubKeyString() const { return ""; }

			std::string MasterPubKeyHDPMString() const { return ""; }

			std::vector<PublicKeyRing> MasterPubKeyRing() const { return _empty_pub_keys; }

			bytes_t OwnerPubKey() const { return bytes_t(); }

			void ChangePassword(const std::string &, const std::string &) {}

			nlohmann::json GetBasicInfo() const {
				nlohmann::json j;
				j["Type"] = "MultiSign";
				j["Readonly"] = true;
				j["SingleAddress"] = true;
				j["M"] = 0;
				j["N"] = 0;
				j["HasPassPhrase"] = false;
				return j;
			}

			SignType GetSignType() const { return MultiSign; }

			bool Readonly() const { return true; }

			bool SingleAddress() const { return true; }

			bool Equal(const boost::shared_ptr<IAccount> &account) const { return false; }

			int GetM() const { return 0; }

			int GetN() const { return 0; }

			std::string DerivationStrategy() const { return ""; }

			nlohmann::json GetPubKeyInfo() const { return nlohmann::json(); }

			HDKeychainPtr MultiSignSigner() const { return nullptr; }

			HDKeychainArray MultiSignCosigner() const { return HDKeychainArray(); }

			int CosignerIndex() const { return 0; }

			std::vector<CoinInfoPtr> SubWalletInfoList() const { return _empty_coin_info; }

			void AddSubWalletInfoList(const CoinInfoPtr &) {}

			void SetSubWalletInfoList(const std::vector<CoinInfoPtr> &) {}

			void RemoveSubWalletInfo(const CoinInfoPtr &) {}

			KeyStore ExportKeyStore(const std::string &) { return KeyStore(); }

			nlohmann::json ExportReadonlyWallet() const { return nlohmann::json(); }

			bool ImportReadonlyWallet(const nlohmann::json &walletJSON) { return true; }

			std::string GetDecryptedMnemonic(const std::string &) const { return ""; }

			void RegenerateKey(const std::string &) const {}

			bool VerifyPrivateKey(const std::string &, const std::string &) const { return false; }

			bool VerifyPassPhrase(const std::string &, const std::string &) const { return false; }

			bool VerifyPayPassword(const std::string &) const { return false; }

			void Save() {}

			void Remove() {}

			std::string GetDataPath() const { return ""; }

		private:
			std::vector<PublicKeyRing> _empty_pub_keys;
			std::vector<CoinInfoPtr> _empty_coin_info;
		};

		class SideAccount : public ISubAccount {
		public:
			SideAccount(const uint256 &genesis_hash);

			nlohmann::json GetBasicInfo() const;

			void Init(const std::vector<TransactionPtr> &tx, Lockable *lock);

			void InitDID();

			bool IsSingleAddress() const;

			bool IsProducerDepositAddress(const Address &address) const;

			bool IsOwnerAddress(const Address &address) const;

			bool IsCRDepositAddress(const Address &address) const;

			void AddUsedAddrs(const Address &address);

			size_t GetAllAddresses(std::vector<Address> &addr, uint32_t start, size_t count, bool internal) const;

			size_t GetAllDID(std::vector<Address> &did, uint32_t start, size_t count) const;

			size_t GetAllPublickeys(std::vector<std::string> &pubkeys, uint32_t start, size_t count,
			                        bool containInternal) const;

			std::vector<Address> UnusedAddresses(uint32_t gapLimit, bool internal);

			bool ContainsAddress(const Address &address) const;

			void ClearUsedAddresses();

			bytes_t OwnerPubKey() const;

			bytes_t DIDPubKey() const;

			void SignTransaction(const TransactionPtr &tx, const std::string &payPasswd);

			std::string SignWithDID(const Address &did, const std::string &msg, const std::string &payPasswd);

			Key DeriveOwnerKey(const std::string &payPasswd);

			Key DeriveDIDKey(const std::string &payPasswd);

			bool FindKey(Key &key, const bytes_t &pubKey, const std::string &payPasswd);

			bool GetCodeAndPath(const Address &addr, bytes_t &code, std::string &path) const;

			size_t InternalChainIndex(const TransactionPtr &tx) const;

			size_t ExternalChainIndex(const TransactionPtr &tx) const;

			AccountPtr Parent() const;

		private:
			// The 'X' address generated by the side chain genesis hash.
			Address side_address;
			AccountPtr parent;
		};

	} // namespace ElaWallet
} // namespace Elastos
#endif // __ELASTOS_SDK_SIDEACCOUNT_H__
