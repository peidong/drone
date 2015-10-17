<?php
    header("Content-Type:application/json");

    // start the session
    session_start();

    // close the session
    session_write_close();

    $conn = mysql_connect('localhost', 'webmaster', '');
    mysql_select_db('edison', $conn);
    
    $query = "SELECT id, status, is_pwm_change, update_time
        FROM `general`
        WHERE id=1";

    $result = mysql_query($query);
    $result_array = mysql_fetch_array($result);

    $status = $result_array[status];
    $is_pwm_change = $result_array[is_pwm_change];
    $update_time = $result_array[update_time];

    $response['status'] = $status;
    $response['is_pwm_change'] = $is_pwm_change;
    $response['update_time'] = $update_time;

    deliver_response(200, "The general info has been got", $response);

    function deliver_response($status,$status_message,$data){
        header("HTTP/1.1 $status $status_message");
        $response['status']=$status;
        $response['status_message']=$status_message;
        $response['data']=$data;

        $json_response=json_encode($response);
        echo $json_response;
    }
?>
