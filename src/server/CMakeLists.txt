
#定义了一个SRC_LIST变量，包含了该目录下所有源文件
aux_source_directory(. SRC_LIST)

#指定生成可执行文件
add_executable(server ${SRC_LIST})
#链接
target_link_libraries(server muduo_net muduo_base mysqlclient hiredis pthread)