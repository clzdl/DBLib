# DBLib
	依赖C++11支持

mysql  odbc 编译运行方式
   1. 安装unixODBC    
   2. mysql官网下载对应的odbc connector ，
   3. 配置编译环境  libmyodbc5a.so  --ansi 驱动库
       -L/path/myodbc5a
   4. 配置/etc/odbc.ini
	[ODBC Data Sources]
		myodbc3     = MyODBC 3.51 Driver DSN
	[myodbc3]
		Driver       = /path/lib/libmyodbc5a.so
		Description  = Connector/ODBC 3.51 Driver DSN
		SERVER       = localhost
		PORT         =
		USER         = root
		Password     =
		Database     = test
		OPTION       = 3
		SOCKET       =	

		
		      
   