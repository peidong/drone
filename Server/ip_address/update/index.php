<?php
    header("Content-Type:application/json");

    $ip_address = $_GET['ip_address'];

    $conn = mysql_connect('localhost', 'webmaster', '');
    mysql_select_db('edison', $conn);
    
    $query = "UPDATE ip_address
        SET ip_address = '$ip_address'
        WHERE id = '1'";

    $result = mysql_query($query);

    $query = "SELECT id, ip_address
        FROM ip_address
        WHERE id = '1'";

    $result = mysql_query($query);
    $result_array = mysql_fetch_array($result);

    $ip_address = $result_array[ip_address];

    $response['ip_address'] = $ip_address;

    deliver_response(200, "The ip_address has been updated", $response);

    function deliver_response($status,$status_message,$data){
        header("HTTP/1.1 $status $status_message");
        $response['status']=$status;
        $response['status_message']=$status_message;
        $response['data']=$data;

        $json_response=json_encode($response);
        echo $json_response;
    }
?>
