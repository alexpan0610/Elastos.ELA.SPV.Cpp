// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "FilterLoadMessage.h"

#include <SDK/P2P/Peer.h>
#include <SDK/P2P/PeerManager.h>
#include <SDK/WalletCore/BIPs/BloomFilter.h>
#include <SDK/Common/ByteStream.h>

namespace Elastos {
	namespace ElaWallet {

		FilterLoadMessage::FilterLoadMessage(const MessagePeerPtr &peer) :
			Message(peer) {

		}

		bool FilterLoadMessage::Accept(const bytes_t &msg) {
			_peer->error("dropping {} message", Type());
			return false;
		}

		void FilterLoadMessage::Send(const SendMessageParameter &param) {
			const FilterLoadParameter &filterLoadParameter = static_cast<const FilterLoadParameter &>(param);
			ByteStream stream;
			filterLoadParameter.Filter->Serialize(stream);
			PEER_DEBUG(_peer, "filter len = {}", stream.size());
			_peer->SetSentFilter(true);
			_peer->SetSentMempool(false);
			SendMessage(stream.GetBytes(), Type());
		}

		std::string FilterLoadMessage::Type() const {
			return MSG_FILTERLOAD;
		}
	}
}