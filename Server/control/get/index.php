<?php
    header("Content-Type:application/json");

    // start the session
    session_start();

    $mac_address = $_GET['mac_address'];

    // close the session
    session_write_close();

    $conn = mysql_connect('localhost', 'webmaster', '');
    mysql_select_db('edison', $conn);
    
    $query = "SELECT *
        FROM control
        WHERE mac_address = '$mac_address'";

    $result = mysql_query($query);
    $result_array = mysql_fetch_array($result);

    $control_type = $result_array[control_type];
    $auto_control_command = $result_array[auto_control_command];
    $manual_control_command = $result_array[manual_control_command];
    $suspend_pwm1 = $result_array[suspend_pwm1];
    $suspend_pwm2 = $result_array[suspend_pwm2];
    $suspend_pwm3 = $result_array[suspend_pwm3];
    $suspend_pwm4 = $result_array[suspend_pwm4];
    $update_time = $result_array[update_time];

    $response['control_type'] = $control_type;
    $response['auto_control_command'] = $auto_control_command;
    $response['manual_control_command'] = $manual_control_command;
    $response['suspend_pwm1'] = $suspend_pwm1;
    $response['suspend_pwm2'] = $suspend_pwm2;
    $response['suspend_pwm3'] = $suspend_pwm3;
    $response['suspend_pwm4'] = $suspend_pwm4;
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
