<?php
// 建立server端socket
$tcp = getprotobyname("tcp");
$socket = socket_create(AF_INET, SOCK_STREAM, $tcp);
socket_bind($socket, 'fryer.ee.ucla.edu', 10008);       //绑定要监听的端口
socket_listen($socket);       //监听端口

//初始化一个数据，和客户端通信
$buffer = "connect";
while (true) {
    // 接受一个socket连接
    $connection = socket_accept($socket);
    if(!$connection){
        echo "connect fail";
    }else{
            echo "Socket connected\n";
            // 向客户端传递一个信息数据
            if ($buffer != "") {
                echo "send data to client\n";
                socket_write($connection, $buffer . "\n");
                echo "Wrote to socket\n";
            } else {
                echo "no data in the buffer\n" ;
            }
            // 从客户端取得信息
            while ($data = @socket_read($connection, 1024, PHP_NORMAL_READ)) {
                    printf("Buffer: " . $data . "\n");
                    //取得信息给客户端一个反馈
                    socket_write($connection, "Information Received\n");
            }
    }

    socket_close($connection);
    //关闭 socket
    printf("Closed the socket\n");
}
?>
