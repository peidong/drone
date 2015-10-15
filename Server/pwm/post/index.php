<?php
    header("Content-Type:application/json");

    // start the session
    session_start();

    $pwm1 = $_POST['pwm1'];
    $pwm2 = $_POST['pwm2'];
    $pwm3 = $_POST['pwm3'];
    $pwm4 = $_POST['pwm4'];

    $conn = mysql_connect('localhost', 'webmaster', '');
    mysql_select_db('edison', $conn);

    $query = "UPDATE `pwm`
        SET pwm1 = '$pwm1', pwm2 = '$pwm2', pwm3 = '$pwm3', pwm4 = '$pwm4', update_time = now()
        WHERE id = '1'";

    $result = mysql_query($query);

    // close the session
    session_write_close();

    deliver_response(200, "The ip_address has been updated", NULL);

    function deliver_response($status,$status_message,$data){
        header("HTTP/1.1 $status $status_message");
        $response['status']=$status;
        $response['status_message']=$status_message;
        $response['data']=$data;

        $json_response=json_encode($response);
        echo $json_response;
    }
?>
