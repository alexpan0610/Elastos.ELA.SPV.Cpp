// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __ELASTOS_SDK_ACCOUNT_H__
#define __ELASTOS_SDK_ACCOUNT_H__

#include <SDK/Common/Mstream.h>
#include <SDK/SpvService/LocalStore.h>
#include <SDK/WalletCore/BIPs/Address.h>
#include <SDK/WalletCore/BIPs/Mnemonic.h>

#include <nlohmann/json.hpp>

namespace Elastos {
	namespace ElaWallet {

#define MAX_MULTISIGN_COSIGNERS 6

		class Account {
		public:
			enum SignType {
				Standard,
				MultiSign,
			};

		public:
			Account() {}

			// for test
			Account(const LocalStorePtr &store);

			// init from localstore
			Account(const std::string &path);

			// multi-sign readonly
			Account(const std::string &path, const std::vector<PublicKeyRing> &cosigners, int m, bool singleAddress,
					bool compatible);

			// multi-sign xprv
			Account(const std::string &path, const std::string &xprv, const std::string &payPasswd,
					const std::vector<PublicKeyRing> &cosigners, int m, bool singleAddress, bool compatible);

			// multi-sign mnemonic + passphrase
			Account(const std::string &path, const std::string &mnemonic, const std::string &passphrase,
					const std::string &payPasswd, const std::vector<PublicKeyRing> &cosigners, int m,
					bool singleAddress, bool compatible);

			// HD standard with mnemonic + passphrase
			Account(const std::string &path, const std::string &mnemonic, const std::string &passphrase,
					const std::string &payPasswd, bool singleAddress);

			// Read-Only wallet JSON
			Account(const std::string &path, const nlohmann::json &walletJSON);

			// keystore
			Account(const std::string &path, const KeyStore &ks, const std::string &payPasswd);

			virtual bytes_t RequestPubKey() const;

			virtual Key RequestPrivKey(const std::string &payPassword) const;

			virtual HDKeychainPtr RootKey(const std::string &payPassword) const;

			virtual HDKeychainPtr MasterPubKey() const;

			virtual std::string GetxPrvKeyString(const std::string &payPasswd) const;

			virtual const std::string &MasterPubKeyString() const;

			virtual const std::string &MasterPubKeyHDPMString() const;

			virtual const std::vector<PublicKeyRing> &MasterPubKeyRing() const;

			virtual bytes_t OwnerPubKey() const;

			virtual void ChangePassword(const std::string &oldPassword, const std::string &newPassword);

			virtual nlohmann::json GetBasicInfo() const;

			virtual SignType GetSignType() const;

			virtual bool Readonly() const;

			virtual bool SingleAddress() const;

			virtual bool Equal(const Account &account) const;

			virtual int GetM() const;

			virtual int GetN() const;

			virtual const std::string &DerivationStrategy() const;

			virtual nlohmann::json GetPubKeyInfo() const;

			virtual HDKeychainPtr MultiSignSigner() const;

			virtual HDKeychainArray MultiSignCosigner() const;

			virtual int CosignerIndex() const;

			virtual const std::vector<CoinInfoPtr> &SubWalletInfoList() const;

			virtual void AddSubWalletInfoList(const CoinInfoPtr &info);

			virtual void SetSubWalletInfoList(const std::vector<CoinInfoPtr> &info);

			virtual void RemoveSubWalletInfo(const CoinInfoPtr &info);

			virtual KeyStore ExportKeyStore(const std::string &payPasswd);

			virtual nlohmann::json ExportReadonlyWallet() const;

			virtual bool ImportReadonlyWallet(const nlohmann::json &walletJSON);

			virtual std::string GetDecryptedMnemonic(const std::string &payPasswd) const;

			virtual bool VerifyPrivateKey(const std::string &mnemonic, const std::string &passphrase) const;

			virtual bool VerifyPassPhrase(const std::string &passphrase, const std::string &payPasswd) const;

			virtual bool VerifyPayPassword(const std::string &payPasswd) const;

			virtual void Save();

			virtual void Remove();

			virtual const std::string &GetDataPath() const;

			virtual void RegenerateKey(const std::string &payPasswd) const;

		private:
			void Init() const;

		private:
			LocalStorePtr _localstore;
			mutable HDKeychainPtr _xpub;
			mutable int _cosignerIndex;
			mutable HDKeychainPtr _curMultiSigner; // multi sign current wallet signer
			mutable HDKeychainArray _allMultiSigners; // including _multiSigner and sorted
			mutable bytes_t _ownerPubKey, _requestPubKey;
		};

		typedef boost::shared_ptr<Account> AccountPtr;

	} // namespace ElaWallet
} // namespace Elastos

#endif //__ELASTOS_SDK_ACCOUNT_H__
