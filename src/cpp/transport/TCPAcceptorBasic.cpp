// Copyright 2018 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <fastrtps/transport/TCPAcceptorBasic.h>
#include <fastrtps/transport/TCPTransportInterface.h>
#include <fastrtps/utils/IPLocator.h>

namespace eprosima{
namespace fastrtps{
namespace rtps{

TCPAcceptorBasic::TCPAcceptorBasic(
        asio::io_service& io_service,
        TCPTransportInterface* parent,
        const Locator_t& locator)
    : TCPAcceptor(io_service, parent, locator)
{
    endpoint_ = asio::ip::tcp::endpoint(parent->generate_protocol(), IPLocator::getPhysicalPort(locator_));
}

TCPAcceptorBasic::TCPAcceptorBasic(
        asio::io_service& io_service,
        const std::string& interface,
        const Locator_t& locator)
    : TCPAcceptor(io_service, interface, locator)
{
    endpoint_ = asio::ip::tcp::endpoint(asio::ip::address_v4::from_string(interface),
        IPLocator::getPhysicalPort(locator_));
}

void TCPAcceptorBasic::accept(
        TCPTransportInterface* parent,
        asio::io_service&)
{
    Locator_t locator = locator_;
    using asio::ip::tcp;
    acceptor_.async_accept(
        [this, locator, parent](const std::error_code& error, tcp::socket socket)
        {
            tcp_basic::eProsimaTCPSocket socket_ptr = tcp_basic::createTCPSocket(std::move(socket));
            parent->SocketAccepted(this, locator, socket_ptr, error);
        });
}

} // namespace rtps
} // namespace fastrtps
} // namespace eprosima