本程序是采用最短路径算法找出任意两个北京地铁站之间花费时间最少的乘车路线，可用的算法有三个：Dijkstra 算法，Bellman-Ford 算法和 SPFA 算法，用户可通过可选参数 -algo_type 指定所用的算法类型，默认使用 Dijkstra 算法。

注意事项：
1. 本程序完全采用 C++14 编写，当您要编译程序时，请先确认你的 C++ 编译器是否支持 C++14；
2. 本程序采用的编程规范为 《Google C++ Coding Style》和《Effective Modern C++》，如果您觉得不符你的编码习惯，建议采用 astyle 对代码进行格式化；
3. 编译时，请 cd src && make；如果是首次编译，那一定要输入 make all；
4. 用法：./bin/a.out [-algo_type] start_station end_station；当代码首次被 clone 到本地时，并没有 bin 目录，必须要输入 make all 来穿建 bin/, obj/, result/ 目录，-algo_type 的选项为：-(dij|dijk|bell|bf|sp|spfa)，其中，-dij 或 -dijk 表示使用 Dijkstra 算法，-bell 或 -bf 表示使用 Bellmam-Ford 算法，-sp 或 -spfa 表示使用 SPFA 算法。start_station 和 end_station 为 起点站和终点站，目前只支持拼音输入，且文字拼音间用 '_' 分隔。例如，东直门就输入 dong_zhi_men, 五道口就输入 wu_dao_kou。
5. 注意，北京地铁站中，15 线关庄站与八通线管庄站的拼音相同，为了区分两者，关庄站请输入 guan_zhuang[15]，管庄站请输入 guan_zhuang[ba_tong]。
