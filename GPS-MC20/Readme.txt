//MC20是一块集成了GPRS功能的GPS模块（同时支持北斗），对它的移植有两方面：
1、GPRS通信功能，主要移植libreference.so文件，以及修改init.rc文件中的RILD服务；
2、GPS功能，主要移植的是gps.default.so文件。
具体的移植文档和说明请查看印象笔记和本地文档。