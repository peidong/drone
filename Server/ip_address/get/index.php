<?php
    header("Content-Type:application/json");

    $conn = mysql_connect('localhost', 'webmaster', '');
    mysql_select_db('edison', $conn);
    
    $query = "SELECT id, ip_address, network_name, update_time
        FROM ip_address
        WHERE id = '1'";

    $result = mysql_query($query);
    $result_array = mysql_fetch_array($result);

    $ip_address = $result_array[ip_address];
    $network_name = $result_array[network_name];
    $update_time = $result_array[update_time];

    $response['ip_address'] = $ip_address;
    $response['network_name'] = $network_name;
    $response['update_time'] = $update_time;

    deliver_response(200, "The ip address has been got", $response);

    function deliver_response($status,$status_message,$data){
        header("HTTP/1.1 $status $status_message");
        $response['status']=$status;
        $response['status_message']=$status_message;
        $response['data']=$data;

        $json_response=json_encode($response);
        echo $json_response;
    }
?>
