# Log dump

Inference -> feature log

Predictor -> log -> flume -> Kafka -> offline read feature ->training platform

Exposure Log -> training platform

CTR Log -> training platform

Ad server -> sort -> 1000 CTR -> get top 10 back

session_id, sku_id



best only get top 10 back to Ad server

进1000个，回10个

Predictor wait log 200ms 后

请求 jimdb = redis，与本地 join

将 <seddion_id, 落的 sku-id>写入jimdb

session_id 可能会重复，两个如果重复的话，

不用 key，用 model_request_id，

用 model_request_id, 注册 reg_sid_rand

session_id = session_id + reg_sid_rand

sku_id = sku_id + log_unique_id 后者批量递增，每次请求唯一



Ad server ---> proxy 业务协议A ---> feature_server ---> feature 协议B ---> predictor





特征与请求无关，

用batch，集中 mget 访问，200 条聚一次

使用 brpc 的 [Execution Queue](https://github.com/apache/incubator-brpc/blob/master/docs/cn/execution_queue.md) ，多线程放任务，多个生产者，出口只有一个消费者

最小粒度 50

最多等 400ms，直接处理

最大粒度 400，处理一次



多个请求，在一个EQ中聚合，然后分发给多个消费EQ，轮询 消费后，join，访问jimdb，再使用一个 EQ 批量写入 jimdb 队列，并落盘

Log Handler 处理第一层log



Log 

V2	Ad Exte	Model queen	

V1	Log ---> 异类，等一层



特征 log，可以开关，在高峰时关了，cpu使用率从 60% 降低到 40% 利用率



特征日志落盘 dump

算完日志后返回

req ---> resp 这里收到返回信息后，返回resp

没执行完的部分 ---> then pb-body 继续交由后台执行



RPC service 接收 Model Message

Model Message 里，主要是 二进制的 pb_body 实际使用





注意 Com + P 搜索并进入文件

inference_server.cc

inference->ProcessFeatureLog  入口

ExecutionQueue  执行队列

LogManager::LogHandler  第一层执行队列

LogHandler::TaskHandler  第二层执行队列

LogProcessorPbMap  V2 版本

对照着下面这个文档看

https://cf.jd.com/pages/viewpage.action?pageId=75294616

ArchiveLogInfo.proto



**assert() 用法**

**1)在函数开始处检验传入参数的合法性**

**2)每个assert只检验一个条件,因为同时检验多个条件时,如果断言失败,无法直观的判断是哪个条件失败**

**3)不能使用改变环境的语句,因为assert只在DEBUG个生效,如果这么做,会使用程序在真正运行时遇到问题**

**4)assert和后面的语句应空一行,以形成逻辑和视觉上的一致感**

**ASSERT 只有在 Debug 版本中才有效，如果编译为 Release 版本则被忽略**



### 1. Bytes == ValueMap

finished

view in : 

Brpc example echo-c++



### 2. 开发V1 V2的特征日志流程

predictor_interface/app/log/log_manger.h

predictor_lib_rpc/app/log/log_manager.h

#### V1:

change the RequestInstance in predictor_lib_rpc/proto/pino_instance_pb.proto

predictor_lib_rpc/app/log/log_process.h

ArchiveLogInfo.proto，注意此处修改 QualityModelFeature quality_model_msg

predictor_interface/app/log/log_processer.h











#### V2:

问题：list-wise 模型 pb 数据结构支持：

线上模型pb是以单个sku为样本，repeated类型对标batch结构。

list-wise模型上线每个样本是一个sku的list，会按照batch请求结果。

例如，一个list是长度为L，共N个序列，需要对N*L形式的样本请求模型结果N*k。

同一个list的sku的基本信息，比如说cid3，希望能用一个ns表示.

请问，按照目前pb结构如何支持list-wise的请求以及是特征落盘的操作。



change the RequestInstance in predictor_lib_rpc/proto/predictor_request.proto

predictor_lib_rpc/proto/predictor_archive_log.proto

predictor_interface/app/log/log_manger.h

predictor_interface/app/log/predictor_process_pb_map.h



##### V2 AdPredictorLogInfo

RequestInsance request_instance = 1;

// predictor calculation result

repeated pino.Resp instance_outputs = 2;



// predictor append bytes

int32 request_num = 3; // the ads number of a pos_id

string client_sid = 4; // clients ask url parameter for sid, different pos_id has different algorithm, may the same sid or may not, so when client_sid is not empty, only client_sid+pos_id can indicate one ask



message AdMsgInfo{entity.AdPredictInfo}





##### V2 ArchiveLogInfo

AdPredictorLogInfo search_log_info = 2;



##### V2 FeatureLogPbMap

ArchiveLogInfo* log;

string request_pb_bytes;

string response_pb_bytes;



##### V2 ProcessFilterWithJimdb(vector<FeatureLogPbMap*>* log_list) 

vector<string> keys; --> keys.size = log_list->size

for FeatureLogPbMap*log in *log_list:

​	RequestInstance* pb_req = log->log->











