// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __ELASTOS_SDK_SUBACCOUNT_H__
#define __ELASTOS_SDK_SUBACCOUNT_H__

#include "Account.h"

#include <SDK/Common/Lockable.h>

#include <set>

namespace Elastos {
	namespace ElaWallet {

		class Transaction;

		typedef boost::shared_ptr<Transaction> TransactionPtr;

		class SubAccount {
		public:
			SubAccount(const AccountPtr &parent, uint32_t coinIndex);

			virtual nlohmann::json GetBasicInfo() const;

			virtual void Init(const std::vector<TransactionPtr> &tx, Lockable *lock);

			virtual bool IsSingleAddress() const;

			virtual bool IsProducerDepositAddress(const Address &address) const;

			virtual bool IsOwnerAddress(const Address &address) const;

			virtual bool IsCRDepositAddress(const Address &address) const;

			virtual void AddUsedAddrs(const Address &address);

			virtual size_t GetAllAddresses(std::vector<Address> &addr,
										   uint32_t start,
										   size_t count,
										   bool internal) const;

			virtual std::vector<Address> UnusedAddresses(uint32_t gapLimit, bool internal);

			virtual bool ContainsAddress(const Address &address) const;

			virtual void ClearUsedAddresses();

			virtual bytes_t OwnerPubKey() const;

			virtual bytes_t DIDPubKey() const;

			virtual void SignTransaction(const TransactionPtr &tx, const std::string &payPasswd);

			virtual Key DeriveOwnerKey(const std::string &payPasswd);

			virtual Key DeriveDIDKey(const std::string &payPasswd);

			virtual bool FindKey(Key &key, const bytes_t &pubKey, const std::string &payPasswd);

			virtual bool GetCodeAndPath(const Address &addr, bytes_t &code, std::string &path) const;

			virtual size_t InternalChainIndex(const TransactionPtr &tx) const;

			virtual size_t ExternalChainIndex(const TransactionPtr &tx) const;

			virtual const AccountPtr &Parent() const { return _parent; }

		private:
			uint32_t _coinIndex;
			std::vector<Address> _internalChain, _externalChain;
			std::set<Address> _usedAddrs, _allAddrs;
			mutable Address _depositAddress, _ownerAddress, _crDepositAddress;

			AccountPtr _parent;
			Lockable *_lock;
		};

		typedef boost::shared_ptr<SubAccount> SubAccountPtr;

	} // namespace ElaWallet
} // namespace Elastos

#endif //__ELASTOS_SDK_SUBACCOUNT_H__
