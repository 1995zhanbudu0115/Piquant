# Piquant
Piquant-Quantitative Toolbox(CTP)
分层架构，基于多线程间异步通讯建立事件驱动的本地消息机制  
实现时间轮询、被动连询、主动查询的任务优先级调度  
基于c++11线程安全队列建立数据中心，并通过线程监听及MFC的Doc-View结构分发行情数据  
优化自动重连机制　
国内5大交易所行情回报展示及合约基础信息展示  
账户盈亏回报(动态权益/可用资金/占用保证金/手续费等)  
基于libcurl爬取东方财富与新浪财经API的数据，绘制分时图及tick切片展示  
提供基于新浪财经API获取历史数据的接口  
提供限市价委托、预埋单(预埋撤单)、预埋撤单(预埋撤单撤单)、条件止盈损单  
实现持仓/持仓明细/委托/未成委托/成交/平仓/条件单/预埋单/预埋撤单列表展示  
提供主动查询手续费、保证金率、汇率、报撤单手续费、主力合约等功能  
实现超链接  
基于OTL库->ODBC数据源操作Sql Server数据库  
提供ini读写、log读写、json读写、xml读写接口  
对策略dll、日志txt、脚本py、可执行程序exe的功能整合  
开放策略接口(dll)并动态展示运行状态及消息  

