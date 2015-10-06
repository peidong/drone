<?php
    header("Content-Type:application/json");

    //$led_status = $_POST['led_status'];
    //$led_on_ms = $_POST['led_on_ms'];
    //$led_off_ms = $_POST['led_off_ms'];
    $led_status = $_GET['led_status'];
    $led_on_ms = $_GET['led_on_ms'];
    $led_off_ms = $_GET['led_off_ms'];

    $conn = mysql_connect('localhost', 'webmaster', '');
    mysql_select_db('edison', $conn);
    
    $query = "UPDATE led
        SET led_status = '$led_status',led_on_ms = '$led_on_ms', led_off_ms = '$led_off_ms'
        WHERE id = '1'";

    $result = mysql_query($query);

    $query = "SELECT id, led_status, led_on_ms, led_off_ms
        FROM led
        WHERE id = '1'";

    $result = mysql_query($query);
    $result_array = mysql_fetch_array($result);

    $led_status = $result_array[led_status];
    $led_on_ms = $result_array[led_on_ms];
    $led_off_ms = $result_array[led_off_ms];

    $response['led_status'] = $led_status;
    $response['led_on_ms'] = $led_on_ms;
    $response['led_off_ms'] = $led_off_ms;

    deliver_response(200, "The led_status has been updated", $response);

    function deliver_response($status,$status_message,$data){
        header("HTTP/1.1 $status $status_message");
        $response['status']=$status;
        $response['status_message']=$status_message;
        $response['data']=$data;

        $json_response=json_encode($response);
        echo $json_response;
    }
?>
