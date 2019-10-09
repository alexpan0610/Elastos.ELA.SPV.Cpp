// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "Account.h"
#include "SubAccount.h"

#ifndef __ELASTOS_SDK_SIDEACCOUNT_H__
#define __ELASTOS_SDK_SIDEACCOUNT_H__

namespace Elastos {
	namespace ElaWallet {
		class FakeParent : public Account {
		public:
			bytes_t RequestPubKey() const { return bytes_t(0); }

			HDKeychainPtr RootKey(const std::string &) const { return nullptr; }

			Key RequestPrivKey(const std::string &) const { return Key(); }

			HDKeychainPtr MasterPubKey() const { return nullptr; }

			std::string GetxPrvKeyString(const std::string &) const { return ""; }

			const std::string &MasterPubKeyString() const { return _empty_str; }

			const std::string &MasterPubKeyHDPMString() const { return _empty_str; }

			const std::vector<PublicKeyRing> &MasterPubKeyRing() const {
				return _empty_pub_keys;
			}

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

			int GetM() const { return 0; }

			int GetN() const { return 0; }

			const std::string &DerivationStrategy() const { return _empty_str; }

			nlohmann::json GetPubKeyInfo() const { return nlohmann::json(); }

			HDKeychainPtr MultiSignSigner() const { return nullptr; }

			HDKeychainArray MultiSignCosigner() const { return HDKeychainArray(); }

			int CosignerIndex() const { return 0; }

			const std::vector<CoinInfoPtr> &SubWalletInfoList() const {
				return _empty_coin_info;
			}

			void AddSubWalletInfoList(const CoinInfoPtr &) {}

			void SetSubWalletInfoList(const std::vector<CoinInfoPtr> &) {}

			void RemoveSubWalletInfo(const CoinInfoPtr &) {}

			KeyStore ExportKeyStore(const std::string &) { return KeyStore(); }

			nlohmann::json ExportReadonlyWallet() const { return nlohmann::json(); }

			bool ImportReadonlyWallet(const nlohmann::json &walletJSON) { return true; }

			std::string GetDecryptedMnemonic(const std::string &) const { return ""; }

			void RegenerateKey(const std::string &) const {}

			bool VerifyPrivateKey(const std::string &, const std::string &) const {
				return false;
			}

			bool VerifyPassPhrase(const std::string &, const std::string &) const {
				return false;
			}

			bool VerifyPayPassword(const std::string &) const { return false; }

			void Save() {}

			void Remove() {}

			const std::string &GetDataPath() const { return _empty_str; }

		private:
			std::string _empty_str;
			std::vector<PublicKeyRing> _empty_pub_keys;
			std::vector<CoinInfoPtr> _empty_coin_info;

		};

		class SideAccount : public SubAccount {
		public:
			SideAccount(const uint256 &genesis_hash);

			nlohmann::json GetBasicInfo() const;

			void Init(const std::vector<TransactionPtr> &tx, Lockable *lock);

			bool IsSingleAddress() const;

			bool IsProducerDepositAddress(const Address &address) const;

			bool IsOwnerAddress(const Address &address) const;

			bool IsCRDepositAddress(const Address &address) const;

			void AddUsedAddrs(const Address &address);

			size_t GetAllAddresses(std::vector<Address> &addr, uint32_t start, size_t count, bool internal) const;

			std::vector<Address> UnusedAddresses(uint32_t gapLimit, bool internal);

			bool ContainsAddress(const Address &address) const;

			void ClearUsedAddresses();

			bytes_t OwnerPubKey() const;

			bytes_t DIDPubKey() const;

			void SignTransaction(const TransactionPtr &tx, const std::string &payPasswd);

			Key DeriveOwnerKey(const std::string &payPasswd);

			Key DeriveDIDKey(const std::string &payPasswd);

			bool FindKey(Key &key, const bytes_t &pubKey, const std::string &payPasswd);

			bool GetCodeAndPath(const Address &addr, bytes_t &code, std::string &path) const;

			size_t InternalChainIndex(const TransactionPtr &tx) const;

			size_t ExternalChainIndex(const TransactionPtr &tx) const;

		private:
			// The 'X' address generated by the side chain genesis hash.
			Address side_address;
		};

	} // namespace ElaWallet
} // namespace Elastos
#endif // __ELASTOS_SDK_SIDEACCOUNT_H__
