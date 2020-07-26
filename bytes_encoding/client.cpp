// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

// A client sending requests to server every 1 second.

#include <gflags/gflags.h>
#include <butil/logging.h>
#include <butil/time.h>
#include <brpc/channel.h>
#include "echo.pb.h"

#include <string>
#include <sstream>
#include "google/protobuf/text_format.h"
using namespace google::protobuf;

DEFINE_string(attachment, "", "Carry this along with requests");
DEFINE_string(protocol, "baidu_std", "Protocol type. Defined in src/brpc/options.proto");
DEFINE_string(connection_type, "", "Connection type. Available values: single, pooled, short");
DEFINE_string(server, "0.0.0.0:10005", "IP Address of server");
DEFINE_string(load_balancer, "", "The algorithm for load balancing");
DEFINE_int32(timeout_ms, 100, "RPC timeout in milliseconds");
DEFINE_int32(max_retry, 3, "Max retries(not including the first RPC)"); 
DEFINE_int32(interval_ms, 1000, "Milliseconds between consecutive requests");

void FillInstance(example::RequestInstanceByte* instance) {
  std::string h = "ad feature";
  *instance->mutable_header() = h;
  // instance->set_header(h);
  std::string extend_info = "jingdong.com";
  instance->set_ad_extend_info(extend_info);
  
  example::ValueMap value_map;
  example::Value v1;
  std::string *name = v1.add_value();
  *name = "Bob Dylan";
  name = v1.add_value();
  *name = "Pink Floyd";
  name = v1.add_value();
  *name = "Rust Lee";

  example::Value v2;
  std::string *company = v2.add_value();
  *company = "Jing dong";
  company = v2.add_value();
  *company = "Baidu";
  company = v2.add_value();
  *company = "Google";

  value_map.mutable_value_map()->insert({"person", v1});
  value_map.mutable_value_map()->insert({"company", v2});

  // std::string bytes1 = value_map.SerializeAsString();

  // size_t size = value_map->ByteSizeLong();
  // void* buffer = malloc(size);
  // value_map->SerializeToArray(buffer, size);

  // std::ostringstream stream;
  // value_map->SerializeToOstream(&stream);
  // std::string text = stream.str();

  std::string bytes2;
  value_map.SerializeToString(&bytes2);

  std::cout << "Print value map " << std::endl;
  std::cout << bytes2 << std::endl;
  std::cout << "End pritn of value map \n" << std::endl;

  example::ValueMap vm2;
  vm2.ParseFromString(bytes2);

  std::cout << "Print exact map" << std::endl;
  for (auto it = vm2.value_map().cbegin(); 
        it != vm2.value_map().cend();
        ++it) {
    std::string key = it->first;
    int size = it->second.value_size();
    for (int i = 0; i < size; i++) {
      std::cout << key << " " << it->second.value(i) << std::endl;
    }
  }
  std::cout << "End of pritn exact map\n" << std::endl;

  instance->set_common_features(bytes2);
  std::string data;
  instance->SerializeToString(&data);

  example::RequestInstanceByte inst1;
  inst1.ParseFromString(data);
  std::string data1;
  inst1.SerializeToString(&data1);

  example::RequestInstanceValue inst2;
  inst2.ParseFromString(data1);


  std::string strTest;
  TextFormat::PrintToString(inst2, &strTest);
  std::cout << "Print instance " << std::endl;
  std::cout << strTest << std::endl;
  std::cout << "End print instance \n" << std::endl;
}


int main(int argc, char* argv[]) {
    // Parse gflags. We recommend you to use gflags as well.
    GFLAGS_NS::ParseCommandLineFlags(&argc, &argv, true);
    
    // A Channel represents a communication line to a Server. Notice that 
    // Channel is thread-safe and can be shared by all threads in your program.
    brpc::Channel channel;
    
    // Initialize the channel, NULL means using default options.
    brpc::ChannelOptions options;
    options.protocol = FLAGS_protocol;
    options.connection_type = FLAGS_connection_type;
    options.timeout_ms = FLAGS_timeout_ms/*milliseconds*/;
    options.max_retry = FLAGS_max_retry;
    if (channel.Init(FLAGS_server.c_str(), FLAGS_load_balancer.c_str(), &options) != 0) {
        LOG(ERROR) << "Fail to initialize channel";
        return -1;
    }

    // Normally, you should not call a Channel directly, but instead construct
    // a stub Service wrapping it. stub can be shared by all threads as well.
    example::EchoService_Stub stub(&channel);

    // Send a request and wait for the response every 1 second.
    int log_id = 0;
    while (!brpc::IsAskedToQuit()) {
        // We will receive response synchronously, safe to put variables
        // on stack.
        example::EchoRequest request;
        example::EchoResponse response;
        brpc::Controller cntl;

        // request.set_message("hello world");
        FillInstance(request.mutable_instance());

        cntl.set_log_id(log_id ++);  // set by user
        // Set attachment which is wired to network directly instead of 
        // being serialized into protobuf messages.
        cntl.request_attachment().append(FLAGS_attachment);

        // Because `done'(last parameter) is NULL, this function waits until
        // the response comes back or error occurs(including timedout).
        stub.Echo(&cntl, &request, &response, NULL);
        if (!cntl.Failed()) {
            LOG(INFO) << "Received response from " << cntl.remote_side()
                << " to " << cntl.local_side()
                << ": " << response.instance().header() << " (attached="
                << cntl.response_attachment() << ")"
                << " latency=" << cntl.latency_us() << "us";
        } else {
            LOG(WARNING) << cntl.ErrorText();
        }
        usleep(FLAGS_interval_ms * 1000L);

        break;
    }

    LOG(INFO) << "EchoClient is going to quit";
    return 0;
}