<?php
    header("Content-Type:application/json");

    // start the session
    session_start();

    // close the session
    session_write_close();

    $conn = mysql_connect('localhost', 'webmaster', '');
    mysql_select_db('edison', $conn);
    
    $query = "SELECT id, pwm1, pwm2, pwm3, pwm4, update_time
        FROM pwm
        WHERE id=1";

    $result = mysql_query($query);
    $result_array = mysql_fetch_array($result);

    $pwm1 = $result_array[pwm1];
    $pwm2 = $result_array[pwm2];
    $pwm3 = $result_array[pwm3];
    $pwm4 = $result_array[pwm4];
    $update_time = $result_array[update_time];

    $response['pwm1'] = $pwm1;
    $response['pwm2'] = $pwm2;
    $response['pwm3'] = $pwm3;
    $response['pwm4'] = $pwm4;
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
