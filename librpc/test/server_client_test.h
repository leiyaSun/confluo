#ifndef TEST_SERVER_CLIENT_TEST_H_
#define TEST_SERVER_CLIENT_TEST_H_

#include "dialog_server.h"
#include "gtest/gtest.h"

#include "../rpc/rpc_dialog_reader.h"

using namespace ::dialog::rpc;
using namespace ::dialog;

class ServerClientTest : public testing::Test {
};

TEST_F(ServerClientTest, ConcurrentConnectionsTest) {
  auto store = new dialog_store("/tmp");
  auto server = dialog_server::create(store, "127.0.0.1", 9090);
  std::thread serve_thread([&server] {
    server->serve();
  });

  std::vector<rpc_dialog_reader> clients(server->getConcurrentClientLimit());
  for (auto& client : clients) {
    client.connect("127.0.0.1", 9090);
  }

  sleep(1);

  for (auto& client : clients) {
    client.disconnect();
  }

  server->stop();

  if (serve_thread.joinable()) {
    serve_thread.join();
  }
}

#endif /* TEST_SERVER_CLIENT_TEST_H_ */